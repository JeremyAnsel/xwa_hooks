#pragma once
#include "hook_function.h"

#include "res1200.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x4D3DE5, Res1200Hook1 },
	{ 0x4D3EF5, Res1200Hook2 },
	{ 0x4D3F12, Res1200Hook3 },
};

static const HookPatchItem g_res1200Patch[] =
{
	{ 0x0D31E0, "8B156C4B7D00", "E83B4D0D00C3" },
	{ 0x0D32F0, "C705A0D57400A0907F00", "E82B4C0D009090909090" },
	{ 0x0D330D, "8B54241C33C985D27620", "E80E4C0D005F5E5D5BC3" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that fix res 1200 crash", g_res1200Patch),
};
