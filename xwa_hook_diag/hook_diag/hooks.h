#pragma once
#include "hook_function.h"

#include "diag.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x4CC92F, LoadingScreenStepHook },
	{ 0x53F045, ConcoursePresentHook },
	{ 0x45AFEA, InFlightPresentHook },
	{ 0x4F225A, InFlightPresentHook },

	{ 0x4CCA40, OptLoadHook },
	{ 0x4323E8, DatLoadHook },

	{ 0x53E794, MainGameInitCallbackHook },
	{ 0x528B41, LoadSfxLstHook },
};

static const HookPatchItem g_diagMessagesPatch[] =
{
	{ 0x0CBD2A, "E8114F0600", "E801C20D00" },
	{ 0x13E440, "A12A709F00", "E8DB9A0600" },
	{ 0x05A3E5, "A1B0127700", "E836DB1400" },
	{ 0x0F1655, "A1B0127700", "E8C6680B00" },

	{ 0x0CBE3B, "E810190400", "E8E0C00D00" },
	{ 0x0317E3, "E858BA0900", "E838671700" },

	{ 0x13DB8F, "A171C0A100", "E88CA30600" },
	{ 0x127F3C, "E86F4E0300", "E8DFFF0700" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that shows diag messages", g_diagMessagesPatch),
};
