#pragma once
#include "hook_function.h"

#include "multiplayer.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x54D8E9, FixRosterFgNamesHook },
	{ 0x5721DE, FixRosterFgNamesClientsHook },
	{ 0x544F99, LoadPanelButtonsCombatTourDirectoriesHook },
	{ 0x545091, LoadBriefingButtonCombatTourHook },
	{ 0x54534E, FixBackButtonRacingHook },
	{ 0x55127A, ShowRankRosterHook1 },
	{ 0x5512D3, ShowRankRosterHook2 },

	{ 0x472FDA, IFFColorsCraftShortNameHook },
	{ 0x4730B8, IFFColorsCraftNameHook },
	{ 0x473396, IFFColorsFGIDHook },
	{ 0x47342E, IFFColorsFGNameHook },
	{ 0x5038B9, IFFColorsPlayerTargetBoxHook },
	{ 0x49FA30, IFFColorsMapPlayerTargetBoxHook },
	{ 0x49FE63, IFFColorFGMapMarkerHook },
	{ 0x4C292C, IFFColorCraftListNames },

	{ 0x546A05, PanelButtonsHook },
	{ 0x551476, FixCraftSelectionTourHook },
	{ 0x545DA7, ForcePlayerInSlotHook, },
	{ 0x54AB21, LoadCancelDeleteButtonHook },
	{ 0x54C73C, DifficultySettingHook },
	{ 0x5507CA, XVTCraftSelectionHook },
	{ 0x543906, CombatSimMusicHook },
	{ 0x53C230, CombatSimBackgroundHostOrJoinHook },
	{ 0x565317, CombatSimBackgroundBriefingHook },
	{ 0x56E81E, CombatSimBackgroundUnknownHook },
	{ 0x56E67E, CombatSimBackgroundJoinRoomHook },
	{ 0x56FD8B, CombatSimBackgroundConnectToLobbyHook },
	{ 0x57198D, CombatSimBackgroundMultiHostStartedLobbyHook },
	{ 0x57F45C, CombatSimBackgroundDebriefingRoomHook },
	{ 0x53D84A, CombatSimSettingBarHostOrJoinHook1 },
	{ 0x53DA14, CombatSimSettingBarHostOrJoinHook2 },
	{ 0x54CFB0, CombatSimBackgroundGeneralHook },
	{ 0x54CFE1, CombatSimSettingBarGeneral },
	{ 0x54D302, CombatSimSettingBarGeneral2 },
	{ 0x4FE92B, QuitMessageHook },
	{ 0x47A998, IFFColorOrangeMessageHook },
	{ 0x49A158, IFFColorOrangeMessageLogHook },
	{ 0x4FE45E, HangarMultiplayerHook },
	{ 0x53D16F, UpdateRateHostJoinRoomHook },
	{ 0x528892, UpdateRateOptionsMenuHook },
	{ 0x5287BA, UpdateRateOptionsMenuClientHook },
	{ 0x4FDC06, KeyHook1 },
	{ 0x5015B8, MapAIHook },
	{ 0x5014DA, SpecRegionChangeHook },
	//{ 0x5319A7, CombatDescriptionLoadHook },
	{ 0x546B9F, TourModeButtonHook },

	{ 0x543CFC, Test1Hook },
};

static const HookPatchItem g_multiPatch[] =
{
	//////////////////// Hook Patches ////////////////////
	// Fix Roster FG names
	{ 0x14CCDB, "33C0F2AEF7D12BF98BC1C1E9028BF78B7C2414", "33C08A47718B44841CE837B20500EB0C909090" },
	// Fix Roster FG Names for Clients
	{ 0x1715D9, "837C24184FB950000000", "E842690300837C24184F" },
	// Loads proper panel buttons for Combat and Tour directories
	{ 0x144394, "B804000000", "E8873B0600" },
	// Loads briefing button instead of Ready if Combat or Tour modes
	{ 0x14448C, "833D8A2AAE0004", "E88F3A06009090" },
	// Fixes back button on Racing mode
	{ 0x144749, "B803000000", "E8D2370600" },
	// Prevents AI Rank from being shown on Roster unless its Skirmish
	{ 0x150675, "E8864E0000", "E8B6780500" },
	{ 0x1506CE, "E83D600000", "E84D780500" },

	// IFF Color Craft Species Hook
	{ 0x0723D5, "84C075046A51", "E8465B130050" },
	// IFF Color Craft Name Hook
	{ 0x0724B3, "84C075046A52", "E8685A130050" },
	// IFF Color FG Species hook
	{ 0x072791, "84DB75046A51", "E88A57130050" },
	// IFF Color FG Name Hook
	{ 0x072829, "84DB75046A52", "E8F256130050" },
	// IFF Color Player Target Box Hook
	{ 0x102CB4, "83F8047713FF2485103A5000", "E867520A00EB139090909090" },
	// IFF Color Map Player Target Box Hook
	{ 0x09EE29, "7719FF24858C044A00C645F83F", "9090E8F090100090908845F890" },
	// IFF Color FG Map marker hook
	{ 0x09F25C, "7723FF2485A0044A00", "9090E8BD8C1000EB21" },
	// IFF Craft Names List Hook
	{ 0x0C1D27, "84C075046A52", "E8F4610E0050" },

	// Combat Button Hook
	{ 0x145E00, "0F8460010000", "E81B21060090" },
	// Fix Craft Selection Groups for Tour mode
	{ 0x150871, "B801000000", "E8AA760500" },
	// Force player in open slot except Multiplayer and Racing
	{ 0x1451A2, "392D8A2AAE00", "E8792D060090" },
	{ 0x1451A8, "755D", "9090" },
	// Fix load button hook
	{ 0x149F1C, "0F8522230000", "E8FFDF050090" },
	// Difficulty Setting Hook
	{ 0x14BB37, "0F85D9010000", "E8E4C3050090" },
	// Craft Selection Setting Hook
	//{ 0x14A88E, "833DB4D7AB0002", "E88DD605009090" },
	// XvT Craft Selection Hook
	{ 0x14FBC5, "0F8533170000", "E85683050090" },
	// Combat Sim music hook
	{ 0x142D01, "391D80CFAB00", "E81A52060090" },
	// Combat Sim background hooks
	{ 0x14C3AB, "E8C04DFEFF", "E870BB0500" },
	{ 0x13B62B, "6808256000", "E8F0C80600" },
	{ 0x164712, "E859CAFCFF", "E809380400" },
	{ 0x16DC19, "E85235FCFF", "E802A30300" },
	{ 0x16DA79, "E8F236FCFF", "E8A2A40300" },
	{ 0x16F186, "E8E51FFCFF", "E8958D0300" },
	{ 0x170D88, "E8E303FCFF", "E893710300" },
	{ 0x17E857, "E81429FBFF", "E8C4960200" },
	// Combat Sim setting bar hooks
	{ 0x14C3DC, "E87F7AFEFF", "E83FBB0500" },
	{ 0x14C6FD, "E85E77FEFF", "E81EB80500" },
	{ 0x13CC45, "83C40C685C010000", "E8D6B20600909090" },
	{ 0x13CE0F, "E84C70FFFF", "E80CB10600" },
	// Inflight Quit message hook
	{ 0xFDD26, "80BEF1948B0001", "E8F5A10A009090" },
	// IFF Orange Message Color change
	{ 0x79D93, "668B15CA9D7C00", "E888E112009090" },
	// IFF Orange Message Log Color change
	{ 0x99553, "8A8178685B00", "E8C8E9100090" },
	// Multiplayer Hangar fix hook
	{ 0xFD859, "E8C297F5FF", "E8C2A60A00" },
	// Update rate hook for host or join room
	{ 0x13C56A, "E8918F0100", "E8B1B90600" },
	{ 0x13C57E, "881D76C8B000", "909090909090" },
	// Update rate hook for Options menu
	{ 0x127C8D, "E8DEA9FFFF", "E88E020800" },
	{ 0x127C9D, "A276C8B000", "9090909090" },
	// Update rate hook for client Options menu
	{ 0x127BB5, "E8F6AAFFFF", "E866030800" },
	{ 0x127BC6, "880D76C8B000", "909090909090" },
	// Key hook	
	{ 0xFD001, "668B0DC0538000", "E81AAF0A009090" },
	// Map AI Takeover hook
	{ 0x1009B3, "E848E4F1FF", "E868750A00" },
	// Spectator Region change hook
	{ 0x1008D5, "0F84A9030000", "E846760A0090" },
	// Combat Engagement Description Load screen fix hook
	//{ 0x130DA2, "0F8432020000", "E87971070090" },

	//////////////////// UI ////////////////////
	// Enable Tour of Duty Button in Multiplayer
	{ 0x145F60, "0F853D080000", "909090909090" },
	// Fix for panel
	{ 0x145807, "893DC4609F00", "909090909090" },
	// Use 5 button panel instead of 2
	{ 0x142FD2, "C705C4609F0002000000", "C705C4609F0005000000" },
	// TODO
	// Allow new pilots to access Tour mode even if they haven't beaten the first mission
	//{ 0x145F9A, "0F84A2030000", "909090909090" },
	{ 0x145F9A, "0F84A2030000", "E8811F060090" },

	{ 0x146160, "7521", "EB21" },
	{ 0x146441, "7514", "EB14" },
	{ 0x146635, "7514", "EB14" },
	{ 0x15379F, "751B", "EB1B" },
	{ 0x1535CF, "751B", "EB1B" },

	// Fixes a bug with Skirmish mode being loaded after a mission when it wasn't skirmish prior
	{ 0x142F1D, "750A", "EB0A" },
	// Fix crash when loading Multiplayer mission briefings in Singleplayer
	{ 0x164B78, "0F8466020000", "909090909090" },
	// Sets the Internet option to off when selecting a TCP/IP game
	{ 0x13C74E, "C60575C8B00001", "C60575C8B00000" },
	// Sets the Internet option to off when launching a game with TCP/IP
	{ 0x128B4C, "C60575C8B00001", "C60575C8B00000" },
	// Fix settings alignment
	{ 0x14A331, "BE04000000", "BE03000000" },
	{ 0x14A35C, "83C605", "83C608" },
	// Allow multiregion option in Skirmish
	//{ 0x142CD5, "0F95C0", "0F94C0" },
	//{ 0x14AF3D, "3C02", "3C05" },

	//////////////////// Gameplay ////////////////////
	// Allow waves for any gamemode set from mission file
	{ 0x105974, "7409", "EB09" },
	// Allow craft jumping for any gamemode set from mission file
	{ 0xFB394, "7408", "EB08" },
	// Changes Targeting box color to white
	{ 0xA89, "6A3B", "6A2F" },
	// Changes Targeting box color to white on map
	{ 0x9EFFF, "6A3B", "6A2F" },
	// Changes the camera target box to a bluish purple color (differentiates itself from the target box and is unlike any IFF player target box)
	{ 0x9EFEE, "6A2F", "6AA8" },
	// Changes normal waypoint marker color to white
	{ 0x9F1DF, "6A36", "6A2F" },
	// Removes white target boxes in Skirmish/Melee
	{ 0x102BEE, "7515", "EB15" },
	// Changes winning team Melee targeting box from purple to dark blue
	{ 0x102C16, "B2D4", "B2CB" },
	// Changes Target's color box to their IFF
	{ 0x102C28, "0F84A4000000", "909090909090" },
	{ 0x102DC8, "7419", "7519" },
	// Removes S-foils from Missile Boat
	{ 0xC4B, "02", "05" },
	// Removes S-foils from Assault Gunboat
	{ 0xC4F, "03", "05" },
	// Removes S-foils from Skipray Blastboat
	{ 0xC50, "03", "05" },

	// Tests
	//{ 0x1430F7, "C705C84B9F0006000000", "E8244E06009090909090" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that enables multiplayer tour", g_multiPatch),
};
