#pragma once
#include "hook_function.h"

#include "weapon.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x46148A, SelectedWarheadMenuHook },
	{ 0x45CAAD, WarheadCountMenuHook },
	{ 0x4B13FD, BoardReloadTypeHook },
	{ 0x4FC68C, WeaponSwitchWarheadCountHook },
	{ 0x4FC6A5, WeaponSwitchSetIndexHook },
	{ 0x461338, SecondaryWeaponsSelectionHook },
	{ 0x41BBB6, WeaponRacksCountHook },
};

static const HookPatchItem g_weaponPatch[] =
{
	{ 0x060798, "0F872D020000", "0F8711000000" },
	{ 0x060885, "8B0DFC0D9100", "E89676140090" },
	{ 0x05BFCB, "7516", "EB16" },
	{ 0x05BEA8, "A1FC0D9100", "E873C01400" },
	{ 0x05BF04, "FEC1", "9090" },
	{ 0x0B0801, "7505", "EB05" },
	{ 0x0B07F8, "660FB63455EEDC8000", "E823770F0090909090" },
	//{ 0x073B6A, "03C1", "03C8" },
	//{ 0x073B75, "6685C0", "6685C9" },
	//{ 0x073B7E, "6683FE01", "6685C090" },
	//{ 0x073B82, "0F8542010000", "0F854C010000" },
	{ 0x0FBA87, "3A83D10100007218", "E894C40A0090EB18" },
	{ 0x0FBAA0, "C68613958B0000", "E87BC40A009090" },
	{ 0x060733, "66837A020C754C", "E8E87714009090" },
};

static const HookPatchItem g_missilePatch[] =
{
	{ 0x0A7A0B, "740A", "EB0A" },
	{ 0x01AFB1, "8882AE010000", "E86ACF180090" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that sets the warhead menu", g_weaponPatch),
	MAKE_HOOK_PATCH("To call the hook that fixes missile count", g_missilePatch),
};
