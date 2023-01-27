#pragma once
#include "hook_function.h"

#include "gimbal_lock.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x4FABAB, GimbalLockUserInputHook },
};

static const HookPatchItem g_gimbalLockPatch[] =
{
	{ 0x0F9FA6, "E845970000", "E875DF0A00" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that fixes the gimbal lock", g_gimbalLockPatch),
};
