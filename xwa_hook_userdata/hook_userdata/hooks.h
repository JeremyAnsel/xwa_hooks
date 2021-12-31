#pragma once
#include "hook_function.h"

#include "userdata.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x528C3B, CreatePilotHook },
	{ 0x529732, CreatePilotHook },
	{ 0x539833, CreatePilotHook },
	{ 0x528D42, CreatePilot2Hook },
	{ 0x51178D, SavePilotHook },
	{ 0x52907C, SavePilotHook },
	{ 0x5298A7, SavePilotHook },
	{ 0x53E102, SavePilotHook },
	{ 0x571115, SavePilotHook },
	{ 0x571613, SavePilotHook },
	{ 0x528C21, OpenPilotHook },
	{ 0x529646, OpenPilotHook },
};

static const HookPatchItem g_pilotDataPatch[] =
{
	{ 0x1249B1, "6A0C", "6A0D" },
	{ 0x1249CF, "C60563C8B00000", "C60564C8B00000" },
	{ 0x128036, "E8B5000000", "E8F5FE0700" },
	{ 0x128B2D, "E8BEF5FFFF", "E8F5FE0700" },
	{ 0x138C2E, "E8BDF4FEFF", "E8FDF20600" },
	{ 0x12813D, "E8EE1F0000", "E8EEFD0700" },
	{ 0x110B88, "E803790100", "E8A3730900" },
	{ 0x128477, "E814000000", "E8B4FA0700" },
	{ 0x128CA2, "E8E9F7FFFF", "E889F20700" },
	{ 0x13D4FD, "E88EAFFEFF", "E82EAA0600" },
	{ 0x170510, "E87B7FFBFF", "E81B7A0300" },
	{ 0x170A0E, "E87D7AFBFF", "E81D750300" },
	{ 0x12801C, "E8DF050000", "E80FFF0700" },
	{ 0x128A41, "E8BAFBFFFF", "E8EAF40700" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that reads and writes pilot files", g_pilotDataPatch),
};
