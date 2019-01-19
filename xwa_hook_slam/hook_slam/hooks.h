#pragma once
#include "hook_function.h"

#include "slam.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x501879, SlamHook },
};

static const HookPatchItem g_slamPatch[] =
{
	{ 0x100DB3, "19", "00" },
	{ 0x100D5E, "26", "19" },
	{ 0x100CF8, "AEC14F00", "74185000" },
	{ 0x100C72, "5F5E5B8BE55DC3", "EB10E8A7720A00" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that enables SLAM", g_slamPatch),
};
