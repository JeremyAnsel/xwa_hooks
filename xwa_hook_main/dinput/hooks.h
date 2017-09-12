#pragma once
#include "hook_function.h"

static const HookPatchItem g_setupDInputHookPatch[] =
{
	{ 0x1A7F20, "00000000000000000000000000000000", "506033C054505050E8EB42F7FF6158C3" },
	{ 0x1A7F30, "00000000000000000000000000000000", "536033C054505050E8DB42F7FF615BC3" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To setup the dinput hook", g_setupDInputHookPatch),
};
