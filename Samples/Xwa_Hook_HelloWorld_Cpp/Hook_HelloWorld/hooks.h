#pragma once
#include "hook_function.h"

#include "HelloWorld.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x51D53A, HelloWorldHook },
};

static const HookPatchItem g_HelloWorldPatch[] =
{
	{ 0x11C935, "E846D10700", "E8E6B50800" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("Hello World Patch", g_HelloWorldPatch),
};
