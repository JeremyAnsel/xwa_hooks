#pragma once
#include "hook_function.h"

#include "shield.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x49032F, ShieldRechargeHook },
	{ 0x49066F, ShieldRechargeHook },
};

static const HookPatchItem g_shieldPatch[] =
{
	{ 0x090254, "0F8510F8FFFF", "90E910F8FFFF" },
	{ 0x08F72A, "C744241814000000", "E8F1871100909090" },
	{ 0x08FA6A, "C744241814000000", "E8B1841100909090" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that defines shield recharge rate", g_shieldPatch),
};
