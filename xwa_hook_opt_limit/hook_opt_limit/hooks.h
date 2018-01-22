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

static const HookPatchItem g_optLimitPatch[] =
{
	//{ 0x193C4A, "750C", "9090" },
	//{ 0x193C52, "0002", "0080" },
	{ 0x193C52, "0008", "0080" },
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

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that removes the opt limit of 512 vertices per mesh", g_optLimitPatch),
};
