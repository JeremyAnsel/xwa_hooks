#pragma once
#include "hook_function.h"

#include "mission.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x431D81, MissionObjectsHook },
	{ 0x50E7E2, MissionObjectsHook },
	{ 0x43161F, MissionObjectsHook },
	{ 0x43185E, MissionObjectsHook },
	{ 0x43190A, MissionObjectsHook },
	{ 0x431AAD, MissionObjectsHook },
	{ 0x501D95, CraftTurretHook },

	{ 0x491057, LaserShootHook },
	{ 0x49106B, LaserShootHook },
	{ 0x491090, LaserShootHook },
	{ 0x4910EF, LaserShootHook },
	{ 0x491181, LaserShootHook },
	{ 0x4911EE, WarheadShootHook },

	{ 0x415AFB, SetXwaCraftsHook },
	{ 0x41D304, ClearCraftDataHook },
	{ 0x41EE36, CloneObjectHook },
	{ 0x4FACED, ComputeCraftAnglesHook },
	{ 0x48134D, SetRotationAngle1Hook },
	{ 0x48131D, SetRotationAngle2Hook },
	{ 0x483DF9, SetRotationAngle1Hook },
	{ 0x483DCC, SetRotationAngle2Hook },
	{ 0x4921A6, SetL00491EB0Angle1Hook },
	{ 0x4921CF, SetL00491EB0Angle2Hook },
	{ 0x49253F, SetL00491EB0Angle1Hook },
	{ 0x4925D5, SetL00491EB0Angle2Hook },
	{ 0x497863, SetL00497610Angle1Hook},
	{ 0x497922, SetL00497610Angle2Hook },
	{ 0x40D750, SetL0040D410Angle1Hook },
	{ 0x40D7E7, SetL0040D410Angle2Hook },
	{ 0x43FA5B, SetL0043F8E0Angle1Hook },
	{ 0x43FA75, SetL0043F8E0Angle2Hook },

	{ 0x497739, ReadTurretPositionXHook },
	{ 0x497742, ReadTurretPositionYHook },
	{ 0x497759, ReadTurretPositionZHook },
	{ 0x43FA0E, ReadTurretOrientationHook },
	{ 0x507BB0, ReadL00507B20HasTurretHook },
	{ 0x507BB0, ReadL004C3EF0HasTurretHook },

	{ 0x480D29, TurretIndex2TransformHook },
	{ 0x490EF9, TurretIndex2BlockedHook },
};

static const HookPatchItem g_loadMissionObjectsPatch[] =
{
	{ 0x03117C, "E8BFAB0900", "E8AF6D1700" },
	{ 0x10DBDD, "E85EE1FBFF", "E84EA30900" },
	{ 0x030A1A, "E821B30900", "E811751700" },
	{ 0x030C59, "E8E2B00900", "E8D2721700" },
	{ 0x030D05, "E836B00900", "E826721700" },
	{ 0x030EA8, "E893AE0900", "E883701700" },
};

static const HookPatchItem g_setCraftTurretPatch[] =
{
	{ 0x101190, "535556578B7C2414", "E88B6D0A00C39090" },
};

static const HookPatchItem g_filterTurretWeaponsPatch[] =
{
	{ 0x0FB941, "0F85B8160000", "909090909090" },
	{ 0x0903FE, "0F8591000000", "909090909090" },
	{ 0x090452, "E869020000", "E8C97A1100" },
	{ 0x090466, "E855020000", "E8B57A1100" },
	{ 0x09048B, "E830020000", "E8907A1100" },
	{ 0x0904EA, "E8D1010000", "E8317A1100" },
	{ 0x09057C, "E83F010000", "E89F791100" },
	{ 0x0905E9, "E802070000", "E832791100" },
};

static const HookPatchItem g_setTurretDataPatch[] =
{
	{ 0x014EF6, "E8F5870F00", "E835301900" },
	{ 0x01C6FF, "668B442428", "E82CB81800" },
	{ 0x01E22F, "B9FE000000F3A5A48B0DC4337B00", "5657E8EA9C180083C40890909090" },
	{ 0x0FA0E6, "0FBF933B958B008B75EC8B45F48955E0", "5355E833DE0A0083C408E9B20A000090" },
	{ 0x080747, "0FBF8C46BF030000", "56E8D37712005990" },
	{ 0x080717, "0FBF8C46C3030000", "56E8037812005990" },
	{ 0x0831F3, "0FBF8456BF030000", "56E8274D12005890" },
	{ 0x0831C6, "0FBF8456C3030000", "56E8544D12005890" },
	{ 0x0915A0, "668B9C41C1030000", "55E87A6911005B90" },
	{ 0x0915C9, "668B8441C5030000", "55E8516911005890" },
	{ 0x091939, "0FBF8C01C1030000", "55E8E16511005990" },
	{ 0x0919CF, "0FBF9408C5030000", "55E84B6511005A90" },
	{ 0x096C5D, "0FBF8C4EBF030000", "56E8BD1211005990" },
	{ 0x096D1C, "0FBF8C56C3030000", "56E8FE1111005990" },
	{ 0x00CB4A, "0FBF8451C1030000", "51E8D0B319005890" },
	{ 0x00CBE1, "0FBF8C42C5030000", "52E839B319005990" },
	{ 0x03EE55, "668B8C47BF030000", "56E8C59016005990" },
	{ 0x03EE6F, "668BB8C3030000", "56E8AB9016005F" },
};

static const HookPatchItem g_readExeCraftsTurretsPatch[] =
{
	{ 0x059F27, "668B8450D0B65B00", "B83D010000909090" },
	{ 0x061C33, "668B8440D2B65B00", "B83D010000909090" },
	{ 0x09178B, "668B8408D2B65B00", "B83D010000909090" },
	{ 0x096B68, "668BB0D2B65B00", "BE3D0100009090" },
	{ 0x096B2A, "8D0450", "909090" },
	{ 0x096B33, "0FBF88C6B65B00", "50E8E713110059" },
	{ 0x096B3C, "0FBF90BEB65B00", "50E8DE1311005A" },
	{ 0x096B53, "0FBF90C2B65B00", "50E8C71311005A" },
	{ 0x00C9DF, "668B9C42D2B65B00", "BB3D010000909090" },
	{ 0x015D77, "668B8440D2B65B00", "B83D010000909090" },
	{ 0x030B3C, "668B8440D2B65B00", "B83D010000909090" },
	{ 0x03EE07, "668B88C8B65B00668B90CCB65B00", "5056E812911600595A9090909090" },
	{ 0x106FAA, "50E88052FDFF25FFFF000083C4048D14808D0CD0", "50E8800F0A0083C40485C00F849E010000EB2390" },
	{ 0x0C342F, "6683B8D2B65B00007424EB18", "E8800F0A009090907424EB28" },
	{ 0x0C34A0, "3B404C00", "21404C00" },
	{ 0x0E8E65, "668B8448D0B65B00", "B83D010000909090" },
	{ 0x0EF00E, "668B8448D0B65B00", "B83D010000909090" },
	{ 0x0EFC0D, "668B8450D0B65B00", "B83D010000909090" },
};

static const HookPatchItem g_setTurretIndexPatch[] =
{
	{ 0x080124, "6683B8F9968B0002", "E8077E120085C090" },
	{ 0x09038D, "7546", "EB46" },
	{ 0x0902F3, "8BAEE0948B0081FDFFFF0000", "53E8377C11005D85C0909090" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that loads mission objects", g_loadMissionObjectsPatch),
	MAKE_HOOK_PATCH("To call the hook that set crafts turret", g_setCraftTurretPatch),
	MAKE_HOOK_PATCH("To call the hook that filters turret weapons", g_filterTurretWeaponsPatch),
	MAKE_HOOK_PATCH("To call the hook that set turrets data", g_setTurretDataPatch),
	MAKE_HOOK_PATCH("To call the hook that read exe crafts turrets data", g_readExeCraftsTurretsPatch),
	MAKE_HOOK_PATCH("To call the hook that set turret index", g_setTurretIndexPatch),
};
