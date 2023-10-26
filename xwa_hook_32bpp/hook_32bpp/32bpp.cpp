#include "targetver.h"
#include "32bpp.h"
#include <vector>
#include <string>
#include <Windows.h>

enum ParamsEnum
{
	Params_ReturnAddress = -1,
	Params_EAX = -3,
	Params_ECX = -4,
	Params_EDX = -5,
	Params_EBX = -6,
	Params_EBP = -8,
	Params_ESI = -9,
	Params_EDI = -10,
};

#pragma pack(push, 1)

struct XwaTextureDescription
{
	unsigned char* Palettes;
	int PalettesType;
	int TextureSize;
	int DataSize;
	int Width;
	int Height;
};

static_assert(sizeof(XwaTextureDescription) == 24, "size of XwaTextureDescription must be 24");

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

struct DatImageHeader
{
	int Length; // sizeof(S0x04CD390_05) + ColorsCount * 3 + Width * Height * 2
	int ColorsOffset; // sizeof(S0x04CD390_05)
	int DataOffset; // sizeof(S0x04CD390_05) + ColorsCount * 3
	int DatImageHeader_m0C;
	char unk10[8];
	int OffsetX;
	int OffsetY;
	char unk20[8];
	int ColorsCount;
};

static_assert(sizeof(DatImageHeader) == 44, "size of DatImageHeader must be 44");

struct XwaSpeciesTMInfo
{
	void* pData;
	unsigned short Width;
	unsigned short Height;
	unsigned int Color;
	unsigned char MipmapLevel;
};

static_assert(sizeof(XwaSpeciesTMInfo) == 13, "size of XwaSpeciesTMInfo must be 13");

struct XwaGlobalLight
{
	int PositionX;
	int PositionY;
	int PositionZ;
	float DirectionX;
	float DirectionY;
	float DirectionZ;
	float Intensity;
	float m1C;
	float ColorR;
	float ColorB;
	float ColorG;
	float BlendStartIntensity;
	float BlendStartColor1C;
	float BlendStartColorR;
	float BlendStartColorB;
	float BlendStartColorG;
	float BlendEndIntensity;
	float BlendEndColor1C;
	float BlendEndColorR;
	float BlendEndColorB;
	float BlendEndColorG;
};

static_assert(sizeof(XwaGlobalLight) == 84, "size of XwaGlobalLight must be 84");

#pragma pack(pop)

class ColorConvert
{
public:
	ColorConvert()
	{
		for (unsigned int color = 0; color < 0x10000; color++)
		{
			unsigned int r = (color >> 11) & 0x1f;
			unsigned int g = (color >> 5) & 0x3f;
			unsigned int b = color & 0x1f;

			r = (r * 539086 + 32768) >> 16;
			g = (g * 265264 + 32768) >> 16;
			b = (b * 539086 + 32768) >> 16;

			this->color16to32[color] = (r << 16) | (g << 8) | b;
		}
	}

	unsigned int color16to32[0x10000];
};

ColorConvert g_colorConvert;

std::string g_optName;
std::vector<unsigned char> g_lightMapBuffer;
std::vector<unsigned char> g_colorMapBuffer;
std::vector<unsigned char> g_illumMapBuffer;

class NetFunctions
{
public:
	typedef int(_cdecl* readOptFunction)(const char*);
	typedef int(_cdecl* getOptVersionFunction)();
	typedef void(_cdecl* writeOptFunction)(void*);

	typedef int(_cdecl* readCompressedDatImageFunction)(void*, int, const void*, int);

	NetFunctions()
	{
		_module = LoadLibrary("hook_32bpp_net.dll");

		if (!_module)
		{
			return;
		}

		_readOptFunction = (readOptFunction)GetProcAddress(_module, "ReadOptFunction");
		_getOptVersionFunction = (getOptVersionFunction)GetProcAddress(_module, "GetOptVersionFunction");
		_writeOptFunction = (writeOptFunction)GetProcAddress(_module, "WriteOptFunction");
		_readCompressedDatImageFunction = (readCompressedDatImageFunction)GetProcAddress(_module, "ReadCompressedDatImageFunction");
	}

	~NetFunctions()
	{
		FreeLibrary(_module);
	}

	HMODULE _module;
	readOptFunction _readOptFunction = nullptr;
	getOptVersionFunction _getOptVersionFunction = nullptr;
	writeOptFunction _writeOptFunction = nullptr;
	readCompressedDatImageFunction _readCompressedDatImageFunction = nullptr;
};

NetFunctions g_netFunctions;

int SetOptNameHook(int* params)
{
	const char* filename = (const char*)params[1];
	int* pVersion = (int*)params[2];

	const auto XwaIOOpenFile = (int(*)(const char*, const char*, int, int))0x00433B40;
	const auto Xwa_fread = (int(*)(void*, int, int, FILE*))0x0059BAB0;
	const auto Xwa_fclose = (int(*)(FILE*))0x0059ACE0;
	const auto Free_Handle = (void(*)(const char*, short))0x0050E240;
	const auto XwaAllocHandleWithoutZeroingMemory = (short(*)(const char*, int))0x0050E0F0;
	const auto Lock_Handle = (void* (*)(short))0x0050E2F0;
	const auto Unlock_Handle = (void(*)(short))0x0050E350;
	const auto PtrAdjustOptimizedPolyObject = (void(*)(void*))0x004849C0;
	const auto XwaIOShowErrorMessage = (void(*)(int))0x004340D0;

	const int StringsFileError_NotEnoughtMemory = 0;

	char* s_XwaIOFileName = (char*)0x0080DA60;
	FILE*& s_XwaIOFile = *(FILE**)0x00910780;
	int& s_XwaLoadOptBufMemSize = *(int*)0x0074C270;
	short& s_XwaLoadOptBufMemHandle = *(short*)0x0074C26C;

	g_optName = filename;

	*pVersion = 0;

	int requiredFileSize = g_netFunctions._readOptFunction(filename);

	int version = 0;
	int filesize = 0;
	FILE* xwaFile = nullptr;

	if (requiredFileSize)
	{
		strcpy_s(s_XwaIOFileName, 256, filename);

		version = g_netFunctions._getOptVersionFunction();
		filesize = requiredFileSize;
	}
	else
	{
		XwaIOOpenFile(filename, "rb", 0, 0);
		xwaFile = s_XwaIOFile;

		if (xwaFile == 0)
		{
			return 0;
		}

		Xwa_fread(&version, 1, 4, xwaFile);

		if (version > 0)
		{
			filesize = version;
			version = 0;
		}
		else
		{
			version = -version;
			Xwa_fread(&filesize, 1, 4, xwaFile);
		}

		if (version < 2)
		{
			Xwa_fclose(xwaFile);
			xwaFile = nullptr;
		}
	}

	if (s_XwaLoadOptBufMemSize < filesize && s_XwaLoadOptBufMemHandle != 0)
	{
		Free_Handle("LOADOPTBUF", s_XwaLoadOptBufMemHandle);
		s_XwaLoadOptBufMemHandle = 0;
		s_XwaLoadOptBufMemSize = 0;
	}

	if (s_XwaLoadOptBufMemHandle == 0)
	{
		s_XwaLoadOptBufMemHandle = XwaAllocHandleWithoutZeroingMemory("LOADOPTBUF", filesize);

		if (s_XwaLoadOptBufMemHandle == 0)
		{
			XwaIOShowErrorMessage(StringsFileError_NotEnoughtMemory);
		}

		s_XwaLoadOptBufMemSize = filesize;
	}

	short optMemHandle = s_XwaLoadOptBufMemHandle;
	void* optPtr = Lock_Handle(optMemHandle);

	if (requiredFileSize)
	{
		g_netFunctions._writeOptFunction(optPtr);
	}
	else
	{
		Xwa_fread(optPtr, 1, filesize, xwaFile);
		Xwa_fclose(xwaFile);
		xwaFile = nullptr;
	}

	PtrAdjustOptimizedPolyObject(optPtr);
	Unlock_Handle(optMemHandle);

	*pVersion = version;

	return optMemHandle;
}

int SetAlphaMaskHook(int* params)
{
	const unsigned char* palette = (unsigned char*)params[708];
	bool hasAlpha = palette[2] != 0;

	return hasAlpha ? 1 : 0;
}

int CreateLightMapHook(int* params)
{
	// esp34 = 1
	params[14] = 1;

	const int size = params[0];
	const int esp2C = params[12];

	const unsigned char* a10 = (unsigned char*)params[706];
	const unsigned char* a14 = (unsigned char*)params[707];

	if (a14 == nullptr)
	{
		return 0;
	}

	if ((int)g_lightMapBuffer.capacity() < size * 8)
	{
		g_lightMapBuffer.reserve(size * 8);
	}

	const unsigned char* src = a10 + esp2C * 4;
	const unsigned char* src_illum = a14 + esp2C;
	unsigned char* dst = g_lightMapBuffer.data() + esp2C * 4;

	memcpy(dst, src, size * 4);

	for (int i = 0; i < size; i++)
	{
		dst[i * 4 + 3] = src_illum[i];
	}

	return (int)g_lightMapBuffer.data() + esp2C * 4;
}

int ConvertColorMapHook(int* params)
{
	static XwaTextureDescription tempTextureDescription;

	// esp28 = 0
	params[10] = 0;

	const char* A4 = (const char*)params[702];
	XwaTextureDescription*& AC = *(XwaTextureDescription**)&params[704];
	int& A10 = params[705];
	int& A14 = params[706];
	unsigned short* A18 = (unsigned short*)params[707];
	unsigned short* A1C = (unsigned short*)params[708];

	const int XwaFlightBrightness = *(int*)0x006002C8;
	const int brightnessLevel = (XwaFlightBrightness - 0x100) / 0x40;

	int size = AC->Width * AC->Height;

	int bytesSize;

	if (size == AC->TextureSize)
	{
		bytesSize = AC->DataSize;
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

	if (bpp == 8)
	{
		if ((int)g_colorMapBuffer.capacity() < size * 8)
		{
			g_colorMapBuffer.reserve(size * 8);
		}

		if ((int)g_illumMapBuffer.capacity() < size * 2)
		{
			g_illumMapBuffer.reserve(size * 2);
		}

		bool hasAlpha = A14 != 0;
		bool hasIllum = false;

		unsigned char* illumBuffer = g_illumMapBuffer.data();
		unsigned char* colorBuffer = g_colorMapBuffer.data();

#pragma omp parallel for
		for (int i = 0; i < bytesSize; i++)
		{
			int colorIndex = *(unsigned char*)(A10 + i);
			unsigned short color = A1C[4 * 256 + colorIndex];
			unsigned short color8 = A1C[8 * 256 + colorIndex];

			unsigned char r = (unsigned char)((color & 0xF800U) >> 11);
			unsigned char g = (unsigned char)((color & 0x7E0U) >> 5);
			unsigned char b = (unsigned char)(color & 0x1FU);

			if (r <= 8 && g <= 16 && b <= 8)
			{
				illumBuffer[i] = 0;
			}
			else if (hasAlpha && *(unsigned char*)(A14 + i) != 255)
			{
				illumBuffer[i] = 0;
			}
			else if (color == color8)
			{
				hasIllum = true;
				//illumBuffer[i] = 0x3f;
				illumBuffer[i] = 0x7f;
			}
			else
			{
				illumBuffer[i] = 0;
			}
		}

#pragma omp parallel for
		for (int i = 0; i < bytesSize; i++)
		{
			int paletteIndex = 4 + brightnessLevel;

			int colorIndex = *(unsigned char*)(A10 + i);
			//unsigned short color = A18[colorIndex];
			unsigned short color = A1C[256 * paletteIndex + colorIndex];
			unsigned char illum = illumBuffer[i];

			unsigned int a = (A14 == 0 || illum != 0) ? (unsigned char)255 : *(unsigned char*)(A14 + i);
			unsigned int color32 = g_colorConvert.color16to32[color];

			*(unsigned int*)(colorBuffer + i * 4) = (a << 24) | color32;
		}

		tempTextureDescription = *AC;
		AC = &tempTextureDescription;

		AC->Palettes[2] = hasAlpha ? 0xff : 0;
		AC->Palettes[4] = hasIllum ? 0xff : 0;
		AC->DataSize = bytesSize * 4;

		A14 = hasIllum ? (int)g_illumMapBuffer.data() : 0;
		//A14 = (int)g_illumMapBuffer.data();
		A10 = (int)g_colorMapBuffer.data();
	}
	else if (bpp == 32)
	{
		bool hasIllum = AC->Palettes[4] != 0;

		if (!hasIllum)
		{
			A14 = 0;
		}

		bytesSize /= 4;

#pragma omp parallel for
		for (int i = 0; i < bytesSize; i++)
		{
			int paletteIndex = 4 + brightnessLevel;
			unsigned char illum = A14 == 0 ? (unsigned char)0 : *(unsigned char*)(A14 + i);

			if (*(unsigned char*)(A10 + i * 4 + 3) != 255)
			{
				illum = 0;
			}

			unsigned int b = *(unsigned char*)(A10 + i * 4 + 0);
			unsigned int g = *(unsigned char*)(A10 + i * 4 + 1);
			unsigned int r = *(unsigned char*)(A10 + i * 4 + 2);
			unsigned int a = illum != 0 ? (unsigned char)255 : *(unsigned char*)(A10 + i * 4 + 3);

			if (illum == 0)
			{
				b = (b * 128 * paletteIndex / 8 + b * 128) / 256;
				g = (g * 128 * paletteIndex / 8 + g * 128) / 256;
				r = (r * 128 * paletteIndex / 8 + r * 128) / 256;
			}

			*(unsigned char*)(A10 + i * 4 + 0) = b;
			*(unsigned char*)(A10 + i * 4 + 1) = g;
			*(unsigned char*)(A10 + i * 4 + 2) = r;
			*(unsigned char*)(A10 + i * 4 + 3) = a;
		}
	}

	return 0;
}

int ComputeGlobalLightsHook(int* params)
{
	const int A8 = params[9];
	const int AC = params[10];

	float& ColorR = *(float*)(A8 + 0x14);
	float& ColorG = *(float*)(A8 + 0x18);
	float& ColorB = *(float*)(A8 + 0x1C);

	float normalX = *(float*)(AC + 0x00);
	float normalY = *(float*)(AC + 0x04);
	float normalZ = *(float*)(AC + 0x08);

	const int XwaGlobalLightsCount = *(int*)0x00782848;
	const XwaGlobalLight* XwaGlobalLights = (XwaGlobalLight*)0x007D4FA0;

	//ColorR = 0.2f;
	//ColorG = 0.2f;
	//ColorB = 0.2f;

	ColorR = 0.4f;
	ColorG = 0.4f;
	ColorB = 0.4f;

	for (int ebx = 0; ebx < XwaGlobalLightsCount; ebx++)
	{
		const XwaGlobalLight* light = &XwaGlobalLights[ebx];

		float esp24 = light->DirectionX * normalX + light->DirectionY * normalY + light->DirectionZ * normalZ;

		//if (fabs(normalX) < 0.4f && fabs(normalY) < 0.4f && fabs(normalZ) > 0.6f)
		//{
		//	esp24 = 1.0f;
		//}

		if (esp24 > 0.0f)
		{
			ColorR += light->ColorR * esp24 * light->Intensity;
			ColorG += light->ColorG * esp24 * light->Intensity;
			ColorB += light->ColorB * esp24 * light->Intensity;
		}
	}

	return 0;
}

int DatImage32Hook(int* params)
{
	const auto ConvertDatImage24To32 = (void(*)(const DatImageDescription*, int, int, int, XwaSpeciesTMInfo*, int))0x004CE4F0;
	const auto XwaMemMalloc = (void* (*)(const char*, unsigned int))0x0050E070;

	const DatImageDescription* A4 = (DatImageDescription*)params[0];
	const int A8 = params[1];
	const int AC = params[2];
	const int A10 = params[3];
	XwaSpeciesTMInfo* A14 = (XwaSpeciesTMInfo*)params[4];
	const int A18 = params[5];

	const DatImageHeader* imageHeader = (DatImageHeader*)(params[0] + sizeof(DatImageDescription));
	const void* imageData = (void*)(params[0] + sizeof(DatImageDescription) + sizeof(DatImageHeader));

	if (A4->ImageType == 25)
	{
		for (int esp10 = 0; esp10 < A18; esp10++)
		{
			//if (A14[esp10].Width == 0)
			//{
			//	break;
			//}

			//int ebp = (A4->Width * A4->Height) >> (A14[esp10].MipmapLevel * 2);
			int ebp = (A4->Width * A4->Height);

			DatImageDescription* esi = (DatImageDescription*)XwaMemMalloc("RESOURCEITEM", sizeof(DatImageDescription) + ebp * 4);

			if (esi != 0)
			{
				memcpy(esi, A4, sizeof(DatImageDescription));

				esi->ImageType = 1;

				unsigned int* esp24 = (unsigned int*)((int)esi + sizeof(DatImageDescription));

				//if (A14[esp10].MipmapLevel == 0)
				//{
				//	memcpy(esp24, (void*)AC, ebp * 4);
				//}
				//else
				//{
				//	memset(esp24, 0, ebp * 4);

				//	esi->Width >>= A14[esp10].MipmapLevel;
				//	esi->Height >>= A14[esp10].MipmapLevel;
				//	esi->DataSize >>= A14[esp10].MipmapLevel * 2;

				//	/*
				//	XwaGetMipBit( AC, A4->Width, A4->Height, A8, A14[esp10].MipmapLevel, esp24 );
				//	*/
				//}

				A14[esp10].MipmapLevel = 0;
				A14[esp10].Width = A4->Width;
				A14[esp10].Height = A4->Height;

				if (imageHeader->ColorsCount == 0)
				{
					if (A4->DataSize - 0x2C >= ebp * 4)
					{
						memcpy(esp24, (void*)AC, ebp * 4);
					}
					else
					{
						esi->ColorsCount = -1;

						memcpy(esp24, (void*)AC, A4->DataSize - 0x2C);
					}
				}
				else if (imageHeader->ColorsCount == 1)
				{
					g_netFunctions._readCompressedDatImageFunction(esp24, ebp * 4, imageData, A4->DataSize - 0x2C);
				}
			}

			A14[esp10].pData = esi;
		}
	}
	else
	{
		ConvertDatImage24To32(A4, A8, AC, A10, A14, A18);
	}

	return 0;
}

int DatImageSetPixelFormatHook(int* params)
{
	params[13] = 0;

	DatImageDescription* ebp = (DatImageDescription*)params[Params_EBP];

	if (ebp->ColorsCount == -1)
	{
		params[13 + 24] = ebp->DataSize;
	}

	return 0;
}

int OptCreateD3DTexturesFromTexturesHook(int* params)
{
	int A4 = params[Params_ESI];
	int numOfNodes = *(int*)(A4 + 0x06);

	if (numOfNodes == 0)
	{
		params[Params_ReturnAddress] = 0x004CD084;
		return 0;
	}

	params[Params_EDI] = numOfNodes - 1;

	return 0;
}

int OptCreateD3DTexturesFromTexturesLoopHook(int* params)
{
	params[Params_ReturnAddress] = 0x004CD191;

	const auto XwaOptCreateD3DTexturesFromTextures = (int(*)(int, int, int, int))0x004CD100;

	int A4 = params[Params_ESI];
	int numOfNodes = *(int*)(A4 + 0x08);
	int* nodes = *(int**)(A4 + 0x0C);

	int A8 = params[6];
	int AC = params[7];
	int A10 = params[8];

	for (int edi = numOfNodes - 1; edi >= 0; edi--)
	{
		AC = XwaOptCreateD3DTexturesFromTextures(nodes[edi], A8, AC, A10);
	}

	params[Params_EAX] = AC;

	return 0;
}

int OptTransformTexturesToD3DTexturesHook(int* params)
{
	int A4 = params[Params_ESI];
	int numOfNodes = *(int*)(A4 + 0x06);

	if (numOfNodes == 0)
	{
		params[Params_ReturnAddress] = 0x004CD0BA;
		return 0;
	}

	params[Params_EDI] = numOfNodes - 1;

	return 0;
}

int OptTransformTexturesToD3DTexturesLoopHook(int* params)
{
	params[Params_ReturnAddress] = 0x004CD292;

	const auto XwaOptTransformTexturesToD3DTextures = (int(*)(int, int, int, int, int*))0x004CD1A0;

	int A4 = params[Params_ESI];
	int numOfNodes = *(int*)(A4 + 0x08);
	int* nodes = *(int**)(A4 + 0x0C);

	int A8 = params[6];
	int AC = params[7];
	int A10 = params[8];
	int* A14 = (int*)params[9];

	for (int edi = numOfNodes - 1; edi >= 0; edi--)
	{
		A10 = XwaOptTransformTexturesToD3DTextures(nodes[edi], A8, AC, A10, A14);
	}

	params[Params_EAX] = A10;

	return 0;
}
