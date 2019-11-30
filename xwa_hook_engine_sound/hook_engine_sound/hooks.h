#pragma once
#include "hook_function.h"

#include "engine_sound.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x4DAD25, ReplaceMissionSoundsHook },

	{ 0x43D111, InteriorSoundHook },
	{ 0x43D352, StopInteriorSoundHook },
	{ 0x43DA01, FlyBySoundHook },
	{ 0x458E76, LaunchSoundHook },
	{ 0x43D875, WashSoundHook },

	{ 0x43C1F1, WeaponSoundHook },
};

static const HookPatchItem g_replaceMissionSoundsPatch[] =
{
	{ 0x0DA120, "8B44240C8B4C2408", "E80BDE0C00C39090" },
};

static const HookPatchItem g_defineEngineSoundsPatch[] =
{
	{ 0x03C4F5, "8A543A02885424108B54241081E2FF000000", "668B543A0281E2FFFF000089542410909090" },
	{ 0x03C572, "8A5424108815B0D06300", "8B5424108915B0D06300" },
	{ 0x03C746, "25FF000000", "9090909090" },
	{ 0x03C507, "8D6AFF83FD40775633D28A9558D44300FF2495FCD34300", "536A005652E80FBA160083C4045E5D5BEB509090909090" },
	{ 0x03C74B, "4883F840773E33C98A88F8D44300", "5650E8CEB7160083C4045EEB3790" },
	{ 0x03CDFA, "4883F8400F87D500000033DB8A98C4DC4300", "5650E81FB1160083C4045EE9CF0000009090" },
	{ 0x05826F, "4983F9400F87CF00000033D28A9178964500", "5051E8AAFC140083C40458E9C90000009090" },
	{ 0x039725, "7515", "9090" },
	{ 0x039733, "7726", "EB26" },
	{ 0x03CC6F, "3D89000000741A3D8A000000", "50E8BBB216008BF858EB1B90" },
};

static const HookPatchItem g_defineWeaponSoundsPatch[] =
{
	{ 0x03B5DE, "8DB8E8FEFFFF83FF190F870801000033D28A972CC34300FF2495FCC24300", "8B56230FBF929500000050525551E83FC9160083C4105F5E5DC390909090" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that replaces mission sounds", g_replaceMissionSoundsPatch),
	MAKE_HOOK_PATCH("To call the hook that defines engine sounds", g_defineEngineSoundsPatch),
	MAKE_HOOK_PATCH("To call the hook that defines weapon sounds", g_defineWeaponSoundsPatch),
};
