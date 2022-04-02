#include "common.h"
#include "hook.h"
#include "hook_function.h"
#include "hooks.h"

bool IsXwaExe()
{
	char filename[4096];

	if (GetModuleFileName(nullptr, filename, sizeof(filename)) == 0)
	{
		return false;
	}

	int length = strlen(filename);

	if (length < 17)
	{
		return false;
	}

	return _stricmp(filename + length - 17, "xwingalliance.exe") == 0;
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

	*(int*)(0x005A8B20 + 0x09) = (int)DirectInputCreateA - 0x005A8B2D;
	*(int*)(0x005A8B30 + 0x09) = (int)DirectInputCreateA - 0x005A8B3D;

	*(int*)(0x0059BF69 + 0x01) = (int)exit - (0x0059BF69 + 0x05);

	return true;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
	{
		if (IsXwaExe())
		{
			SetProcessDPIAware();

			VirtualProtectMemoryReadWrite();

			if (MainPatchMemory())
			{
				LoadAndPatchHooks();
			}

			VirtualProtectMemoryExecuteReadWrite();
		}

		break;
	}

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}
