#pragma once
#include "hook_function.h"

#include "exterior.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x4F221C, Render3DHook },
	{ 0x4661F5, SetHudVisibilityHook },
};

static const HookPatchItem g_render3DPatch[] =
{
	{ 0x0F1617, "E8E4DBFFFF", "E804690B00" },
	{ 0x1080FD, "6689BE69A08B00", "90909090909090" },
	{ 0x10811F, "6689BE44A08B006689BE46A08B00", "9090909090909090909090909090" },
	{ 0x0AEF78, "66898669A08B00", "90909090909090" },
	{ 0x0AEF9F, "66898644A08B0066898646A08B00", "9090909090909090909090909090" },
	{ 0x0FFFFC, "0F85820C0000", "909090909090" },
	{ 0x0655F0, "8B4424048B4C2408", "E82B291400C39090" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that renders 3D exterior", g_render3DPatch),
};
