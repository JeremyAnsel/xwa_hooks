#pragma once
#include "hook_function.h"

#include "wingmen_voices.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x43B82A, WingmenVoicesHook },
	{ 0x43BA52, WingmenVoicesHook },
};

static const HookPatchItem g_defineWingmenVoices[] =
{
	{ 0x03AC24, "8B0D78405B008B157C405B00", "53E8F6D2160083C404EB7B90" },
	{ 0x03AE4C, "8B0D78405B008B157C405B00", "53E8CED0160083C404EB7990" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that defines wingmen voices", g_defineWingmenVoices),
};
