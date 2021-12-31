#include "targetver.h"
#include "crafts.h"
#include "config.h"

#include <Windows.h>

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
		this->ProjectilesCount = GetFileKeyValueInt(lines, "ProjectilesCount", 3072 * 2);
		this->ExplosionsCount = GetFileKeyValueInt(lines, "ExplosionsCount", 512);

		OutputDebugString(("Hook_Crafts_Count CraftsCount=" + std::to_string(this->CraftsCount)).c_str());
		OutputDebugString(("Hook_Crafts_Count ProjectilesCount=" + std::to_string(this->ProjectilesCount)).c_str());
		OutputDebugString(("Hook_Crafts_Count ExplosionsCount=" + std::to_string(this->ExplosionsCount)).c_str());
	}

	int CraftsCount;
	int ProjectilesCount;
	int ExplosionsCount;
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
