#include "targetver.h"
#include "get_time.h"
#include "config.h"
#include <Windows.h>
#include <map>

#pragma comment(lib, "Winmm.lib")

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

class Config
{
public:
	Config()
	{
		auto lines = GetFileLines("hook_time.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_time");
		}

		this->ExplosionAnimationTimeFrame = GetFileKeyValueInt(lines, "ExplosionAnimationTimeFrame", 60);
		this->SmallDebrisAnimationTimeFrame = GetFileKeyValueInt(lines, "SmallDebrisAnimationTimeFrame", 60);
	}

	int ExplosionAnimationTimeFrame;
	int SmallDebrisAnimationTimeFrame;
};

Config g_config;

class ObjectIndexTime
{
public:
	int Get(int objectIndex)
	{
		auto it = this->_time.find(objectIndex);

		if (it != this->_time.end())
		{
			return it->second;
		}
		else
		{
			int value = 0;
			this->_time.insert(std::make_pair(objectIndex, value));
			return value;
		}
	}

	void Set(int objectIndex, int value)
	{
		auto it = this->_time.find(objectIndex);

		if (it != this->_time.end())
		{
			it->second = value;
		}
		else
		{
			this->_time.insert(std::make_pair(objectIndex, value));
		}
	}

	int RetrieveTimeSpeed(int timeFrame, int objectIndex, int elapsedTime)
	{
		int time = this->Get(objectIndex);
		time += elapsedTime;

		int timeSpeed = 0;
		if (time >= timeFrame)
		{
			timeSpeed = 1;
			time = time % timeFrame;
		}

		this->Set(objectIndex, time);

		return timeSpeed;
	}

private:
	std::map<int, int> _time;
};

ObjectIndexTime g_nullMobileObjectObjectIndexTime;
ObjectIndexTime g_shipObjectIndexTime;
ObjectIndexTime g_explosionObjectIndexTime;

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

int AsteroidsAnimationHook(int* params)
{
	const short elapsedTime = *(short*)0x08C1640;

	params[Params_EAX] *= elapsedTime;
	return 0;
}

int NullMobileObjectAnimationHook(int* params)
{
	const short dx = (short)params[Params_EDX];
	const int objectIndex = params[Params_EBX];
	const short elapsedTime = *(short*)0x08C1640;

	if (dx == 0)
	{
		params[Params_ReturnAddress] = 0x0040270E;
		return 0;
	}

	int timeSpeed = g_nullMobileObjectObjectIndexTime.RetrieveTimeSpeed(60 / 4, objectIndex, elapsedTime);

	if (timeSpeed == 0)
	{
		params[Params_ReturnAddress] = 0x0040270E;
		return 0;
	}

	return 0;
}

int ShipAnimationHook(int* params)
{
	const int objectIndex = params[Params_EBX];
	const short elapsedTime = *(short*)0x08C1640;

	params[Params_EAX] &= 0xffff;

	int timeSpeed = g_shipObjectIndexTime.RetrieveTimeSpeed(60 / 4, objectIndex, elapsedTime);

	if (timeSpeed == 0)
	{
		params[Params_ReturnAddress] = 0x0040270E;
		return 0;
	}

	return 0;
}

int ExplosionAnimationHook(int* params)
{
	const int ShipCategory_SmallDebris = 11;
	const int ShipCategory_Explosion = 13;

	const int shipCategory = params[Params_EAX];
	const int objectIndex = params[Params_EBX];
	const short elapsedTime = *(short*)0x08C1640;

	int timeFrame;

	switch (shipCategory)
	{
	case ShipCategory_SmallDebris:
		timeFrame = g_config.SmallDebrisAnimationTimeFrame;
		break;

	case ShipCategory_Explosion:
		timeFrame = g_config.ExplosionAnimationTimeFrame;
		break;

	default:
		timeFrame = 60;
		break;
	}

	int timeSpeed = g_explosionObjectIndexTime.RetrieveTimeSpeed(timeFrame / 4, objectIndex, elapsedTime);

	if (timeSpeed == 0)
	{
		params[Params_ReturnAddress] = 0x0040270E;
		return 0;
	}

	const int ebp = params[Params_EBP];
	const short ebp04 = *(short*)(ebp - 0x04);

	if (ebp04 != 0x00DE)
	{
		params[Params_ReturnAddress] = 0x0040256D;
		return 0;
	}

	return 0;
}
