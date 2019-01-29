#pragma once
#include "hook_function.h"

#include "resolution.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x52134C, ResolutionHook },
	{ 0x5214A3, ResolutionHook },
	{ 0x5214DE, ResolutionHook },
};

static const HookPatchItem g_resolutionPatch[] =
{
	{ 0x120747, "E804B80300", "E8E4770800" },
	{ 0x12089E, "E8ADB60300", "E88D760800" },
	{ 0x1208D9, "E872B60300", "E852760800" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that displays screen resolution", g_resolutionPatch),
};
