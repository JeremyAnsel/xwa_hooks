#include "targetver.h"
#include "hook_function.h"

#include "hooks.h"

#include "config.h"

class MainConfig
{
public:
	MainConfig()
	{
		auto lines = GetFileLines("hook_diag.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_diag");
		}

		this->ShowDiagMessages = GetFileKeyValueInt(lines, "ShowDiagMessages", 0) != 0;
	}

	bool ShowDiagMessages;
};

MainConfig g_mainconfig;

static const int g_hookFunctionsCount = sizeof(g_hookFunctions) / sizeof(HookFunction);

extern "C" __declspec(dllexport) int GetHookFunctionsCount()
{
	if (!g_mainconfig.ShowDiagMessages)
	{
		return 0;
	}

	return g_hookFunctionsCount;
}

extern "C" __declspec(dllexport) HookFunction GetHookFunction(int index)
{
	auto hook = HookFunction{};

	if (!g_mainconfig.ShowDiagMessages)
	{
		return hook;
	}

	if (index >= 0 && index < g_hookFunctionsCount)
	{
		return g_hookFunctions[index];
	}

	return hook;
}

static const int g_patchesCount = sizeof(g_patches) / sizeof(HookPatch);

extern "C" __declspec(dllexport) int GetHookPatchesCount()
{
	if (!g_mainconfig.ShowDiagMessages)
	{
		return 0;
	}

	return g_patchesCount;
}

extern "C" __declspec(dllexport) const HookPatch * GetHookPatch(int index)
{
	if (!g_mainconfig.ShowDiagMessages)
	{
		return nullptr;
	}

	if (index < 0 || index >= g_patchesCount)
	{
		return nullptr;
	}

	return &g_patches[index];
}
