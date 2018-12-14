#pragma once
#include "hook_function.h"

#include "get_time.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x50E415, L0050E410Hook },
	{ 0x50E435, L0050E430Hook },
};

static const HookPatchItem g_reduceCPUUsagePatch[] =
{
	{ 0x10D810, "E82BF8FFFF8B", "E81BA70900C3" },
	{ 0x10D830, "E80BF8FFFF8B", "E8FBA60900C3" },
};

static const HookPatchItem g_fullFpsPatch[] =
{
	//{ 0x10FBD5, "7D24", "EB24" },
	{ 0x10FBD2, "83FA08", "83FA02" },
	{ 0x10FBF6, "83F808", "83F802" },
	//{ 0x10FC8D, "0F8DFCFEFFFF", "90E9FCFEFFFF" },
	{ 0x10FC8A, "83FA08", "83FA02" },
	{ 0x10FCB2, "83F808", "83F802" },
	{ 0x058005, "83FA08", "83FA02" },
	{ 0x058027, "83F908", "83F902" },
};

static const HookPatchItem g_disableFlushTextureCacheInflightPatch[] =
{
	{ 0x0499E0, "E808D21400", "9090909090" },
	{ 0x196B96, "8B5508", "EB5190" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that reduces CPU usage", g_reduceCPUUsagePatch),
	MAKE_HOOK_PATCH("To call the hook that sets full FPS", g_fullFpsPatch),
	MAKE_HOOK_PATCH("To call the hook that disables flush texture cache inflight", g_disableFlushTextureCacheInflightPatch),
};
