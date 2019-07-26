#pragma once
#include "hook_function.h"

#include "textures.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x4326F0, DatImageHook },
	{ 0x4CC951, OptReadHook },
	{ 0x441CE4, OptTextureColorHook },
	{ 0x441DA0, OptTextureLightHook },
	{ 0x5965F5, Create3DTextureSurfaceHook },
};

static const HookPatchItem g_texturesTagPatch[] =
{
	{ 0x031AEB, "E8CE391600", "E840641700" },
	{ 0x0CBD4C, "E8AF010000", "E8DFC10D00" },
	{ 0x0410DF, "E8DA431500", "E84C6E1600" },
	{ 0x04119B, "E81E431500", "E8906D1600" },
	{ 0x1959F0, "E8FB710000", "E82B250100" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that set textures tag", g_texturesTagPatch),
};
