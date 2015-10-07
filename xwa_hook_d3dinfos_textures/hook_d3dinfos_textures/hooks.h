#pragma once
#include "hook_function.h"

#include "d3dinfos.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x441755, InitD3DInfosHook },
};
