#pragma once
#include "hook_function.h"

#include "shield.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x49032F, ShieldRechargeHook },
	{ 0x49066F, ShieldRechargeHook },
	{ 0x48FC39, CraftUpdateHook },
	{ 0x4906D4, CraftUpdateMulShieldHook },
	{ 0x490687, CraftUpdateTimeHook },
	{ 0x4905A0, LasersEnergyToShieldsHook },
};

static const HookPatchItem g_shieldPatch[] =
{
	{ 0x090254, "0F8510F8FFFF", "90E910F8FFFF" },
	{ 0x08F72A, "C744241814000000", "E8F1871100909090" },
	{ 0x08FA6A, "C744241814000000", "E8B1841100909090" },
	{ 0x08F034, "8B0D78F38B00", "E8E78E110090" },
	//{ 0x08FACF, "0FAF542418", "E84C841100" },
	{ 0x08FA82, "A1C4337B00", "E899841100" },
	{ 0x08F99B, "66396C2418736C", "E8808511009090" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that defines shield recharge rate", g_shieldPatch),
};
