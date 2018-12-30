#pragma once
#include "hook_function.h"

#include "interdictor.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x508BAB, InterdictorHook },
};

static const HookPatchItem g_interdictorPatch[] =
{
	{ 0x107FA4, "6681F9890074076681F98D007533", "5051E885FF09005985C058743490" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that enables interdictor feature", g_interdictorPatch),
};
