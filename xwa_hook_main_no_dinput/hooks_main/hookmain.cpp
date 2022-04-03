#include "targetver.h"
#include "hook.h"
#include "hook_function.h"
#include "hooks.h"
#include <string>
#include <Windows.h>
#include <DbgHelp.h>

#pragma comment(lib, "Dbghelp")

std::string int_to_hex(int i);

std::string GetModuleName(void* address)
{
	HMODULE hModule = NULL;
	GetModuleHandleEx(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPCTSTR)address,
		&hModule);

	char moduleFileName[MAX_PATH];
	GetModuleFileName(hModule, moduleFileName, sizeof(moduleFileName));
	std::string moduleName = std::string(strrchr(moduleFileName, '\\') + 1);

	return moduleName;
}

void MakeMinidump(EXCEPTION_POINTERS* pExceptionInfo)
{
	char name[MAX_PATH];
	GetModuleFileName(nullptr, name, sizeof(name));
	SYSTEMTIME time;
	GetLocalTime(&time);

	std::string moduleName = GetModuleName(pExceptionInfo->ExceptionRecord->ExceptionAddress);
	moduleName = moduleName.substr(0, moduleName.rfind('.'));

	wsprintf(
		name + strlen(name) - 4,
		"_%4d%02d%02d_%02d%02d%02d_%s.dmp",
		time.wYear,
		time.wMonth,
		time.wDay,
		time.wHour,
		time.wMinute,
		time.wSecond,
		moduleName.c_str());

	HANDLE hFile = CreateFile(name, GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}

	MINIDUMP_EXCEPTION_INFORMATION exceptionInfo{};
	exceptionInfo.ThreadId = GetCurrentThreadId();
	exceptionInfo.ExceptionPointers = pExceptionInfo;
	exceptionInfo.ClientPointers = FALSE;

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

	OutputDebugString((std::string() + "A crash dump has been generated at " + name).c_str());
}

LONG WINAPI TopLevelExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo)
{
	std::string text;
	text.append("Exception 0x");
	text.append(int_to_hex(pExceptionInfo->ExceptionRecord->ExceptionCode));
	text.append(" at ");
	text.append(int_to_hex((int)pExceptionInfo->ExceptionRecord->ExceptionAddress));
	text.append("  in ");
	text.append(GetModuleName(pExceptionInfo->ExceptionRecord->ExceptionAddress));

	OutputDebugString(text.c_str());
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

		SetUnhandledExceptionFilter(TopLevelExceptionHandler);
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
