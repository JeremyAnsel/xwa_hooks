#pragma once
#include "hook_function.h"

#include "concourse.h"

#include "config.h"

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

class HooksConfig
{
public:
	HooksConfig()
	{
		auto lines = GetFileLines("hook_concourse.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_concourse");
		}

		this->SkipFirst7Missions = GetFileKeyValueInt(lines, "SkipFirst7Missions", 0) != 0;
	}

	bool SkipFirst7Missions;
};

static HooksConfig g_hooksConfig;

static const HookFunction g_hookFunctions[] =
{
	{ 0x563AB9, MedalDetailExistHook },
	{ 0x563B7B, MedalDetailFilenameHook },
	{ 0x51139A, CustomMoviesOnPoints1Hook },
	{ 0x57EDEC, CustomMoviesOnPointsPlayHook },
	{ 0x43013B, CustomPointsCheckHook },
	{ 0x53E8C7, LoadGameStateHook },
	{ 0x53E90D, FreeGameStateHook },
	{ 0x53AC29, DrawConcoursePlanetHook },
	{ 0x53A10B, DrawConcourseDoorsHook },
	{ 0x53A1C2, TrainDoorRectHook },
	{ 0x53A330, CombatDoorRectHook },
	{ 0x53A4B5, TourDoorRectHook },
	{ 0x53A671, MkanimRectHook },
	{ 0x53A7F7, FilmDoorRectHook },
	{ 0x53A970, TechDoorRectHook },
	{ 0x56416A, MedalPositionHook },
	{ 0x564AAB, MedalPositionHook },
	{ 0x564193, MedalDescriptionHook },
	{ 0x5845D4, MedalAndKalidorCrescentDescriptionHook },
	{ 0x5641FD, KalidorCrescentPosition1Hook },
	{ 0x564AFD, KalidorCrescentPosition2Hook },
	{ 0x563B20, FamilyMedalDescriptionHook },
	{ 0x563BDC, FamilyMedalDescriptionHook },
	{ 0x564219, MedalAndKalidorCrescentDescriptionHook },
	{ 0x5846E0, MedalAndKalidorCrescentDescriptionHook },
	{ 0x58486A, MedalAndKalidorCrescentDescriptionHook },
	{ 0x577A8D, PlayPerMissionMovieHook },
	{ 0x5291FB, WritePilotHook },
	{ 0x529935, ReadPilotHook },
	{ 0x528D20, CreatePilotHook },
	{ 0x563275, ReadEmailTxtHook },
	{ 0x53A0D0, SetupConcourseStarfieldHook },
	{ 0x53A103, DrawConcourseStarfieldHook },
	{ 0x539F6F, SwitchFrontPlanetImageHook },
	{ 0x53A04B, LoadFrontPlanetImageHook },
	{ 0x53B75C, DrawMultiDoorHook },
	{ 0x53B84A, DrawSingleDoorHook },
	{ 0x53BFDE, DrawCombatBackDoorHook },
	{ 0x53C0A2, DrawCombatBackDoorHook },
	{ 0x53BBE8, MultiDoorRectHook },
	{ 0x53B9E1, SingleDoorRectHook },
	{ 0x53BF00, CombatBackDoorRectHook },

	{ 0x563EBC, FamilyRoomLadyblueMission1Hook },
	{ 0x563FC8, FamilyRoomLadyblueMission2Hook },
	{ 0x5648CD, FamilyRoomLadyblueMission3Hook },
	{ 0x563FE5, FamilyRoomCologneMission1Hook },
	{ 0x5640F4, FamilyRoomCologneMission2Hook },
	{ 0x5646DD, FamilyRoomCologneMission3Hook },
};

static const std::string g_RankPointsArray = int_to_hex(GetRankPointsArrayPtr() + 0x00);
static const std::string g_PilotRatingPointsArray = int_to_hex(GetPilotRatingPointsArrayPtr() + 0x00);

static const HookPatchItem g_medalDetailsPatch[] =
{
	{ 0x162EB3, "8B048D58446000", "51E86750040058" },
	{ 0x162F75, "8B3C85C8426000", "50E8A54F04005F" },
	{ 0x162F1B, "E83090FFFF", "E810500400" },
	{ 0x162FD7, "E8748FFFFF", "E8544F0400" },
};

#define SkipFirst7MissionsPatchItem(offset, from, to) { (offset), (from), g_hooksConfig.SkipFirst7Missions ? (to) : (from) }

static const HookPatchItem g_skipFirst7MissionsPatch[] =
{
	SkipFirst7MissionsPatchItem(0x138C4D, "743A", "EB3A"),
	SkipFirst7MissionsPatchItem(0x13928B, "7424", "EB24"),
	SkipFirst7MissionsPatchItem(0x13A843, "7454", "EB54"),
	SkipFirst7MissionsPatchItem(0x13A94D, "7426", "EB26"),
	SkipFirst7MissionsPatchItem(0x13A98E, "7414", "EB14"),
	SkipFirst7MissionsPatchItem(0x13AC90, "0F842D010000", "90E92D010000"),
	SkipFirst7MissionsPatchItem(0x13AEB7, "0F8410010000", "90E910010000"),
	SkipFirst7MissionsPatchItem(0x13B0B5, "0F842A020000", "90E92A020000"),
	SkipFirst7MissionsPatchItem(0x13B1C7, "0F8418010000", "90E918010000"),
	SkipFirst7MissionsPatchItem(0x13B36A, "7411", "EB11"),
	SkipFirst7MissionsPatchItem(0x13BAD0, "7427", "EB27"),
	SkipFirst7MissionsPatchItem(0x142D07, "744D", "EB4D"),
	SkipFirst7MissionsPatchItem(0x1447A7, "7425", "EB25"),
	SkipFirst7MissionsPatchItem(0x1448A2, "7425", "EB25"),
	SkipFirst7MissionsPatchItem(0x1449F0, "7424", "EB24"),
	SkipFirst7MissionsPatchItem(0x15F68E, "740E", "EB0E"),
	SkipFirst7MissionsPatchItem(0x1604A3, "7421", "EB21"),
	SkipFirst7MissionsPatchItem(0x160523, "7465", "EB65"),
	SkipFirst7MissionsPatchItem(0x160CA6, "7421", "EB21"),
	SkipFirst7MissionsPatchItem(0x160FFD, "7422", "EB22"),
	SkipFirst7MissionsPatchItem(0x161093, "741C", "EB1C"),
	SkipFirst7MissionsPatchItem(0x16311A, "740C", "EB0C"),
	SkipFirst7MissionsPatchItem(0x163234, "7408", "EB08"),
	SkipFirst7MissionsPatchItem(0x1639A8, "0F84B3000000", "90E9B3000000"),
	SkipFirst7MissionsPatchItem(0x163B26, "7408", "EB08"),
	SkipFirst7MissionsPatchItem(0x163C19, "7408", "EB08"),
	SkipFirst7MissionsPatchItem(0x164454, "7438", "EB38"),
	SkipFirst7MissionsPatchItem(0x16539C, "7424", "EB24"),
	SkipFirst7MissionsPatchItem(0x170B91, "7430", "EB30"),
	SkipFirst7MissionsPatchItem(0x170C72, "7413", "EB13"),
};

static const HookPatchItem g_customMoviesOnPointsPatch[] =
{
	{ 0x110795, "E8B6D3F1FF", "E886770900" },
	{ 0x17E1E7, "E81493FDFF", "E8349D0200" },
	{ 0x02F512, "0F83CB070000", "909090909090" },
	{ 0x02F535, "3B2C95A8305B00720D", "55E8E58917005DEB0D" },
};

static const HookPatchItem g_concourseDoorsPatch[] =
{
	{ 0x13DCC2, "8B154114A200", "E859A2060090" },
	{ 0x13DD08, "891D8DC0A100", "E813A2060090" },
	{ 0x13A002, "E8A9120600", "33C0909090" },
	{ 0x13A011, "E89A120600", "33C0909090" },
	{ 0x13A024, "83C2372BD7", "E8F7DE0600" },
	{ 0x139506, "68AE00000068E7000000", "E815EA0600EB7D909090" },
	{ 0x1395BD, "E80EEB0100", "E85EE90600" },
	{ 0x13972B, "E8A0E90100", "E8F0E70600" },
	{ 0x1398B0, "E81BE80100", "E86BE60600" },
	{ 0x139A6C, "E81FE60100", "E8AFE40600" },
	{ 0x139BF2, "E8D9E40100", "E829E30600" },
	{ 0x139D6B, "E860E30100", "E8B0E10600" },
};

static const HookPatchItem g_medalsPatch[] =
{
	{ 0x163563, "8B0CB5A04260008B17", "5656E8B64904005A59" },
	{ 0x163EA4, "8B14B5A04260008B07", "5656E875400400585A" },
	{ 0x16358D, "52E8BD89FFFF83C404", "56E89D49040083C404" },
	{ 0x1839CF, "E87C85FDFF", "E85C450200" },
	{ 0x1635F8, "E8D34AFFFF", "E823490400" },
	{ 0x163EF8, "E863FFFCFF", "E823400400" },
	{ 0x15DAA4, "07", "14" },
	{ 0x15DA6E, "35", "64" },
	{ 0x15DBC5, "06", "64" },
	{ 0x163632, "06", "64" },
	{ 0x163CC5, "06", "64" },
	{ 0x163F03, "06", "64" },
	{ 0x16360C, "05A6030000", "9090909090" },
	{ 0x183AD2, "05A6030000", "9090909090" },
	{ 0x183C5E, "81C2A6030000", "909090909090" },
	{ 0x163614, "E83789FFFF", "E817490400" },
	{ 0x183ADB, "E87084FDFF", "E850440200" },
	{ 0x183C65, "E8E682FDFF", "E8C6420200" },
};

static const HookPatchItem g_battlesMissionsImagesPatch[] =
{
	{ 0x15D16D, "6A35", "6A7F" },
	{ 0x15D183, "6A35", "6A7F" },
	{ 0x15D19C, "6A08", "6A40" },
};

static const HookPatchItem g_rankPointsPatch[] =
{
	{ 0x02F4CD + 0x03, "C0305B00", g_RankPointsArray.c_str() },
	{ 0x02E7C2 + 0x03, "D42F5B00", g_PilotRatingPointsArray.c_str() },
	{ 0x02E841 + 0x03, "D42F5B00", g_PilotRatingPointsArray.c_str() },
	{ 0x02E943 + 0x03, "D42F5B00", g_PilotRatingPointsArray.c_str() },
	{ 0x02E9BF + 0x03, "D42F5B00", g_PilotRatingPointsArray.c_str() },
};

static const HookPatchItem g_perMissionMoviesPatch[] =
{
	{ 0x176E88, "E89341FEFF", "E893100300" },
};

static const HookPatchItem g_emailsPatch[] =
{
	{ 0x1285F6, "B801000000", "E835F90700" },
	{ 0x128D30, "B801000000", "E8FBF10700" },
	{ 0x12811B, "E8F0200700", "E810FE0700" },
	{ 0x162670, "893D104B9F00", "E8AB58040090" },
};

static const HookPatchItem g_concourseStarfieldPatch[] =
{
	{ 0x1394CB, "E8800B0000", "E860EA0600" },
	{ 0x1394FE, "E84D0C0000", "E82DEA0600" },
};

static const HookPatchItem g_concoursePlanetPatch[] =
{
	{ 0x13936A, "0F8495000000", "E8B1EB060090" },
	{ 0x139446, "E8553C0200", "E8D5EA0600" },
};

static const HookPatchItem g_combatDoorsPatch[] =
{
	{ 0x13AB57, "E80493FFFF", "E8C4D30600" },
	{ 0x13AC45, "E81692FFFF", "E8D6D20600" },
	{ 0x13B3D9, "E8828AFFFF", "E842CB0600" },
	{ 0x13B49D, "E8BE89FFFF", "E87ECA0600" },
	{ 0x13AFE3, "E8A8D00100", "E838CF0600" },
	{ 0x13ADDC, "E8AFD20100", "E83FD10600" },
	{ 0x13B2FB, "E890CD0100", "E820CC0600" },
};

static const HookPatchItem g_concourseInteractivePositionsPatch[] =
{
	{ 0x1632B7, "0F841D010000", "E8644C040090" },
	{ 0x1633C3, "833A17740D", "E8584B0400" },
	{ 0x163CC8, "A18EDBAE00", "E863420400" },
	{ 0x1633E0, "0F844F020000", "E83B4B040090" },
	{ 0x1634EF, "833A097410", "E82C4A0400" },
	{ 0x163AD8, "A1EED8AE00", "E853440400" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that loads medal details", g_medalDetailsPatch),
	MAKE_HOOK_PATCH("Skip the first 7 missions", g_skipFirst7MissionsPatch),
	MAKE_HOOK_PATCH("To call the hook that plays custom movies on points level", g_customMoviesOnPointsPatch),
	MAKE_HOOK_PATCH("To call the hook that draws the concourse doors", g_concourseDoorsPatch),
	MAKE_HOOK_PATCH("To call the hook that defines medals", g_medalsPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the battles and missions images count", g_battlesMissionsImagesPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the rank points", g_rankPointsPatch),
	MAKE_HOOK_PATCH("To call the hook that plays per-mission movies", g_perMissionMoviesPatch),
	MAKE_HOOK_PATCH("To call the hook that defines emails", g_emailsPatch),
	MAKE_HOOK_PATCH("To call the hook that generates concourse starfield", g_concourseStarfieldPatch),
	MAKE_HOOK_PATCH("To call the hook that loads the front planet image", g_concoursePlanetPatch),
	MAKE_HOOK_PATCH("To call the hook that draws the combat doors", g_combatDoorsPatch),
	MAKE_HOOK_PATCH("To call the hook that defines interactive positions", g_concourseInteractivePositionsPatch),
};
