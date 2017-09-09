#include "targetver.h"
#include "hook_function.h"

#include "hooks.h"

static const int g_hookFunctionsCount = sizeof(g_hookFunctions) / sizeof(HookFunction);

extern "C" __declspec(dllexport) int GetHookFunctionsCount()
{
	return g_hookFunctionsCount;
}

extern "C" __declspec(dllexport) HookFunction GetHookFunction(int index)
{
	auto hook = HookFunction{};

	if (index >= 0 && index < g_hookFunctionsCount)
	{
		return g_hookFunctions[index];
	}

	return hook;
}

static const int g_hookPatchesCount = sizeof(g_hookPatches) / sizeof(HookPatch);

extern "C" __declspec(dllexport) const HookPatch *GetHookPatch(int index)
{
	if (index < 0 || index >= g_hookPatchesCount) return nullptr;
	return g_hookPatches + index;
}
