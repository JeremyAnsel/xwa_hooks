#include "targetver.h"
#include "get_time.h"
#include "config.h"
#include <Windows.h>
#include <map>
#include <chrono>

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

static bool g_isTimeMultiplicatorEnabled = false;
static bool g_isTimeDivisorEnabled = false;

int GetTimeMultiplicator()
{
	const unsigned short Key_CONTROL_Q = 287;

	static bool isKeyQPressed = false;

	unsigned short key = *(unsigned short*)0x008053C0;

	if (!isKeyQPressed && key == Key_CONTROL_Q)
	{
		isKeyQPressed = true;
	}

	if (isKeyQPressed && key != Key_CONTROL_Q)
	{
		isKeyQPressed = false;
		g_isTimeMultiplicatorEnabled = !g_isTimeMultiplicatorEnabled;
	}

	if (g_isTimeMultiplicatorEnabled)
	{
		return 4;
	}

	return 1;
}

int GetTimeDivisor()
{
	const unsigned short Key_CONTROL_A = 271;

	static bool isKeyAPressed = false;

	unsigned short key = *(unsigned short*)0x008053C0;

	if (!isKeyAPressed && key == Key_CONTROL_A)
	{
		isKeyAPressed = true;
	}

	if (isKeyAPressed && key != Key_CONTROL_A)
	{
		isKeyAPressed = false;
		g_isTimeDivisorEnabled = !g_isTimeDivisorEnabled;
	}

	if (g_isTimeDivisorEnabled)
	{
		return 8;
	}

	return 1;
}

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

	int mul = GetTimeMultiplicator();
	int div = GetTimeDivisor();

	if (mul != 1)
	{
		div = 1;
	}

	DWORD eax = TimeGetTimeHook(nullptr);
	DWORD ecx = s_V0x0781E64;

	if (ecx == 0)
	{
		ecx = eax;
	}

	eax = (eax - ecx) / 4 / div;

	s_V0x0781E64 = ecx + eax * 4 * div;

	eax *= mul;

	return eax;
}

int L0050E430Hook(int* params)
{
	auto& s_V0x077D028 = *(DWORD*)0x077D028;

	Sleep(8);

	DWORD eax = TimeGetTimeHook(nullptr);
	DWORD ecx = s_V0x077D028;

	if (ecx == 0)
	{
		ecx = eax;
	}

	eax = (eax - ecx) / 4;

	s_V0x077D028 = ecx + eax * 4;

	return eax;
}

int RenderHudTimeHook(int* params)
{
	const char* text = (const char*)params[0];

	const auto RenderText = (void(*)(const char*))0x00434FB0;

	std::string str = std::string(text);

	if (g_isTimeMultiplicatorEnabled)
	{
		str += "+";
	}
	else if (g_isTimeDivisorEnabled)
	{
		str += "-";
	}

	RenderText(str.c_str());

	return 0;
}

int HangarCheckInputsHook(int* params)
{
	params[Params_ECX] = *(unsigned char*)0x0080DB68;

	*(unsigned short*)0x008053C0 = 0;
	g_isTimeMultiplicatorEnabled = false;
	g_isTimeDivisorEnabled = false;

	return 0;
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

int TimeGetTimeHook(int* params)
{
	static auto start = std::chrono::high_resolution_clock::now();

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<long long, std::nano> durationNano = end - start;
	std::chrono::milliseconds durationMilli = std::chrono::duration_cast<std::chrono::milliseconds>(durationNano);

	int milli = (int)durationMilli.count();

	return milli;
}
