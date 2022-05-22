#pragma once
#include "hook_function.h"

#include "windowed.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x50B691, KeyboardIsAnyKeyPressedHook },
	{ 0x50B705, KeyboardGetPressedKeyHook },
	{ 0x4588DA, KeyboardTryUpdateShiftControlAltKeysPressedStateHook },
	{ 0x50C74F, KeyboardTryUpdateShiftControlAltKeysPressedStateHook },
	{ 0x50CA7D, KeyboardTryUpdateShiftControlAltKeysPressedStateHook },
	{ 0x50CA9F, KeyboardTryUpdateShiftControlAltKeysPressedStateHook },
	{ 0x50CDCD, KeyboardTryUpdateShiftControlAltKeysPressedStateHook },
	{ 0x4F9A5F, RetrieveKeyboardStateHook },
	{ 0x4588DF, RetrieveMouseStateHook },
	{ 0x45D62E, RetrieveMouseStateHook },
	{ 0x4F9B64, RetrieveMouseStateHook },
	{ 0x53EB7A, RegisterClassHook },
	{ 0x53EB9F, CreateWindowHook },
	{ 0x53E558, CursorHook },
	{ 0x55BAB1, CursorHook },
	{ 0x55BACB, CursorHook },
	{ 0x53EC80, CursorZeroHook },
	{ 0x5400A7, CursorZeroHook },
	{ 0x54077C, CursorZeroHook },
	{ 0x53F5D5, SplashScreenHook },
	{ 0x4F98C0, MissionPausedHook },
};

static const HookPatchItem g_windowedPatch[] =
{
	{ 0x10AA8C, "E88FFEF1FF", "E89FD40900" },
	{ 0x10AB00, "E83B00F2FF", "E82BD40900" },
	{ 0x057CD5, "E8A62DFDFF", "E856021500" },
	{ 0x10BB4A, "E831EFF1FF", "E8E1C30900" },
	{ 0x10BE78, "E803ECF1FF", "E8B3C00900" },
	{ 0x10BE9A, "E8E1EBF1FF", "E891C00900" },
	{ 0x10C1C8, "E8B3E8F1FF", "E863BD0900" },
	{ 0x0F8E5A, "E8A11EF3FF", "E8D1F00A00" },
	{ 0x057CDA, "E84130FDFF", "E841021500" },
	{ 0x05CA29, "E8F2E2FCFF", "E8F2B41400" },
	{ 0x0F8F5F, "E8BC1DF3FF", "E8BCEF0A00" },
	{ 0x13DF71, "6A0489442428FF1544905A00", "89442424E8B69F0600909090" },
	{ 0x13DF98, "53568B3544925A0053536A01FFD65053FFD6505353680000009068C030600068C030600053FF1540925A00", "5650E8919F060083C408909090909090909090909090909090909090909090909090909090909090909090" },
	{ 0x13D951, "5150FF1574925A00", "9090E8D8A5060090" },
	{ 0x15AE9F, "5150A3ED659F00890DF1659F00FF1574925A00", "9090A3ED659F00890DF1659F00E87FD0040090" },
	{ 0x15AEB9, "5150A3ED659F00890DF1659F00FF1574925A00", "9090A3ED659F00890DF1659F00E865D0040090" },
	{ 0x13E079, "5353FF1574925A00", "9090E8B09E060090" },
	{ 0x13F49E, "6A006A00FF1574925A00", "90909090E8898A060090" },
	{ 0x13FB6E, "6A006A00A30B7F9F00FF1574925A00", "90909090A30B7F9F00E8B483060090" },
};

static const HookPatchItem g_splashScreenPatch[] =
{
	{ 0x13E9D0, "A12A709F0083EC74", "E84B950600C39090" },
};

static const HookPatchItem g_missionPatch[] =
{
	{ 0x0F8CBB, "E8C01D0100", "E870F20A00" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that enables windowed mode", g_windowedPatch),
	MAKE_HOOK_PATCH("To call the hook that shows a splash screen", g_splashScreenPatch),
	MAKE_HOOK_PATCH("To call the hook that paused the game", g_missionPatch),
};
