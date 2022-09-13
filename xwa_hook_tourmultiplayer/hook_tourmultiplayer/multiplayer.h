#pragma once

int FixRosterFgNamesHook(int* params);
int FixRosterFgNamesClientsHook(int* params);
int LoadPanelButtonsCombatTourDirectoriesHook(int* params);
int LoadBriefingButtonCombatTourHook(int* params);
int FixBackButtonRacingHook(int* params);
int ShowRankRosterHook1(int* params);
int ShowRankRosterHook2(int* params);

int IFFColorsCraftShortNameHook(int* params);
int IFFColorsCraftNameHook(int* params);
int IFFColorsFGIDHook(int* params);
int IFFColorsFGNameHook(int* params);
int IFFColorsPlayerTargetBoxHook(int* params);
int IFFColorsMapPlayerTargetBoxHook(int* params);
int IFFColorFGMapMarkerHook(int* params);
int IFFColorCraftListNames(int* params);

int PanelButtonsHook(int* params);
int FixCraftSelectionTourHook(int* params);
int ForcePlayerInSlotHook(int* params);
int LoadCancelDeleteButtonHook(int* params);
int DifficultySettingHook(int* params);
int CraftSelectionSettingHook(int* params);
int XVTCraftSelectionHook(int* params);
int CombatSimMusicHook(int* params);
int CombatSimBackgroundGeneralHook(int* params);
int CombatSimBackgroundBriefingHook(int* params);
int CombatSimBackgroundUnknownHook(int* params);
int CombatSimBackgroundJoinRoomHook(int* params);
int CombatSimBackgroundConnectToLobbyHook(int* params);
int CombatSimBackgroundMultiHostStartedLobbyHook(int* params);
int CombatSimBackgroundDebriefingRoomHook(int* params);
int CombatSimBackgroundHostOrJoinHook(int* params);
int CombatSimSettingBarHostOrJoinHook1(int* params);
int CombatSimSettingBarHostOrJoinHook2(int* params);
int CombatSimSettingBarGeneral(int* params);
int CombatSimSettingBarGeneral2(int* params);
int QuitMessageHook(int* params);
int IFFColorOrangeMessageHook(int* params);
int IFFColorOrangeMessageLogHook(int* params);
int HangarMultiplayerHook(int* params);
int UpdateRateHostJoinRoomHook(int* params);
int UpdateRateOptionsMenuHook(int* params);
int UpdateRateOptionsMenuClientHook(int* params);
int KeyHook1(int* params);
int MapAIHook(int* params);
int SpecRegionChangeHook(int* params);
//int CombatDescriptionLoadHook(int* params);
int TourModeButtonHook(int* params);

int Test1Hook(int* params);
