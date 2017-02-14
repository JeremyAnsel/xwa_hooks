#pragma once
#include "hook_function.h"

#include "sfoils.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x4016F9, SFoilsHook1 },
	{ 0x40191A, SFoilsHook2 },
};
