#pragma once
#include "hook_function.h"

#include "weapon.h"

#include "config.h"

class HooksConfig
{
public:
	HooksConfig()
	{
		auto lines = GetFileLines("hook_weapon_rate.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_weapon_rate");
		}

		this->IsWarheadLockHookEnabled = GetFileKeyValueInt(lines, "IsWarheadLockHookEnabled", 0) != 0;
		this->IsProjectileThreatHookEnabled = GetFileKeyValueInt(lines, "IsProjectileThreatHookEnabled", 0) != 0;
	}

	bool IsWarheadLockHookEnabled;
	bool IsProjectileThreatHookEnabled;
};

static HooksConfig g_hooksConfig;

static const HookFunction g_hookFunctions[] =
{
	{ 0x491656, WeaponDechargeHook },
	{ 0x4908D4, WeaponRechargeHook },
	{ 0x49186B, WeaponCooldownTimeHook },
	{ 0x4FBD05, ModelEnergyTransfer1Hook },
	{ 0x4FCDFE, ModelEnergyTransfer2Hook },
	{ 0x4FD304, ModelEnergyTransfer3Hook },
	{ 0x4FCE14, ModelEnergyTransferShieldToLaser1Hook },
	{ 0x4FCEC0, ModelEnergyTransferShieldToLaser2Hook },
	{ 0x4FD2C5, ModelEnergyTransferLaserToShield1Hook },
	{ 0x4FD340, ModelEnergyTransferLaserToShield2Hook },
	{ 0x4A7B28, ModelAIMaxTorpedoCountHook },
	{ 0x40E8BE, ModelIsImpactSpinningEnabledHook },
	{ 0x40C32A, ModelImpactSpinningFactorHook },

	{ 0x4E274A, WeaponDurationOffsetHook },
	{ 0x4E19C8, WeaponFireRatioHook },
	{ 0x4A8D42, WeaponRange_004A8D42_Hook },
	{ 0x4A8FB6, WeaponRange_004A8FB6_Hook },
	{ 0x4A91B2, WeaponRange_004A91B2_Hook },
	{ 0x4A9B15, WeaponRange_004A9B15_Hook },
	{ 0x4E2119, WeaponRange_004E2119_Hook },
	{ 0x4E241F, WeaponSpreadHook },

	{ 0x491AAF, WeaponFireRateHook },
	{ 0x4E4D8B, GetWeaponDurationHook },
	{ 0x40D54C, WeaponDuration_0040D54C_Hook },
	{ 0x40D570, WeaponDuration_0040D570_Hook },
	{ 0x4922DB, WeaponDuration_004922DB_Hook },
	{ 0x4922E3, WeaponDuration_004922E3_Hook },
	{ 0x49312E, WeaponDuration_0049312E_Hook },
	{ 0x493136, WeaponDuration_00493136_Hook },
	{ 0x493616, WeaponDuration_00493616_Hook },
	{ 0x493628, WeaponDuration_00493628_Hook },
	{ 0x497454, WeaponScore_00497454_Hook },
	{ 0x4D9B16, WeaponScore_004D9B16_Hook },
	{ 0x4D9B4E, WeaponScore_004D9B4E_Hook },
	{ 0x40D553, WeaponSpeed_0040D553_Hook },
	{ 0x49221A, WeaponSpeed_0049221A_Hook },
	{ 0x492235, WeaponSpeed_00492235_Hook },
	{ 0x492EE5, WeaponSpeed_00492EE5_Hook },
	{ 0x4930F3, WeaponSpeed_004930F3_Hook },
	{ 0x49359A, WeaponSpeed_0049359A_Hook },
	{ 0x4935C3, WeaponSpeed_004935C3_Hook },
	{ 0x49424E, WeaponSpeed_0049424E_Hook },
	{ 0x4942CF, WeaponSpeed_004942CF_Hook },
	{ 0x496903, WeaponSpeed_00496903_Hook },
	{ 0x4B5F76, WeaponSpeed_004B5F76_Hook },
	{ 0x4E22F7, WeaponSpeed_004E22F7_Hook },
	{ 0x4E2678, WeaponSpeed_004E2678_Hook },
	{ 0x4E26A2, WeaponSpeed_004E26A2_Hook },
	{ 0x4E4D75, WeaponSpeed_004E4D75_Hook },
	{ 0x40F511, WeaponPower_0040F511_Hook },
	{ 0x492279, WeaponPower_00492279_Hook },
	{ 0x4922A2, WeaponPower_004922A2_Hook },
	{ 0x4922BA, WeaponPower_004922BA_Hook },
	{ 0x493121, WeaponPower_00493121_Hook },
	{ 0x4935E1, WeaponPower_004935E1_Hook },
	{ 0x4935FE, WeaponPower_004935FE_Hook },
	{ 0x494265, WeaponPower_00494265_Hook },
	{ 0x4A7983, WeaponPower_004A7983_Hook },
	{ 0x4A7D68, WeaponPower_004A7D68_Hook },
	{ 0x4A7E46, WeaponPower_004A7E46_Hook },
	{ 0x4E2691, WeaponPower_004E2691_Hook },
	{ 0x4E4DAF, WeaponPower_004E4DAF_Hook },
	{ 0x519C36, WeaponPower_00519C36_Hook },

	{ 0x4915F6, WeaponRackHook },
	{ 0x491C03, WeaponRackHook },
	{ 0x4E1C49, WeaponHardpointHook },
	{ 0x431ED7, GetNearestWeaponMeshHook },
	{ 0x433002, MirrorHardpointHook },
	{ 0x491EC7, PlayerFireHook },
	{ 0x41BE55, WarheadCapacity_0041BE55_Hook },
	{ 0x45CBFD, WarheadCapacity_0045CBFD_Hook },
	{ 0x460904, WarheadCapacity_00460904_Hook },
	{ 0x4B1426, WarheadCapacity_004B1426_Hook },
	{ 0x42C6AE, EnergySpeedFactorHook },

	{ 0x4A77C6, LasersLinkAIFighterHook },
	{ 0x491354, LasersLinkPlayerFighterHook },
	{ 0x4FCA77, LasersLinkPlayerKeyXHook },
	{ 0x4A78FE, PreventAILasersIonWhenDisablingHook },
	{ 0x4A7A08, PreventAIIonWhenNotDisablingHook },
	{ 0x4FC9BF, PreventPlayerLinkedLasersIonHook },

	{ g_hooksConfig.IsWarheadLockHookEnabled ? 0x48FE18 : 0, WarheadLockDistanceHook },

	{ g_hooksConfig.IsProjectileThreatHookEnabled ? 0x4A8CCE : 0, ProjectileTurretThreatHook },

	{ 0x46AC8F, DrawEnergyBarHook },
	{ 0x469EA8, SetupLaserChargePositionsHook },
	{ 0x491EDB, SetWeaponsCount1Hook },
	{ 0x41585F, SetWeaponsCount2Hook },
	{ 0x4609B2, HangarWarheadReloadHook },
	{ 0x461490, HangarWarheadCountHook },
};

static const HookPatchItem g_weaponRatePatch[] =
{
	{ 0x090A51, "A1C4337B008A54080484D2", "E8CA741100E98000000090" },
	{ 0x08FCCE, "6683FA0574096683FA078D1436", "52E84C8211005A0FAFD6EB0690" },
	{ 0x090C66, "A1FC0D910083FBFF", "E8B5721100EB2990" },
	{ 0x0FB100, "66F7D81BC0", "E81BCE0A00" },
	{ 0x0FC1F9, "66F7D81BC0", "E822BD0A00" },
	{ 0x0FC6FF, "751283E21F", "E81CB80A00" },
	{ 0x0FC20F, "7F038B45FC", "E80CBD0A00" },
	{ 0x0FC2BB, "66837DF464734B", "E860BC0A009090" },
	{ 0x0FC6C0, "7E07C745F820030000", "E85BB80A0090909090" },
	{ 0x0FC73B, "66837DF4640F83ED000000", "E8E0B70A00909090909090" },
	{ 0x0A6F23, "C744243810000000", "E8F80F1000909090" },
	{ 0x00DCB9, "A015548000", "E872A21900" },
	{ 0x00B725, "897C2410663D00807202F7D8", "E8F6C71900897C241090EB0F" },
};

static const HookPatchItem g_weaponProfilesPatch[] =
{
	// weapons misc
	{ 0x0E1B45, "8BB08D00000083C63B89B08D000000", "E8D6630C0090909090909090909090" },
	{ 0x0E0DC3, "668B9C51D0B55B00", "E858710C00909090" },
	{ 0x0A813D, "8B8C91C4B55B00", "E8DEFD0F009090" },
	{ 0x0A83B1, "8B8C90C4B55B00", "E86AFB0F009090" },
	{ 0x0A85AD, "8B8488C4B55B00", "E86EF90F009090" },
	{ 0x0A8F10, "8B8490C4B55B00", "E80BF00F009090" },
	{ 0x0E1514, "8BB48AC4B55B00", "E8076A0C009090" },
	{ 0x0E181A, "837C081FFF747C", "E801670C009090" },

	// Warheads fire rate
	{ 0x090EAA, "8B15C4337B00", "E87170110090" },
	// s_ExeWeaponDurationIntegerPart, s_ExeWeaponDurationDecimalPart
	{ 0x0E4186, "E825C1FAFF", "E8953D0C00" },
	{ 0x00C947, "668B8160655B00", "E8D4B519009090" },
	{ 0x00C96B, "668B8198655B00", "E8B0B519009090" },
	{ 0x0916D6, "668B1C4530635B00", "E845681100909090" },
	{ 0x0916DE, "668B044568635B00", "E83D681100909090" },
	{ 0x092529, "668B3C4530635B00", "E8F2591100909090" },
	{ 0x092531, "668B044568635B00", "E8EA591100909090" },
	{ 0x092A11, "668B347530635B00", "E80A551100909090" },
	{ 0x092A23, "668B045568635B00", "E8F8541100909090" },
	// s_ExeWeaponScore
	{ 0x09684F, "668B144DF8635B00", "E8CC161100909090" },
	{ 0x0D8F11, "668B1C7DF8635B00", "E80AF00C00909090" },
	{ 0x0D8F49, "668B0C55F8635B00", "E8D2EF0C00909090" },
	// s_ExeWeaponSpeed
	{ 0x00C94E, "668B9928655B00", "E8CDB519009090" },
	{ 0x091615, "668B1C45F8625B00", "E806691100909090" },
	{ 0x091630, "668B1C45F8625B00", "E8EB681100909090" },
	{ 0x0922E0, "668B1542655B00", "E83B5C11009090" },
	{ 0x0924EE, "668B1445F8625B00", "E82D5A1100909090" },
	{ 0x092995, "668B1475F8625B00", "E886551100909090" },
	{ 0x0929BE, "66031475F8625B00", "E85D551100909090" },
	{ 0x093649, "66A15C655B00", "E8D248110090" },
	{ 0x0936CA, "668B0D5C655B00", "E8514811009090" },
	{ 0x095CFE, "668B0D42655B00", "E81D2211009090" },
	{ 0x0B5371, "668B344DF8625B00", "E8AA2B0F00909090" },
	{ 0x0E16F2, "668B044DF8625B00", "E829680C00909090" },
	{ 0x0E1A73, "668B047D28655B00", "E8A8640C00909090" },
	{ 0x0E1A9D, "668B047D28655B00", "E87E640C00909090" },
	{ 0x0E4170, "668B047DF8625B00", "E8AB3D0C00909090" },
	// s_ExeWeaponPower
	{ 0x00E90C, "8B048558605B00", "E80F9619009090" },
	{ 0x091674, "8B048558605B00", "E8A76811009090" },
	{ 0x09169D, "8B0DC4337B00", "E87E68110090" },
	{ 0x0916B5, "8B048558605B00", "E8666811009090" },
	{ 0x09251C, "8B148558605B00", "E8FF5911009090" },
	{ 0x0929DC, "8B04B558605B00", "E83F5511009090" },
	{ 0x0929F9, "8B0CB558605B00", "E8225511009090" },
	{ 0x093660, "A120655B00", "E8BB481100" },
	{ 0x0A6D7E, "8B048558605B00", "E89D1110009090" },
	{ 0x0A7163, "8B048558605B00", "E8B80D10009090" },
	{ 0x0A7241, "8B048558605B00", "E8DA0C10009090" },
	{ 0x0E1A8C, "8B04BDB8645B00", "E88F640C009090" },
	{ 0x0E41AA, "8B14BD58605B00", "E8713D0C009090" },
	{ 0x119031, "8B150C655B00", "E8EAEE080090" },
};

static const HookPatchItem g_weaponRacksPatch[] =
{
	{ 0x0909F1, "E8BA080000", "E83A751100" },
	{ 0x090FFE, "E8AD020000", "E82D6F1100" },
	{ 0x0E1044, "E88763FAFF", "E8E76E0C00" },
	{ 0x0312D2, "E8A9570500", "E8596C1700" },
	{ 0x0323FD, "81FF000100007D40", "E81E5B1700909090" },
};

static const HookPatchItem g_fireMeshPatch[] =
{
	{ 0x0912C2, "BBFFFF0000", "E8596C1100" },
};

static const HookPatchItem g_warheadProfilesPatch[] =
{
	{ 0x01B241, "668B1445C0125B00", "8BD0909090909090" },
	{ 0x01B250, "E83B8E0700", "E8DBCC1800" },
	{ 0x05BFF8, "E893800300", "E833BF1400" },
	{ 0x05FCFF, "E88C430300", "E82C821400" },
	{ 0x0B0821, "E86A38FEFF", "E80A770F00" },
};

static const HookPatchItem g_energyProfilesPatch[] =
{
	{ 0x02BAA9, "6683F90775076800100000", "E882C41700509090909090" },
};

static const HookPatchItem g_weaponLinkingProfilesPatch[] =
{
	{ 0x0A6BC1, "8B15CCA17C00", "E85A13100090" },
	{ 0x09074F, "8A840DB6010000", "E8DC7711009090" },
	{ 0x0FBE72, "8A45F033C9", "E8A9C00A00" },
	{ 0x0A6CF9, "0F8234010000", "E82212100090" },
	{ 0x0A6E03, "8B1578F38B00", "E81811100090" },
	{ 0x0FBDBA, "660FB68403B6010000", "E861C10A0090909090" },
};

#define WarheadLockPatchItem(offset, from, to) { (offset), g_hooksConfig.IsWarheadLockHookEnabled ? (from) : "", g_hooksConfig.IsWarheadLockHookEnabled ? (to) : "" }

static const HookPatchItem g_warheadLockDistancePatch[] =
{
	WarheadLockPatchItem(0x08F213, "BBAD8D0100722C", "E8088D11009090"),
};

#define ProjectileThreatPatchItem(offset, from, to) { (offset), g_hooksConfig.IsProjectileThreatHookEnabled ? (from) : "", g_hooksConfig.IsProjectileThreatHookEnabled ? (to) : "" }

static const HookPatchItem g_projectileTurretThreatPatch[] =
{
	ProjectileThreatPatchItem(0x0A80C9, "E8D29AFFFF", "E852FE0F00"),
};

static const HookPatchItem g_drawEnergyBarPatch[] =
{
	{ 0x06A08A, "BE01000000", "E891DE1300" },
	{ 0x0692A3, "A1C81C8C00", "E878EC1300" },
};

static const HookPatchItem g_weaponsCountPatch[] =
{
	{ 0x0912D6, "8D0440C745D8060000008D34818A82F0645B0084C08D7E0C", "E8456C1100C745D8060000008A82F0645B0084C090909090" },
	{ 0x014C5A, "8D047FC1E002", "E8C132190090" },
	{ 0x01B25C, "B001", "9090" },
	{ 0x01B288, "B001", "9090" },
	{ 0x01B2A6, "B009", "9090" },
	{ 0x05C004, "B001", "9090" },
	{ 0x05C034, "B001", "9090" },
	{ 0x05C042, "B009", "9090" },
	{ 0x05FD15, "C744243801000000", "9090909090909090" },
	{ 0x05FD7B, "C744243801000000", "9090909090909090" },
	{ 0x05FD95, "C744243809000000", "9090909090909090" },
	{ 0x0B0835, "896C2414", "90909090" },
	{ 0x0B0877, "896C2414", "90909090" },
};

static const HookPatchItem g_weaponsShipTypeFilterPatch[] =
{
	{ 0x00E825, "3C0474083C050F8508010000", "909090909090909090909090" },
};

static const HookPatchItem g_hangarWarheadReloadPatch[] =
{
	{ 0x05FDAD, "663BCA7369", "E86E811400" },
	{ 0x06088B, "89156C6D6800", "E89076140090" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that defines weapon decharge and recharge rates", g_weaponRatePatch),
	MAKE_HOOK_PATCH("To call the hook that sets the weapons profiles", g_weaponProfilesPatch),
	MAKE_HOOK_PATCH("To call the hook that filters weapon racks", g_weaponRacksPatch),
	MAKE_HOOK_PATCH("To call the hook that checks mesh index on fire", g_fireMeshPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the warheads profiles", g_warheadProfilesPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the energy profiles", g_energyProfilesPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the weapon linking profiles", g_weaponLinkingProfilesPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the warhead lock distance", g_warheadLockDistancePatch),
	MAKE_HOOK_PATCH("To call the hook that sets the projectile turret threat", g_projectileTurretThreatPatch),
	MAKE_HOOK_PATCH("To call the hook that draws the energy bar", g_drawEnergyBarPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the weapons count", g_weaponsCountPatch),
	MAKE_HOOK_PATCH("To call the hook that filters ship type for weapons", g_weaponsShipTypeFilterPatch),
	MAKE_HOOK_PATCH("To call the hook that reloads warheads in hangar", g_hangarWarheadReloadPatch),
};
