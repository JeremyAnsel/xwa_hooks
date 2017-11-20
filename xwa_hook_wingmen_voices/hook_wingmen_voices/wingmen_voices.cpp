#include "targetver.h"
#include "wingmen_voices.h"
#include <string>

#pragma pack(push, 1)

struct TieFlightGroup
{
	char Unk000[3581];
	char PilotVoice[20];
	char UnkE11[49];
};

static_assert(sizeof(TieFlightGroup) == 3650, "size of TieFlightGroup must be 3650");

#pragma pack(pop)

int WingmenVoicesHook(int* params)
{
	const TieFlightGroup* flightGroups = (TieFlightGroup*)0x080DC80;
	int ebx = params[0];
	const char* pilotVoice = flightGroups[ebx].PilotVoice;

	char* fileName = (char*)(params + 52);
	const int fileNameSize = 48;

	if (_stricmp(pilotVoice, "IP1") == 0)
	{
		strcpy_s(fileName, fileNameSize, "ISP1.LST");
	}
	else if (_stricmp(pilotVoice, "IP2") == 0)
	{
		strcpy_s(fileName, fileNameSize, "ISP2.LST");
	}
	else if (_stricmp(pilotVoice, "IP3") == 0)
	{
		strcpy_s(fileName, fileNameSize, "ISP3.LST");
	}
	else if (_stricmp(pilotVoice, "IP4") == 0)
	{
		strcpy_s(fileName, fileNameSize, "ISP4.LST");
	}
	else if (_stricmp(pilotVoice, "IP5") == 0)
	{
		strcpy_s(fileName, fileNameSize, "ISP5.LST");
	}
	else if (_stricmp(pilotVoice, "IP6") == 0)
	{
		strcpy_s(fileName, fileNameSize, "ISP6.LST");
	}
	else
	{
		strcpy_s(fileName, fileNameSize, "ISP1.LST");
	}

	return 0;
}
