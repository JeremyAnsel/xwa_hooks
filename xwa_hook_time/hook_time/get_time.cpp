#include "targetver.h"
#include "get_time.h"
#include <Windows.h>

#pragma comment(lib, "Winmm.lib")

static int loopSources[] =
{
	0x00458C0F,
	0x004EBD4E,
	0x004ECCA1,
	0x004EDCFE,
	0x004EDF88,
	0x004EE373,
	0x0051069C,
	0x005107DC,
	0x00510898,
	0x00510CD6,
};

int L0050E410Hook(int* params)
{
	auto& s_V0x0781E64 = *(DWORD*)0x0781E64;

	int returnOffset = params[0];
	bool fromLoop = false;

	for (int from : loopSources)
	{
		if (returnOffset == from)
		{
			fromLoop = true;
			break;
		}

	}

	if (fromLoop)
	{
		Sleep(8);
	}

	DWORD eax = timeGetTime();
	DWORD ecx = s_V0x0781E64;

	if (ecx == 0)
	{
		ecx = eax;
	}

	eax = (eax - ecx) / 4;

	s_V0x0781E64 = ecx + eax * 4;

	return eax;
}

int L0050E430Hook(int* params)
{
	auto& s_V0x077D028 = *(DWORD*)0x077D028;

	Sleep(8);

	DWORD eax = timeGetTime();
	DWORD ecx = s_V0x077D028;

	if (ecx == 0)
	{
		ecx = eax;
	}

	eax = (eax - ecx) / 4;

	s_V0x077D028 = ecx + eax * 4;

	return eax;
}
