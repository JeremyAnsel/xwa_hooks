#pragma once
#include "hook_function.h"

#include "hangar.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x4579F3, HangarOptLoadHook },
	{ 0x45FCA8, HangarCameraPositionHook },
	{ 0x455F4B, HangarMapHook },
	{ 0x456479, FamHangarMapHook },
	{ 0x57EB9D, SelectHangarTypeHook },
	{ 0x4095B5, SelectHangarInsideAnimation },
	{ 0x457F80, SelectHangarModelIndex },
	{ 0x46264F, CraftElevationHook },
};

static const HookPatchItem g_selectHangarTypePatch[] =
{
	{ 0x17DF97, "390C858E2AAE000F92C2", "50E8939F02008AD05890" },
};

static const HookPatchItem g_selectInsideAnimationPatch[] =
{
	{ 0x89AF, "663D8700740A663D9400", "50E87BF5190085C05890" },
};

static const HookPatchItem g_selectHangarModelIndexPatch[] =
{
	{ 0x5737A, "663D8700740F663D9400", "50E8B00B150085C05890" },
};

static const HookPatchItem g_loadHangarOptPatch[] =
{
	{ 0x056DEE, "E84D4F0700", "E83D111500" },
};

static const HookPatchItem g_removeOptFilterPatch[] =
{
	{ 0x563C3, "83FB0E0F8FCB0000000F84", "33C089442410E9BF090000" },
};

static const HookPatchItem g_setHangarCameraPatch[] =
{
	{ 0x5F0A0, "81C76A04000081ED1009000081EE2C010000", "565557E8788E14005F5D5E90909090909090" },
	{ 0x5F7E8, "B7FC4500", "7EFC4500" },
	{ 0x5F7EC, "CFFE4500", "7EFC4500" },
	{ 0x5F7F8, "26FD4500", "7EFC4500" },
	{ 0x5F7FC, "5FFD4500", "7EFC4500" },
	{ 0x5F804, "4DFF4500", "7EFC4500" },
	{ 0x5F86C, "08004600", "7EFC4500" },
	{ 0x5F870, "41004600", "7EFC4500" },
	{ 0x5F874, "7A004600", "7EFC4500" },
	{ 0x5F87C, "E2004600", "7EFC4500" },
	{ 0x5F880, "CD014600", "7EFC4500" },
	{ 0x5F884, "06024600", "7EFC4500" },
	{ 0x5F888, "3C024600", "7EFC4500" },
	{ 0x5F88C, "6F024600", "7EFC4500" },
};

static const HookPatchItem g_setHangarMapPatch[] =
{
	{ 0x55345, "68C896000068FFFFFF7F68", "58E8D52B1500E9BD030000" },
};

static const HookPatchItem g_setFamilyHangarMapPatch[] =
{
	{ 0x55873, "688877000068FFFFFF7F68", "58E8A7261500E98E020000" },
};

static const HookPatchItem g_setCraftElevationPatch[] =
{
	{ 0x61A49, "83F804740F50E8CC310200", "50E8E164140083C404C390" },
	{ 0x55183, "83F804741C50E892FA0200", "50E8B7C8000083C404EB1B" },
	{ 0x562F4, "83F804741050E821E90200", "50E846B7000083C404EB0F" },
	{ 0x5AF8C, "83F804741650E8899C0200", "50E8AE6A000083C404EB15" },
	{ 0x5B203, "83F804741650E8129A0200", "50E83768000083C404EB15" },
	{ 0x5B350, "83F804741650E8C5980200", "50E8EA66000083C404EB15" },
	{ 0x5BDA4, "83F804741050E8718E0200", "50E8965C000083C404EB0F" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that selects the hangar type", g_selectHangarTypePatch),
	MAKE_HOOK_PATCH("To call the hook that selects the inside animation", g_selectInsideAnimationPatch),
	MAKE_HOOK_PATCH("To call the hook that selects the hangar model index", g_selectHangarModelIndexPatch),
	MAKE_HOOK_PATCH("To call the hook that loads hangar opt", g_loadHangarOptPatch),
	MAKE_HOOK_PATCH("To remove the opt filter", g_removeOptFilterPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the hangar camera", g_setHangarCameraPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the hangar map", g_setHangarMapPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the family hangar map", g_setFamilyHangarMapPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the craft elevation", g_setCraftElevationPatch),
};
