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
};

static const HookPatchItem g_joystickSmallMovementsPatch[] =
{
	{ 0x0338A0, "51", "C3" },
	{ 0x10AE69, "7E22", "9090" },
	{ 0x10AEAE, "7E2F", "9090" },
	{ 0x10AF00, "7E28", "9090" },
	{ 0x10AF4B, "7E3E", "9090" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that selects the joystick ff device", g_joystickEnumPatch),
	MAKE_HOOK_PATCH("To call the hook that allows joystick small movements", g_joystickSmallMovementsPatch),
};
