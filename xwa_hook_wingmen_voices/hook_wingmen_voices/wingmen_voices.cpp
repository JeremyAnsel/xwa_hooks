#include "targetver.h"
#include "wingmen_voices.h"
#include <string>
#include <vector>

#pragma pack(push, 1)

struct TieFlightGroupEx
{
	char Unk000[3581];
	char PilotVoice[20];
	char UnkE11[49];
};

static_assert(sizeof(TieFlightGroupEx) == 3650, "size of TieFlightGroupEx must be 3650");

struct XwaPlayer
{
	char Unk0000[10];
	short Iff;
	char Unk000C[3011];
};

static_assert(sizeof(XwaPlayer) == 3023, "size of XwaPlayer must be 3023");

#pragma pack(pop)

int WingmenVoicesHook(int* params)
{
	const TieFlightGroupEx* flightGroups = (TieFlightGroupEx*)0x080DC80;
	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x08B94E0;
	int XwaCurrentPlayerId = *(int*)0x08C1CC8;
	int ebx = params[0];
	const char* pilotVoice = flightGroups[ebx].PilotVoice;

	char* fileName = (char*)(params + 52);
	const int fileNameSize = 48;

	for (int i = 1; i <= 12; i++)
	{
		char temp[9];
		sprintf_s(temp, "IP%d", i);

		if (_stricmp(pilotVoice, temp) == 0)
		{
			sprintf_s(fileName, fileNameSize, "ISP%d.LST", i);
			return 0;
		}
	}

	if (XwaPlayers[XwaCurrentPlayerId].Iff == 1)
	{
		strcpy_s((char*)0x05B4068, 13, "ISP%d.LST");
	}
	else
	{
		strcpy_s((char*)0x05B4068, 13, "RSPXWA%d.LST");
	}

	return 1;
}
