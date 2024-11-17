#pragma once
#include "hook_function.h"

#include "weapon.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x48FE10, WarheadLockDistance0Hook },
	{ 0x48FE46, WarheadLockDistance1Hook },
	{ 0x48FE3F, WarheadLockDistance2Hook },
	{ 0x48FF62, WarheadLockTime0Hook },
	{ 0x46BE45, WarheadLockTime1Hook },
	{ 0x43C9D1, WarheadLockTime2Hook },
	{ 0x491CBB, WarheadLockStrengthHook },
	{ 0x4A7AFF, WarheadAILockDistance0Hook },
	{ 0x4A7B35, WarheadAILockDistance1Hook },
	{ 0x4A80E9, WarheadAILockTimeFactorHook },
};

static const HookPatchItem g_warheadLockPatch[] =
{
	{ 0x08F20B, "8B0DC4337B00", "E8108D110090" },
	{ 0x08F241, "BB6CBA0300", "E8DA8C1100" },
	{ 0x08F23A, "BB487C0200", "E8E18C1100" },
	{ 0x08F35D, "25FFFF0000", "E8BE8B1100" },
	{ 0x06B240, "81E6FFFF0000", "E8DBCC130090" },
	{ 0x03BDCC, "81E1FFFF0000", "E84FC1160090" },
	{ 0x0910B6, "B8BF3CB622", "E8656E1100" },
	{ 0x0A6EFA, "C7442428AD8D0100", "E821101000909090" },
	{ 0x0A6F30, "056CBA0300", "E8EB0F1000" },
	{ 0x0A74E4, "2BF0C1E603", "E8370A1000" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that sets warhead lock settings", g_warheadLockPatch),
};
