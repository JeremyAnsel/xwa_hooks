#pragma once
#include "hook_function.h"

#include "joystick.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x4355D5, JoystickFFEnumHook },
	{ 0x4344A5, JoystickSmallMovement1Hook },
	{ 0x50B966, JoystickSmallMovement2Hook },
	{ 0x540DBB, MainControllerIndexHook },
	{ 0x50B7FC, InitControllerHook },
	{ 0x50BA50, UpdateControllerHook },
	{ 0x434684, SetControllerPressedButtonHook },
	//{ 0x4ED7FB, SetControllerPressedButtonLogHook },
	{ 0x51F546, ControllerRemapTextHook },
	{ 0x51F5E8, ControllerRemapRunHook },
	{ 0x51F7D4, ControllerRemapReloadHook },
	{ 0x4F9ABD, NumPadLookMoveHook },
};

static const HookPatchItem g_joystickFFEnumPatch[] =
{
	{ 0x0349D0, "83EC108B44241456", "E85B351700C20800" },
};

static const HookPatchItem g_joystickSmallMovementsPatch[] =
{
	//{ 0x0338A0, "51", "C3" },
	{ 0x0338A0, "5166A1C21C8C00", "E87B461700C390" },
	//{ 0x10AE69, "7E22", "9090" },
	//{ 0x10AEAE, "7E2F", "9090" },
	//{ 0x10AF00, "7E28", "9090" },
	//{ 0x10AF4B, "7E3E", "9090" },
	{ 0x10AD61, "8915C4337700", "E8BAD1090090" },
};

static const HookPatchItem g_controllerIndexPatch[] =
{
	{ 0x1401B6, "A01E699F00", "E8757D0600" },
	{ 0x10ABF4, "752655E8445A0300", "909055E834D30900" },
	{ 0x10AE4B, "0F8586010000", "E8D0D0090090" },
};

static const HookPatchItem g_controllerButtonsPatch[] =
{
	{ 0x033A7E, "B901000000BF91C8B000668B37", "55E89C4417005DE9AF00000090" },
	//{ 0x0ECBF6, "E8756DF4FF", "E825B30B00" },
};

static const HookPatchItem g_controllerRemapPatch[] =
{
	{ 0x11E941, "E80AD60300", "E8EA950800" },
	{ 0x11E9E3, "C705B82978000D000000891DA0297800", "E8389508009090909090909090909090" },
	{ 0x11EBCF, "891DB8297800", "E84C93080090" },
};

static const HookPatchItem g_numpadLookMovePatch[] =
{
	{ 0x0F8EB7, "668B0DC053800084C0A100A05A00", "51E873F00A0059803D6B959E0000" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that selects the joystick ff device", g_joystickFFEnumPatch),
	MAKE_HOOK_PATCH("To call the hook that allows joystick small movements", g_joystickSmallMovementsPatch),
	MAKE_HOOK_PATCH("To call the hook that selects the controller", g_controllerIndexPatch),
	MAKE_HOOK_PATCH("To call the hook that defines the controller buttons", g_controllerButtonsPatch),
	MAKE_HOOK_PATCH("To call the hook that defines the remap config menu", g_controllerRemapPatch),
	MAKE_HOOK_PATCH("To call the hook that defines the look move", g_numpadLookMovePatch),
};
