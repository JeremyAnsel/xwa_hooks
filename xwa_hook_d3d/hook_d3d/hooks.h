#pragma once
#include "hook_function.h"

#include "d3d.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x594E75, BeginSceneHook },
	{ 0x594F03, EndSceneHook },
	{ 0x59500F, ExecuteBufferLockHook },
	{ 0x59509D, ExecuteBufferAddVerticesHook },
	{ 0x59510E, ExecuteBufferProcessVerticesHook },
	{ 0x59519C, ExecuteBufferAddTrianglesHook },
	{ 0x5954E1, ExecuteBufferUnlockAndExecuteHook },
};

static const HookPatchItem g_disableFlushTextureCacheInflightPatch[] =
{
	{ 0x0499E0, "E808D21400", "9090909090" },
	{ 0x196B96, "8B5508", "EB5190" },
};

static const HookPatchItem g_scenePatch[] =
{
	{ 0x194270, "A180117B00", "E8BB3C0100" },
	{ 0x1942FE, "A180117B00", "E82D3C0100" },
	{ 0x1944B4, "760432C0EB4A", "909090909090" },
	{ 0x19440A, "C705281D7B0000000000", "E8213B010085C0747E90" },
	{ 0x194498, "A100AA9E0003450CA300AA9E00", "E8933A010085C0746390909090" },
	{ 0x194509, "A1CC1C7B00A3D01C7B00", "E8223A010085C0747D90" },
	{ 0x194597, "A16CA99E0003450CA36CA99E00", "E89439010085C00F842E030000" },
	{ 0x1948DC, "A1CC1C7B00C6000B8B0DCC1C7B00C6410100", "E84F36010085C00F84030100009090909090" },
	{ 0x19486E, "A1CC1C7B008B4DE8", "9090909090909090" },
	{ 0x19487E, "A1CC1C7B008B4DE8", "9090909090909090" },
	{ 0x19488E, "A1CC1C7B00", "9090909090" },
	{ 0x194899, "8B0DCC1C7B00", "8BC890909090" },
	{ 0x1946E7, "A1CC1C7B008B4DE8", "9090909090909090" },
	{ 0x1946F7, "A1CC1C7B008B4DE8", "9090909090909090" },
	{ 0x194707, "A1CC1C7B00", "9090909090" },
	{ 0x194712, "8B0DCC1C7B00", "8BC890909090" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that disables flush texture cache inflight", g_disableFlushTextureCacheInflightPatch),
	MAKE_HOOK_PATCH("To call the hook that sets 3d scene", g_scenePatch),
};
