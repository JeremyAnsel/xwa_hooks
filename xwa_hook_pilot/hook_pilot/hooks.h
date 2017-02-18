#pragma once
#include "hook_function.h"

#include "pilot.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x40320D, PilotHook },
};
