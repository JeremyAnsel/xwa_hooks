#pragma once
#include "hook_function.h"

#include "crafts.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x415843, CraftsCountHook },
	{ 0x415865, ProjectilesCountHook },
	{ 0x415888, ExplosionsCountHook },
	{ 0x415A0B, AllocObjectsHook },
	{ 0x4AA1B0, AIHandlingArrayGetHook },
	{ 0x4AA6DB, AIHandlingArraySet0Hook },
	{ 0x4AA765, AIHandlingArraySet1Hook },
	{ 0x43A127, OfficerSoundsBuffer1Hook },
	{ 0x43F448, OfficerSoundsBuffer2Hook },
	{ 0x43F463, OfficerSoundsBuffer3Hook },
	{ 0x4C085A, FriendlyCraftListHook },
	{ 0x4C195C, EnemyCraftListHook },
};

static const HookPatchItem g_craftsCountPatch[] =
{
	{ 0x014C3D, "890D5CF38B00892D507E9100", "56E8DD3219005B9090909090" },
	{ 0x014C60, "83FF01A3804B7D00", "E8CB321900EB1290" },
	{ 0x014C83, "8BC6C1E005", "E8A8321900" },
	{ 0x014E06, "A180FD7F00", "E825311900" },
	{ 0x0A95AA, "8A847980F19A00", "57E880E90F005F" },
	{ 0x0A9AD5, "C6847080F19A0000", "56E845E40F005E90" },
	{ 0x0A9B5F, "C6847180F19A0001", "56E8BBE30F005E90" },
	{ 0x039521, "8D3C8560FD9C00", "50E8F9E916005F" },
	{ 0x03E842, "8B0CB560FD9C00", "56E8D896160059" },
	{ 0x03E85D, "8904B560FD9C00", "50E8BD96160058" },
};

static const HookPatchItem g_rsCraftsCountPatch[] =
{
	{ 0x0BFC55, "25FFFF0000", "E8C6820E00" },
	{ 0x0C0D57, "25FFFF0000", "E8C4710E00" },
	{ 0x030216, "68007D0000", "6880A21900" },
	{ 0x03B4E9, "7407", "EB07" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that defines crafts count", g_craftsCountPatch),
	MAKE_HOOK_PATCH("To call the hook that applies RS's crafts count", g_rsCraftsCountPatch),
};
