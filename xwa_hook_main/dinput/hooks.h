#pragma once
#include "hook_function.h"

static const HookPatchItem g_setupDInputHookPatch[] =
{
	{ 0x1A7F20, "00000000000000000000000000000000", "9C6033C054505050E8EB42F7FF619DC3" },
	{ 0x1A7F30, "00000000000000000000000000000000", "9C6033C054505050E8DB42F7FF619DC3" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To setup the dinput hook", g_setupDInputHookPatch),
};
