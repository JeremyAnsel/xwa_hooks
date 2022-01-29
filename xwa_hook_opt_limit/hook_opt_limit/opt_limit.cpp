#include "targetver.h"
#include "opt_limit.h"
#include "config.h"

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
		auto lines = GetFileLines("hook_opt_limit.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_opt_limit");
		}

		this->MeshesCount = GetFileKeyValueInt(lines, "MeshesCount", 255);
		this->EnginesCount = 255;
		// 22E (MeshesCount) + 260 (MeshesCount) + 292 (MeshesCount) + 2CF (EnginesCount)
		this->Craft_Size = 0x3F9 + GetFileKeyValueInt(lines, "Craft_ExtraSize", this->MeshesCount * 3 + this->EnginesCount);
		this->Craft_Offset_22E = 0x3F9 + GetFileKeyValueInt(lines, "Craft_Offset_22E", 0);
		this->Craft_Offset_260 = 0x3F9 + GetFileKeyValueInt(lines, "Craft_Offset_260", this->MeshesCount * 1);
		this->Craft_Offset_292 = 0x3F9 + GetFileKeyValueInt(lines, "Craft_Offset_292", this->MeshesCount * 2);
		this->Craft_Offset_2CF = 0x3F9 + GetFileKeyValueInt(lines, "Craft_Offset_2CF", this->MeshesCount * 3);

		if (this->MeshesCount == 0)
		{
			this->MeshesCount = 50;
			this->EnginesCount = 16;
			this->Craft_Size = 0;
			this->Craft_Offset_22E = 0x22E;
			this->Craft_Offset_260 = 0x260;
			this->Craft_Offset_292 = 0x292;
			this->Craft_Offset_2CF = 0x2CF;
		}
	}

	int MeshesCount;
	int EnginesCount;
	int Craft_Size;
	int Craft_Offset_22E;
	int Craft_Offset_260;
	int Craft_Offset_292;
	int Craft_Offset_2CF;
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
		auto lines = GetFileLines("hook_opt_limit.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_opt_limit");
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

int GetCraftEnginesCount()
{
	return GetConfig().EnginesCount;
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

int GetCraftOffset_2CF()
{
	return GetConfig().Craft_Offset_2CF;
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

int GetExeCraftEnginesArrayPtr()
{
	static bool init = true;
	static std::vector<void*> s_engines;

	if (init)
	{
		init = false;
		s_engines.reserve(GetCraftEnginesCount());
	}

	return (int)s_engines.data();
}

int GetExeCraftEngineMeshIdsArrayPtr()
{
	static bool init = true;
	static std::vector<unsigned char> s_engineMeshIds;

	if (init)
	{
		init = false;
		s_engineMeshIds.reserve(GetCraftEnginesCount());
	}

	return (int)s_engineMeshIds.data();
}

int GetSpaceBombEnginesArrayPtr()
{
	static bool init = true;
	static std::vector<void*> s_engines;

	if (init)
	{
		init = false;
		s_engines.reserve(GetCraftEnginesCount());
	}

	return (int)s_engines.data();
}

int GetSpaceBombEngineMeshIdsArrayPtr()
{
	static bool init = true;
	static std::vector<unsigned char> s_engineMeshIds;

	if (init)
	{
		init = false;
		s_engineMeshIds.reserve(GetCraftEnginesCount());
	}

	return (int)s_engineMeshIds.data();
}

int GetCockpitEnginesArrayPtr()
{
	static bool init = true;
	static std::vector<void*> s_engines;

	if (init)
	{
		init = false;
		s_engines.reserve(GetCraftEnginesCount());
	}

	return (int)s_engines.data();
}

int GetCockpitEngineMeshIdsArrayPtr()
{
	static bool init = true;
	static std::vector<unsigned char> s_engineMeshIds;

	if (init)
	{
		init = false;
		s_engineMeshIds.reserve(GetCraftEnginesCount());
	}

	return (int)s_engineMeshIds.data();
}

int GetExteriorEnginesArrayPtr()
{
	static bool init = true;
	static std::vector<void*> s_engines;

	if (init)
	{
		init = false;
		s_engines.reserve(GetCraftEnginesCount());
	}

	return (int)s_engines.data();
}

int GetExteriorEngineMeshIdsArrayPtr()
{
	static bool init = true;
	static std::vector<unsigned char> s_engineMeshIds;

	if (init)
	{
		init = false;
		s_engineMeshIds.reserve(GetCraftEnginesCount());
	}

	return (int)s_engineMeshIds.data();
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

int CraftZeroMemoryHook(int* params)
{
	XwaMobileObject* pMobileObject = (XwaMobileObject*)params[0];
	int pCraft = pMobileObject->pCraft;

	int pObject = *(int*)(pCraft + 0x3F5);

	memset((void*)pCraft, 0, GetConfig().Craft_Size);

	*(int*)(pCraft + 0x3F5) = pObject;

	return 0;
}

int OptGetMeshEngineGlowCountExeCraftHook(int* params)
{
	const int A4 = params[0];
	const int A8 = params[1];

	const auto XwaOptGetMeshEngineGlowCount = (int(*)(int, int))0x00488680;

	int maxCount = GetCraftEnginesCount();
	int currentCount = *(unsigned char*)(0x005BB6B2 + params[Params_ESI]);
	int count = XwaOptGetMeshEngineGlowCount(A4, A8);

	if (currentCount + count > maxCount)
	{
		count = maxCount - currentCount;
	}

	return count;
}

int OptGetMeshEngineGlowCountSpaceBombHook(int* params)
{
	const int A4 = params[0];
	const int A8 = params[1];

	const auto XwaOptGetMeshEngineGlowCount = (int(*)(int, int))0x00488680;

	int maxCount = GetCraftEnginesCount();
	int currentCount = *(unsigned char*)0x008D57B0;
	int count = XwaOptGetMeshEngineGlowCount(A4, A8);

	if (currentCount + count > maxCount)
	{
		count = maxCount - currentCount;
	}

	return count;
}

int OptGetMeshEngineGlowCountCockpitHook(int* params)
{
	const int A4 = params[0];
	const int A8 = params[1];

	const auto XwaOptGetMeshEngineGlowCount = (int(*)(int, int))0x00488680;

	int maxCount = GetCraftEnginesCount();
	int currentCount = *(unsigned char*)0x007FFBD0;
	int count = XwaOptGetMeshEngineGlowCount(A4, A8);

	if (currentCount + count > maxCount)
	{
		count = maxCount - currentCount;
	}

	return count;
}

int OptGetMeshEngineGlowCountExteriorHook(int* params)
{
	const int A4 = params[0];
	const int A8 = params[1];

	const auto XwaOptGetMeshEngineGlowCount = (int(*)(int, int))0x00488680;

	int maxCount = GetCraftEnginesCount();
	int currentCount = *(unsigned char*)0x009109B0;
	int count = XwaOptGetMeshEngineGlowCount(A4, A8);

	if (currentCount + count > maxCount)
	{
		count = maxCount - currentCount;
	}

	return count;
}

int L004E28F0_GetMeshHook(int* params)
{
	const int A4 = params[0];
	const int A8 = params[1];

	const auto L00488A50 = (int(*)(int, int))0x00488A50;

	const int* ebp = (int*)params[Params_EBP];
	int pCraft = ebp[-12];
	int meshIndex = params[Params_ESI];
	unsigned char* pCraft_292 = (unsigned char*)(pCraft + GetCraftOffset_292());

	if (pCraft_292[meshIndex] == 0)
	{
		return 0;
	}

	int result = L00488A50(A4, A8);

	return result;
}

int L0047AAA0_SetCraftHook(int* params)
{
	const XwaMobileObject* pMobileObject = (XwaMobileObject*)params[Params_EDX];
	int pCraft = pMobileObject->pCraft;
	params[Params_EBX] = pCraft;

	if (pCraft == 0)
	{
		return 0;
	}

	int meshIndex = *(short*)(0x008B9515 + params[Params_ECX]);
	unsigned char* pCraft_292 = (unsigned char*)(pCraft + GetCraftOffset_292());

	if (meshIndex != -1 && pCraft_292[meshIndex] == 0)
	{
		params[Params_EBX] = 0;
	}

	return 0;
}

int L004E0E10_GetEnginesCountHook(int* params)
{
	params[Params_EAX] = *(unsigned char*)(0x005BB6B2 + params[Params_EDI]);

	int meshIndex = (short)params[Params_EBP];
	int pCraft = *(int*)0x00910DFC;
	unsigned char* pCraft_292 = (unsigned char*)(pCraft + GetCraftOffset_292());

	if (meshIndex != -1 && pCraft_292[meshIndex] == 0)
	{
		params[Params_EAX] = 0;
	}

	return 0;
}

int L004E0FA0_ComputePercentOfActiveEnginesHook(int* params)
{
	unsigned char* exeCraftEngineMeshIds = (unsigned char*)GetExeCraftEngineMeshIdsArrayPtr();
	int enginesCount = params[Params_ECX];
	int pCraft = params[Params_ESI];
	unsigned char* pCraft_292 = (unsigned char*)(pCraft + GetCraftOffset_292());
	unsigned char* pCraft_2CF = (unsigned char*)(pCraft + GetCraftOffset_2CF());

	int totalCount = 0;
	int inactiveCount = 0;

	for (int index = 0; index < enginesCount; index++)
	{
		int meshIndex = exeCraftEngineMeshIds[index];

		if (pCraft_292[meshIndex] == 0)
		{
			continue;
		}

		totalCount++;

		if (pCraft_2CF[index] == 0)
		{
			inactiveCount++;
		}
	}

	params[Params_ECX] = totalCount;
	params[Params_EDX] = inactiveCount;

	return 0;
}

int L004F22B0_EngineGlowIsDisabledHook(int* params)
{
	XwaMobileObject* pMobileObject = (XwaMobileObject*)params[Params_ECX];
	int pCraft = pMobileObject->pCraft;

	params[Params_EAX] = pCraft;

	int meshIndex = (unsigned char)params[6];
	unsigned char* pCraft_292 = (unsigned char*)(pCraft + GetCraftOffset_292());

	if (pCraft_292[meshIndex] == 0)
	{
		params[Params_ReturnAddress] = 0x004F2E31;
	}

	return 0;
}

int L0042D590_EngineGlowIsDisabledHook(int* params)
{
	int meshIndex = (unsigned char)params[9];
	int pCraft = *(int*)0x00910DFC;
	unsigned char* pCraft_292 = (unsigned char*)(pCraft + GetCraftOffset_292());

	int ebx = params[Params_EBX];

	if (pCraft_292[meshIndex] == 0 || *(int*)ebx != 0)
	{
		params[Params_ReturnAddress] = 0x0042D922;
	}

	return 0;
}
