#include "targetver.h"
#include "hook_function.h"

#include "hooks.h"

#include "config.h"

class Config
{
public:
	Config()
	{
		auto lines = GetFileLines("hook_d3d.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_d3d");
		}

		this->IsHookD3DEnabled = GetFileKeyValueInt(lines, "IsHookD3DEnabled", 1) != 0;
	}

	bool IsHookD3DEnabled;
};

Config g_config;

static const int g_hookFunctionsCount = sizeof(g_hookFunctions) / sizeof(HookFunction);

extern "C" __declspec(dllexport) int GetHookFunctionsCount()
{
	if (!g_config.IsHookD3DEnabled)
	{
		return 0;
	}

	return g_hookFunctionsCount;
}

extern "C" __declspec(dllexport) HookFunction GetHookFunction(int index)
{
	auto hook = HookFunction{};

	if (!g_config.IsHookD3DEnabled)
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
	if (!g_config.IsHookD3DEnabled)
	{
		return 0;
	}

	return g_patchesCount;
}

extern "C" __declspec(dllexport) const HookPatch* GetHookPatch(int index)
{
	if (!g_config.IsHookD3DEnabled)
	{
		return nullptr;
	}

	if (index < 0 || index >= g_patchesCount)
	{
		return nullptr;
	}

	return &g_patches[index];
}
