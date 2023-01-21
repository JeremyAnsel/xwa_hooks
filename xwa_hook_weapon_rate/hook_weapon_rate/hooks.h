#pragma once
#include "hook_function.h"

#include "weapon.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x491656, WeaponDechargeHook },
	{ 0x4908D4, WeaponRechargeHook },
	{ 0x49186B, WeaponCooldownTimeHook },
	{ 0x4915F6, WeaponRackHook },
	{ 0x491C03, WeaponRackHook },
	{ 0x4E1C49, WeaponHardpointHook },
};

static const HookPatchItem g_weaponRatePatch[] =
{
	{ 0x090A51, "A1C4337B008A54080484D2", "E8CA741100E98000000090" },
	{ 0x08FCCE, "6683FA0574096683FA078D1436", "52E84C8211005A0FAFD6EB0690" },
	{ 0x090C66, "A1FC0D910083FBFF", "E8B5721100EB2990" },
};

static const HookPatchItem g_weaponRacksPatch[] =
{
	{ 0x0909F1, "E8BA080000", "E83A751100" },
	{ 0x090FFE, "E8AD020000", "E82D6F1100" },
	{ 0x0E1044, "E88763FAFF", "E8E76E0C00" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that defines weapon decharge and recharge rates", g_weaponRatePatch),
	MAKE_HOOK_PATCH("To call the hook that filters weapon racks", g_weaponRacksPatch),
};
