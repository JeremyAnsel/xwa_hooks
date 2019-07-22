#pragma once
#include "hook_function.h"

#include "opt_limit.h"

#include <string>
#include <sstream>
#include <iomanip>

std::string int_to_hex(int i)
{
	std::stringstream stream;
	stream << std::setfill('0') << std::hex
		<< std::setw(2) << (i & 0xff)
		<< std::setw(2) << ((i >> 8) & 0xff)
		<< std::setw(2) << ((i >> 16) & 0xff)
		<< std::setw(2) << ((i >> 24) & 0xff);
	return stream.str();
}

static const HookFunction g_hookFunctions[] =
{
	{}
};

static const std::string g_hitDataArray0 = int_to_hex(GetHitDataArrayPtr());
static const std::string g_hitDataArray4 = int_to_hex(GetHitDataArrayPtr() + 4);
static const std::string g_hitDataArray8 = int_to_hex(GetHitDataArrayPtr() + 8);

static const HookPatchItem g_trianglingPatch[] =
{
	{ 0x193C4A, "750C", "9090" },
	{ 0x193C52, "0002", "0080" },
};

static const HookPatchItem g_briefingWireframeOverlayPatch[] =
{
	{ 0x07E09C, "7D", "EB" },
	{ 0x07E12E, "7D", "EB" },
	{ 0x07E1B9, "47", "90" },
};

static const HookPatchItem g_texturesSizePatch[] =
{
	{ 0x19564A, "81790C000100007717", "909090909090909090" },
	{ 0x1956C2, "817A10000100007717", "909090909090909090" },
};

static const HookPatchItem g_texturesCountPatch[] =
{
	{ 0x0CC432, "2003", "0010" },
	{ 0x0CC43A, "2803", "0810" },
	{ 0x0CC445, "2C03", "0C10" },
	{ 0x0CC44C, "2003", "0010" },
	{ 0x0CC466, "C800", "0004" },
	{ 0x0CC497, "3003", "1010" },
	{ 0x0CC4C9, "3003", "1010" },
	{ 0x0CC4E6, "3003", "1010" },
	{ 0x0CC4EE, "2003", "0010" },
};

static const HookPatchItem g_optLimitPatch[] =
{
	{ 0x193BFE, "01", "20" },
	{ 0x0E7616, "68D37600", g_hitDataArray8.c_str() },
	{ 0x0E7888, "68D37600", g_hitDataArray8.c_str() },
	{ 0x0E7898, "68D37600", g_hitDataArray8.c_str() },
	{ 0x0E78A6, "68D37600", g_hitDataArray8.c_str() },
	{ 0x0E78B9, "68D37600", g_hitDataArray8.c_str() },
	{ 0x0E78DE, "60D37600", g_hitDataArray0.c_str() },
	{ 0x0E78F5, "60D37600", g_hitDataArray0.c_str() },
	{ 0x0E790C, "60D37600", g_hitDataArray0.c_str() },
	{ 0x0E792C, "60D37600", g_hitDataArray0.c_str() },
	{ 0x0E79DA, "60D37600", g_hitDataArray0.c_str() },
	{ 0x0E79E1, "64D37600", g_hitDataArray4.c_str() },
	{ 0x0E7A02, "60D37600", g_hitDataArray0.c_str() },
	{ 0x0E7A08, "64D37600", g_hitDataArray4.c_str() },
};

static const HookPatchItem g_glowMarkUVArrayPatch[] =
{
	{ 0x0E758F, "A148D37600", "A1066D9B00" },
	{ 0x0E75B8, "A148D37600", "A1066D9B00" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("'triangling to infinity' patch", g_trianglingPatch),
	MAKE_HOOK_PATCH("'disable briefing wireframe overlay' patch", g_briefingWireframeOverlayPatch),
	MAKE_HOOK_PATCH("'remove textures size limit' patch", g_texturesSizePatch),
	MAKE_HOOK_PATCH("'opt textures count limit' patch", g_texturesCountPatch),
	MAKE_HOOK_PATCH("To call the hook that removes the opt limit of 512 vertices per mesh", g_optLimitPatch),
	MAKE_HOOK_PATCH("'GlowMarkUVArray vertices count' patch", g_glowMarkUVArrayPatch),
};
