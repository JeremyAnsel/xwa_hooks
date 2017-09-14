#pragma once
#include "hook_function.h"

#include "lightmaps.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x441B68, LightmapsHook },
};

static const HookPatchItem g_createLightmapsPatch[] =
{
	{ 0x040F60, "8D8BB0D164003BC2895424347E4B", "505356E8C86F160083C40C90EB4B" },
	{ 0x040FCD, "8D83B0D16400", "909090909090" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that creates lightmaps", g_createLightmapsPatch),
};
