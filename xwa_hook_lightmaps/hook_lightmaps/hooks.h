#pragma once
#include "hook_function.h"

#include "lightmaps.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x441B68, LightmapsHook },
};
