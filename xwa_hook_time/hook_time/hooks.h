#pragma once
#include "hook_function.h"

#include "get_time.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x50E415, L0050E410Hook },
	{ 0x50E435, L0050E430Hook },
};
