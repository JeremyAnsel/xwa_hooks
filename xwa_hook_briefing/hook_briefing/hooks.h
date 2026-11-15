#pragma once
#include "hook_function.h"

#include "briefing.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x564DC6, CustomBriefingHook },
	{ 0x565DAF, CustomToHangarTextHook },
	{ 0x565E90, CustomToHangarTextHook },
	{ 0x565675, SwitchToCustomBriefingHook },
	{ 0x5718D2, SwitchToCustomDebriefingHook },
	{ 0x4D4936, CheckFGGoalStateHook },

	{ 0x5483BA, MissionSetupLoadMissionDescTextHook },
	{ 0x5487AD, MissionSetupBuildBriefingTextHook },
	{ 0x583EBC, MissionDebriefBuildTextHook },
};

static const HookPatchItem g_briefingPatch[] =
{
	{ 0x1641C1, "E86A170000", "E85A3D0400" },
	{ 0x1651AA, "E8A16DFFFF", "E8812D0400" },
	{ 0x16528B, "E8C06CFFFF", "E85A3D0400" },
	{ 0x164A70, "E89BC1FDFF", "E8AB340400" },
	{ 0x170CCD, "6850EC5700", "E84E720300" },
	{ 0x0D3D31, "E8AA110000", "E8FA410D00" },
};

static const HookPatchItem g_briefingTextPatch[] =
{
	{ 0x143013, "6800100000", "6800000100" },
	{ 0x1647FF, "6800100000", "6800000100" },
	{ 0x16DA5D, "6800100000", "6800000100" },
	{ 0x16F16A, "6800100000", "6800000100" },
	{ 0x176C64, "6800100000", "6800000100" },
	{ 0x17E78B, "6800100000", "6800000100" },
	{ 0x17E7B5, "6800100000", "6800000100" },
	{ 0x1477B5, "E8162AFEFF", "E866070600" },
	{ 0x147BA8, "E82326FEFF", "E873030600" },
	{ 0x1832B7, "E8146FFAFF", "E8644C0200" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that creates briefing", g_briefingPatch),
	MAKE_HOOK_PATCH("To call the hook that creates text briefing", g_briefingTextPatch),
};
