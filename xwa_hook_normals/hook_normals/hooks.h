#pragma once
#include "hook_function.h"

#include "normals.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x489159, SetSceneVertListsHook },
	{ 0x442A30, SetVertexNormalHook },
	{ 0x4435D8, SetSpecularSolidHook },
	{ 0x4435F0, SetSpecularSolidHook },
	{ 0x443552, SetSpecularTransparentHook },
	{ 0x443574, SetSpecularTransparentHook },
	{ 0x4432B2, CopyVertHook },
	{ 0x4433AE, L004477E0Hook },
	{ 0x443420, L00444030Hook },
	{ 0x443462, L00444DF0Hook },
	{ 0x4434A4, L00445C00Hook },
	{ 0x4434E7, L004469D0Hook },
};

static const HookPatchItem g_normalsPatch[] =
{
	{ 0x088554, "8915C66C9B00", "E8C7F9110090" },
	{ 0x041E2B, "E850FAFFFF", "E8F0601600" },
	{ 0x0429D3, "E808520000", "E858551600" },
	{ 0x0429EB, "E8F0510000", "E840551600" },
	{ 0x04294D, "E86E540000", "E8DE551600" },
	{ 0x04296F, "E84C540000", "E8BC551600" },
	{ 0x0426AB, "8B79108978108B79148978148B79188978188B791C89781C", "5150E86E58160083C4089090909090909090909090909090" },
	{ 0x0427A9, "E832440000", "E872571600" },
	{ 0x04281B, "E8100C0000", "E800571600" },
	{ 0x04285D, "E88E190000", "E8BE561600" },
	{ 0x04289F, "E85C270000", "E87C561600" },
	{ 0x0428E2, "E8E9340000", "E839561600" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that transmits normals", g_normalsPatch),
};
