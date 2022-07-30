#pragma once
#include "hook_function.h"

#include "hyperspace.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x4AFD77, AIOutOfHyperspaceInitSpeedHook },
	{ 0x4AFD8E, AIOutOfHyperspaceInitDelayHook },
	{ 0x4AFDE9, AIOutOfHyperspaceInitTimeHook },
	{ 0x4AFE83, AIOutOfHyperspaceStepSpeedHook },
	{ 0x4AFEFA, AIOutOfHyperspaceDistanceCheckHook },
	{ 0x4AEFB8, AIIntoHyperspacePlaySoundHook},
	{ 0x42CAA1, AIIntoHyperspaceSpeedHook},
};

static const HookPatchItem g_hyperspacePatch[] =
{
	{ 0x0AF172, "66C78085000000100E", "E8A98D0F0090909090" },
	{ 0x0AF189, "C74062EC000000", "E8928D0F009090" },
	{ 0x0AF1E4, "C7425E240A0000", "E8378D0F009090" },
	{ 0x0AF27E, "66898285000000", "E89D8C0F009090" },
	{ 0x0AF2F5, "750F3B484B7E0D", "E8268C0F00EB23" },
	{ 0x0AE3B3, "E8589DFEFF", "E8689B0F00" },
	{ 0x02BE9C, "66399085000000", "E87FC01700EB0D" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that defines the hyperspace animation", g_hyperspacePatch),
};
