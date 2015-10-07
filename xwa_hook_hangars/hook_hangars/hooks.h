#pragma once
#include "hook_function.h"

#include "hangar.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x4579F3, HangarOptLoadHook },
	{ 0x45FCA8, HangarCameraPositionHook },
	{ 0x455F4B, HangarMapHook },
	{ 0x456479, FamHangarMapHook },
	{ 0x57EB9D, SelectHangarTypeHook },
	{ 0x4095B5, SelectHangarInsideAnimation },
	{ 0x457F80, SelectHangarModelIndex },
};
