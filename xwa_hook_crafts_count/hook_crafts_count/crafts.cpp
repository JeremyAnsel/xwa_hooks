#include "targetver.h"
#include "crafts.h"
#include "config.h"

#include <Windows.h>

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
		auto lines = GetFileLines("hook_crafts_count.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_crafts_count");
		}

		this->CraftsCount = GetFileKeyValueInt(lines, "CraftsCount", 3072);
		this->ProjectilesCount = GetFileKeyValueInt(lines, "ProjectilesCount", 1024);
		this->ExplosionsCount = GetFileKeyValueInt(lines, "ExplosionsCount", 256);
		this->Radar2DItemsCount = GetFileKeyValueInt(lines, "Radar2DItemsCount", 48);

		OutputDebugString(("Hook_Crafts_Count CraftsCount=" + std::to_string(this->CraftsCount)).c_str());
		OutputDebugString(("Hook_Crafts_Count ProjectilesCount=" + std::to_string(this->ProjectilesCount)).c_str());
		OutputDebugString(("Hook_Crafts_Count ExplosionsCount=" + std::to_string(this->ExplosionsCount)).c_str());
		OutputDebugString(("Hook_Crafts_Count Radar2DItemsCount=" + std::to_string(this->Radar2DItemsCount)).c_str());
	}

	int CraftsCount;
	int ProjectilesCount;
	int ExplosionsCount;
	int Radar2DItemsCount;
};

Config g_config;

std::vector<unsigned char> g_aiHandlingArray;
std::vector<int> g_officerSoundsBuffer;

int CraftsCountHook(int* params)
{
	const int regionsCount = params[0];

	*(int*)0x007D4B94 = g_config.CraftsCount;
	*(int*)0x008C1CE4 = g_config.CraftsCount * regionsCount;

	*(int*)0x008BF35C = 0;
	*(int*)0x00917E50 = 0;

	params[0] = *(int*)0x008C1CE4;

	return 0;
}

int ProjectilesCountHook(int* params)
{
	const int s_XwaNetworkPlayersCount = *(int*)0x0910DEC;

	int& s_V0x07D4B80 = *(int*)0x07D4B80;
	int& s_V0x07D4C58 = *(int*)0x07D4C58;

	s_V0x07D4B80 = s_XwaNetworkPlayersCount * 12;
	s_V0x07D4C58 = s_XwaNetworkPlayersCount * 16;

	return s_V0x07D4B80 + s_V0x07D4C58 + g_config.ProjectilesCount;
}

int ExplosionsCountHook(int* params)
{
	const int regionsCount = *(int*)0x08D9624;

	return g_config.ExplosionsCount * regionsCount;
}

int AllocObjectsHook(int* params)
{
	int s_XwaObjectsCount = *(int*)0x007FFD80;

	g_aiHandlingArray.reserve(s_XwaObjectsCount * 2);
	g_officerSoundsBuffer.reserve(s_XwaObjectsCount);

	return s_XwaObjectsCount;
}

int AIHandlingArrayGetHook(int* params)
{
	const int objectIndex = params[0];
	const int orderType = params[14];

	return g_aiHandlingArray.data()[objectIndex * 2 + orderType];
}

int AIHandlingArraySet0Hook(int* params)
{
	const int objectIndex = params[0];
	const int orderType = params[11];

	g_aiHandlingArray.data()[objectIndex * 2 + orderType] = 0;

	return 0;
}

int AIHandlingArraySet1Hook(int* params)
{
	const int objectIndex = params[0];
	const int orderType = params[11];

	g_aiHandlingArray.data()[objectIndex * 2 + orderType] = 1;

	return 0;
}

int OfficerSoundsBuffer1Hook(int* params)
{
	const int eax = params[0];

	params[0] = (int)(g_officerSoundsBuffer.data() + eax);

	return 0;
}

int OfficerSoundsBuffer2Hook(int* params)
{
	const int esi = params[0];

	params[0] = g_officerSoundsBuffer.data()[esi];

	return 0;
}

int OfficerSoundsBuffer3Hook(int* params)
{
	const int eax = params[0];
	const int esi = params[8];

	g_officerSoundsBuffer.data()[esi] = eax;

	return 0;
}

int FriendlyCraftListHook(int* params)
{
	int count = params[Params_EBP];

	if (count >= 192)
	{
		params[Params_ReturnAddress] = 0x004C0872;
	}

	return 0;
}

int EnemyCraftListHook(int* params)
{
	int count = params[Params_EBP];

	if (count >= 192)
	{
		params[Params_ReturnAddress] = 0x004C1974;
	}

	return 0;
}

int Radar2DItemsCountHook(int* params)
{
	static std::vector<unsigned char> s_V0x0068BEC8;
	static std::vector<unsigned char> s_V0x0068BDA8;
	static std::vector<unsigned char> s_V0x0068C300;
	static std::vector<unsigned char> s_V0x0068C1E0;
	static bool isInitialized = false;

	if (!isInitialized)
	{
		isInitialized = true;

		int itemsCount = g_config.Radar2DItemsCount;
		s_V0x0068BEC8.reserve(6 * itemsCount);
		s_V0x0068BDA8.reserve(6 * itemsCount);
		s_V0x0068C300.reserve(6 * itemsCount);
		s_V0x0068C1E0.reserve(6 * itemsCount);
	}

	bool s_V0x068C950 = *(unsigned char*)0x0068C950 != 0;

	if (s_V0x068C950)
	{
		*(unsigned char**)0x0068C868 = s_V0x0068BEC8.data();
		*(unsigned char**)0x0068C870 = s_V0x0068BDA8.data();
		*(unsigned char**)0x0068C86C = s_V0x0068C300.data();
		*(unsigned char**)0x0068C874 = s_V0x0068C1E0.data();
	}
	else
	{
		*(unsigned char**)0x0068C868 = s_V0x0068BDA8.data();
		*(unsigned char**)0x0068C870 = s_V0x0068BEC8.data();
		*(unsigned char**)0x0068C86C = s_V0x0068C1E0.data();
		*(unsigned char**)0x0068C874 = s_V0x0068C300.data();
	}

	return 0;
}

int Radar2DItemsCount1Hook(int* params)
{
	short& s_V0x068C858 = *(short*)0x0068C858;

	if (s_V0x068C858 == g_config.Radar2DItemsCount)
	{
		s_V0x068C858 = g_config.Radar2DItemsCount - 1;
	}

	return 0;
}

int Radar2DItemsCount2Hook(int* params)
{
	short& s_V0x068C85C = *(short*)0x0068C85C;

	if (s_V0x068C85C == g_config.Radar2DItemsCount)
	{
		s_V0x068C85C = g_config.Radar2DItemsCount - 1;
	}

	return 0;
}

