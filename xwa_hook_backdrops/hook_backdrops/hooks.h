#pragma once
#include "hook_function.h"

#include "backdrops.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x57AFE4, BackdropsHook },
	{ 0x51034F, LoadMissionHook },
	{ 0x511524, LoadMissionHook },
};

static const HookPatchItem g_addStarfieldBackdropsPatch[] =
{
	{ 0x17A3D9, "33DB85C08944241C0F8E4D0300008B74241469F63E0E0000", "8D5C24145350E83CDB020083C408E9480300009090909090" },
};

static const HookPatchItem g_loadMissionPatch[] =
{
	{ 0x10F74A, "E81154F0FF", "E8E1870900" },
	{ 0x11091F, "E83C42F0FF", "E80C760900" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that adds starfield backdrops", g_addStarfieldBackdropsPatch),
	MAKE_HOOK_PATCH("To call the hook that loads mission", g_loadMissionPatch),
};
