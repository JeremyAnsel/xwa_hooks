#pragma once
#include "XwaTypes.h"
#include "XwaStructs.h"
#include "XwaEnums.h"


// Structs
//Object* ObjectPtr = *(Object**)0x7B33C4; // Set this at runtime
Player* PlayerPtr = (Player*)0x8B94E0;
//CraftInstance* CraftInstancePtr = *(CraftInstance**)0x9106A0; // Set this at runtime
FlightGroupEntry* FlightGroupPtr = (FlightGroupEntry*)0x80DC80;
CraftDefinition* CraftDefinitionPtr = (CraftDefinition*)0x5BB480;
PilotData* PilotDataFrontEndPtr = (PilotData*)0xAE2A60;
//QuickMissionData* QuickMissionDataPtr = *(QuickMissionData**)0x9EB8E0; // Set this at runtime


// Frontend Functions
const auto CopyRectToFrom = (LONG(*)(RECT *pRectDest, RECT *pRectSrc))0x558CB0;
const auto CreateButtonWithHighlightAndSnd = (int(*)(RECT *rect, const char *highlightedImage, const char *imageStr, const char *hoverText, int highlightColor, int color, int offset, const char *soundStr))0x5563C0;
const auto GetFrontTxtString = (const char *(*)(int stringIndex))0x55CB50;
const auto LoadSkirmishFile = (int(*)(const char *fileStr, int))0x5529B0;
const auto PlayDirectSound = (int(*)(const char *soundStr, int, int, int, int, int))0x538850;
const auto LoadMissionsFromLstFile = (char(*)())0x566D90;
const auto SetNumOfPlayerSlotsPerTeam = (int(*)())0x54D620;
const auto FormatFGRoster = (int(*)())0x54D6D0;
const auto DisplayMissionDescription = (void(*)(const char *))0x548140;
const auto CreateSelectedButton = (int(*)(RECT *rect, const char *imageStr, const char *hoverText, int selectedButtonColor))0x5568E0;
//const auto LoadImage = (void(*)(const char *imageStr))0x532080;
const auto CombatSimulatorBackgrounds = (int(*)(int))0x54CF90;
const auto AppendCharToString = (char(*)(unsigned char))0x434E30;
const auto DP_PlayerIsHost = (int(*)())0x52DD80;
const auto MouseCursorCollidedWithRect = (bool(*)(RECT *pRect, int cursor_X_Pos, int cursor_Y_Pos))0x5592E0;
const auto GetMousePosition = (int(*)(int* cursorX, int* cursorY))0x55BA50;
const auto SyncCombatSimulatorConfig = (bool(*)(int))0x549330;
const auto SetRectDimensions = (int(*)(RECT *pRect, int left, int top, int right, int bottom))0x558C90;
const auto LoadWaveLst = (int(*)(const char*, int, int))0x43A150;
const auto AlignRectToDatapad = (int(*)(LONG *))0x558D10;
const auto CreateSettingButton = (int(*)(RECT* rect, const char*, const char*, int, int, int textColor, int, const char* sound))0x556660;
const auto UI_DisplayTextWithRect = (int(*)(int pixels, const char *text, RECT *pRect, int textColor))0x5575A0;
const auto ReadSkirmishFile = (int(*)(const char*, int))0x552370;
const auto SyncRoster = (int(*)(int))0x549570;
const auto sprintf_ = (int(*)(char* str, const char * format, ...))0x59A680;
const auto remove_ = (int(*)(const char *lpFileName))0x59BC30;
const auto LoadMissionLst = (void(*)(int missionDirectory))0x547800;
const auto UI_CreateTransparentImage = (int(*)(const char* resStr, int x, int y))0x532350;
const auto UI_DisplayTextWidthHeight = (int(*)(int pixels, unsigned __int8 *text, int width, int height, int textColor))0x557310;
const auto UI_CreateScrollBar = (int(*)(RECT *rect, int scrollPosition, int, int, int, int, int))0x5555D0;
const auto GetCraftSpeciesShortNamePtr = (int(*)(int speciesIndex))0x4DCE50;
const auto UI_CreateClickableText = (int(*)(int, int, const char* buffer, int, int highlightColor, int, int, const char* sound))0x556100;
const auto GetFGPointWorth = (int(*)(int craftType, int fgCreationFlags, int countermeasures, int warheads, int fgAIRank, int fgDuty))0x555420;
const auto GetSpeciesExternalData = (int(*)(int *speciesPtr))0x5552A0;
const auto GetShipBMP = (int(*)(int speciesIndex))0x55DC20;
const auto DisplayShipBMP = (int(*)())0x55DC70;
const auto SetRectWidthHeight = (RECT*(*)(RECT* rect, int width, int height))0x558CD0;
const auto Imuse_ChangeMusicState = (int(*)(int musicID))0x49AA40;
const auto Imuse_StopMusic = (char(*)())0x49ADA0;
const auto TryLoadFrontResEntryForceEncode = (void**(*)(const char* resFileName, const char* resName))0x531D70;
const auto DrawDatapadFrontResRLETransparent = (int(*)(const char* resName, int xPos, int yPos))0x534A60;
const auto DisplayMessage = (int(*)(int messageIndex, int playerIndex))0x497D40;
const auto GetKeyboardDeviceState = (int(*)())0x42B900;
const auto DirectInputKeyboardReaquire = (char(*)())0x42B920;
const auto WillPlayerBeCaptured = (bool(*)(int playerIndex))0x509530;
const auto PlaySoundFromObject = (int(*)(unsigned int soundIndex, int objectIndexToPlaySoundFrom, unsigned int playerIndex))0x43BF90;
const auto GetPaletteColorForText = (char(*)(unsigned int colorIndex))0x434AD0;
const auto UI_DisplayText = (int(*)(unsigned __int8* text, signed int pixels))0x557F10;
const auto UI_CreateOptionSettingButton = (int(*)(__int8* settingPtr, unsigned int FrontTxtIndex, int, int, int*, int*, __int8*, int))0x523270;
const auto CreateImage = (int(*)(const char* image, int size))0x5322F0;
const auto CreateRectBasedOnResData = (int(*)(const char* resData, RECT* rect))0x532180;
const auto MouseButton1Pressed = (int(*)())0x558190;
const auto MouseButton2Pressed = (int(*)())0x5581B0;
const auto MouseButton1Released = (int(*)())0x5581D0;
const auto MouseButton2Released = (int(*)())0x5581F0;

// In Flight Functions
const auto ReEnterHangar = (int(*)(unsigned __int16))0x457C20;
const auto UpdateFGStatsOfRemovedObject = (char(*)(unsigned __int16 objectIndex, unsigned __int16 FGIndex, unsigned __int16))0x4D8140;
const auto ResetUnknownPlayerPtr = (int(*)(Craft* craftPtr))0x41D3F0;
const auto SpawnFG = (signed __int16(*)(__int16))0x4195E0;
const auto PlayerEject = (signed int(*)(unsigned int playerIndex))0x507D60;
const auto UnableToSpawnPlayer = (signed int(*)(int playerIndex, int, int jumpToNextCraftID, int))0x41EF60;
const auto SavePlayerCraftSettings = (void(*)(int playerIndex))0x420240;
const auto CheckPlayerFGForNewWave = (char(*)(unsigned __int16 playerFGIndex))0x418FA0;
const auto RemovePlayerFromPlay = (char(*)(int playerIndex))0x505FA0;
const auto GetFlightGroupLeaderCraft = (__int16(*)(__int16 fgIndex))0x4A2640;
const auto GetCraftDefinitionIndex = (__int16(*)(unsigned __int16 objectSpecies))0x4DCE30;
const auto PlayOrStopBeamSound = (char(*)(int playBeamWeaponSounds, int playerIndex))0x43CAE0;
const auto PlayOrStopMissileLockWarningSound = (char(*)(int missileLockSound))0x43CDF0;
const auto ActivateSubMessageWithCraftName = (int(*)(int inFlightStringID))0x499B00;
const auto DisplayRemainingPlayersMessage = (int(*)(int playerIndex))0x506130;
const auto IsCraftJumpPossible = (signed int(*)(int playerIndex, int searchForJumpableCraft, int dontResetOrders))0x41FA00;
const auto CheckRegionObjectsForPlayersInPlay = (signed int(*)(int playerIndex))0x5063B0;
const auto PlayerOrderedToHyperOrHome = (char(*)(Craft* craftPtr, int playerIndex, char region))0x508AF0;


// Globals

// UI
const auto missionDirectory = (int *)0xAE2A8A;
const auto implogoStringPtr = (char *)0x60492C;
const auto selectedColor = (int *)0xAE2A30;
const auto normalColor = (int *)0xABD224;
const auto highlightedTextColor = (int *)0xABD228;
const auto highlightedTextColor2 = (int *)0xAE2A48;
const auto combatSimScreenDisplayed = (int *)0x9F4BC8;
const auto rightPanelState = (int *)0x9F4B94;
const auto rightPanelState2 = (int *)0x9F4B48;
const auto leftPanelState = (int *)0x9F4B4C;
const auto configConcourseSFXVolume = (__int8 *)0xB0C887;
const auto configDifficulty = (__int8 *)0xB0C8BB;
const auto missionDescriptionPtr = *(char **)0x9F4BD0;
const auto missionLstPtr = (int *)0x9F4B98;
const auto missionIndexLoaded = (int *)0x9F5E74;
const auto missionSelectedOnLoadScrn = (int *)0x7830C0;
const auto missionCountInLst = (int *)0x9F5EC0;
const auto missionDirectoryMissionSelected = (int *)0xAE2A8E;
const auto missionDescriptionScrollPosition = (int *)0x7831B0;
RECT* rectStandard3 = (RECT *)0x6031A8;
const auto battleSelectScrollMovement = (int *)0x78317C;
const auto loadScrnTotalMissionsListed = (int *)0x7830BC;
const auto thisPlayerIndex = (int *)0x8C1CC8;
const auto thisPlayerType = (int *)0xABD7B4;
const auto flightGroupInfo = (int *)0x783194;
const auto battleSelectMissionScrollIndex = (int *)0x783174;
const auto tacOfficerLoaded = (int *)0x7B6FAE;
const auto battleSelectScrollReturnMovement = (int *)0x783184;
int* FGCreationSlotNum_ = (int*)0x783188;
const auto FGCreationSelectCraftMenu = (int*)0x7830DC;
const auto FGCreationSpecies = (__int16*)0x9F4B82;
const auto FGCreationNumCraft = (__int8*)0x9F4B8D;
const auto FGCreationNumWaves = (__int8*)0x9F4B8C;
const auto FGCreationPlayerInSlotTable = (int*)0x9F5EE0;
const auto mainStringBuffer = (char(*)[])0xABD680;
const auto aSS_3 = (char*)0x603394;
const auto skirmishConstStr = (char*)0x603174;
const auto configGoalType = (__int8*)0xB0C8DE;
const auto colorOfLastDisplayedMessage = (__int16*)0x91AC9C;
const auto g_messageColor = (char*)0x8D6C4C;
const auto FGCreationNumOfCraftListed = (int*)0x7830D0;
const auto FGCreationScrollPosition = (int*)0x78319C;
const auto FGCreationCraftIteration = (int*)0x783178;
const auto totalCraftInCraftTable = (int*)0xABD7DC;
const auto speciesShipListPtr = (int*)0xABD22C;
const auto FGCreationPlayerNum = (int*)0x9F4B84;
const auto FGCreationPlayerGunnerNum = (int*)0x9F4B88;
const auto FGCreationGenusSelected = (int*)0x7831A4;
const auto aSS_4 = (char*)0x603548;
const auto aS_0 = (char*)0x5B0E4C;
const auto FGCreationFlags = (int*)0x9F4B90;
const auto FGCreationAIRank = (__int8*)0x9F4B8E;
const auto FGCreationDuty = (__int8*)0x9F4B8F;
const auto FGCreationNumOfWaves = (__int8*)0x9F4B8C;
const auto FGCreationCraftSlotNumTable = (int*)0xABD280;
const auto aD = (char*)0x6012D4;
const auto campaignIncomplete = (int*)0xABCF80;
const auto currentMissionInCampaign = (int*)0xABC970;
const auto currentMusicPlaying = (int*)0xAE2A44;
const auto configDatapadMusic = (__int8*)0xB0C88E;
const auto dword_B0C784 = (int*)0xB0C784;
const auto grayedOutTextColor = (int*)0xABD218;
const auto configSFXDatapad = (char*)0xB0C8D4;
const auto dword_ABD7BC = (int*)0xABD7BC;
const auto rectStandard4 = (RECT*)0x6031B8;
const auto rectStandard5 = (RECT*)0x6031C8;


// In Flight

const auto provingGrounds = (__int8*)0x8053E5;
const auto tacOfficerInMission = (__int8*)0x7B6FAE;
const auto waveLstString = (int*)0x5B328C;
const auto numberOfPlayersInGame = (int*)0x910DEC;
const auto viewingFilmState = (__int8*)0x80DB68;
const auto mouseLook = (__int8*)0x77129C;
const auto mouseLookWasNotEnabled = (__int8*)0x7712A0;
const auto keyPressedAfterLocaleAfterMapping = (__int16*)0x8053C0;
const auto win32NumPad2Pressed = (__int8*)0x9E9570;
const auto win32NumPad4Pressed = (__int8*)0x9E956B;
const auto win32NumPad5Pressed = (__int8*)0x9E956C;
const auto win32NumPad6Pressed = (__int8*)0x9E956D;
const auto win32NumPad8Pressed = (__int8*)0x9E9568;
const auto inMissionFilmState = (__int8*)0x7D4C4D;
const auto mouseLook_Y = (int*)0x9E9624;
const auto mouseLook_X = (int*)0x9E9620;
const auto mouseLookInverted = (__int8*)0x771298;
const auto mouseLookResetPosition = (int*)0x9E962C;
const auto missionType = (__int8*)0x7B6FAA;
const auto isMelee = (__int8*)0x7B6FAB;
const auto TeamMissionOutcomeTable = (__int8*)0x807A60;
const auto missionMessageBufferColor = (__int16*)0x7C9DCA;
const auto MissionMessageColorTable = (char*)0x5B6878;
const auto missionMessageLogPtr = (int*)0x9B6320;
const auto thisPlayerObjectIndex = (int*)0x634370;
const auto configUpdateRate = (__int8*)0xB0C876;
const auto latestNonInstantiatedRegion = (int*)0x80DB60;
const auto ticksPerFrame = (int*)0x8C1640;
const auto missionScrollPosition = (int*)0x7830E8;
const auto IsObjectEnemy = (bool(*)(unsigned __int16 objectIndex, int playerTeam))0x505D40;

// Unknown Globals

const auto dword_7833D4 = (int*)0x7833D4;
const auto dword_B07C6C = (int*)0xB07C6C;
const auto dword_AE2A7E = (int*)0xAE2A7E;
