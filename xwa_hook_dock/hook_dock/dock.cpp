#include "targetver.h"
#include "dock.h"

#pragma pack(push, 1)

struct ExeCraftEntry
{
	char Unk0000[606];
	int DockPositionY;
	int DockFromSmallPositionZ;
	int DockFromBigPositionZ;
	int DockToSmallPositionZ;
	int DockToBigPositionZ;
	char unk0272[361];
};

static_assert(sizeof(ExeCraftEntry) == 987, "size of ExeCraftEntry must be 987");

struct XwaCraft
{
	char Unk0000[4];
	short CraftIndex;
	char Unk0006[1011];
};

static_assert(sizeof(XwaCraft) == 1017, "size of XwaCraft must be 1017");

#pragma pack(pop)

int DockReadPositionHook(int* params)
{
	const int A4 = (unsigned short)params[1047];
	const int positionZ = params[5];

	ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;

	ExeCraftTable[A4].DockToSmallPositionZ = positionZ;
	ExeCraftTable[A4].DockToBigPositionZ = positionZ;

	return 0;
}

int AIDockPickUpHook(int* params)
{
	const int A4 = params[0];
	const int A8 = params[1];
	const int AC = params[2];
	const int A10 = params[3];

	const auto L004A2FB0 = (void(*)(int, int, int, int))0x004A2FB0;

	const ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;
	const XwaCraft* s_pXwaCurrentCraft = *(XwaCraft**)0x00910DFC;

	int positionY = A10 - ExeCraftTable[s_pXwaCurrentCraft->CraftIndex].DockPositionY;

	L004A2FB0(A4, A8, AC, positionY);

	return 0;
}

int AIDockPickedUpHook(int* params)
{
	const int A4 = params[0];
	const int A8 = params[1];
	const int AC = params[2];
	const int A10 = params[3];
	const XwaCraft* ebp10 = (XwaCraft*)params[15];

	const auto L004A2FB0 = (void(*)(int, int, int, int))0x004A2FB0;

	const ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;

	int positionY = ExeCraftTable[ebp10->CraftIndex].DockPositionY - A10;

	L004A2FB0(A4, A8, AC, positionY);

	return 0;
}
