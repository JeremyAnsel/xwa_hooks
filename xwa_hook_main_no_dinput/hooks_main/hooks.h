#pragma once
#include "hook_function.h"

static const HookPatchItem g_callMainHookPatch[] =
{
	{ 0x1A7F20, "00000000000000000000000000000000", "506054FF15608B5A0083C4046158C390" },
	{ 0x1A7F30, "00000000000000000000000000000000", "536054FF15608B5A0083C404615BC390" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the main hook", g_callMainHookPatch),
};
