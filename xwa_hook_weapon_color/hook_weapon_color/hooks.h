#pragma once
#include "hook_function.h"

#include "weapon.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x49204E, WeaponColorHook },
	{ 0x4E25E6, WeaponColorCapitalShipHook },
	{ 0x4E2299, WeaponAIColorHook },
	{ 0x4E86D6, WeaponImpactColorHook },
	{ 0x40AFBA, WeaponImpactColorSetIndexHook },
	//{ 0x40B257, WeaponImpactColorSetIndexHook },
	{ 0x46AB2E, WeaponEnergyBarLaserColorHook },
	{ 0x46AB80, WeaponEnergyBarIonColorHook },
	{ 0x4F28C8, WeaponLightColorHook },
};

static const HookPatchItem g_weaponColorPatch[] =
{
	{ 0x091449, "A1C4337B00", "E8E26A1100" },
	{ 0x0E19E1, "A1C4337B00", "E84A650C00" },
	{ 0x0E1692, "8A8848655B0080F9FF741B", "5756E887680C005E5FEB1B" },
	{ 0x0E7ACF, "663D2802750E668B4608", "5056E85A040C00EB1690" },
	{ 0x00A3B5, "E886E40D00", "E866DB1900" },
	//{ 0x00A652, "E8E9E10D00", "E8C9D81900" },
	{ 0x069F29, "6681BDB00100001801", "E8F2DF1300EB539090" },
	{ 0x069F7B, "C7442418926900FF", "E8A0DF1300909090" },
	{ 0x0F1CC2, "3D190100007F403D18010000", "56E858620B005EE992020000" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that defines weapon color", g_weaponColorPatch),
};
