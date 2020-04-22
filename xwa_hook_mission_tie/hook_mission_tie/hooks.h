#pragma once
#include "hook_function.h"

#include "tie.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x415E1F, TieHook },
	{ 0x41D304, CreateObjectHook },
};

static const HookPatchItem g_overrideTiePatch[] =
{
	{ 0x01521A, "E8E1A40000", "E8112D1900" },
	{ 0x01C6FF, "668B442428", "E82CB81800" },
};

static const HookPatchItem g_craftsCountPerRegionPatch[] =
{
	{ 0x014C2E, "60000000C1E305", "C0000000C1E306" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that override tie mission", g_overrideTiePatch),
	MAKE_HOOK_PATCH("'crafts count per region' patch", g_craftsCountPerRegionPatch),
};
