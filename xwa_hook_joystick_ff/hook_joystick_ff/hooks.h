#pragma once
#include "hook_function.h"

#include "joystick.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x4355D5, JoystickEnumHook },
};

static const HookPatchItem g_joystickEnumPatch[] =
{
	{ 0x0349D0, "83EC108B44241456", "E85B351700C20800" },
	{ 0x0338A0, "51", "C3" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that selects the joystick ff device", g_joystickEnumPatch),
};
