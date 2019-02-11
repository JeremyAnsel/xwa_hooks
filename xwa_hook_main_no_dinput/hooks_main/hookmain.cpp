#include "targetver.h"
#include "hook.h"
#include "hook_function.h"
#include "hooks.h"

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
		esp[7] = Hook(esp[9], esp + 10);
	}
}
