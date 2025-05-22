#include "targetver.h"
#include <Windows.h>
#include "hookexe.h"
#include "SharedMemory.h"

HWND g_hookexe = NULL;

enum ExeDataType
{
	ExeDataType_None,
	ExeDataType_ShowMessage,
	ExeDataType_SetSettings,
	ExeDataType_ReadOpt,
	ExeDataType_GetOptVersion,
	ExeDataType_WriteOpt,
	ExeDataType_FreeOptMemory,
};

void ExeShowMessage(const std::string& message)
{
	if (!g_hookexe)
	{
		return;
	}

	COPYDATASTRUCT cds{};
	cds.dwData = ExeDataType_ShowMessage;
	cds.cbData = message.length() + 1;
	cds.lpData = (void*)message.data();

	SendMessage(g_hookexe, WM_COPYDATA, NULL, (LPARAM)&cds);
}

static unsigned int g_optRequiredSize = 0;
static SharedMemoryReader g_optSharedMemory;

std::string GetMissionFileName()
{
	int currentGameState = *(int*)(0x09F60E0 + 0x25FA9);
	int updateCallback = *(int*)(0x09F60E0 + 0x25FB1 + currentGameState * 0x850 + 0x844);
	bool isTechLibraryGameStateUpdate = updateCallback == 0x00574D70;

	if (isTechLibraryGameStateUpdate)
	{
		return std::string();
	}

	int missionDirectoryId = *(int*)(0x00AE2A60 + 0x002A);
	std::string missionDirectory = *(const char**)(0x00603168 + missionDirectoryId * 4);
	int s_V0x09F5E74 = *(int*)0x009F5E74;
	int s_XwaMissionDescriptions = *(int*)0x009F4B98;

	std::string missionFilename;

	if (s_XwaMissionDescriptions != 0)
	{
		missionFilename = missionDirectory + "\\" + (const char*)(s_XwaMissionDescriptions + s_V0x09F5E74 * 0x148 + 0x0000);
	}
	else
	{
		std::string xwaMissionFileName = (const char*)0x06002E8;
		missionFilename = xwaMissionFileName;
	}

	return missionFilename;
}

struct ExeSetSettingsParameters
{
	char missionFileName[256];
	int missionFileNameIndex;
	int isTechLibraryGameStateUpdate;
	char hangar[256];
	int hangarIff;
};

void ExeSetSettings()
{
	if (!g_hookexe)
	{
		return;
	}

	std::string xwaMissionFileName = GetMissionFileName();
	int xwaMissionFileNameIndex = *(int*)0x06002E4;
	int currentGameState = *(int*)(0x09F60E0 + 0x25FA9);
	int updateCallback = *(int*)(0x09F60E0 + 0x25FB1 + currentGameState * 0x850 + 0x844);
	bool isTechLibraryGameStateUpdate = updateCallback == 0x00574D70;
	std::string hangar = (const char*)0x00ABD680;
	int hangarIff = *(unsigned char*)(0x00ABD680 + 255);

	ExeSetSettingsParameters parameters{};
	strncpy_s(parameters.missionFileName, xwaMissionFileName.c_str(), xwaMissionFileName.length());
	parameters.missionFileNameIndex = xwaMissionFileNameIndex;
	parameters.isTechLibraryGameStateUpdate = isTechLibraryGameStateUpdate ? 1 : 0;
	strncpy_s(parameters.hangar, hangar.c_str(), hangar.length());
	parameters.hangarIff = hangarIff;

	COPYDATASTRUCT cds{};
	cds.dwData = ExeDataType_SetSettings;
	cds.cbData = sizeof(ExeSetSettingsParameters);
	cds.lpData = &parameters;

	SendMessage(g_hookexe, WM_COPYDATA, NULL, (LPARAM)&cds);
}

struct ExeReadOptParameters
{
	char optFilename[256];
	int loadSkins;
	int groupFaceGroups;
};

unsigned int ExeReadOpt(const std::string& filename)
{
	if (!g_hookexe)
	{
		return 0;
	}

	bool isInSkirmishShipScreen = *(int*)0x007838A0 != 0;

	ExeReadOptParameters parameters{};
	strncpy_s(parameters.optFilename, filename.c_str(), filename.length());
	parameters.loadSkins = isInSkirmishShipScreen ? 0 : 1;
	parameters.groupFaceGroups = 1;

	COPYDATASTRUCT cds{};
	cds.dwData = ExeDataType_ReadOpt;
	cds.cbData = sizeof(ExeReadOptParameters);
	cds.lpData = &parameters;

	unsigned int requiredFileSize = (unsigned int)SendMessage(g_hookexe, WM_COPYDATA, NULL, (LPARAM)&cds);
	g_optRequiredSize = requiredFileSize;
	return requiredFileSize;
}

unsigned int ExeGetOptVersion()
{
	if (!g_hookexe)
	{
		return 0;
	}

	COPYDATASTRUCT cds{};
	cds.dwData = ExeDataType_GetOptVersion;
	cds.cbData = 0;
	cds.lpData = nullptr;

	return (unsigned int)SendMessage(g_hookexe, WM_COPYDATA, NULL, (LPARAM)&cds);
}

void* ExeWriteOpt()
{
	if (!g_hookexe)
	{
		return nullptr;
	}

	COPYDATASTRUCT cds{};
	cds.dwData = ExeDataType_WriteOpt;
	cds.cbData = 0;
	cds.lpData = nullptr;

	SendMessage(g_hookexe, WM_COPYDATA, NULL, (LPARAM)&cds);

	g_optSharedMemory.Open(L"Local\\Xwa32bppHookSemaphore", g_optRequiredSize);
	return g_optSharedMemory._lpData;
}

void ExeFreeOptMemory()
{
	g_optSharedMemory.Close();

	COPYDATASTRUCT cds{};
	cds.dwData = ExeDataType_FreeOptMemory;
	cds.cbData = 0;
	cds.lpData = nullptr;

	SendMessage(g_hookexe, WM_COPYDATA, NULL, (LPARAM)&cds);
}
