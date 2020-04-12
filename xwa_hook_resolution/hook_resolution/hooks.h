#pragma once
#include "hook_function.h"

#include "resolution.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x52134C, ResolutionHook },
	{ 0x5214A3, ResolutionHook },
	{ 0x5214DE, ResolutionHook },
	{ 0x51014B, FlightRunLoopHook },
	{ 0x50B023, FlightMainHook },
};

static const HookPatchItem g_resolutionPatch[] =
{
	{ 0x120747, "E804B80300", "E8E4770800" },
	{ 0x12089E, "E8ADB60300", "E88D760800" },
	{ 0x1208D9, "E872B60300", "E852760800" },
};

static const HookPatchItem g_setResolutionPatch[] =
{
	{ 0x10F546, "E825CFFFFF", "E8D5890900" },
	{ 0x10A41E, "E8FD0B0000", "E80DDB0900" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that displays screen resolution", g_resolutionPatch),
	MAKE_HOOK_PATCH("To call the hook that set the resolution", g_setResolutionPatch),
};
