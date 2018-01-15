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

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that loads mission objects", g_loadMissionObjectsPatch),
	MAKE_HOOK_PATCH("To call the hook that set crafts turret", g_setCraftTurretPatch),
};
