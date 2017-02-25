#pragma once
#include "hook_function.h"

#include "backdrops.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x57AFE4, BackdropsHook },
};
