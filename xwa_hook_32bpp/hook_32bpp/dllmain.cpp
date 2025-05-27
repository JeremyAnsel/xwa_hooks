#include "targetver.h"
#include <Windows.h>
#include "config.h"
#include "hookexe.h"

class MainConfig
{
public:
	MainConfig()
	{
		auto lines = GetFileLines("hook_32bpp.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_32bpp");
		}

		this->EnableSideProcess = GetFileKeyValueInt(lines, "EnableSideProcess", 0) != 0;
	}

	bool EnableSideProcess;
};

MainConfig g_mainConfig;

DWORD WINAPI ThreadFunction(LPVOID lpParameter)
{
	g_hookexe = nullptr;

	HWND hookexe = FindWindow("XWA32BPPPLAYER", NULL);

	if (hookexe)
	{
		g_hookexe = hookexe;
		return 1;
	}

	STARTUPINFO startup_info{};
	startup_info.cb = sizeof(STARTUPINFO);
	startup_info.dwFlags = STARTF_USESHOWWINDOW;
	startup_info.wShowWindow = SW_MINIMIZE;

	PROCESS_INFORMATION g_process_info{};

	if (!CreateProcessA("Xwa32bppPlayer.exe", NULL, NULL, NULL, FALSE, 0, (LPVOID)"SHIM_MCCOMPAT=0x800000001", NULL, &startup_info, &g_process_info))
	{
		OutputDebugString("hook_32bpp error: Xwa32BppPlayer.exe not found");
		MessageBox(nullptr, "Xwa32bppPlayer.exe not found", "hook_32bpp", MB_OK);
		return -1;
	}

	if (WaitForInputIdle(g_process_info.hProcess, 10000))
	{
		OutputDebugString("hook_32bpp error: Launching Xwa32bppPlayer.exe failed");
		MessageBox(nullptr, "Launching Xwa32bppPlayer.exe failed", "hook_32bpp", MB_OK);
		return -1;
	}

	g_hookexe = FindWindow("XWA32BPPPLAYER", NULL);

	if (!g_hookexe)
	{
		OutputDebugString("hook_32bpp error: Xwa32bppPlayer window not found");
		MessageBox(nullptr, "Xwa32bppPlayer window not found", "hook_32bpp", MB_OK);
		return -1;
	}

	return 1;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
	{
		if (!g_mainConfig.EnableSideProcess)
		{
			break;
		}

		HANDLE threadHandle = CreateThread(NULL, 0, ThreadFunction, NULL, 0, NULL);

		if (threadHandle)
		{
			CloseHandle(threadHandle);
		}

		break;
	}

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
	{
		HWND hookexe = FindWindow("XWA32BPPPLAYER", NULL);

		if (hookexe)
		{
			SendMessage(hookexe, WM_CLOSE, 0, 0);
		}

		break;
	}
	}

	return TRUE;
}
