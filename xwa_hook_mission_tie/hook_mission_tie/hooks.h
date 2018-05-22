#pragma once
#include "hook_function.h"

#include "tie.h"

static const HookFunction g_hookFunctions[] =
{
	{0x415E1F, TieHook },
};

static const HookPatchItem g_overrideTiePatch[] =
{
	{ 0x01521A, "E8E1A40000", "E8112D1900" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that override tie mission", g_overrideTiePatch),
};
