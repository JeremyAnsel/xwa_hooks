#pragma once
#include "hook_function.h"

#include "weapon.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x491656, WeaponDechargeHook },
	{ 0x4908D4, WeaponRechargeHook },
};

static const HookPatchItem g_weaponRatePatch[] =
{
	{ 0x090A51, "A1C4337B008A54080484D2", "E8CA741100E98000000090" },
	{ 0x08FCCE, "6683FA0574096683FA078D1436", "52E84C8211005A0FAFD6EB0690" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that defines weapon decharge and recharge rates", g_weaponRatePatch),
};
