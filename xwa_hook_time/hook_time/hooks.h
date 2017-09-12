#pragma once
#include "hook_function.h"

#include "get_time.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x50E415, L0050E410Hook },
	{ 0x50E435, L0050E430Hook },
};

static const HookPatchItem g_reduceCPUUsagePatch[] =
{
	{ 0x10D810, "E82BF8FFFF8B", "E81BA70900C3" },
	{ 0x10D830, "E80BF8FFFF8B", "E8FBA60900C3" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that reduces CPU usage", g_reduceCPUUsagePatch),
};
