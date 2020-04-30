#pragma once
#include "hook_function.h"

#include "reticle.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x46C193, SetReticleHook },
	{ 0x46C1C5, SetReticleHook },
	{ 0x46C3E9, SetReticleHook },
	{ 0x46C420, SetReticleHook },
	{ 0x46C458, SetReticleHook },
	{ 0x46C4DA, SetReticleHook },
	{ 0x46C517, SetReticleHook },
	{ 0x477FA0, SetReticleHook },
	{ 0x478059, SetReticleHook },
	{ 0x478114, SetReticleHook },
	{ 0x4781DD, SetReticleHook },
};

static const HookPatchItem g_reticlePatch[] =
{
	{ 0x06B58E, "E8BD65FCFF", "E88DC91300" },
	{ 0x06B5C0, "E88B65FCFF", "E85BC91300" },
	{ 0x06B7E4, "E86763FCFF", "E837C71300" },
	{ 0x06B81B, "E83063FCFF", "E800C71300" },
	{ 0x06B853, "E8F862FCFF", "E8C8C61300" },
	{ 0x06B8D5, "E87662FCFF", "E846C61300" },
	{ 0x06B912, "E83962FCFF", "E809C61300" },
	{ 0x07739B, "E8B0A7FBFF", "E8800B1300" },
	{ 0x077454, "E8F7A6FBFF", "E8C70A1300" },
	{ 0x07750F, "E83CA6FBFF", "E80C0A1300" },
	{ 0x0775D8, "E873A5FBFF", "E843091300" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that sets the reticle image index", g_reticlePatch),
};
