#pragma once
#include "hook_function.h"

#include "pilot.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x40320D, PilotHook },
	{ 0x45E1F4, HangarDroid1Hook },
	{ 0x45E158, HangarDroid2Hook },
};

static const HookPatchItem g_definePilotMeshesAnimationPatch[] =
{
	{ 0x0025FC, "483DCB0000000F87C501000033D28A90F8334000FF2495D4334000", "508B441E238BA8DD00000055E813591A0083C4085F5E5D5BC39090" },
	{ 0x0018EB, "742A", "EB3D" },
	{ 0x05809E, "66A124168C00663BC3", "E8DD8BFAFFEB539090" },
	{ 0x05D5EF, "890DFC0D9100", "E82CA9140090" },
	{ 0x05D553, "A3FC0D9100", "E8C8A91400" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that defines pilot meshes animation", g_definePilotMeshesAnimationPatch),
};
