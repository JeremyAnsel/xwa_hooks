#include "targetver.h"
#include <Windows.h>
#include "hookexe.h"

DWORD WINAPI ThreadFunction(LPVOID lpParameter)
{
	g_hookexe = nullptr;

	HWND hookexe = FindWindow("XWACONCOURSEPLAYER", NULL);

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

	if (!CreateProcessA("XwaConcoursePlayer.exe", NULL, NULL, NULL, FALSE, 0, (LPVOID)"SHIM_MCCOMPAT=0x800000001", NULL, &startup_info, &g_process_info))
	{
		OutputDebugString("hook_concourse error: XwaConcoursePlayer.exe not found");
		MessageBox(nullptr, "XwaConcoursePlayer.exe not found", "hook_concourse", MB_OK);
		return -1;
	}

	if (WaitForInputIdle(g_process_info.hProcess, 10000))
	{
		OutputDebugString("hook_concourse error: Launching XwaConcoursePlayer.exe failed");
		MessageBox(nullptr, "Launching XwaConcoursePlayer.exe failed", "hook_concourse", MB_OK);
		return -1;
	}

	g_hookexe = FindWindow("XWACONCOURSEPLAYER", NULL);

	if (!g_hookexe)
	{
		OutputDebugString("hook_concourse error: XwaConcoursePlayer window not found");
		MessageBox(nullptr, "XwaConcoursePlayer window not found", "hook_concourse", MB_OK);
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
		HWND hookexe = FindWindow("XWACONCOURSEPLAYER", NULL);

		if (hookexe)
		{
			SendMessage(hookexe, WM_CLOSE, 0, 0);
		}

		break;
	}
	}

	return TRUE;
}
