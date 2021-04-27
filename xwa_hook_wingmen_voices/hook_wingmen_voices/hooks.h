#pragma once
#include "hook_function.h"

#include "wingmen_voices.h"

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
	{ 0x43B82A, WingmenVoicesHook },
	{ 0x43BA52, WingmenVoicesHook },
};

static const HookPatchItem g_defineWingmenVoices[] =
{
	{ 0x03AC22, "7528", "9090" },
	{ 0x03AC24, "8B0D78405B008B157C405B00A080405B00", "53E806D3160083C40485C07479EB199090" },
	{ 0x03AE4A, "7528", "9090" },
	{ 0x03AE4C, "8B0D78405B008B157C405B00A080405B00", "53E8DED0160083C40485C07477EB199090" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that defines wingmen voices", g_defineWingmenVoices),
};
