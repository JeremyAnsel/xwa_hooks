#pragma once
#include "hook_function.h"

#include "engine_sound.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x43D111, InteriorSoundHook },
	{ 0x43D352, StopInteriorSoundHook },
	{ 0x43DA01, FlyBySoundHook },
	{ 0x458E76, LaunchSoundHook },

	{ 0x43C1F1, WeaponSoundHook },
};
