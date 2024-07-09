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
	{ 0x564757, MedalDetailDrawHook },
	{ 0x563B42, MedalDetailInAreaHook },
	{ 0x55DB8F, MedalDetailSetAreaHook },
	//{ 0x563A61, MedalDetailInteractiveHook },
	{ 0x55E645, MedalDetailLoadImagesHook },
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
	{ 0x55DD65, BattlesMissionsImagesCountHook },
	{ 0x577A8D, PlayPerMissionMovieHook },
	{ 0x584CCD, PlayPerMissionMovieHook },
	{ 0x577849, LoadPerMissionMovieBeforeBackgroundHook },
	{ 0x577AD2, LoadPerMissionMovieAfterBackgroundHook },
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
	{ 0x573048, PlayFilmPodMovieHook },
	{ 0x53C14E, PlayMultiPodMovieHook },
	{ 0x5438FE, PlaySinglePodMovieHook },
	{ 0x5730F3, LoadFilmRoomBackgroundHook },
	{ 0x538855, PlaySoundHook },

	{ 0x55E816, LoadDatImageHook },
	{ 0x537AB6, LoadCbmImageHook },
	{ 0x53208C, FrontResFreeItemHook },
	{ 0x534A6A, FrontResDrawHook },
	{ 0x53547A, FrontResDrawCurrentImageHook },
	{ 0x53235A, FrontResDrawCurrentImageAddHook },
	{ 0x532C9A, FrontResDrawMapIconHook },
	{ 0x533BFA, FrontResDrawGreyMapIconHook },
	{ 0x5338CA, FrontResDrawIconHook },
	{ 0x5563E7, FrontResIconRectHook },
	{ 0x55BA09, DrawCursorMessageHook },
	{ 0x558D95, DrawFillRectangleHook },
	{ 0x558F94, DrawRectangleHook },
	{ 0x5590DB, DrawRectangleBorderHook },
	{ 0x5697BA, DrawBriefingGridVerticalHook },
	{ 0x56987D, DrawBriefingGridVerticalHook },
	{ 0x569814, DrawBriefingGridHorizontalHook },
	{ 0x5698C4, DrawBriefingGridHorizontalHook },
	{ 0x542CA6, DrawLineHook },
	{ 0x5655AB, DrawBriefingBackgroundHook },
	{ 0x567C7B, DrawBriefingBottomTextHook },
	{ 0x51D1DB, DrawConfigMenuBackHook },
	{ 0x559618, DrawMessageBoxBackgroundHook },
	{ 0x575180, DrawTechLibraryBackgroundHook },
	{ 0x573185, DrawFilmRoomBackgroundHook },
	{ 0x559BD4, DrawCreatePilotBackgroundHook },
	{ 0x54443F, DrawCombatSingleGameStateBackgroundHook },

	{ 0x5602CF, FamilyRoomBacktoconcoursePositionHook },
	{ 0x56039D, FamilyRoomBacktoconcoursePositionHook },
	{ 0x56047B, FamilyRoomStatisticsPositionHook },
	{ 0x5605E5, FamilyRoomPlaymissionPositionHook },
	{ 0x560236, FamilyRoomMkeyePositionHook },
	{ 0x564C02, FamilyRoomMonitorPositionHook },
	{ 0x564C3F, FamilyRoomMonitorPositionHook },
	{ 0x563EBC, FamilyRoomLadyblueMission1Hook },
	{ 0x563FC8, FamilyRoomLadyblueMission2Hook },
	{ 0x5648CD, FamilyRoomLadyblueMission3Hook },
	{ 0x563FE5, FamilyRoomCologneMission1Hook },
	{ 0x5640F4, FamilyRoomCologneMission2Hook },
	{ 0x5646DD, FamilyRoomCologneMission3Hook },
	{ 0x532189, FrontResGetAreaHook },
	{ 0x53233A, FrontResSetCurrentImageHook },

	{ 0x556B61, LoadFontHook },
	{ 0x557158, FreeFontsHook },
	{ 0x557EF4, FontGetLineHeightHook },
	{ 0x557F2E, FontGetStringWidthHook },
	{ 0x557B50, L00557B10Hook },
	{ 0x557946, L00557910Hook },
	{ 0x557756, L00557720Hook },
	{ 0x5575D6, L005575A0Hook },
	{ 0x557488, L00557450Hook },
	{ 0x557348, L00557310Hook },
	{ 0x5571DA, L005571A0Hook },
	{ 0x51D51F, ConfigMenuSetVersionStringHook },

	{ 0x531CCB, ReviewBriefingDrawProgressBarHook },
	{ 0x531C42, ReviewBriefingDrawMissionItemHook },
	{ 0x531BEF, ReviewBriefingDrawMissionTitleHook },
	{ 0x5319C6, ReviewBriefingDrawSkirmishTitleHook },
	{ 0x531B05, ReviewBriefingDrawSkirmishItem1Hook },
	{ 0x531B93, ReviewBriefingDrawSkirmishItem2Hook },
	{ 0x531BBD, ReviewBriefingDrawSkirmishItem3Hook },

	{ 0x5487D4, MissionSelectionScreenPositionHook },

	{ 0x5847CD, DrawBattleZoomImage1Hook },
	{ 0x584846, DrawKalidorZoomImage1Hook },
	{ 0x5848D8, DrawBattleKalidorZoomText1Hook },
	{ 0x5845B6, DrawBattleZoomImage2Hook },
	{ 0x584640, DrawBattleZoomText2Hook },
	{ 0x5846BE, DrawKalidorZoomImage2Hook },
	{ 0x58475E, DrawKalidorZoomText2Hook },

	{ 0x568FAA, DSBriefLoadMovieHook },
	{ 0x564E6E, DSBriefFreeMovieHook },
	{ 0x5684A9, DSBriefMoveToNextImageHook },
	{ 0x5685B7, DSBriefDrawImageHook },
	{ 0x5688F5, DSBriefDrawImageHook },

	{ 0x571A57, DrawMultiHook },
	{ 0x52B484, DrawMulti2Hook },

	{ 0x527173, ViewCustscenesConfigMenuSetPositionHook },
};

static const std::string g_RankPointsArray = int_to_hex(GetRankPointsArrayPtr() + 0x00);
static const std::string g_PilotRatingPointsArray = int_to_hex(GetPilotRatingPointsArrayPtr() + 0x00);

static const HookPatchItem g_medalDetailsPatch[] =
{
	{ 0x162EB3, "8B048D58446000", "51E86750040058" },
	{ 0x162F75, "8B3C85C8426000", "50E8A54F04005F" },
	{ 0x163B52, "A1C05E9F00", "E8C9430400" },
	{ 0x162F3D, "0F84FC000000", "E8DE4F040090" },
	{ 0x15CF8A, "E84103F7FF", "E8A1AF0400" },
	//{ 0x162E5C, "A1C05E9F00", "E8BF500400" },
	//{ 0x16303F, "8B74241C", "83C444C3" },
	{ 0x15DA40, "568D442408", "E8DBA40400" },
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
	//{ 0x15DA6E, "35", "64" },
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
	{ 0x15D160, "686A3B0000", "E8BBAD0400" },
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
	{ 0x1840C8, "E8536FFDFF", "E8533E0200" },
	{ 0x176C44, "E827A5FBFF", "E8D7120300" },
	{ 0x176ECD, "E89EA2FBFF", "E84E100300" },
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

static const HookPatchItem g_playPodMoviePatch[] =
{
	{ 0x172443, "E8D88BFEFF", "E8D85A0300" },
	{ 0x13B549, "E8D2FA0100", "E8D2C90600" },
	{ 0x142CF9, "E822830100", "E822520600" },
	{ 0x1724EE, "E87DECFBFF", "E82D5A0300" },
};

static const HookPatchItem g_removePanelsPatch[] =
{
	{ 0x133E65, "E8D62A0000", "E8C6400700" },
	{ 0x137C50, "A10F7F9F00", "E8DB020700" },
};

static const HookPatchItem g_concourseHDPatch[] =
{
	{ 0x15DC11, "B8FFFFFF7F", "E81AA30400" },
	{ 0x136EB1, "E87A32FFFF", "E87A100700" },
	{ 0x131487, "E8B4540000", "E8A46A0700" },
	{ 0x133E65, "E8D62A0000", "E8C6400700" },
	{ 0x134875, "E8C6200000", "E8B6360700" },
	{ 0x131755, "E8E6510000", "E8D6670700" },
	{ 0x132095, "E8A6480000", "E8965E0700" },
	{ 0x132FF5, "E846390000", "E8364F0700" },
	{ 0x132CC5, "E8763C0000", "E866520700" },
	{ 0x1557E2, "E8C9280000", "E839270500" },
	{ 0x15AE04, "E897BBFFFF", "E827D10400" },
	{ 0x158190, "8B4424048B4808", "E88BFD04009090" },
	{ 0x15838F, "8B45088B4808", "E88CFB040090" },
	{ 0x1584D6, "8B45088B4808", "E845FA040090" },
	{ 0x168BB5, "E8269EFDFF", "E866F30300" },
	{ 0x168C78, "E8639DFDFF", "E8A3F20300" },
	{ 0x168C0F, "E87C9CFDFF", "E80CF30300" },
	{ 0x168CBF, "E8CC9BFDFF", "E85CF20300" },
	{ 0x1420A1, "C745F800080000", "E87A5E06009090" },
	{ 0x1649A6, "A0BCC7B000", "E885350400" },
	{ 0x11C5D6, "E8B5250200", "E845B90800" },
	{ 0x167076, "E8651F0000", "E8A50E0400" },
	{ 0x158A13, "E8B8030000", "E818F50400" },
	{ 0x17457B, "E8B0BBFCFF", "E8B0390300" },
	{ 0x172580, "391DEC3F78000F8463020000", "E89B59030090909090909090" },
	{ 0x158FCF, "E8BCF0FFFF", "E84CEF0400" },
	{ 0x14383A, "391DB03078000F8411010000", "E8E146060090909090909090" },
};

static const HookPatchItem g_concourseInteractivePositionsPatch[] =
{
	{ 0x15F6CA, "E8C189FFFF", "E851880400" },
	{ 0x15F798, "E8F388FFFF", "E883870400" },
	{ 0x15F876, "E81588FFFF", "E8A5860400" },
	{ 0x15F9E0, "E8AB86FFFF", "E83B850400" },
	{ 0x15F631, "E82A48FDFF", "E8FA880400" },
	{ 0x163FFD, "E85EFEFCFF", "E82E3F0400" },
	{ 0x16403A, "E821FEFCFF", "E8F13E0400" },
	{ 0x1632B7, "0F841D010000", "E8644C040090" },
	{ 0x1633C3, "833A17740D", "E8584B0400" },
	{ 0x163CC8, "A18EDBAE00", "E863420400" },
	{ 0x1633E0, "0F844F020000", "E83B4B040090" },
	{ 0x1634EF, "833A097410", "E82C4A0400" },
	{ 0x163AD8, "A1EED8AE00", "E853440400" },
	{ 0x131584, "8038007518", "E897690700" },
	{ 0x131735, "7C038D41FF", "E8E6670700" },
};

static const HookPatchItem g_concourseHDTextPatch[] =
{
	{ 0x155F5C, "8B04BD69BC9F00", "E8BF1F05009090" },
	{ 0x156553, "BEFB7F9F00", "E8C8190500" },
	{ 0x1572EF, "8B048569BC9F00", "E82C0C05009090" },
	{ 0x157329, "8B348569BC9F00", "E8F20B05009090" },
	{ 0x156F4B, "8B3C8569BC9F00", "E8D00F05009090" },
	{ 0x156D41, "8B34AD69BC9F00", "E8DA1105009090" },
	{ 0x156B51, "8B2CB569BC9F00", "E8CA1305009090" },
	{ 0x1569D1, "8B349D69BC9F00", "E84A1505009090" },
	{ 0x156883, "8B348569BC9F00", "E8981605009090" },
	{ 0x156743, "8B348569BC9F00", "E8D81705009090" },
	{ 0x1565D5, "8B348569BC9F00", "E8461905009090" },
	{ 0x11C91A, "C744241C322E3032", "E801B60800909090" },
};

static const HookPatchItem g_reviewBriefingPatch[] =
{
	{ 0x1310C6, "B8EF010000B909000000", "E8556E0700EB50909090" },
	{ 0x13103D, "E84E700200", "E8DE6E0700" },
	{ 0x130FEA, "E8A1700200", "E8316F0700" },
	{ 0x130DC1, "E8CA720200", "E85A710700" },
	{ 0x130F00, "E80B580200", "E81B700700" },
	{ 0x130F8E, "E87D570200", "E88D6F0700" },
	{ 0x130FB8, "E853570200", "E8636F0700" },
};

static const HookPatchItem g_missionSelectionScreenPatch[] =
{
	{ 0x147BCF, "7507BBE6000000", "E84C0306009090" },
};

static const HookPatchItem g_battleMedalZoomPatch[] =
{
	{ 0x183BC8, "E89302FBFF", "E853430200" },
	{ 0x183C41, "E81A02FBFF", "E8DA420200" },
	{ 0x183CD3, "E8B844FDFF", "E848420200" },
	{ 0x1839B1, "E8AA04FBFF", "E86A450200" },
	{ 0x183A3B, "E85047FDFF", "E8E0440200" },
	{ 0x183AB9, "E8A203FBFF", "E862440200" },
	{ 0x183B59, "E8C22FFDFF", "E8C2430200" },
};

static const HookPatchItem g_dsbriefPatch[] =
{
	{ 0x1683A5, "E8C68DFCFF", "E876FB0300" },
	{ 0x164269, "E812D2FCFF", "E8B23C0400" },
	{ 0x1678A4, "E8879DFCFF", "E877060400" },
	{ 0x1679B2, "E8A9C4FCFF", "E869050400" },
	{ 0x167CF0, "E86BC1FCFF", "E82B020400" },
};

static const HookPatchItem g_multiPatch[] =
{
	{ 0x170E52, "E819DFFCFF", "E8C9700300" },
	{ 0x12A87F, "E88C3B0100", "E89CD60700" },
};

static const HookPatchItem g_viewCutscenesConfigMenuPatch[] =
{
	{ 0x12656E, "E81D1B0300", "E8AD190800" },
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
	MAKE_HOOK_PATCH("To call the hook that plays pod movie", g_playPodMoviePatch),
	MAKE_HOOK_PATCH("To call the hook that removes panels", g_removePanelsPatch),
	MAKE_HOOK_PATCH("To call the hook that draws hd concourse", g_concourseHDPatch),
	MAKE_HOOK_PATCH("To call the hook that defines interactive positions", g_concourseInteractivePositionsPatch),
	MAKE_HOOK_PATCH("To call the hook that draws hd text", g_concourseHDTextPatch),
	MAKE_HOOK_PATCH("To call the hook that draws review briefing", g_reviewBriefingPatch),
	MAKE_HOOK_PATCH("To call the hook that draws the mission selection screen", g_missionSelectionScreenPatch),
	MAKE_HOOK_PATCH("To call the hook that draws the battlezoom and kalidorzoom", g_battleMedalZoomPatch),
	MAKE_HOOK_PATCH("To call the hook that plays the ds brief movie", g_dsbriefPatch),
	MAKE_HOOK_PATCH("To call the hook that draws multi", g_multiPatch),
	MAKE_HOOK_PATCH("To call the hook that draws the view cutscenes config menu", g_viewCutscenesConfigMenuPatch),
};
