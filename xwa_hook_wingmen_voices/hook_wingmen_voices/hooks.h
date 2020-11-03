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

static const std::string g_SoundEffectBufferArrayCount = int_to_hex(GetSoundEffectBufferArrayCount());
static const std::string g_SoundEffectBufferArray00 = int_to_hex(GetSoundEffectBufferArrayPtr() + 0x00);
static const std::string g_SoundEffectBufferArray40 = int_to_hex(GetSoundEffectBufferArrayPtr() + 0x40);
static const std::string g_SoundEffectBufferArrayC0 = int_to_hex(GetSoundEffectBufferArrayPtr() + 0xC0);
static const std::string g_SoundEffectBufferArrayC4 = int_to_hex(GetSoundEffectBufferArrayPtr() + 0xC4);
static const std::string g_SoundEffectBufferArrayC8 = int_to_hex(GetSoundEffectBufferArrayPtr() + 0xC8);
static const std::string g_SoundEffectBufferArray_1_00 = int_to_hex(GetSoundEffectBufferArrayPtr() + (GetSoundEffectBufferArrayCount() - 1) * 0xC9 + 0x00);
static const std::string g_SoundEffectBufferArray_0_C4 = int_to_hex(GetSoundEffectBufferArrayPtr() + GetSoundEffectBufferArrayCount() * 0xC9 + 0xC4);

static const HookPatchItem g_soundEffectBufferPatch[] =
{
	{ 0x0DA16F, "00060000", g_SoundEffectBufferArrayCount.c_str() },
	{ 0x0136D1, "50B89100", g_SoundEffectBufferArray00.c_str() },
	{ 0x01373A, "50B89100", g_SoundEffectBufferArray00.c_str() },
	{ 0x0D9B0A, "14B99100", g_SoundEffectBufferArrayC4.c_str() },
	{ 0x0D9B1F, "146F9600", g_SoundEffectBufferArray_0_C4.c_str() },
	{ 0x0D9EE5, "876D9600", g_SoundEffectBufferArray_1_00.c_str() },
	{ 0x0D9F12, "50B89100", g_SoundEffectBufferArray00.c_str() },
	{ 0x0D9F70, "50B89100", g_SoundEffectBufferArray00.c_str() },
	{ 0x0D9F92, "50B89100", g_SoundEffectBufferArray00.c_str() },
	{ 0x0D9FEE, "14B99100", g_SoundEffectBufferArrayC4.c_str() },
	{ 0x0D9FFA, "10B99100", g_SoundEffectBufferArrayC0.c_str() },
	{ 0x0DA078, "50B89100", g_SoundEffectBufferArray00.c_str() },
	{ 0x0DA0BA, "14B99100", g_SoundEffectBufferArrayC4.c_str() },
	{ 0x0DA0D0, "146F9600", g_SoundEffectBufferArray_0_C4.c_str() },
	{ 0x0DA195, "50B89100", g_SoundEffectBufferArray00.c_str() },
	{ 0x0DA33B, "50B89100", g_SoundEffectBufferArray00.c_str() },
	{ 0x0DA386, "876D9600", g_SoundEffectBufferArray_1_00.c_str() },
	{ 0x0DA3B3, "50B89100", g_SoundEffectBufferArray00.c_str() },
	{ 0x0DA411, "50B89100", g_SoundEffectBufferArray00.c_str() },
	{ 0x0DA433, "50B89100", g_SoundEffectBufferArray00.c_str() },
	{ 0x0DA48F, "14B99100", g_SoundEffectBufferArrayC4.c_str() },
	{ 0x0DA49B, "10B99100", g_SoundEffectBufferArrayC0.c_str() },
	{ 0x0DA519, "50B89100", g_SoundEffectBufferArray00.c_str() },
	{ 0x0DA6CB, "50B89100", g_SoundEffectBufferArray00.c_str() },
	{ 0x0DA6F6, "50B89100", g_SoundEffectBufferArray00.c_str() },
	{ 0x0DA72A, "50B89100", g_SoundEffectBufferArray00.c_str() },
	{ 0x0DA7D5, "50B89100", g_SoundEffectBufferArray00.c_str() },
	{ 0x0DA8A6, "50B89100", g_SoundEffectBufferArray00.c_str() },
	{ 0x0DAA6A, "50B89100", g_SoundEffectBufferArray00.c_str() },
	{ 0x0DAAF2, "18B99100", g_SoundEffectBufferArrayC8.c_str() },
	{ 0x0DAAF8, "50B89100", g_SoundEffectBufferArray00.c_str() },
	{ 0x0DAB06, "50B89100", g_SoundEffectBufferArray00.c_str() },
	{ 0x0DABDE, "10B99100", g_SoundEffectBufferArrayC0.c_str() },
	{ 0x0DABFB, "10B99100", g_SoundEffectBufferArrayC0.c_str() },
	{ 0x0DAD1E, "10B99100", g_SoundEffectBufferArrayC0.c_str() },
	{ 0x0DB08E, "18B99100", g_SoundEffectBufferArrayC8.c_str() },
	{ 0x0DB0AC, "50B89100", g_SoundEffectBufferArray00.c_str() },
	{ 0x0DB0C1, "10B99100", g_SoundEffectBufferArrayC0.c_str() },
	{ 0x0DB0CA, "90B89100", g_SoundEffectBufferArray40.c_str() },
	{ 0x0DB924, "50B89100", g_SoundEffectBufferArray00.c_str() },
	{ 0x0DB92B, "50B89100", g_SoundEffectBufferArray00.c_str() },
	{ 0x0DB94C, "50B89100", g_SoundEffectBufferArray00.c_str() },
	{ 0x0DBA5C, "50B89100", g_SoundEffectBufferArray00.c_str() },
	{ 0x0DBC3B, "18B99100", g_SoundEffectBufferArrayC8.c_str() },
	{ 0x0DBCDD, "50B89100", g_SoundEffectBufferArray00.c_str() },
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
	MAKE_HOOK_PATCH("To call the hook that increases the sound effect buffer size", g_soundEffectBufferPatch),
	MAKE_HOOK_PATCH("To call the hook that defines wingmen voices", g_defineWingmenVoices),
};
