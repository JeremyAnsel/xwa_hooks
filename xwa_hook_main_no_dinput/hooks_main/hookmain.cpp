#include "targetver.h"
#include "hook.h"
#include "hook_function.h"
#include "hooks.h"
#include <Windows.h>

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

		VirtualProtectHookMemoryWrite();

		*(int*)0x005A8B60 = (int)CallHook;

		if (MainPatchMemory())
		{
			LoadAndPatchHooks();
		}

		VirtualProtectHookMemory();
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
