#pragma once
#include "hook_function.h"

#include "sfoils.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x4016F9, SFoilsHook1 },
	{ 0x40191A, SFoilsHook2 },
};

static const HookPatchItem g_disableSFoilModelFilterPatch[] =
{
	{ 0x0FB86E, "7419", "EB19" },
};

static const HookPatchItem g_defineSFoilsPatch[] =
{
	{ 0x000AF3, "4883F8310F872001000033C98A8840184000", "50E827741A0083C4045F5E5D5BC390909090" },
	{ 0x000D14, "4883F8310F87B101000033D28A90C42C4000", "50E816721A0083C4048BF8E94E0100009090" },
};

static const HookPatchItem g_alignCameraBridgePatch[] =
{
	{ 0x08058C, "0F85F5000000", "909090909090" },
	{ 0x080D7C, "0F85E0000000", "909090909090" },
	{ 0x08304B, "0F85E9000000", "909090909090" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("Disable S-Foil model filter", g_disableSFoilModelFilterPatch),
	MAKE_HOOK_PATCH("To call the hook that defines S-Foils", g_defineSFoilsPatch),
	MAKE_HOOK_PATCH("Align the camera with the bridge", g_alignCameraBridgePatch),
};
