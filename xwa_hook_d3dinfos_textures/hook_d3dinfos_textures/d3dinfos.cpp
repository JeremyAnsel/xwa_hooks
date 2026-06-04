#include "targetver.h"
#include "d3dinfos.h"
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

class FlightModelsList
{
public:
	FlightModelsList()
	{
		for (const auto& line : GetFileLines("FlightModels\\Spacecraft0.LST"))
		{
			this->_spacecraftList.push_back(GetStringWithoutExtension(line));
		}

		for (const auto& line : GetFileLines("FlightModels\\Equipment0.LST"))
		{
			this->_equipmentList.push_back(GetStringWithoutExtension(line));
		}
	}

	std::string GetLstLine(int modelIndex)
	{
		const int xwaObjectStats = 0x05FB240;
		const int dataIndex1 = *(short*)(xwaObjectStats + modelIndex * 0x18 + 0x14);
		const int dataIndex2 = *(short*)(xwaObjectStats + modelIndex * 0x18 + 0x16);

		switch (dataIndex1)
		{
		case 0:
			if ((unsigned int)dataIndex2 < this->_spacecraftList.size())
			{
				return this->_spacecraftList[dataIndex2];
			}

			break;

		case 1:
			if ((unsigned int)dataIndex2 < this->_equipmentList.size())
			{
				return this->_equipmentList[dataIndex2];
			}

			break;
		}

		return std::string();
	}

private:
	std::vector<std::string> _spacecraftList;
	std::vector<std::string> _equipmentList;
};

FlightModelsList g_flightModelsList;

#pragma pack(push, 1)

enum ShipCategoryEnum : unsigned char
{
	ShipCategory_Starfighter = 0,
	ShipCategory_Transport = 1,
	ShipCategory_UtilityVehicle = 2,
	ShipCategory_Freighter = 3,
	ShipCategory_Starship = 4,
	ShipCategory_Platform = 5,
	ShipCategory_PlayerProjectile = 6,
	ShipCategory_OtherProjectile = 7,
	ShipCategory_Mine = 8,
	ShipCategory_Satellite = 9,
	ShipCategory_NormalDebris = 10,
	ShipCategory_SmallDebris = 11,
	ShipCategory_Backdrop = 12,
	ShipCategory_Explosion = 13,
	ShipCategory_Obstacle = 14,
	ShipCategory_DeathStarII = 15,
	ShipCategory_People = 16,
	ShipCategory_Container = 17,
	ShipCategory_Droid = 18,
	ShipCategory_Armament = 19,
	ShipCategory_LargeDebris = 20,
	ShipCategory_SalvageYard = 21,
};

struct XwaD3DInfo
{
	char Unk0000[35];
	char ColorMap[24];
	char LightMap[24];
	XwaD3DInfo* pNext;
	XwaD3DInfo* pPrevious;
};

static_assert(sizeof(XwaD3DInfo) == 91, "size of XwaD3DInfo must be 91");

struct XwaCraft
{
	char Unk0000[389];
	unsigned short m185; // flags
	char Unk0187[626];
};

static_assert(sizeof(XwaCraft) == 1017, "size of XwaCraft must be 1017");

struct XwaMobileObject
{
	char Unk0000[221];
	XwaCraft* pCraft;
	char Unk00E1[4];
};

static_assert(sizeof(XwaMobileObject) == 229, "size of XwaMobileObject must be 229");

struct XwaObject
{
	char unk00[2];
	unsigned short ModelIndex;
	ShipCategoryEnum ShipCategory;
	char unk05[30];
	XwaMobileObject* pMobileObject;
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

struct SceneCompData
{
	XwaObject* pObject;
	char unk04[180];
	XwaD3DInfo* D3DInfo;
	char unkBC[96];
};

static_assert(sizeof(SceneCompData) == 284, "size of SceneCompData must be 284");

struct XwaVisibleObject
{
	int m00;
	int ObjectIndex;
	XwaVisibleObject* pNext;
	int m0C;
	int m10;
	int m14;
	int m18;
	int m1C;
};

static_assert(sizeof(XwaVisibleObject) == 32, "size of XwaVisibleObject must be 32");

#pragma pack(pop)

class D3DInfoArray
{
public:
	D3DInfoArray()
	{
		auto lines = GetFileLines("hook_d3dinfos_textures.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_d3dinfos_textures");
		}

		int count = abs(GetFileKeyValueInt(lines, "Size"));

		if (count == 0)
		{
			count = 10000;
		}

		this->_d3dinfos.reserve(count);
	}

	int Size()
	{
		return this->_d3dinfos.capacity();
	}

	XwaD3DInfo* Data()
	{
		return this->_d3dinfos.data();
	}

private:
	std::vector<XwaD3DInfo> _d3dinfos;
};

D3DInfoArray g_XwaD3DInfos;

int InitD3DInfosHook(int* params)
{
	auto& XwaD3DInfosFirstEmpty = *(XwaD3DInfo**)0x00686B24;
	auto& XwaD3DInfosFirstUsed = *(XwaD3DInfo**)0x00686B28;
	auto& XwaD3DInfosCount = *(int*)0x00686B2C;

	int size = g_XwaD3DInfos.Size();
	XwaD3DInfo* data = g_XwaD3DInfos.Data();

	for (int i = 0; i < size - 1; i++)
	{
		data[i] = {};
		data[i].pNext = &data[i + 1];
	}

	data[size - 1] = {};

	XwaD3DInfosFirstEmpty = &data[0];
	XwaD3DInfosFirstUsed = nullptr;
	XwaD3DInfosCount = 0;

	return 0;
}

static bool IsInTechLibraryOrBriefing()
{
	int currentGameState = *(int*)(0x09F60E0 + 0x25FA9);
	int updateCallback = *(int*)(0x09F60E0 + 0x25FB1 + currentGameState * 0x850 + 0x0844);

	int XwaTechLibraryGameStateUpdate = 0x00574D70;
	bool isInTechLibrary = updateCallback == XwaTechLibraryGameStateUpdate;

	bool isBriefingIsPlaying = *(short*)0x009EB826 != 0;

	return isInTechLibrary || isBriefingIsPlaying;
}

int TestTextureIlluminationHook(int* params)
{
	const int esp14 = params[5];
	const SceneCompData* A4 = (SceneCompData*)params[18];

	bool jump = false;

	if (esp14 == 0)
	{
		jump = true;
	}
	else if (!IsInTechLibraryOrBriefing() && A4->pObject != nullptr && A4->pObject->pMobileObject != nullptr && A4->pObject->pMobileObject->pCraft != nullptr)
	{
		const XwaCraft* craft = A4->pObject->pMobileObject->pCraft;

		if (craft->m185 == 0)
		{
			jump = true;
		}
	}

	if (jump)
	{
		params[-1] = 0x443B9A;
	}

	return 0;
}

int RenderOptNodeHook(int* params)
{
	static XwaD3DInfo s_d3dInfos;

	SceneCompData* AC = (SceneCompData*)params[Params_ESI];

	bool jump = false;

	if (!IsInTechLibraryOrBriefing() && AC->pObject != nullptr && AC->pObject->pMobileObject != nullptr && AC->pObject->pMobileObject->pCraft != nullptr)
	{
		const XwaCraft* craft = AC->pObject->pMobileObject->pCraft;

		if (craft->m185 == 0)
		{
			jump = true;
		}
	}

	if (AC->D3DInfo && jump)
	{
		s_d3dInfos = *AC->D3DInfo;
		memset(s_d3dInfos.LightMap, 0, sizeof(s_d3dInfos.LightMap));
		AC->D3DInfo = &s_d3dInfos;
	}

	params[Params_ECX] = *(int*)(params[Params_ESI] + 0xA8);

	return 0;
}

int AddVisibleObjectToListHook(int* params)
{
	*(unsigned char*)0x0049F730 = 0xC3; // ret

	params[Params_EAX] = *(int*)0x007CA350;

	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	int& s_XwaVisibleObjectsListLength = *(int*)0x007CA350;
	XwaVisibleObject* s_XwaVisibleObjects = *(XwaVisibleObject**)0x008C1638;
	XwaVisibleObject*& s_XwaVisibleObjectsList = *(XwaVisibleObject**)0x007D4B74;

	int A4 = params[1];
	int A8 = params[2];
	int AC = params[3];
	int A10 = params[4];
	int A14 = params[5];
	int A18 = params[6];
	int A1C = params[7];

	if (s_XwaVisibleObjectsListLength < 0x680)
	{
		s_XwaVisibleObjects[s_XwaVisibleObjectsListLength].ObjectIndex = A4;
		s_XwaVisibleObjects[s_XwaVisibleObjectsListLength].m00 = A8;
		s_XwaVisibleObjects[s_XwaVisibleObjectsListLength].m0C = AC;
		s_XwaVisibleObjects[s_XwaVisibleObjectsListLength].m10 = A10;
		s_XwaVisibleObjects[s_XwaVisibleObjectsListLength].m14 = A14;
		s_XwaVisibleObjects[s_XwaVisibleObjectsListLength].m18 = A18;
		s_XwaVisibleObjects[s_XwaVisibleObjectsListLength].m1C = A1C;

		//s_XwaVisibleObjects[s_XwaVisibleObjectsListLength].pNext = s_XwaVisibleObjectsList;
		//s_XwaVisibleObjectsList = &s_XwaVisibleObjects[s_XwaVisibleObjectsListLength];

		s_XwaVisibleObjects[s_XwaVisibleObjectsListLength].pNext = 0;

		if (s_XwaVisibleObjectsListLength > 0)
		{
			s_XwaVisibleObjects[s_XwaVisibleObjectsListLength - 1].pNext = &s_XwaVisibleObjects[s_XwaVisibleObjectsListLength];
		}

		s_XwaVisibleObjectsList = &s_XwaVisibleObjects[0];

		s_XwaVisibleObjectsListLength++;
	}

	params[Params_ReturnAddress] = 0x0049F53A;
	return 0;
}
