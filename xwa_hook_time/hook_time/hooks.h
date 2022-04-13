#pragma once
#include "hook_function.h"

#include "get_time.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x50E415, L0050E410Hook },
	{ 0x50E435, L0050E430Hook },
	{ 0x401B82, AsteroidsAnimationHook },
	{ 0x401BA1, AsteroidsAnimationHook },
	{ 0x401BC7, AsteroidsAnimationHook },
	{ 0x402638, NullMobileObjectAnimationHook },
	{ 0x401C1C, ShipAnimationHook },
	{ 0x40253F, ExplosionAnimationHook },
};

static const HookPatchItem g_reduceCPUUsagePatch[] =
{
	{ 0x10D810, "E82BF8FFFF8B", "E81BA70900C3" },
	{ 0x10D830, "E80BF8FFFF8B", "E8FBA60900C3" },
	{ 0x13E328, "99F7FE", "33C040" },
	{ 0x13EC26, "F77C2404", "33C04090" },
	{ 0x13FED3, "A142709F0085C0750EEBC1", "6A30FF1574905A0090EB0C" },
	{ 0x13DEEF, "0F844AFDFFFF", "909090909090" },
	{ 0x13F262, "0F84C1FEFFFF", "909090909090" },
};

static const HookPatchItem g_fullFpsPatch[] =
{
	//{ 0x10FBD5, "7D24", "EB24" },
	{ 0x10FBD2, "83FA08", "83FA01" },
	{ 0x10FBF6, "83F808", "83F801" },
	//{ 0x10FC8D, "0F8DFCFEFFFF", "90E9FCFEFFFF" },
	{ 0x10FC8A, "83FA08", "83FA01" },
	{ 0x10FCB2, "83F808", "83F801" },
	{ 0x058005, "83FA08", "83FA04" },
	{ 0x058027, "83F908", "83F904" },
	{ 0x0F739C, "730A", "EB0A" },
	{ 0x10FD86, "7D07", "EB07" },
	{ 0x11034C, "7D07", "EB07" },
};

static const HookPatchItem g_disableFlushTextureCacheInflightPatch[] =
{
	{ 0x0499E0, "E808D21400", "9090909090" },
	{ 0x196B96, "8B5508", "EB5190" },
};

static const HookPatchItem g_animationsSpeedPatch[] =
{
	{ 0x000EF5, "0F85310C0000", "909090909090" },
	{ 0x000F7D, "8D04408D0480", "E89E6F1A0090" },
	{ 0x000F9C, "8D04408D0480", "E87F6F1A0090" },
	{ 0x000FC2, "8D04408D0480", "E8596F1A0090" },
	{ 0x001A33, "0F84D5000000", "E8E8641A0090" },
	{ 0x001017, "25FFFF0000", "E8046F1A00" },
	{ 0x00193A, "66817DFCDE00752B", "E8E1651A00909090" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that reduces CPU usage", g_reduceCPUUsagePatch),
	MAKE_HOOK_PATCH("To call the hook that sets full FPS", g_fullFpsPatch),
	MAKE_HOOK_PATCH("To call the hook that disables flush texture cache inflight", g_disableFlushTextureCacheInflightPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the animations speed", g_animationsSpeedPatch),
};
