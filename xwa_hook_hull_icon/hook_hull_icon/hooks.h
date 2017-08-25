#pragma once
#include "hook_function.h"

#include "hull_icon.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x467C94, HullIconHook },
	{ 0x46ADCB, HullIconHook },
	{ 0x46B07B, HullIconHook },
	{ 0x46B44C, HullIconHook },
};
