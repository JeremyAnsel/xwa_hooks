#include "targetver.h"
#include "exterior.h"

#pragma pack(push, 1)

struct S0x08B94E0_B48
{
	char Unk0000[65];
	short IsExteriorView;
	char Unk0043[10];
};

static_assert(sizeof(S0x08B94E0_B48) == 77, "size of S0x08B94E0_B48 must be 77");

struct XwaPlayer
{
	int ObjectIndex;
	char Unk004[19];
	unsigned char HyperspacePhase;
	char Unk018[13];
	short XwaPlayer_m025;
	char Unk027[61];
	unsigned char IsHudVisible;
	unsigned char XwaPlayer_m065;
	unsigned char XwaPlayer_m066;
	unsigned char IsHudMfd1Visible;
	unsigned char IsHudMfd2Visible;
	unsigned char XwaPlayer_m069;
	unsigned char XwaPlayer_m06A;
	char Unk06B[402];
	short MousePositionX;
	short MousePositionY;
	char Unk201[2375];
	S0x08B94E0_B48 XwaPlayer_mB48;
	char UnkB95[58];
};

static_assert(sizeof(XwaPlayer) == 3023, "size of XwaPlayer must be 3023");

#pragma pack(pop)

int Render3DHook(int* params)
{
	XwaPlayer* xwaPlayers = (XwaPlayer*)0x08B94E0;
	const int currentPlayerId = *(int*)0x08C1CC8;

	// 3d render scene
	((void(*)())0x004EFE00)();

	if (xwaPlayers[currentPlayerId].XwaPlayer_mB48.IsExteriorView == 1)
	{
		xwaPlayers[currentPlayerId].XwaPlayer_mB48.IsExteriorView = 0;

		unsigned char hyper = xwaPlayers[currentPlayerId].HyperspacePhase;
		xwaPlayers[currentPlayerId].HyperspacePhase = 0;

		// force the game to recompute the reticle position
		xwaPlayers[currentPlayerId].MousePositionX = 1;
		xwaPlayers[currentPlayerId].MousePositionY = 1;

		// render MFDs
		((void(*)())0x00465B30)();
		((void(*)())0x00489690)();

		// render MFDs text
		((void(*)())0x0046D610)();

		// render target miniature
		if (xwaPlayers[currentPlayerId].XwaPlayer_m025 != -1)
		{
			short di = *(short*)0x068C5BC;
			short ebx = *(short*)0x068C5C0;
			short esi = *(short*)0x07D4B6C - *(short*)0x068C5C0;
			short cx = *(short*)0x068C520 - *(short*)0x068C5BC / 2;

			((void(*)(int, int, int, int, int))0x00478490)(cx, esi, di, ebx, 0x01);
		}

		xwaPlayers[currentPlayerId].HyperspacePhase = hyper;
		xwaPlayers[currentPlayerId].XwaPlayer_mB48.IsExteriorView = 1;
	}

	return 0;
}

int SetHudVisibilityHook(int* params)
{
	const int callback = params[0];
	const int playerIndex = params[1];
	const unsigned char visibility = (unsigned char)params[2];

	XwaPlayer* xwaPlayers = (XwaPlayer*)0x08B94E0;

	if (callback == 0x0045D2B5 || callback == 0x00500C24 || callback == 0x004603B9)
	{
		xwaPlayers[playerIndex].IsHudVisible = visibility;
	}
	else
	{
		xwaPlayers[playerIndex].IsHudVisible = xwaPlayers[playerIndex].XwaPlayer_m065;
	}

	return 0;
}
