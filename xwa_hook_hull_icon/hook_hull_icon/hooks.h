#pragma once
#include "hook_function.h"

#include "hull_icon.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x467C94, HullIconHook },
	{ 0x46ADCB, HullIconHook },
	{ 0x46B07B, HullIconHook },
	{ 0x46B44C, HullIconHook },
	{ 0x4A063E, MapIconHook },
};

static const HookPatchItem g_defineHullIconsPatch[] =
{
	{ 0x067089, "663D4700771925FFFF0000668B0C4508965A00", "25FFFF000050E89C0E140083C4048BC8909090" },
	{ 0x06A1C0, "663D4700770E25FFFF0000668B044508965A00", "25FFFF000050E865DD130083C4049090909090" },
	{ 0x06A470, "663D4700771925FFFF0000668B0C4508965A00", "25FFFF000050E8B5DA130083C4048BC8909090" },
	{ 0x06A841, "663D4700771925FFFF0000668B0C4508965A00", "25FFFF000050E8E4D6130083C4048BC8909090" },
};

static const HookPatchItem g_defineMapIconsPatch[] =
{
	{ 0x09FA2B, "6681FEDE00771A", "90909090909090" },
	{ 0x09FA38, "668B0C75D89A5A00", "56E8E28410005990" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that defines hull icons", g_defineHullIconsPatch),
	MAKE_HOOK_PATCH("To call the hook that defines map icons", g_defineMapIconsPatch),
};
