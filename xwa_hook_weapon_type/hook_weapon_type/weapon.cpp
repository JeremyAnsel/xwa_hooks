#include "targetver.h"
#include "weapon.h"
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

class CraftConfig
{
public:
	CraftConfig()
	{
		auto lines = GetFileLines("hook_opt_limit.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_opt_limit");
		}

		this->MeshesCount = GetFileKeyValueInt(lines, "MeshesCount", 0);
		this->Craft_Size = 0x3F9 + GetFileKeyValueInt(lines, "Craft_ExtraSize", 0);
		this->Craft_Offset_22E = 0x3F9 + GetFileKeyValueInt(lines, "Craft_Offset_22E", 0);
		this->Craft_Offset_260 = 0x3F9 + GetFileKeyValueInt(lines, "Craft_Offset_260", 0);
		this->Craft_Offset_292 = 0x3F9 + GetFileKeyValueInt(lines, "Craft_Offset_292", 0);
		this->Craft_Offset_2CF = 0x3F9 + GetFileKeyValueInt(lines, "Craft_Offset_2CF", 0);
		this->Craft_Offset_2DF = 0x3F9 + GetFileKeyValueInt(lines, "Craft_Offset_2DF", 0x2DF - 0x3F9);
	}

	int MeshesCount;
	int Craft_Size;
	int Craft_Offset_22E;
	int Craft_Offset_260;
	int Craft_Offset_292;
	int Craft_Offset_2CF;
	int Craft_Offset_2DF;
};

CraftConfig g_craftConfig;

#pragma pack(push, 1)

struct XwaCraftWeaponRack
{
	char Unk0000[5];
	unsigned char Count;
	char Unk0006[8];
};

static_assert(sizeof(XwaCraftWeaponRack) == 14, "size of XwaCraftWeaponRack must be 14");

struct XwaCraft
{
	char Unk0000[4];
	unsigned short CraftIndex;
	char Unk0006[422];
	unsigned char LasersCount;
	char Unk01AD[36];
	unsigned char WarheadsCount;
	unsigned short WarheadsModelIndex[2];
	char Unk01D6[265];
	XwaCraftWeaponRack WeaponRacks[16];
	char Unk03BF[58];
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
	char Unk0000[2];
	short ModelIndex;
	unsigned char ShipCategory;
	unsigned char TieFlightGroupIndex;
	unsigned char Region;
	int PositionX;
	int PositionY;
	int PositionZ;
	short HeadingXY;
	short HeadingZ;
	short HeadingRoll;
	short XwaObject_m19;
	char Unk001B[4];
	int PlayerIndex;
	XwaMobileObject* pMobileObject;
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

struct ExeCraftEntry
{
	char Unk0000[342];
	unsigned short WarheadTypeId[2];
	unsigned char WarheadStartRack[2];
	unsigned char WarheadEndRack[2];
	char Unk015E[637];
};

static_assert(sizeof(ExeCraftEntry) == 987, "size of ExeCraftEntry must be 987");

struct ExeObjectEntry
{
	unsigned char EnableOptions; // flags
	unsigned char RessourceOptions; // flags
	unsigned char ObjectCategory;
	unsigned char ShipCategory;
	unsigned int ObjectSize;
	void* pData1;
	void* pData2;
	unsigned short GameOptions; // flags
	unsigned short CraftIndex;
	short DataIndex1;
	short DataIndex2;
};

static_assert(sizeof(ExeObjectEntry) == 24, "size of ExeEnableEntry must be 24");

struct XwaPlayer
{
	char Unk0000[51];
	unsigned char XwaPlayer_m033;
	unsigned char XwaPlayer_m034;
	char Unk0035[2970];
};

static_assert(sizeof(XwaPlayer) == 3023, "size of XwaPlayer must be 3023");

#pragma pack(pop)

int SelectedWarheadMenuHook(int* params)
{
	const int s_pXwaCurrentCraft = *(int*)0x00910DFC;
	const int* s_V0x068BC28 = (int*)0x068BC28;

	int index = s_V0x068BC28[0] == 0x04 ? 0 : 1;

	params[Params_ECX] = s_pXwaCurrentCraft + index * 0x02;

	return 0;
}

int WarheadCountMenuHook(int* params)
{
	XwaCraft* s_pXwaCurrentCraft = *(XwaCraft**)0x00910DFC;

	unsigned char count = 0;

	if (s_pXwaCurrentCraft->WarheadsModelIndex[0] != 0)
	{
		count++;

		if (s_pXwaCurrentCraft->WarheadsModelIndex[1] != 0)
		{
			count++;
		}
	}

	s_pXwaCurrentCraft->WarheadsCount = count;

	params[Params_EAX] = (int)s_pXwaCurrentCraft;

	return 0;
}

int GetWarheadType(unsigned short modelIndex)
{
	const unsigned short* s_XwaTieWarheadTypeToModelIndex = (unsigned short*)0x005B12A8;

	int type = 0;

	for (int i = 0; i < 11; i++)
	{
		if (modelIndex == s_XwaTieWarheadTypeToModelIndex[i])
		{
			type = i;
			break;
		}
	}

	return type;
}

int BoardReloadTypeHook(int* params)
{
	const unsigned short warheadIndex = (unsigned short)params[8];
	const XwaCraft* currentCraft = (XwaCraft*)params[4];

	unsigned short modelIndex = currentCraft->WarheadsModelIndex[warheadIndex];

	int type = GetWarheadType(modelIndex);

	params[Params_ESI] = type;

	return 0;
}

int GetWarheadsCount(const XwaCraft* pCraft, int warheadIndex)
{
	const ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;

	const XwaCraftWeaponRack* weaponRacks = (XwaCraftWeaponRack*)((int)pCraft + g_craftConfig.Craft_Offset_2DF);

	unsigned short craftIndex = pCraft->CraftIndex;
	int count = 0;

	if (warheadIndex < pCraft->WarheadsCount)
	{
		unsigned char startRack = ExeCraftTable[craftIndex].WarheadStartRack[warheadIndex];
		unsigned char endRack = ExeCraftTable[craftIndex].WarheadEndRack[warheadIndex];

		for (int rack = startRack; rack <= endRack; rack++)
		{
			count += weaponRacks[rack].Count;
		}
	}

	return count;
}

int WeaponSwitchWarheadCountHook(int* params)
{
	const XwaCraft* pCraft = (XwaCraft*)params[Params_EBX];
	const int playerIndex = params[Params_EDI];

	XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	XwaPlayer* pPlayer = &XwaPlayers[playerIndex];

	if (pPlayer->XwaPlayer_m033 >= pCraft->WarheadsCount || GetWarheadsCount(pCraft, 1) == 0)
	{
		if (pCraft->LasersCount != 0)
		{
			pPlayer->XwaPlayer_m034 = 0;
		}

		pPlayer->XwaPlayer_m033 = 0;
	}

	return 0;
}

int WeaponSwitchSetIndexHook(int* params)
{
	const XwaCraft* pCraft = (XwaCraft*)params[Params_EBX];
	const int playerIndex = params[Params_EDI];

	XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	XwaPlayer* pPlayer = &XwaPlayers[playerIndex];

	pPlayer->XwaPlayer_m033 = 0;

	if (pPlayer->XwaPlayer_m034 == 1)
	{
		if (pCraft->WarheadsCount > 1 && GetWarheadsCount(pCraft, 0) == 0)
		{
			pPlayer->XwaPlayer_m033 = 1;
		}
	}

	return 0;
}

int SecondaryWeaponsSelectionHook(int* params)
{
	ExeObjectEntry* ExeEnableTable = (ExeObjectEntry*)0x005FB240;
	ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;

	XwaObject* pObject = (XwaObject*)params[Params_EDX];
	unsigned short modelIndex = pObject->ModelIndex;
	unsigned short craftIndex = ExeEnableTable[modelIndex].CraftIndex;

	if (ExeCraftTable[craftIndex].WarheadTypeId[1] == 0)
	{
		params[Params_ReturnAddress] = 0x00461386;
	}

	return 0;
}
