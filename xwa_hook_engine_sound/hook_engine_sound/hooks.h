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
	{ 0x43D941, WashSoundStopHook },
	{ 0x45B7D5, TakeOffSoundHook },
	{ 0x45BEE9, EngineSlowingSoundHook },
	{ 0x45C08E, CanopyOpeningSoundHook },
	{ 0x45ECF3, HangarShuttleSoundsHook },
	{ 0x45ED08, HangarShuttleSoundsHook },
	{ 0x45FBDE, HangarShuttleSoundsHook },
	{ 0x45FBF3, HangarShuttleSoundsHook },

	{ 0x43C1F1, WeaponSoundHook },
	{ 0x43BCB5, WeaponExplosionsSoundHook },

	{ 0x403E6F, HyperStartSoundHook },
	{ 0x403F7A, HyperZoomSoundHook },
	{ 0x403BCA, HyperEndSoundHook },

	{ 0x4F6A25, CustomSoundsHook },
	{ 0x493A94, PlayerCraftTargetedSoundHook },
	{ 0x503ECA, MapEnterSoundHook },
	{ 0x41F726, MapExitSoundHook },
	{ 0x42022F, TurretSwitchSoundHook },

	{ 0x539195, LoadSoundsHook },

	{ 0x497F6D, AppendRadioMessageHook },
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
	{ 0x03CD3C, "52E80EEF090083C404", "E8DFB11600EB369090" },
	{ 0x05ABD0, "6683F93A74266683F941", "E84BD31400EB20909090" },
	{ 0x05B2E4, "E8A700FEFF", "E837CC1400" },
	{ 0x05B489, "E802FFFDFF", "E892CA1400" },
	{ 0x05E0EE, "E89DD2FDFF", "E82D9E1400" },
	{ 0x05E103, "E888D2FDFF", "E8189E1400" },
	{ 0x05EFD9, "E8B2C3FDFF", "E8428F1400" },
	{ 0x05EFEE, "E89DC3FDFF", "E82D8F1400" },
};

static const HookPatchItem g_defineWeaponSoundsPatch[] =
{
	{ 0x03B5DE, "8DB8E8FEFFFF83FF190F870801000033D28A972CC34300FF2495FCC24300", "8B56230FBF929500000050525551E83FC9160083C4105F5E5DC390909090" },
};

static const HookPatchItem g_defineWeaponExplosionsSoundsPatch[] =
{
	{ 0x03B0B0, "A1D4D06300", "E86BCE1600" },
};

static const HookPatchItem g_hyperSoundsPatch[] =
{
	{ 0x00326A, "E821810300", "E8B14C1A00" },
	{ 0x003353, "7528", "9090" },
	{ 0x003375, "E816800300", "E8A64B1A00" },
	{ 0x002FC5, "E8C6830300", "E8564F1A00" },
};

static const HookPatchItem g_customSoundsPatch[] =
{
	{ 0x0F5E20, "E8FBC2F0FF", "E8FB200B00" },
	{ 0x092E8F, "E82CBAFAFF", "E88C501100" },
	{ 0x1032C5, "E8C680F3FF", "E8564C0A00" },
	{ 0x1032B3, "0F84FE020000", "909090909090" },
	{ 0x1032BD, "5068FFFF00006A4B", "9090909090909090" },
	{ 0x1032D8, "83C40C", "909090" },
	{ 0x01EB21, "E86AC80100", "E8FA931800" },
	{ 0x01F62A, "E861BD0100", "E8F1881800" },
};

static const HookPatchItem g_loadSoundsPatch[] =
{
	{ 0x138590, "8B4424085356", "E89BF90600C3" },
};

static const HookPatchItem g_playRadioMessagePatch[] =
{
	{ 0x097368, "8B0C9500649B00", "E8B30B11009090" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that replaces mission sounds", g_replaceMissionSoundsPatch),
	MAKE_HOOK_PATCH("To call the hook that defines engine sounds", g_defineEngineSoundsPatch),
	MAKE_HOOK_PATCH("To call the hook that defines weapon sounds", g_defineWeaponSoundsPatch),
	MAKE_HOOK_PATCH("To call the hook that defines weapon explosions sounds", g_defineWeaponExplosionsSoundsPatch),
	MAKE_HOOK_PATCH("To call the hook that defines hyper sounds", g_hyperSoundsPatch),
	MAKE_HOOK_PATCH("To call the hook that defines custom sounds", g_customSoundsPatch),
	MAKE_HOOK_PATCH("To call the hook that loads sounds", g_loadSoundsPatch),
	MAKE_HOOK_PATCH("To call the hook that plays radio messages", g_playRadioMessagePatch),
};
