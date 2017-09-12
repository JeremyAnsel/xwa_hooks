#pragma once
#include "hook_function.h"

#include "backdrops.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x57AFE4, BackdropsHook },
};

static const HookPatchItem g_addStarfieldBackdropsPatch[] =
{
	{ 0x17A3D9, "33DB85C08944241C0F8E4D0300008B74241469F63E0E0000", "8D5C24145350E83CDB020083C408E9480300009090909090" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that adds starfield backdrops", g_addStarfieldBackdropsPatch),
};
