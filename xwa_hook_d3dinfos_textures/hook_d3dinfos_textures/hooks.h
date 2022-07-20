#pragma once
#include "hook_function.h"

#include "d3dinfos.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x441755, InitD3DInfosHook },
	{ 0x443921, TestTextureIlluminationHook },
	{ 0x482970, RenderOptNodeHook },
};

static const HookPatchItem g_removeD3DInfosCountLimitPatch[] =
{
	{ 0x040B50, "5657BAC3296600", "E8CB731600C390" },
};

static const HookPatchItem g_testTextureIlluminationPatch[] =
{
	{ 0x042D1C, "0F8478020000", "E8FF51160090" },
	{ 0x081D6B, "8B8EA8000000", "E8B061120090" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that removes D3DInfos count limit", g_removeD3DInfosCountLimitPatch),
	MAKE_HOOK_PATCH("To call the hook that tests the texture illumination", g_testTextureIlluminationPatch),
};
