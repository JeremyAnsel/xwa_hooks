#pragma once
#include "hook_function.h"

#include "sfoils.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x4016F9, SFoilsHook1 },
	{ 0x40191A, SFoilsHook2 },
	{ 0x45EECC, SFoilsHangarShuttleHook },
	{ 0x45F1C8, SFoilsHangarShuttleHook },
	{ 0x45F407, SFoilsHangarShuttleHook },
	{ 0x45F6FC, SFoilsHangarShuttleHook },
	{ 0x45F9C7, SFoilsHangarShuttleHook },
};

static const HookPatchItem g_disableSFoilModelFilterPatch[] =
{
	{ 0x0FB86E, "7419", "EB19" },
	{ 0x002EF0, "740A", "EB0A" },
	{ 0x058920, "7406", "EB06" },
};

static const HookPatchItem g_defineSFoilsPatch[] =
{
	{ 0x000AF3, "4883F8310F872001000033C98A8840184000", "50E827741A0083C4045F5E5D5BC390909090" },
	{ 0x000D14, "4883F8310F87B101000033D28A90C42C4000", "50E816721A0083C4048BF8E94E0100009090" },
};

static const HookPatchItem g_defineHangarShuttleSFoilsPatch[] =
{
	{ 0x05E2C4, "8B15C4337B00668B4C320251E88B9A0200", "6A0057E8549C140083C408EB6B90909090" },
	{ 0x05E5C0, "8B0DC4337B00668B44310250E88F970200", "6A0057E85899140083C408EB6090909090" },
	{ 0x05E7FF, "8B15C4337B0033C9668B4C3202", "6A0157E81997140083C408EB6D" },
	{ 0x05EAF4, "8B0DC4337B00668B44310250E85B920200", "6A0157E82494140083C408EB6A90909090" },
	{ 0x05EDBF, "8B15C4337B0033C9668B4C3202", "6A0157E85991140083C408EB5F" },
};

static const HookPatchItem g_alignCameraBridgePatch[] =
{
	{ 0x08058C, "0F85F5000000", "909090909090" },
	{ 0x08059B, "7519", "9090" },
	{ 0x080D7C, "0F85E0000000", "909090909090" },
	{ 0x080D8A, "7517", "9090" },
	{ 0x08304B, "0F85E9000000", "909090909090" },
	{ 0x083059, "7517", "9090" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("Disable S-Foil model filter", g_disableSFoilModelFilterPatch),
	MAKE_HOOK_PATCH("To call the hook that defines S-Foils", g_defineSFoilsPatch),
	MAKE_HOOK_PATCH("To call the hook that defines hangar shuttle S-Foils", g_defineHangarShuttleSFoilsPatch),
	MAKE_HOOK_PATCH("Align the camera with the bridge", g_alignCameraBridgePatch),
};
