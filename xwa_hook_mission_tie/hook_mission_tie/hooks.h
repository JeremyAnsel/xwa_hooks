#pragma once
#include "hook_function.h"

#include "tie.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x415E1F, TieHook },
	{ 0x41D304, CreateObjectHook },
	{ 0x41FF48, StatsProfiles_L0041FA00_Speed_Hook },
	{ 0x42CFE7, StatsProfiles_L0042CF90_Acceleration_Hook },
	{ 0x42D020, StatsProfiles_L0042CF90_Acceleration_Hook },
	{ 0x42D11D, StatsProfiles_L0042CF90_Deceleration_Hook },
	{ 0x42D15D, StatsProfiles_L0042CF90_Deceleration_Hook },
	{ 0x42D0BB, StatsProfiles_L0042CF90_SpeedIncrement_Hook },
	{ 0x42D1E8, StatsProfiles_L0042CF90_SpeedDecrement_Hook},
	{ 0x40B88E, StatsProfiles_L0040B4F0_HullStrength_Hook },
	{ 0x41063E, StatsProfiles_SystemStrength_Hook },
	{ 0x4107C3, StatsProfiles_SystemStrength_Hook },
	{ 0x473B50, StatsProfiles_SystemStrength_Hook },
	{ 0x4A7CFD, StatsProfiles_SystemStrength_Hook },
	{ 0x4F3B5D, StatsProfiles_SystemStrength_Hook },
	{ 0x41F374, StatsProfiles_ShieldStrength_Hook1 },
	{ 0x4606E3, StatsProfiles_ShieldStrength_Hook1 },
	{ 0x473AAB, StatsProfiles_ShieldStrength_Hook1 },
	{ 0x490498, StatsProfiles_ShieldStrength_Hook2 },
	{ 0x490707, StatsProfiles_ShieldStrength_Hook2 },
	{ 0x49076B, StatsProfiles_ShieldStrength_Hook2 },
	{ 0x4907CA, StatsProfiles_ShieldStrength_Hook2 },
	{ 0x49082B, StatsProfiles_ShieldStrength_Hook2 },
	{ 0x490E7A, StatsProfiles_ShieldStrength_Hook1 },
	{ 0x4D5FB2, StatsProfiles_L004D5AE0_ShieldStrength_Hook },
	{ 0x4FD00C, StatsProfiles_ShieldStrength_Hook1 },
	{ 0x4FD2A5, StatsProfiles_ShieldStrength_Hook1 },
	{ 0x4BA8AF, StatsProfiles_HasHyperdrive_Hook },
	{ 0x4BCA6F, StatsProfiles_HasHyperdrive_Hook },
	{ 0x4BDE11, StatsProfiles_HasHyperdrive_Hook },
	{ 0x45C35E, MissionIdRedAlertHook },
	{ 0x498D33, MissionIdSkipHyperspacedMessagesHook },
	{ 0x501472, MissionIdForcePlayerInTurret1Hook },
	{ 0x4F94DA, MissionIdForcePlayerInTurret2Hook },
};

static const HookPatchItem g_overrideTiePatch[] =
{
	{ 0x01521A, "E8E1A40000", "E8112D1900" },
	{ 0x01C6FF, "668B442428", "E82CB81800" },
};

static const HookPatchItem g_craftsCountPerRegionPatch[] =
{
	{ 0x014C28, "C705944B7D0060000000C1E305", "C705944B7D00C0000000C1E306" },
	{ 0x014BF2, "8D1CF633D2C1E302", "8D1C7633D2C1E306" },
};

static const HookPatchItem g_statsProfilesPatch[] =
{
	// L0041FA00 - speed
	{ 0x01F343, "E838250800", "E8D88B1800" },
	// L004B8A60 - speed
	{ 0x0B80DA, "668BB440A0B45B00", "668BB2C400000090" },
	{ 0x0B810E, "8D0CD08D04C8668BBC40A0B45B00", "909090909090668BB9C400000090" },
	// L0042CF90 - acceleration
	{ 0x02C3E2, "668B8440A2B45B00", "E849BB1700909090" },
	{ 0x02C41B, "668B9440A2B45B00", "E810BB17008BD090" },
	// L0042CF90 - deceleration
	{ 0x02C518, "668B9440A4B45B00", "E813BA17008BD090" },
	{ 0x02C558, "668B8440A4B45B00", "E8D3B91700909090" },
	// L0042CF90 -  speed increment
	{ 0x02C4B6, "66FF8285000000", "E865BA17009090" },
	// L0042CF90 - speed decrement
	{ 0x02C5E3, "66FF8885000000", "E838B917009090" },
	// L0040B4F0 - hull strength
	{ 0x00AC89, "8B844095B45B00", "E8A2D219009090" },
	// L0040F230 - system strength
	{ 0x00FA35, "8D04D0668B84409DB45B00", "8B450850E8F28419005A90" },
	{ 0x00FBBA, "8D04D0668B84409DB45B00", "8B450850E86D8319005A90" },
	// L00473590 - system strength
	{ 0x072F4A, "668B84409DB45B00", "55E8E04F13005A90" },
	// L004A75B0 - system strength
	{ 0x0A70EE, "8D3CF8896C24308D04F8668B84409DB45B00", "896C2430A1A0A17C0050E8330E10005F9090" },
	// L004F3A80 - system strength
	{ 0x0F2F57, "8D04D0663B8C409DB45B00", "57E8D34F0B005A663BC890" },
	// L0041EF60 - shield strength
	{ 0x01E76E, "8B8C408FB45B00", "53E8AC97180059" },
	// L00460650 - shield strength
	{ 0x05FAD8, "8B8C408FB45B00898BA20100008D84408FB45B008B008983A6010000", "A108BC680050E83D84140059898BA2010000898BA601000090909090" },
	// L00473590 - shield strength
	{ 0x072EA5, "8B84408FB45B00", "55E87550130058" },
	// L0048FC00 - shield strength
	{ 0x08F892, "8B84408FB45B00", "53E88886110058" },
	{ 0x08FB01, "8B84408FB45B00", "55E81984110058" },
	{ 0x08FB65, "8B84408FB45B00", "55E8B583110058" },
	{ 0x08FBC4, "8BB4408FB45B00", "55E8568311005E" },
	{ 0x08FC25, "8B84408FB45B00", "55E8F582110058" },
	// L00490E70 - shield strength
	{ 0x090274, "8B15C4337B0025FFFF0000", "50E8A67C110058D1E0C390" },
	// L004D5AE0 - shield strength
	{ 0x0D53AD, "8B868FB45B00", "E87E2B0D0090" },
	// L004FBA80 - shield strength
	{ 0x0FC400, "8D14C88D04D08B84408FB45B00", "8B86E0948B0050E814BB0A0058" },
	{ 0x0FC68A, "8D0C808D14C88B8BA60100008D04D08B93A20100008B84408FB45B00", "8B86E0948B008B8BA60100009090908B93A201000050E87BB80A0058" },
	// L0041A960 - has hyperdrive
	{ 0x01B861, "7529", "EB29" },
	// L004BA6F0 - has hyperdrive
	{ 0x0B9CAA, "389C408CB45B00", "E881E20E003AC3" },
	// L004BCA30 - has hyperdrive
	{ 0x0BBE6A, "8A94408CB45B00", "E8C1C00E008AD0" },
	// L004BDDF0 - has hyperdrive
	{ 0x0BD20C, "8A8C408CB45B00", "E81FAD0E008AC8" },
	// L0041A960 - has shield generator
	{ 0x01BA3D, "754F", "EB4F" },
};

static const HookPatchItem g_missionIdsCheckPatch[] =
{
	{ 0x05B759, "83FE147523", "E8C2C71400" },
	{ 0x05B6EB, "7416", "EB16" },
	{ 0x09812A, "83F9318B4C24107522", "8B4C2410E8EDFD1000" },
	{ 0x10086D, "0F8411040000", "E8AE760A0090" },
	{ 0x0F88D5, "390C858E2AAE007539", "E846F60A0090909090" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that override tie mission", g_overrideTiePatch),
	MAKE_HOOK_PATCH("'crafts count per region' patch", g_craftsCountPerRegionPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the stats profiles", g_statsProfilesPatch),
	MAKE_HOOK_PATCH("To call the hook that checks mission ids", g_missionIdsCheckPatch),
};
