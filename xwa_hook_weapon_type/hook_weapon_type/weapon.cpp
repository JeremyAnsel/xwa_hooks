#include "targetver.h"
#include "weapon.h"

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

struct ExeCraftEntry
{
	char Unk0000[346];
	unsigned char WarheadStartRack[2];
	unsigned char WarheadEndRack[2];
	char Unk015E[637];
};

static_assert(sizeof(ExeCraftEntry) == 987, "size of ExeCraftEntry must be 987");

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

	unsigned short craftIndex = pCraft->CraftIndex;
	int count = 0;

	if (warheadIndex < pCraft->WarheadsCount)
	{
		unsigned char startRack = ExeCraftTable[craftIndex].WarheadStartRack[warheadIndex];
		unsigned char endRack = ExeCraftTable[craftIndex].WarheadEndRack[warheadIndex];

		for (int rack = startRack; rack <= endRack; rack++)
		{
			count += pCraft->WeaponRacks[rack].Count;
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
