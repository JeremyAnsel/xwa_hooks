#pragma once
#include "hook_function.h"

#include "dock.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x432C51, DockReadPositionHook },
	{ 0x4B0976, AIDockPickUpHook },
	{ 0x4B0C1A, AIDockPickUpHook },
	{ 0x4965DF, AIDockPickedUpHook },
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

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that reads the dock position", g_dockReadPositionPatch),
	MAKE_HOOK_PATCH("To call the hook that fixes the ai dock position", g_aiDockPatch),
};
