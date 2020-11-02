#pragma once
#include "hook_function.h"

#include "dock.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x432C51, DockReadPositionHook },
	{ 0x4B0976, AIDockPickUpHook },
	{ 0x4B0C1A, AIDockPickUpHook },
	{ 0x4965DF, AIDockPickedUpHook },
	{ 0x4B67B4, PlayerDockHook },
	{ 0x4B6793, PLayerDockElevationHook },
	{ 0x4B6AFC, PLayerDockOrientationHook },
};

static const HookPatchItem g_dockReadPositionPatch[] =
{
	{ 0x03204C, "898EEAB65B00", "E8CF5E170090" },
};

static const HookPatchItem g_aiDockPatch[] =
{
	{ 0x0AFD71, "E83A26FFFF", "E8AA810F00" },
	{ 0x0B0015, "E89623FFFF", "E8067F0F00" },
	{ 0x0959DA, "E8D1C90000", "E841251100" },
};

static const HookPatchItem g_playerDockPatch[] =
{
	{ 0x0B5BAF, "E8FCC7FEFF", "E86C230F00" },
	{ 0x0B5B70, "742B", "9090" },
	{ 0x0B5B8D, "8B9C49EEB65B002BD3", "50E88D230F005A9090" },
	{ 0x0B5EF7, "8996EC000000", "E824200F0090" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that reads the dock position", g_dockReadPositionPatch),
	MAKE_HOOK_PATCH("To call the hook that fixes the ai dock position", g_aiDockPatch),
	MAKE_HOOK_PATCH("To call the hook that modifies the player dock position", g_playerDockPatch),
};
