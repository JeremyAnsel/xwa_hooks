#include "targetver.h"
#include "hook.h"
#include "hook_function.h"
#include "hooks.h"
#include "config.h"
#include <string>
#include <Windows.h>
#include <DbgHelp.h>

#pragma comment(lib, "Dbghelp")

std::string int_to_hex(int i);

class MainConfig
{
public:
	MainConfig()
	{
		auto lines = GetFileLines("dinput.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "dinput");
		}

		this->GenerateDumpFilesOnCrash = GetFileKeyValueInt(lines, "GenerateDumpFilesOnCrash", 1) != 0;
	}

	bool GenerateDumpFilesOnCrash;
};

MainConfig g_mainconfig;

const char* GetModuleName(void* address)
{
	static char moduleFileName[MAX_PATH];

	HMODULE hModule = NULL;
	GetModuleHandleEx(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPCTSTR)address,
		&hModule);

	GetModuleFileName(hModule, moduleFileName, sizeof(moduleFileName));
	*strrchr(moduleFileName, '.') = 0;

	const char* moduleName = strrchr(moduleFileName, '\\') + 1;
	return moduleName;
}

void MakeMinidump(EXCEPTION_POINTERS* pExceptionInfo)
{
	char name[MAX_PATH];
	GetModuleFileName(nullptr, name, sizeof(name));
	SYSTEMTIME time;
	GetLocalTime(&time);

	const char* moduleName = GetModuleName(pExceptionInfo->ExceptionRecord->ExceptionAddress);

	wsprintf(
		name + strlen(name) - 4,
		"_%4d%02d%02d_%02d%02d%02d%03d_%s.dmp",
		time.wYear,
		time.wMonth,
		time.wDay,
		time.wHour,
		time.wMinute,
		time.wSecond,
		time.wMilliseconds,
		moduleName);

	HANDLE hFile = CreateFile(name, GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		OutputDebugString("Crash dump creation has failed");
		return;
	}

	MINIDUMP_EXCEPTION_INFORMATION exceptionInfo{};
	exceptionInfo.ThreadId = GetCurrentThreadId();
	exceptionInfo.ExceptionPointers = pExceptionInfo;
	exceptionInfo.ClientPointers = TRUE;

	MiniDumpWriteDump(
		GetCurrentProcess(),
		GetCurrentProcessId(),
		hFile,
		MINIDUMP_TYPE(
			MiniDumpNormal
			| MiniDumpWithCodeSegs
			| MiniDumpWithDataSegs
			| MiniDumpWithIndirectlyReferencedMemory
			| MiniDumpWithHandleData
			| MiniDumpWithUnloadedModules
			| MiniDumpWithFullMemoryInfo
			| MiniDumpWithThreadInfo
			| MiniDumpWithTokenInformation),
		pExceptionInfo ? &exceptionInfo : nullptr,
		nullptr,
		nullptr);

	CloseHandle(hFile);

	char text[MAX_PATH];
	strcpy_s(text, "A crash dump has been generated at ");
	strcat_s(text, name);
	OutputDebugString(text);
}

LONG WINAPI TopLevelExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo)
{
	char text[MAX_PATH];
	wsprintf(
		text,
		"Exception 0x%08X at %08X in %s",
		pExceptionInfo->ExceptionRecord->ExceptionCode,
		(unsigned int)pExceptionInfo->ExceptionRecord->ExceptionAddress,
		GetModuleName(pExceptionInfo->ExceptionRecord->ExceptionAddress));

	OutputDebugString(text);
	MakeMinidump(pExceptionInfo);

	return EXCEPTION_CONTINUE_SEARCH;
}

bool MainPatchMemory()
{
	for (const auto& patch : g_patches)
	{
		if (!PatchMemory(patch))
		{
			return false;
		}
	}

	return true;
}

extern "C" __declspec(dllexport) void CallHook(int* esp)
{
	if (esp == nullptr)
	{
		SetProcessDPIAware();

		VirtualProtectMemoryReadWrite();

		*(int*)0x005A8B60 = (int)CallHook;

		*(int*)(0x0059BF69 + 0x01) = (int)exit - (0x0059BF69 + 0x05);

		if (MainPatchMemory())
		{
			LoadAndPatchHooks();
		}

		VirtualProtectMemoryExecuteReadWrite();

		if (g_mainconfig.GenerateDumpFilesOnCrash)
		{
			SetUnhandledExceptionFilter(TopLevelExceptionHandler);
		}
	}
	else
	{
		int result = Hook(esp[9], esp + 10);

		if (esp[-2] == 0x5A8B39)
		{
			esp[7] = result;
		}
	}
}
