#include "framework.h"
#include "32bpp.h"
#include "SharedMemory.h"

extern "C"
{
	// http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;

	// https://gpuopen.com/amdpowerxpressrequesthighperformance/
	__declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
}

class NetFunctions
{
public:
	typedef void(_cdecl* setSettingsFunction)(const char*, int, int, const char*, int);
	typedef int(_cdecl* readOptFunction)(const char*, int, int);
	typedef int(_cdecl* getOptVersionFunction)();
	typedef void(_cdecl* writeOptFunction)(void*);

	NetFunctions()
	{
		_module = LoadLibrary(L"Xwa32bppPlayerNet.dll");

		if (!_module)
		{
			return;
		}

		_setSettingsFunction = (setSettingsFunction)GetProcAddress(_module, "SetSettingsFunction");
		_readOptFunction = (readOptFunction)GetProcAddress(_module, "ReadOptFunction");
		_getOptVersionFunction = (getOptVersionFunction)GetProcAddress(_module, "GetOptVersionFunction");
		_writeOptFunction = (writeOptFunction)GetProcAddress(_module, "WriteOptFunction");
	}

	~NetFunctions()
	{
		FreeLibrary(_module);
	}

	HMODULE _module;
	setSettingsFunction _setSettingsFunction = nullptr;
	readOptFunction _readOptFunction = nullptr;
	getOptVersionFunction _getOptVersionFunction = nullptr;
	writeOptFunction _writeOptFunction = nullptr;
};

NetFunctions g_netFunctions;

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

void ExeShowMessage(DWORD cbData, PVOID lpData)
{
	std::string message = (const char*)lpData;
	MessageBoxA(NULL, message.c_str(), "hook_32bpp", MB_OK);
}

static unsigned int g_optRequiredSize = 0;
static SharedMemoryWriter g_optSharedMemory;

struct ExeSetSettingsParameters
{
	char missionFileName[256];
	int missionFileNameIndex;
	int isTechLibraryGameStateUpdate;
	char hangar[256];
	int hangarIff;
};

void ExeSetSettings(DWORD cbData, PVOID lpData)
{
	ExeSetSettingsParameters* data = (ExeSetSettingsParameters*)lpData;

	g_netFunctions._setSettingsFunction(
		data->missionFileName,
		data->missionFileNameIndex,
		data->isTechLibraryGameStateUpdate,
		data->hangar,
		data->hangarIff);
}

struct ExeReadOptParameters
{
	char optFilename[256];
	int loadSkins;
	int groupFaceGroups;
};

unsigned int ExeReadOpt(DWORD cbData, PVOID lpData)
{
	ExeReadOptParameters* data = (ExeReadOptParameters*)lpData;

	unsigned int requiredFileSize = (unsigned int)g_netFunctions._readOptFunction(data->optFilename, data->loadSkins, data->groupFaceGroups);
	g_optRequiredSize = requiredFileSize;
	return requiredFileSize;
}

unsigned int ExeGetOptVersion(DWORD cbData, PVOID lpData)
{
	return (unsigned int)g_netFunctions._getOptVersionFunction();
}

void ExeWriteOpt(DWORD cbData, PVOID lpData)
{
	g_optSharedMemory.Create(L"Local\\Xwa32bppHookSemaphore", g_optRequiredSize);
	g_netFunctions._writeOptFunction(g_optSharedMemory._lpData);
}

void ExeFreeOptMemory(DWORD cbData, PVOID lpData)
{
	g_optSharedMemory.Close();
}

LRESULT HandleMessage(ULONG_PTR dwData, DWORD cbData, PVOID lpData)
{
	switch (dwData)
	{
	case ExeDataType_ShowMessage:
		ExeShowMessage(cbData, lpData);
		return 0;

	case ExeDataType_SetSettings:
		ExeSetSettings(cbData, lpData);
		return 0;

	case ExeDataType_ReadOpt:
		return ExeReadOpt(cbData, lpData);

	case ExeDataType_GetOptVersion:
		return ExeGetOptVersion(cbData, lpData);

	case ExeDataType_WriteOpt:
		ExeWriteOpt(cbData, lpData);
		return 0;

	case ExeDataType_FreeOptMemory:
		ExeFreeOptMemory(cbData, lpData);
		return 0;
	}

	return 0;
}
