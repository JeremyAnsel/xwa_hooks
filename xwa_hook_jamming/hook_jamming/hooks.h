#pragma once
#include "hook_function.h"

#include "jamming.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x490394, HangarJammingHook },
};

static const HookPatchItem g_jammingPatch[] =
{
	{ 0x08F78F, "E8AC2A0500", "E88C871100" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that defines jamming", g_jammingPatch),
};
