#include "targetver.h"
#include "get_time.h"
#include <Windows.h>

#pragma comment(lib, "Winmm.lib")

class Timer
{
public:
	Timer()
	{
		timeBeginPeriod(1);
	}

	~Timer()
	{
		timeEndPeriod(1);
	}

	DWORD GetTime()
	{
		return timeGetTime();
	}
};

Timer g_timer;

int L0050E410Hook(int* params)
{
	auto& s_V0x0781E64 = *(DWORD*)0x0781E64;

	Sleep(1);

	DWORD eax = g_timer.GetTime();
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

	Sleep(1);

	DWORD eax = g_timer.GetTime();
	DWORD ecx = s_V0x077D028;

	if (ecx == 0)
	{
		ecx = eax;
	}

	eax = (eax - ecx) / 4;

	s_V0x077D028 = ecx + eax * 4;

	return eax;
}
