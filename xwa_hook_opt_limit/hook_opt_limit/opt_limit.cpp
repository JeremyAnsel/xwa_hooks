#include "targetver.h"
#include "opt_limit.h"
#include "config.h"

#pragma pack(push, 1)

struct HitData
{
	float m00;
	float m04;
	char m08;
};

static_assert(sizeof(HitData) == 9, "size of HitData must be 9");

struct XwaMobileObject
{
	char Unk0000[221];
	int pCraft;
	char Unk00E1[4];
};

static_assert(sizeof(XwaMobileObject) == 229, "size of XwaMobileObject must be 229");

struct XwaObject
{
	char Unk00[35];
	XwaMobileObject* pMobileObject;
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

#pragma pack(pop)

class Config
{
public:
	Config()
	{
		auto lines = GetFileLines("hooks.ini", "hook_opt_limit");

		if (lines.empty())
		{
			lines = GetFileLines("hook_opt_limit.cfg");
		}

		this->MeshesCount = GetFileKeyValueInt(lines, "MeshesCount", 255);
		this->Craft_Size = 0x3F9 + GetFileKeyValueInt(lines, "Craft_ExtraSize", this->MeshesCount * 3);
		this->Craft_Offset_22E = 0x3F9 + GetFileKeyValueInt(lines, "Craft_Offset_22E", 0);
		this->Craft_Offset_260 = 0x3F9 + GetFileKeyValueInt(lines, "Craft_Offset_260", this->MeshesCount * 1);
		this->Craft_Offset_292 = 0x3F9 + GetFileKeyValueInt(lines, "Craft_Offset_292", this->MeshesCount * 2);
	}

	int MeshesCount;
	int Craft_Size;
	int Craft_Offset_22E;
	int Craft_Offset_260;
	int Craft_Offset_292;
};

const Config& GetConfig()
{
	static Config s_config;
	return s_config;
}

class HitDataArray
{
public:
	HitDataArray()
	{
		auto lines = GetFileLines("hooks.ini", "hook_opt_limit");

		if (lines.empty())
		{
			lines = GetFileLines("hook_opt_limit.cfg");
		}

		int count = abs(GetFileKeyValueInt(lines, "HitDataArraySize"));

		if (count == 0)
		{
			count = 65536;
		}

		this->_hitData.reserve(count);
	}

	int Size()
	{
		return this->_hitData.capacity();
	}

	HitData* Data()
	{
		return this->_hitData.data();
	}

private:
	std::vector<HitData> _hitData;
};

int GetHitDataArrayPtr()
{
	static HitDataArray g_XwaHitDataArray;

	return (int)g_XwaHitDataArray.Data();
}

int GetCraftSize()
{
	return GetConfig().Craft_Size;
}

int GetCraftMeshesCount()
{
	return GetConfig().MeshesCount;
}

int GetCraftOffset_22E()
{
	return GetConfig().Craft_Offset_22E;
}

int GetCraftOffset_260()
{
	return GetConfig().Craft_Offset_260;
}

int GetCraftOffset_292()
{
	return GetConfig().Craft_Offset_292;
}

int GetOptModelMeshesInfoSize()
{
	return 4 + GetConfig().MeshesCount * 4 * 2;
}

int GetOptModelMeshesInfoPtr()
{
	static bool init = true;
	static std::vector<char> s_XwaOptModelMeshesInfos;

	if (init)
	{
		init = false;
		int size = 4 + GetConfig().MeshesCount * 4 * 2;
		s_XwaOptModelMeshesInfos.reserve(size * 557);
		*(int*)0x008D9760 = (int)s_XwaOptModelMeshesInfos.data();
	}

	return (int)s_XwaOptModelMeshesInfos.data();
}

int GetOptModelMeshesInfoOffsetType()
{
	return 4;
}

int GetOptModelMeshesInfoOffsetDescriptor()
{
	return 4 + GetConfig().MeshesCount * 4;
}

int GenerateSpecRciHook(int* params)
{
	const auto XwaReadShiplist = (void(*)())0x00529950;
	const auto XwaGenerateSpecRci = (void(*)())0x00577490;
	const auto XwaFileOpen = (void*(*)(const char*, const char*))0x0052AD30;
	const auto XwaFileClose = (void(*)(void*))0x0052ADD0;

	XwaReadShiplist();

	void* specRciFile = XwaFileOpen("spec.rci", "rb");

	if (specRciFile)
	{
		XwaFileClose(specRciFile);
	}
	else
	{
		XwaGenerateSpecRci();
	}

	return 0;
}

int CraftInitHook(int* params)
{
	const int objectIndex = (short)params[0];

	const auto L0040C0E0 = (void(*)(int))0x0040C0E0;

	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;

	int pCraft = xwaObjects[objectIndex].pMobileObject->pCraft;
	memset((char*)(pCraft + GetConfig().Craft_Offset_292), 0xff, GetConfig().MeshesCount);

	L0040C0E0(objectIndex);

	return 0;
}

int Set0x07821E8Hook(int* params)
{
	static std::vector<char> s_V0x07821E8;

	s_V0x07821E8.reserve(GetConfig().Craft_Size);
	memset(s_V0x07821E8.data(), 0, GetConfig().Craft_Size);

	*(int*)0x00782705 = (int)s_V0x07821E8.data();

	return 0;
}

int OptModelMeshesInfoHook1(int* params)
{
	const int modelIndex = (short)params[0];

	if (modelIndex < 0 || modelIndex >= 557)
	{
		return 0;
	}

	const auto OptGetNumOfMeshes = (int(*)(int))0x00485190;
	const auto OptGetMeshType = (int(*)(int, int))0x00485A70;
	const auto OptGetMeshDescriptor = (int(*)(int, int))0x004859B0;

	int ptr = GetOptModelMeshesInfoPtr() + GetOptModelMeshesInfoSize() * modelIndex;
	int offsetType = GetOptModelMeshesInfoOffsetType();
	int offsetDescriptor = GetOptModelMeshesInfoOffsetDescriptor();

	int numMeshes = OptGetNumOfMeshes(modelIndex);
	*(int*)ptr = numMeshes;

	for (int i = 0; i < numMeshes; i++)
	{
		((int*)(ptr + offsetType))[i] = OptGetMeshType(modelIndex, i);
		((int*)(ptr + offsetDescriptor))[i] = OptGetMeshDescriptor(modelIndex, i);
	}

	return 0;
}

int OptModelMeshesInfoHook2(int* params)
{
	const int modelIndex = (short)params[0];
	const int numMeshes = params[1];

	const auto OptGetMeshType = (int(*)(int, int))0x00485A70;
	const auto OptGetMeshDescriptor = (int(*)(int, int))0x004859B0;

	int ptr = GetOptModelMeshesInfoPtr() + GetOptModelMeshesInfoSize() * modelIndex;
	int offsetType = GetOptModelMeshesInfoOffsetType();
	int offsetDescriptor = GetOptModelMeshesInfoOffsetDescriptor();

	*(int*)ptr = numMeshes;

	for (int i = 0; i < numMeshes; i++)
	{
		((int*)(ptr + offsetType))[i] = OptGetMeshType(modelIndex, i);
		((int*)(ptr + offsetDescriptor))[i] = OptGetMeshDescriptor(modelIndex, i);
	}

	return 0;
}

int OptModelMeshesInfoHook3(int* params)
{
	const int modelIndex = (short)params[0];

	int ptr = GetOptModelMeshesInfoPtr() + GetOptModelMeshesInfoSize() * modelIndex;

	int numMeshes = *(int*)ptr;

	return numMeshes;
}

int OptModelMeshesInfoHook4(int* params)
{
	const int modelIndex = (short)params[1];
	const int meshIndex = params[2];

	int ptr = GetOptModelMeshesInfoPtr() + GetOptModelMeshesInfoSize() * modelIndex;
	int numMeshes = *(int*)ptr;
	int offsetType = GetOptModelMeshesInfoOffsetType();

	if (meshIndex < 0 || meshIndex >= numMeshes)
	{
		return 0;
	}

	int type = ((int*)(ptr + offsetType))[meshIndex];

	return type;
}

int OptModelMeshesInfoHook5(int* params)
{
	const int modelIndex = (short)params[0];
	const int meshIndex = params[1];

	int ptr = GetOptModelMeshesInfoPtr() + GetOptModelMeshesInfoSize() * modelIndex;
	int numMeshes = *(int*)ptr;
	int offsetDescriptor = GetOptModelMeshesInfoOffsetDescriptor();

	if (meshIndex < 0 || meshIndex >= numMeshes)
	{
		return 0;
	}

	int descriptor = ((int*)(ptr + offsetDescriptor))[meshIndex];

	return descriptor;
}

std::vector<int> s_V0x09107A4;
std::vector<int> s_V0x091086C;

int CockpitOptInfoHook(int* params)
{
	s_V0x09107A4.reserve(GetConfig().MeshesCount);
	s_V0x091086C.reserve(GetConfig().MeshesCount);

	const auto OptGetMeshType = (int(*)(int, int))0x00485A70;
	const auto OptGetMeshDescriptor = (int(*)(int, int))0x004859B0;

	const int count = *(int*)0x09107A0;

	for (int i = 0; i < count; i++)
	{
		s_V0x09107A4.data()[i] = OptGetMeshType(0, i);
		s_V0x091086C.data()[i] = OptGetMeshDescriptor(0, i);
	}

	return 0;
}

int CockpitOptInfoGetDescriptorHook(int* params)
{
	const int meshIndex = params[0];

	return s_V0x091086C.data()[meshIndex];
}

std::vector<int> s_V0x0808164;
std::vector<int> s_V0x080822C;

int ExteriorOptInfoHook(int* params)
{
	s_V0x0808164.reserve(GetConfig().MeshesCount);
	s_V0x080822C.reserve(GetConfig().MeshesCount);

	const auto OptGetMeshType = (int(*)(int, int))0x00485A70;
	const auto OptGetMeshDescriptor = (int(*)(int, int))0x004859B0;

	const int count = *(int*)0x0808160;

	for (int i = 0; i < count; i++)
	{
		s_V0x0808164.data()[i] = OptGetMeshType(0, i);
		s_V0x080822C.data()[i] = OptGetMeshDescriptor(0, i);
	}

	return 0;
}

int ExteriorOptInfoGetDescriptorHook(int* params)
{
	const int meshIndex = params[0];

	return s_V0x080822C.data()[meshIndex];
}

int EscapePodCraftInitHook(int* params)
{
	int pCraft = params[0];
	int meshesCount = GetConfig().MeshesCount;

	memset((char*)(pCraft + GetConfig().Craft_Offset_22E), 0, meshesCount);
	memset((char*)(pCraft + GetConfig().Craft_Offset_260), 0, meshesCount);
	memset((char*)(pCraft + GetConfig().Craft_Offset_292), 0xff, meshesCount);

	return 0;
}

int CurrentCraftInitHook(int* params)
{
	int pCurrentCraft = *(int*)0x00910DFC;
	int meshesCount = GetConfig().MeshesCount;

	memset((char*)(pCurrentCraft + GetConfig().Craft_Offset_22E), 0, meshesCount);
	memset((char*)(pCurrentCraft + GetConfig().Craft_Offset_260), 0, meshesCount);
	memset((char*)(pCurrentCraft + GetConfig().Craft_Offset_292), 0xff, meshesCount);

	return 0;
}

int CraftMeshesFilterHook(int* params)
{
	int pCraft = params[0];
	unsigned char index = (unsigned char)params[1];

	unsigned char* array292 = (unsigned char*)(pCraft + GetConfig().Craft_Offset_292);
	unsigned char value = array292[index];

	params[1] = value == 0 ? 0 : 1;
	return 0;
}
