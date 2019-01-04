#pragma once
#include "hook_function.h"

#include "weapon.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x49204E, WeaponColorHook },
	{ 0x4E25E6, WeaponColorCapitalShipHook },
};

static const HookPatchItem g_weaponColorPatch[] =
{
	{ 0x091449, "A1C4337B00", "E8E26A1100" },
	{ 0x0E19E1, "A1C4337B00", "E84A650C00" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that defines weapon color", g_weaponColorPatch),
};
