#include "targetver.h"
#include "textures.h"
#include "config.h"

#include <Windows.h>

bool StringEndsWith(const char* str, const char* suffix)
{
	size_t strLength = strlen(str);
	size_t suffixLength = strlen(suffix);

	if (strLength < suffixLength)
	{
		return false;
	}

	return _stricmp(str + strLength - suffixLength, suffix) == 0;
}

class Config
{
public:
	Config()
	{
		auto lines = GetFileLines("hook_textures_tag.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_textures_tag");
		}

		this->OutputDebugStrings = GetFileKeyValueInt(lines, "OutputDebugStrings") != 0;
	}

	bool OutputDebugStrings;
};

Config g_config;

#pragma pack(push, 1)

struct DatImageDescription
{
	short ImageType;
	short Width;
	short Height;
	unsigned short ColorKey;
	short ColorsCount;
	short GroupId;
	short ImageId;
	int DataSize;
};

static_assert(sizeof(DatImageDescription) == 18, "size of DatImageDescription must be 18");

struct XwaSpeciesTMInfo
{
	void* pData;
	unsigned short Width;
	unsigned short Height;
	unsigned int Color;
	unsigned char MipmapLevel;
};

static_assert(sizeof(XwaSpeciesTMInfo) == 13, "size of XwaSpeciesTMInfo must be 13");

struct XwaVBuffer
{
	char unk00[212];
	const char* tag;
};

static_assert(sizeof(XwaVBuffer) == 216, "size of XwaVBuffer must be 216");

struct XwaTextureDescription
{
	unsigned char* Palettes;
	int m04;
	int TextureSize;
	int DataSize;
	int Width;
	int Height;
};

static_assert(sizeof(XwaTextureDescription) == 24, "size of XwaTextureDescription must be 24");

struct DDSURFACEDESC
{
	char unk00[32];
	const char* tag;
	char unk24[72];
};

static_assert(sizeof(DDSURFACEDESC) == 108, "size of DDSURFACEDESC must be 108");

#pragma pack(pop)

std::string g_textureTag;
std::string g_textureTag2;
std::string g_currentOptFileName;

int DatImageHook(int* params)
{
	const auto XwaCreateTextureSurface = (char(*)(void*, XwaVBuffer**, int, void*, int, char))0x005960BE;

	void* A4 = (void*)params[0];
	XwaVBuffer** A8 = (XwaVBuffer**)params[1];
	int AC = params[2];
	void* A10 = (void*)params[3];
	int A14 = params[4];
	char A18 = (char)params[5];

	XwaVBuffer* esp1C = A8[0];

	XwaSpeciesTMInfo* tmInfo = (XwaSpeciesTMInfo*)params[68];
	DatImageDescription* imageDesc = (DatImageDescription*)tmInfo->pData;

	g_textureTag = "dat," + std::to_string(imageDesc->GroupId) + "," + std::to_string(imageDesc->ImageId) + "," + std::to_string(tmInfo->MipmapLevel);

	esp1C->tag = g_textureTag.c_str();

	return XwaCreateTextureSurface(A4, A8, AC, A10, A14, A18);
}

int OptReadHook(int* params)
{
	const auto OptRead = (int(*)(const char*, int*))0x004CCB00;

	const char* fileName = (const char*)params[0];
	int* pVersion = (int*)params[1];

	g_currentOptFileName = fileName;

	int result = OptRead(fileName, pVersion);

	if (!result)
	{
		if (!StringEndsWith(fileName, "Cockpit.opt") && !StringEndsWith(fileName, "Exterior.opt"))
		{
			MessageBox(nullptr, (g_currentOptFileName + " was not found").c_str(), "OPT read", MB_OK | MB_ICONERROR);
		}
	}

	return result;
}

int OptTextureColorHook(int* params)
{
	const auto XwaCreateTextureSurface = (char(*)(void*, XwaVBuffer**, int, void*, int, char))0x005960BE;

	void* A4 = (void*)params[0];
	XwaVBuffer** A8 = (XwaVBuffer**)params[1];
	int AC = params[2];
	void* A10 = (void*)params[3];
	int A14 = params[4];
	char A18 = (char)params[5];

	const char* textureFileName = (const char*)params[708];
	XwaTextureDescription* textureDescription = (XwaTextureDescription*)params[710];
	int texureAlphaIllum = params[712];

	int size = textureDescription->Width * textureDescription->Height;

	int bytesSize;

	if (size == textureDescription->TextureSize)
	{
		bytesSize = textureDescription->DataSize;
	}
	else
	{
		bytesSize = size;
	}

	int bpp = 0;

	if (bytesSize >= size && bytesSize < size * 2)
	{
		bpp = 8;
	}
	else if (bytesSize >= size * 4 && bytesSize < size * 8)
	{
		bpp = 32;
	}

	bool hasAlpha;

	if (bpp == 8)
	{
		hasAlpha = texureAlphaIllum != 0;
	}
	else if (bpp == 32)
	{
		hasAlpha = textureDescription->Palettes[2] != 0 || (textureDescription->Palettes[4] == 0 && texureAlphaIllum != 0);
	}
	else
	{
		hasAlpha = false;
	}

	g_textureTag = "opt," + g_currentOptFileName + "," + textureFileName + ",color";

	if (hasAlpha)
	{
		g_textureTag.append("-transparent");
	}

	for (int i = 0; i < A14; i++)
	{
		XwaVBuffer* vBuffer = A8[i];

		vBuffer->tag = g_textureTag.c_str();
	}

	return XwaCreateTextureSurface(A4, A8, AC, A10, A14, A18);
}

int OptTextureLightHook(int* params)
{
	const auto XwaCreateTextureSurface = (char(*)(void*, XwaVBuffer**, int, void*, int, char))0x005960BE;

	void* A4 = (void*)params[0];
	XwaVBuffer** A8 = (XwaVBuffer**)params[1];
	int AC = params[2];
	void* A10 = (void*)params[3];
	int A14 = params[4];
	char A18 = (char)params[5];

	const char* textureFileName = (const char*)params[708];

	g_textureTag = "opt," + g_currentOptFileName + "," + textureFileName + ",light";

	for (int i = 0; i < A14; i++)
	{
		XwaVBuffer* vBuffer = A8[i];

		vBuffer->tag = g_textureTag.c_str();
	}

	return XwaCreateTextureSurface(A4, A8, AC, A10, A14, A18);
}

int Create3DTextureSurfaceHook(int* params)
{
	const auto XwaMemCopy = (void(*)(void*, void*, int))0x0059D7F0;

	void* dst = (void*)params[0];
	void* src = (void*)params[1];
	int size = params[2];

	XwaMemCopy(dst, src, size);

	DDSURFACEDESC* pSurfaceDesc = (DDSURFACEDESC*)dst;

	XwaVBuffer** A8 = (XwaVBuffer**)params[106];
	int ebpFC = params[40];

	//g_textureTag2 = std::string(A8[ebpFC]->tag) + "," + std::to_string(ebpFC);
	g_textureTag2 = g_textureTag + "," + std::to_string(ebpFC);

	pSurfaceDesc->tag = g_textureTag2.c_str();

	if (g_config.OutputDebugStrings)
	{
		OutputDebugString(g_textureTag2.c_str());
	}

	return 0;
}
