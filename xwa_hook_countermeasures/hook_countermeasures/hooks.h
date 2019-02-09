#pragma once
#include "hook_function.h"

#include "countermeasures.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x460A73, CountermeasuresCountHook },
	{ 0x4B154C, CountermeasuresCountHook },
};

static const HookPatchItem g_countermeasuresPatch[] =
{
	{ 0x05FE6C, "8D04D08A8440B3B65B00", "5753E8BD80140083C408" },
	{ 0x0B0945, "8D04D08A8440B3B65B00", "5051E8E4750F0083C408" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that fixes countermesures count", g_countermeasuresPatch),
};
