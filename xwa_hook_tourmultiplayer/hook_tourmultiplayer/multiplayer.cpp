#include "targetver.h"
#include "multiplayer.h"
#include "config.h"
//#include <Windows.h>
#include "XwaFramework.h"

// Stack contents for function params
enum ParamsEnum
{
	Params_ReturnAddress = -1,
	Params_EAX = -3,
	Params_ECX = -4,
	Params_EDX = -5,
	Params_EBX = -6,
	Params_EBP = -8,
	Params_ESI = -9,
	Params_EDI = -10,
};

#pragma pack(push, 1)

#pragma pack(pop)

class Config
{
public:
	Config()
	{
		auto lines = GetFileLines("hooks.ini", "hook_tourmultiplayer");

		if (lines.empty())
		{
			lines = GetFileLines("hook_tourmultiplayer.cfg");
		}

		this->SimulatorBackground = GetFileKeyValue(lines, "SimulatorBackground");

		if (this->SimulatorBackground.empty() || _stricmp(this->SimulatorBackground.c_str(), "blank"))
		{
			//"frontres\\concourse\\create.bmp";
			//"frontres\\combat\\multiplayer.bmp";
			this->SimulatorBackground = "frontres\\concourse\\create.bmp";
		}

		this->SettingsBar = GetFileKeyValueInt(lines, "SettingsBar", 0) != 0;
		this->ChangeCombatSimMusic = GetFileKeyValueInt(lines, "ChangeCombatSimMusic", 1) != 0;
		this->TourOfDutyButtonRequiredMissionIndex = GetFileKeyValueInt(lines, "TourOfDutyButtonRequiredMissionIndex", 1);
		this->GreenAndRedForIFFColorsOnly = GetFileKeyValueInt(lines, "GreenAndRedForIFFColorsOnly", 0);
	}

	std::string SimulatorBackground;
	bool SettingsBar;
	bool ChangeCombatSimMusic;
	int TourOfDutyButtonRequiredMissionIndex;
	bool GreenAndRedForIFFColorsOnly;
};

Config g_config;

int FixRosterFgNamesHook(int* params)
{
	const int index = params[Params_EAX];
	const char* name = (const char*)params[Params_EDI];

	typedef char T0x09F5D20[20];
	T0x09F5D20* s_SkmFlightGroupsNames = (T0x09F5D20*)0x009F5D20;

	strcpy_s(s_SkmFlightGroupsNames[index], name);

	return 0;
}

int FixRosterFgNamesClientsHook(int* params)
{
	const auto L00566D90 = (void(*)())0x00566D90;
	const auto L0054D620 = (void(*)())0x0054D620;
	const auto L0054D6D0 = (void(*)())0x0054D6D0;

	int V0x00AE2A8A = *(int*)0x00AE2A8A;

	if (V0x00AE2A8A != 0x03)
	{
		L00566D90();
		L0054D620();
		L0054D6D0();
	}

	params[Params_ECX] = 0x50;
	return 0;
}

int LoadPanelButtonsCombatTourDirectoriesHook(int* params)
{
	const int ecx = params[Params_ECX];

	if (ecx == 0x02 || ecx >= 0x04)
	{
		params[-1] = 0x00544F9D;
	}
	else
	{
		params[-1] = 0x00544FCB;
	}

	params[Params_EAX] = 0x04;
	return 0;
}

int LoadBriefingButtonCombatTourHook(int* params)
{
	int V0x00AE2A8A = *(int*)0x00AE2A8A;

	if (V0x00AE2A8A == 0x02 || V0x00AE2A8A >= 0x04)
	{
		params[-1] = 0x00545095;
	}
	else
	{
		params[-1] = 0x005450C4;
	}

	return 0;
}

int FixBackButtonRacingHook(int* params)
{
	const int ecx = params[Params_ECX];

	if (ecx == 0x01 || ecx == 0x03)
	{
		params[-1] = 0x00545352;
	}
	else
	{
		params[-1] = 0x0054536D;
	}

	params[Params_EAX] = 0x03;
	return 0;
}

int ShowRankRosterHook1(int* params)
{
	const auto L00556100 = (int(*)(int, int, int, int, int, int, int, int))0x00556100;

	int V0x00AE2A8A = *(int*)0x00AE2A8A;

	int ret = 0;

	if (V0x00AE2A8A == 0x03)
	{
		ret = L00556100(params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7]);
	}

	return ret;
}

int ShowRankRosterHook2(int* params)
{
	const auto L00557310 = (void(*)(int, int, int, int, int))0x00557310;

	int V0x00AE2A8A = *(int*)0x00AE2A8A;

	if (V0x00AE2A8A == 0x03)
	{
		L00557310(params[0], params[1], params[2], params[3], params[4]);
	}

	return 0;
}

int IFFColorsCraftShortNameHook(int* params)
{
	unsigned char targetIFF = (unsigned char)params[Params_EAX];
	int targetIndex = params[Params_ESI] / sizeof(Object);

	int color = 0;

	if (g_config.GreenAndRedForIFFColorsOnly)
	{
		if (IsObjectEnemy(targetIndex, PlayerPtr[*thisPlayerIndex].team))
		{
			color = FlightTextColor_DarkRed;
		}
		else
		{
			color = FlightTextColor_DarkGreen;
		}
	}
	else
	{
		switch (targetIFF)
		{
		case IFF_Rebel:
			color = FlightTextColor_DarkGreen;
			break;

		case IFF_Imperial:
			color = FlightTextColor_DarkRed;
			break;

		case IFF_Red:
			color = FlightTextColor_LightRed;
			break;

		case IFF_Blue:
			color = FlightTextColor_DarkBlue;
			break;

		case IFF_Purple:
			color = FlightTextColor_DarkPurple;
			break;

		case IFF_Yellow:
			color = FlightTextColor_DarkYellow;
			break;

		default:
			color = FlightTextColor_DarkYellow;
			break;
		}
	}
	
	params[Params_EAX] = color;

	return 0;
}

int IFFColorsCraftNameHook(int* params)
{
	unsigned char targetIFF = (unsigned char)params[Params_EAX];
	int targetIndex = params[Params_ESI] / sizeof(Object);

	int color;

	if (g_config.GreenAndRedForIFFColorsOnly)
	{
		if (IsObjectEnemy(targetIndex, PlayerPtr[*thisPlayerIndex].team))
		{
			color = FlightTextColor_LightRed;
		}
		else
		{
			color = FlightTextColor_LightGreen;
		}
	}
	else
	{
		switch (targetIFF)
		{
		case IFF_Rebel:
			color = FlightTextColor_LightGreen;
			break;

		case IFF_Imperial:
			color = FlightTextColor_LightRed;
			break;

		case IFF_Red:
			color = FlightTextColor_LightestRed;
			break;

		case IFF_Blue:
			color = FlightTextColor_LightBlue;
			break;

		case IFF_Purple:
			color = FlightTextColor_LightPurple;
			break;

		case IFF_Yellow:
			color = FlightTextColor_LightYellow;
			break;

		default:
			color = FlightTextColor_LightYellow;
			break;
		}
	}

	params[Params_EAX] = color;

	return 0;
}

int IFFColorsFGIDHook(int* params)
{
	unsigned char targetIFF = (unsigned char)params[Params_EBX];
	int targetIndex = params[Params_ESI] / sizeof(Object);

	int color;

	if (g_config.GreenAndRedForIFFColorsOnly)
	{
		if (IsObjectEnemy(targetIndex, PlayerPtr[*thisPlayerIndex].team))
		{
			color = FlightTextColor_DarkRed;
		}
		else
		{
			color = FlightTextColor_DarkGreen;
		}
	}
	else
	{
		switch (targetIFF)
		{
		case IFF_Rebel:
			color = FlightTextColor_DarkGreen;
			break;

		case IFF_Imperial:
			color = FlightTextColor_DarkRed;
			break;

		case IFF_Red:
			color = FlightTextColor_LightRed;
			break;

		case IFF_Blue:
			color = FlightTextColor_DarkBlue;
			break;

		case IFF_Purple:
			color = FlightTextColor_LightPurple;
			break;

		case IFF_Yellow:
			color = FlightTextColor_DarkYellow;
			break;

		default:
			color = FlightTextColor_DarkYellow;
			break;
		}
	}

	

	params[Params_EAX] = color;

	return 0;
}

int IFFColorsFGNameHook(int* params)
{
	unsigned char targetIFF = (unsigned char)params[Params_EBX];
	int targetIndex = params[Params_ESI] / sizeof(Object);

	int color;

	if (g_config.GreenAndRedForIFFColorsOnly)
	{
		if (IsObjectEnemy(targetIndex, PlayerPtr[*thisPlayerIndex].team))
		{
			color = FlightTextColor_LightRed;
		}
		else
		{
			color = FlightTextColor_LightGreen;
		}
	}
	else
	{

	}

	switch (targetIFF)
	{
	case IFF_Rebel:
		color = FlightTextColor_LightGreen;
		break;

	case IFF_Imperial:
		color = FlightTextColor_LightRed;
		break;

	case IFF_Red:
		color = FlightTextColor_LightestRed;
		break;

	case IFF_Blue:
		color = FlightTextColor_LightBlue;
		break;

	case IFF_Purple:
		color = FlightTextColor_LightPurple;
		break;

	case IFF_Yellow:
		color = FlightTextColor_LightYellow;
		break;

	default:
		color = FlightTextColor_LightYellow;
		break;
	}

	params[Params_EAX] = color;

	return 0;
}

int IFFColorsPlayerTargetBoxHook(int* params)
{
	unsigned char targetIFF = (unsigned char)params[Params_EAX];
	int targetIndex = params[Params_ECX] / sizeof(Object);

	int color;

	if (g_config.GreenAndRedForIFFColorsOnly)
	{
		if (IsObjectEnemy(targetIndex, PlayerPtr[*thisPlayerIndex].team))
		{
			color = FlightTextColor_MainPal_LightestRed;
		}
		else
		{
			color = FlightTextColor_MainPal_LightestGreen;
		}
	}
	else
	{
		switch (targetIFF)
		{
		case IFF_Rebel:
			color = FlightTextColor_MainPal_LightestGreen;
			break;

		case IFF_Imperial:
			//color = 0xC3;
			color = FlightTextColor_MainPal_LightRed;
			break;

		case IFF_Blue:
			color = FlightTextColor_MainPal_LightestBlue;
			break;

		case IFF_Yellow:
			color = FlightTextColor_MainPal_LightestYellow;
			break;

		case IFF_Red:
			//color = 0xC2;
			color = FlightTextColor_MainPal_LightestRed;
			break;

		case IFF_Purple:
			//color = 0xD3;
			color = FlightTextColor_LightestPurple;
			break;

		default:
			color = 0x86;
			break;
		}
	}

	

	params[Params_EDX] = color;

	return 0;
}

int IFFColorsMapPlayerTargetBoxHook(int* params)
{
	unsigned char targetIFF = (unsigned char)params[Params_EAX];
	int targetIndex = params[Params_ESI] / sizeof(Object);

	int color;

	if (g_config.GreenAndRedForIFFColorsOnly)
	{
		if (IsObjectEnemy(targetIndex, PlayerPtr[*thisPlayerIndex].team))
		{
			color = FlightTextColor_MainPal_LightestRed;
		}
		else
		{
			color = FlightTextColor_MainPal_LightestGreen;
		}
	}
	else
	{
		switch (targetIFF)
		{
		case IFF_Rebel:
			color = FlightTextColor_MainPal_LightestGreen;
			break;

		case IFF_Imperial:
			//color = 0xC3;
			color = FlightTextColor_MainPal_LightRed;
			break;

		case IFF_Blue:
			color = FlightTextColor_MainPal_LightestBlue;
			break;

		case IFF_Yellow:
			color = FlightTextColor_MainPal_LightestYellow;
			break;

		case IFF_Red:
			//color = 0xC2;
			color = FlightTextColor_MainPal_LightestRed;
			break;

		case IFF_Purple:
			//color = 0xD3;
			color = FlightTextColor_LightestPurple;
			break;

		default:
			color = 0x86;
			break;
		}
	}
	

	

	params[Params_EAX] = color;

	return 0;
}

int IFFColorFGMapMarkerHook(int* params)
{
	unsigned char targetIFF = (unsigned char)params[Params_EAX];
	int targetIndex = params[Params_ESI] / sizeof(Object);

	int color;

	if (g_config.GreenAndRedForIFFColorsOnly)
	{
		if (IsObjectEnemy(targetIndex, PlayerPtr[*thisPlayerIndex].team))
		{
			color = FlightTextColor_MainPal_LightestRed;
		}
		else
		{
			color = FlightTextColor_MainPal_LightestGreen;
		}
	}
	else
	{
		switch (targetIFF)
		{
		case IFF_Rebel:
			color = FlightTextColor_MainPal_LightestGreen;
			break;

		case IFF_Imperial:
			color = FlightTextColor_MainPal_LightRed;
			break;

		case IFF_Blue:
			color = FlightTextColor_MainPal_LightestBlue;
			break;

		case IFF_Yellow:
			color = FlightTextColor_MainPal_LightestYellow;
			break;

		case IFF_Red:
			color = FlightTextColor_MainPal_LightestRed;
			break;

		case IFF_Purple:
			color = FlightTextColor_LightPurple;
			break;

		default:
			color = 0x86;
			break;
		}
	}
	

	

	params[Params_EAX] = color;

	return 0;
}

int IFFColorCraftListNames(int* params)
{
	unsigned char targetIFF = (unsigned char)params[Params_EAX];
	int targetIndex = params[Params_EDI] / sizeof(Object);

	int color;

	if (g_config.GreenAndRedForIFFColorsOnly)
	{
		if (IsObjectEnemy(targetIndex, PlayerPtr[*thisPlayerIndex].team))
		{
			color = FlightTextColor_LightRed;
		}
		else
		{
			color = FlightTextColor_LightGreen;
		}
	}
	else
	{
		switch (targetIFF)
		{
		case IFF_Rebel:
			color = FlightTextColor_LightGreen;
			break;

		case IFF_Imperial:
			color = FlightTextColor_LightRed;
			break;

		case IFF_Blue:
			color = FlightTextColor_LightBlue;
			break;

		case IFF_Yellow:
			color = FlightTextColor_LightYellow;
			break;

		case IFF_Red:
			color = FlightTextColor_LightestRed;
			break;

		case IFF_Purple:
			color = FlightTextColor_LightPurple;
			break;

		default:
			color = 0x87;
			break;
		}
	}
	

	

	params[Params_EAX] = color;

	return 0;
}

bool shiftMPressed = false;

int PanelButtonsHook(int* params)
{
	int playerIsHost = DP_PlayerIsHost();
	RECT rectButton[4] = { 0,0,0,0 };
	// Orginally: 70, 449, 97, 473 
	RECT rectImpLogo[4] = { 246, 449, 263, 473 };

	CopyRectToFrom(rectButton, rectImpLogo);

	// Create Combat Engagements button
	if (*missionDirectory != MissionDirectory_Combat)
	{
		if (*thisPlayerType == PlayerType_Singleplayer || playerIsHost)
		{
			if (CreateButtonWithHighlightAndSnd(rectButton, "implogo1", "implogo1", GetFrontTxtString(STR_COMBAT), *highlightedTextColor, *normalColor, 12, "jewelsound"))
			{
				LoadSkirmishFile("temp\temp6753908", 0);
				// Fixes background
				// TODO
				//params[-12] = 2;
				params[Params_EDI] = 2;
				*missionDirectory = MissionDirectory_Combat;
				*combatSimScreenDisplayed = CombatSimulatorScreenType_Roster;

				if (*rightPanelState2 != 1)
				{
					*rightPanelState2 = 0;
					*rightPanelState = 1;
					PlayDirectSound("panelarm", 1, 0, 255, 12 * *configConcourseSFXVolume, 63);
				}
				*leftPanelState = 4;
				if (*configDifficulty == Difficulty_NULL)
				{
					*configDifficulty = Difficulty_Easy;
				}
				LoadMissionsFromLstFile();
				SetNumOfPlayerSlotsPerTeam();
				FormatFGRoster();
				DisplayMissionDescription(missionDescriptionPtr);
				*missionDescriptionScrollPosition = 0;
				params[-1] = 0x546C6A;
			}
		}
		else
		{
			//Client Version
			CreateSelectedButton(rectButton, "implogo1", GetFrontTxtString(STR_COMBAT), *normalColor);
		}
	}
	else
	{
		// Create a selected Combat Engagements button that does nothing when clicked
		CreateSelectedButton(rectButton, "implogo1", GetFrontTxtString(STR_COMBAT), *selectedColor);
	}

	// Create Load button
	if (*missionDirectory != MissionDirectory_Skirmish)
	{
		CopyRectToFrom(rectButton, rectStandard3);
		if (*thisPlayerType == PlayerType_Singleplayer || playerIsHost)
		{
			if (CreateButtonWithHighlightAndSnd(rectButton, "load", "load", GetFrontTxtString(STR_GAME_LOAD), *highlightedTextColor, *normalColor, 13, "jewelsound"))
			{
				*combatSimScreenDisplayed = CombatSimulatorScreenType_Load;
				*battleSelectScrollMovement = 0;
				*battleSelectScrollReturnMovement = 0;
				*missionSelectedOnLoadScrn = 0;
				*loadScrnTotalMissionsListed = *missionCountInLst;

				signed int missionsListed = *missionCountInLst;
				int missionIndex = 0;
				char missionTitleBuffer[128]{};
				memset(&missionTitleBuffer, 0, 128);
				int missionIndexSelected = 0;
				MissionLSTEntry* missionLst = *(MissionLSTEntry**)0x9F4B98;

				if (*missionCountInLst > 0)
				{
					do
					{
						if (missionLst[missionIndex].IsUnselectable)
						{
							*loadScrnTotalMissionsListed = --missionsListed;
						}
						else
						{
							const char* missionTitle = missionLst[missionIndex].MissionTitle;
							if (strcmp(missionLst[missionIndex].MissionTitle, missionTitleBuffer))
							{
								*loadScrnTotalMissionsListed = ++missionsListed;
								strcpy_s(missionTitleBuffer, missionTitle);
							}
						}
						missionIndexSelected = ++missionIndex;
					} while (missionIndex < *missionCountInLst);
				}

				if (*rightPanelState2 != 3)
				{
					*rightPanelState2 = 2;
					PlayDirectSound("panelarm", 1, 0, 255, 12 * *configConcourseSFXVolume, 63);
				}
			}
		}
		else
		{
			// Create Client version of button
			CreateSelectedButton(rectButton, "load", GetFrontTxtString(STR_GAME_LOAD), *normalColor);
		}
	}

	// Create Campaign button

	//RECT campaignRect[4] = { 10, 349, 17, 373 };

	//CopyRectToFrom(rectButton, campaignRect);
	//if (*missionDirectory != 5)
	//{
	//	if (*localPlayerConnectedAs == PlayerType_Singleplayer || playerIsHost)
	//	{
	//		if (CreateButtonWithHighlightAndSnd(rectButton, "azrecord", "azrecord", "Campaign", *highlightedTextColor, *normalColor, 12, "jewelsound"))
	//		{

	//		}
	//	}
	//	else
	//	{
	//		// Client version
	//		CreateSelectedButton(rectButton, "azrecord", "Campaign", *normalColor);
	//	}
	//}
	//else
	//{
	//	CreateSelectedButton(rectButton, "azrecord", "Campaign", *selectedColor);
	//}

	// Create Racing Button

	RECT racingRect[4] = { 278, 449, 295, 473 };

	CopyRectToFrom(rectButton, racingRect);

	if (*missionDirectory != MissionDirectory_Melee)
	{
		if (*thisPlayerType == PlayerType_Singleplayer || playerIsHost)
		{
			if (CreateButtonWithHighlightAndSnd(rectButton, "yard", "yard", GetFrontTxtString(STR_MELEE), *highlightedTextColor, *normalColor, 12, "jewelsound"))
			{
				params[-1] = 0x546A6E;
			}
			else
			{
				params[-1] = 0x546B59;
			}
		}
		else
		{
			// Client version
			CreateSelectedButton(rectButton, "yard", GetFrontTxtString(STR_MELEE), *normalColor);
			params[-1] = 0x546B59;
		}
	}
	else
	{
		int curX, curY;
		CreateSelectedButton(rectButton, "yard", GetFrontTxtString(STR_MELEE), *selectedColor);
		GetMousePosition(&curX, &curY);
		if (!playerIsHost || !MouseCursorCollidedWithRect(rectButton, curX, curY))
		{
			params[-1] = 0x546B59;
		}
		else
		{
			params[-1] = 0x547566;
		}
	}

	return 0;
}

int FixCraftSelectionTourHook(int* params)
{
	int* fg = (int*)(276 * *FGCreationSlotNum_ + 0x9F4BE0);
	if (fg[30] == MissionCraftOptions_AllFlyable || *missionDirectory == MissionDirectory_Melee)
	{
		*FGCreationSelectCraftMenu = FGCreationSelectCraftMenu_AutoDetect;
		params[-1] = 0x5514DB;
	}
	else if (fg[30] == MissionCraftOptions_None)
	{
		params[-1] = 0x551503;
	}
	else if (fg[30] == MissionCraftOptions_AllRebelFlyable || fg[30] == MissionCraftOptions_AllImperialFlyable)
	{
		if (fg[30] == MissionCraftOptions_AllRebelFlyable)
			*FGCreationSelectCraftMenu = FGCreationSelectCraftMenu_AllRebelFlyable;
		else if (fg[30] == MissionCraftOptions_AllImperialFlyable)
			*FGCreationSelectCraftMenu = FGCreationSelectCraftMenu_AllImperialFlyable;
		params[-1] = 0x551503;
	}
	else if (fg[30] == MissionCraftOptions_Custom)
	{
		int _edx = fg[31];
		int _esi = fg[32] + 1;
		fg[32] = _esi;
		if (_esi >= _edx)
		{
			fg[32] = 0;
		}
		*FGCreationSpecies = fg[fg[32] + 33];
		// 0 craft is interpreted as 0 craft so it will be buggy.
		// Set Number of craft to 1 if so.
		if (fg[fg[32] + 45] == 0)
		{
			*FGCreationNumCraft = 1;
		}
		else
		{
			*FGCreationNumCraft = fg[fg[32] + 45];
		}
		// 0 Waves on the other hand is interpreted as 1 wave which is fine.
		*FGCreationNumWaves = fg[fg[32] + 57];
		params[-1] = 0x551503;
	}
	return 0;
}

int ForcePlayerInSlotHook(int* params)
{
	if (*thisPlayerType != PlayerType_Singleplayer && *missionDirectory != MissionDirectory_Melee)
	{
		params[-1] = 0x545E07;
	}

	return 0;
}

int LoadCancelDeleteButtonHook(int* params)
{
	params[-1] = 0x54CE44;

	if (*combatSimScreenDisplayed != CombatSimulatorScreenType_Load)
	{
		return 0;
	}

	int curX, curY;
	GetMousePosition(&curX, &curY);
	int highlightColor = *highlightedTextColor;
	RECT pRect;
	RECT pRect2;

	// Load button

	if (*missionDirectory == MissionDirectory_Skirmish)
	{
		SetRectDimensions(&pRect, 170, 351, 269, 425);
	}
	else
	{
		SetRectDimensions(&pRect, 220, 351, 319, 425);
	}

	CopyRectToFrom(&pRect2, &pRect);
	AlignRectToDatapad(&pRect2.left);
	if (MouseCursorCollidedWithRect(&pRect2, curX, curY))
	{
		highlightColor = *highlightedTextColor2;
	}
	int loadSettingClicked = CreateSettingButton(&pRect, "loadsetting", "loadsetting", 0, 10, *normalColor, 20, "settingsound");

	if (*missionDirectory == MissionDirectory_Skirmish)
	{
		SetRectDimensions(&pRect, 170, 351, 269, 363);
	}
	else
	{
		SetRectDimensions(&pRect, 220, 351, 319, 363);
	}

	UI_DisplayTextWithRect(10, GetFrontTxtString(STR_GAME_LOAD), &pRect, highlightColor);

	if (loadSettingClicked)
	{
		if (*missionDirectory == MissionDirectory_Skirmish)
		{
			if ((*(char*)*missionLstPtr + 82 * *missionSelectedOnLoadScrn + 80))
			{
				ReadSkirmishFile(*(char**)missionLstPtr + 328 * *missionSelectedOnLoadScrn, 1);
				*combatSimScreenDisplayed = 1;
			}
			else
			{
				memset(FGCreationPlayerInSlotTable, 0, 0x140);
				LoadMissionsFromLstFile();
				SetNumOfPlayerSlotsPerTeam();
				FormatFGRoster();
				DisplayMissionDescription(missionDescriptionPtr);
			}
			*missionDescriptionScrollPosition = 0;
			SyncCombatSimulatorConfig(0);
			SyncRoster(1);
			*combatSimScreenDisplayed = 1;
			*rightPanelState2 = 0;
			*dword_7833D4 = 0;
			PlayDirectSound("panelarm", 1, 0, 255, 12 * *configConcourseSFXVolume, 63);
		}
		else
		{
			int missionSelected = *missionSelectedOnLoadScrn;
			int* missionLst = missionLstPtr;
			int missionOffset = (*missionLstPtr + missionSelected * 328);

			if (*thisPlayerType == PlayerType_Singleplayer)
			{
				missionDirectoryMissionSelected[*missionDirectory] = *(int*)(missionOffset + 320);
				int missionCount = 0;
				*missionIndexLoaded = 0;

				if (*missionCountInLst > 0)
				{
					int missionFromMissionDirectory = missionDirectoryMissionSelected[*missionDirectory];
					do
					{
						if (missionLst[82 * missionCount + 80] == missionFromMissionDirectory)
							break;
						*missionIndexLoaded = ++missionCount;
					} while (missionCount < *missionCountInLst);
				}
				LoadMissionsFromLstFile();
				SetNumOfPlayerSlotsPerTeam();
				FormatFGRoster();
				DisplayMissionDescription(missionDescriptionPtr);
				*missionDescriptionScrollPosition = 0;
				*combatSimScreenDisplayed = CombatSimulatorScreenType_Roster;
				*rightPanelState2 = 0;
				*dword_7833D4 = 0;
				PlayDirectSound("panelarm", 1, 0, 255, 12 * *configConcourseSFXVolume, 63);
			}
			else
			{
				if (*(char*)(*missionLstPtr + missionSelected * 328) != '1')
				{
					int missionOffset = (*missionLstPtr + missionSelected * 328);
					missionDirectoryMissionSelected[*missionDirectory] = *(int*)(missionOffset + 320);
					int missionCount = 0;
					*missionIndexLoaded = 0;

					if (*missionCountInLst > 0)
					{
						int missionFromMissionDirectory = missionDirectoryMissionSelected[*missionDirectory];
						do
						{
							if (missionLst[82 * missionCount + 80] == missionFromMissionDirectory)
								break;
							*missionIndexLoaded = ++missionCount;
						} while (missionCount < *missionCountInLst);
					}

					LoadMissionsFromLstFile();
					SetNumOfPlayerSlotsPerTeam();
					FormatFGRoster();
					DisplayMissionDescription(missionDescriptionPtr);
					*missionDescriptionScrollPosition = 0;
					SyncCombatSimulatorConfig(0);
					*combatSimScreenDisplayed = CombatSimulatorScreenType_Roster;
					*rightPanelState2 = 0;
					*dword_7833D4 = 0;
					PlayDirectSound("panelarm", 1, 0, 255, 12 * *configConcourseSFXVolume, 63);
				}
			}
		}
	}

	// Cancel Button

	highlightColor = *highlightedTextColor;
	if (*missionDirectory == MissionDirectory_Skirmish)
	{
		SetRectDimensions(&pRect, 270, 351, 369, 425);
	}
	else
	{
		SetRectDimensions(&pRect, 320, 351, 419, 425);
	}

	CopyRectToFrom(&pRect2, &pRect);
	AlignRectToDatapad(&pRect2.left);
	if (MouseCursorCollidedWithRect(&pRect2, curX, curY))
	{
		highlightColor = *highlightedTextColor2;
	}
	int cancelSettingClicked = CreateSettingButton(&pRect, "cancelsetting", "cancelsetting", 0, 10, *normalColor, 21, "settingsound");
	if (*missionDirectory == MissionDirectory_Skirmish)
	{
		SetRectDimensions(&pRect, 270, 351, 369, 363);
	}
	else
	{
		SetRectDimensions(&pRect, 320, 351, 419, 363);
	}

	UI_DisplayTextWithRect(10, GetFrontTxtString(STR_CANCEL), &pRect, highlightColor);
	if (cancelSettingClicked)
	{
		*combatSimScreenDisplayed = CombatSimulatorScreenType_Roster;
		*rightPanelState2 = 0;
		*dword_7833D4 = 0;
		PlayDirectSound("panelarm", 1, 0, 255, 12 * *configConcourseSFXVolume, 63);
	}

	if (*missionDirectory != MissionDirectory_Skirmish)
	{
		return 0;
	}

	// Delete Button

	highlightColor = *highlightedTextColor;
	SetRectDimensions(&pRect, 370, 351, 469, 425);
	CopyRectToFrom(&pRect2, &pRect);
	AlignRectToDatapad(&pRect2.left);
	if (MouseCursorCollidedWithRect(&pRect2, curX, curY))
	{
		highlightColor = *highlightedTextColor2;
	}
	int deleteButtonWasClicked = CreateSettingButton(&pRect, "deletesetting", "deletesetting", 0, 10, *normalColor, 22, "settingsound");
	SetRectDimensions(&pRect, 370, 351, 469, 363);
	UI_DisplayTextWithRect(10, GetFrontTxtString(STR_GAME_DELETE), &pRect, highlightColor);

	if (!deleteButtonWasClicked || !*missionSelectedOnLoadScrn)
	{
		return 0;
	}

	sprintf_(*mainStringBuffer, aSS_3, "skirmish", *missionLstPtr + 328 * *missionSelectedOnLoadScrn);
	remove_(*mainStringBuffer);
	LoadMissionLst(*missionDirectory);
	*missionSelectedOnLoadScrn = 0;

	params[-1] = 0x54AEE8;

	return 0;
}

int DifficultySettingHook(int* params)
{
	if (*missionDirectory == MissionDirectory_Combat || *missionDirectory == MissionDirectory_Missions)
	{
		params[-1] = 0x54C746;
	}
	else
	{
		params[-1] = 0x54C916;
	}

	return 0;
}

int CraftSelectionSettingHook(int* params)
{
	// Changes the Craft Selection setting button in different modes.
	if ((*thisPlayerType != PlayerType_Singleplayer || *missionDirectory != MissionDirectory_Skirmish || *configGoalType) && *missionDirectory != 4)
	{
		params[-1] = 0x54B4AD;
	}
	else
	{
		params[-1] = 0x54B6BB;
	}

	return 0;
}

int XVTCraftSelectionHook(int* params)
{
	// This allows XvT Craft Selection if set in the mission file.

	if (*FGCreationSelectCraftMenu != FGCreationSelectCraftMenu_AllFlyableCraft)
	{
		if (*FGCreationSelectCraftMenu != FGCreationSelectCraftMenu_AllImperialFlyable && *FGCreationSelectCraftMenu != FGCreationSelectCraftMenu_AllRebelFlyable)
		{
			// Jump to AutoDetect code
			return 0;
		}
	}

	if (*FGCreationSelectCraftMenu == FGCreationSelectCraftMenu_AllImperialFlyable || *FGCreationSelectCraftMenu == FGCreationSelectCraftMenu_AllRebelFlyable)
	{
		*FGCreationNumOfCraftListed = 6;

		char displayCraftMenuName[30];
		if (*FGCreationSelectCraftMenu == FGCreationSelectCraftMenu_AllRebelFlyable)
		{
			strcpy_s(displayCraftMenuName, GetFrontTxtString(STR_REBEL1));
			strcat_s(displayCraftMenuName, " ");
			strcat_s(displayCraftMenuName, GetFrontTxtString(STR_CRAFT_CAPS));
		}
		else
		{
			strcpy_s(displayCraftMenuName, GetFrontTxtString(STR_IMPERIAL));
			strcat_s(displayCraftMenuName, " ");
			strcat_s(displayCraftMenuName, GetFrontTxtString(STR_CRAFT_CAPS));
		}

		UI_CreateTransparentImage("leftframe", 70, 86);
		UI_DisplayTextWidthHeight(10, (unsigned char*)displayCraftMenuName, 70, 90, *selectedColor);
		int curX, curY;
		RECT rect;
		GetMousePosition(&curX, &curY);
		if (*FGCreationNumOfCraftListed > 16)
		{
			SetRectDimensions(&rect, 551, 90, 570, 330);
			*FGCreationScrollPosition = UI_CreateScrollBar(&rect, *FGCreationScrollPosition, *FGCreationNumOfCraftListed - 16, 0, 5, *dword_B07C6C, 10);
		}
		SetRectDimensions(&rect, 280, 90, 550, 104);
		int scrollIteration = 0;
		*FGCreationCraftIteration = 0;
		int craftInTableOffset = 0;
		if (*totalCraftInCraftTable <= 0)
		{
			params[-1] = 0x551EFE;
			return 0;
		}

		char* fgCreationCraftListString = (char*)*speciesShipListPtr;
		int statOffset = 0;
		int rectHeight = params[-8];
		while (1)
		{
			int shipFound = 0;
			if (*FGCreationPlayerNum || *FGCreationPlayerGunnerNum)
			{
				if (*&fgCreationCraftListString[statOffset + 268] && *&fgCreationCraftListString[statOffset + 276])
				{
				Locate_Ship:
					if (*FGCreationSelectCraftMenu == FGCreationSelectCraftMenu_AllRebelFlyable)
					{
						if (*&fgCreationCraftListString[statOffset + 256] == 1 || *&fgCreationCraftListString[statOffset + 256] == 2 || *&fgCreationCraftListString[statOffset + 256] == 3 || *&fgCreationCraftListString[statOffset + 256] == 4 || *&fgCreationCraftListString[statOffset + 256] == 14)
						{
							shipFound = 1;
							goto Ship_Found;
						}
					}
					else if (*FGCreationSelectCraftMenu == FGCreationSelectCraftMenu_AllImperialFlyable)
					{
						if (*&fgCreationCraftListString[statOffset + 256] == 5 || *&fgCreationCraftListString[statOffset + 256] == 6 || *&fgCreationCraftListString[statOffset + 256] == 7 || *&fgCreationCraftListString[statOffset + 256] == 8 || *&fgCreationCraftListString[statOffset + 256] == 16)
						{
							shipFound = 1;
							goto Ship_Found;
						}
					}
				}
			}
			else if (*&fgCreationCraftListString[statOffset + 260] == *FGCreationGenusSelected && *&fgCreationCraftListString[statOffset + 276])
			{
				goto Locate_Ship;
			}

		Ship_Found:
			if (shipFound)
			{
				if (scrollIteration >= *FGCreationScrollPosition)
				{
					if (GetCraftSpeciesShortNamePtr(*&fgCreationCraftListString[statOffset + 256]))
					{
						int speciesShortName = GetCraftSpeciesShortNamePtr(*((char*)*speciesShipListPtr + statOffset + 256));
						sprintf_(*mainStringBuffer, aSS_4, *speciesShipListPtr + statOffset, speciesShortName);
					}
					else
					{
						sprintf_(*mainStringBuffer, aS_0, *speciesShipListPtr + statOffset);
					}

					int speciesTextClicked = UI_CreateClickableText(rect.left, rectHeight + 3, *mainStringBuffer, 10, *highlightedTextColor, craftInTableOffset + 80, 0, "settingsound");
					rectHeight += 15;

					if (speciesTextClicked)
					{
						*FGCreationSpecies = *((char*)*speciesShipListPtr + statOffset + 256);
						unsigned __int16 fgCreationSpecies = *FGCreationSpecies;
						if (*FGCreationSpecies)
						{
							int fgCreationSpeciesBuffer[49];
							memset(fgCreationSpeciesBuffer, 0, 0x30u);
							GetSpeciesExternalData(fgCreationSpeciesBuffer);
							fgCreationSpecies = *FGCreationSpecies;
						}
						*FGCreationSelectCraftMenu = 0;
						if (fgCreationSpecies)
							GetShipBMP(fgCreationSpecies);
						else
							DisplayShipBMP();
						CombatSimulatorBackgrounds(0);
					}
					SetRectWidthHeight(&rect, 0, 15);
					*FGCreationCraftIteration += 1;
					unsigned int incrementedCraftIteration = *FGCreationCraftIteration + 1;
					if (*FGCreationCraftIteration >= *FGCreationNumOfCraftListed || incrementedCraftIteration >= 16)
					{
						params[-1] = 0x551EFE;
						return 0;
					}
					fgCreationCraftListString = (char*)*speciesShipListPtr;
				}
				++scrollIteration;
			}
			statOffset += 296;
			if (++craftInTableOffset >= *totalCraftInCraftTable)
			{
				params[-1] = 0x551EFE;
				return 0;
			}
		}
	}
	else
	{
		// Jump to AllFlyableCraft code
		params[-1] = 0x550338;
	}


	return 0;
}

int CombatSimMusicHook(int* params)
{
	if (g_config.ChangeCombatSimMusic)
	{
		if (*campaignIncomplete)
		{
			if (*currentMissionInCampaign >= 7)
			{
				*currentMusicPlaying = ImuseMusicState_StRival;
				if (*configDatapadMusic)
				{
					Imuse_ChangeMusicState(ImuseMusicState_StRival);
					params[-1] = 0x54397D;
					return 0;
				}
			}
			else
			{
				*currentMusicPlaying = ImuseMusicState_StRival;
				if (*configDatapadMusic)
				{
					Imuse_ChangeMusicState(ImuseMusicState_StRival);
					params[-1] = 0x54397D;
					return 0;
				}
			}
		}
		else
		{
			*currentMusicPlaying = ImuseMusicState_StRival;
			if (*configDatapadMusic)
			{
				Imuse_ChangeMusicState(ImuseMusicState_StRival);
				params[-1] = 0x54397D;
				return 0;
			}
		}
		Imuse_StopMusic();
		params[-1] = 0x54397D;
	}

	return 0;
}

int CombatSimBackgroundGeneralHook(int* params)
{
	TryLoadFrontResEntryForceEncode((char*)g_config.SimulatorBackground.c_str(), "background");
	return 0;
}

int CombatSimBackgroundBriefingHook(int* params)
{
	QuickMissionData* QuickMissionDataPtr = *(QuickMissionData**)0x9EB8E0;
	if (*thisPlayerType == PlayerType_Singleplayer && PilotDataFrontEndPtr->campaignMode == 1)
	{
		if (QuickMissionDataPtr->header.MissionType == MissionType_Azzameen)
		{
			TryLoadFrontResEntryForceEncode("frontres\\family\\markoholo.bmp", "background");
		}
		else
			TryLoadFrontResEntryForceEncode("frontres\\combat\\solo.bmp", "background");
	}
	else
	{
		TryLoadFrontResEntryForceEncode((char*)g_config.SimulatorBackground.c_str(), "background");
	}

	return 0;
}

int CombatSimBackgroundUnknownHook(int* params)
{
	TryLoadFrontResEntryForceEncode((char*)g_config.SimulatorBackground.c_str(), "background");
	return 0;
}

int CombatSimBackgroundJoinRoomHook(int* params)
{
	TryLoadFrontResEntryForceEncode((char*)g_config.SimulatorBackground.c_str(), "background");
	return 0;
}

int CombatSimBackgroundConnectToLobbyHook(int* params)
{
	TryLoadFrontResEntryForceEncode((char*)g_config.SimulatorBackground.c_str(), "background");
	return 0;
}

int CombatSimBackgroundMultiHostStartedLobbyHook(int* params)
{
	TryLoadFrontResEntryForceEncode((char*)g_config.SimulatorBackground.c_str(), "background");
	return 0;
}

int CombatSimBackgroundDebriefingRoomHook(int* params)
{
	TryLoadFrontResEntryForceEncode((char*)g_config.SimulatorBackground.c_str(), "background");
	return 0;
}

int CombatSimBackgroundHostOrJoinHook(int* params)
{
	TryLoadFrontResEntryForceEncode((char*)g_config.SimulatorBackground.c_str(), "background");
	params[-1] = 0x53C23D;
	return 0;
}

int CombatSimSettingBarGeneral(int* params)
{
	if (g_config.SettingsBar)
	{
		DrawDatapadFrontResRLETransparent("settingbar", 57, 348);
	}

	return 0;
}

int CombatSimSettingBarGeneral2(int* params)
{
	if (g_config.SettingsBar)
	{
		DrawDatapadFrontResRLETransparent("settingbar", 57, 348);
	}

	return 0;
}

int CombatSimSettingBarHostOrJoinHook1(int* params)
{
	if (g_config.SettingsBar)
	{
		DrawDatapadFrontResRLETransparent("settingbar", 57, 348);
	}
	params[-1] = 0x53D859;

	return 0;
}

int CombatSimSettingBarHostOrJoinHook2(int* params)
{
	if (g_config.SettingsBar)
	{
		DrawDatapadFrontResRLETransparent("settingbar", 57, 348);
	}

	return 0;
}

int QuitMessageHook(int* params)
{
	// Displays a different quit message if Player is to be captured
	int playerIndex = params[Params_EDI];

	if (PlayerPtr[playerIndex].participationState == Participation_InPlay)
	{
		if (*missionType == MissionType_Skirmish
			&& *isMelee
			&& *dword_AE2A7E == 1
			&& TeamMissionOutcomeTable[3 * *(&PlayerPtr[0].team + playerIndex)] != MissionOutcome_Victory)
		{
			PlaySoundFromObject(0x3Du, 0xFFFF, playerIndex);
			DisplayMessage(InFlightStrings_MSG_END_MISSION_PENALTY, playerIndex);
		}
		else if (*numberOfPlayersInGame == 1
			&& TeamMissionOutcomeTable[3 * *(&PlayerPtr[0].team + playerIndex)] == MissionOutcome_Victory
			&& WillPlayerBeCaptured(playerIndex))
		{
			PlaySoundFromObject(0x3Du, 0xFFFF, playerIndex);
			DisplayMessage(InFlightStrings_MSG_END_MISSION_INCOMPLETE, playerIndex);
		}
		else
		{
			PlaySoundFromObject(0x43u, 0xFFFF, playerIndex);
			DisplayMessage(InFlightStrings_MSG_END_MISSION, playerIndex);
		}

		PlayerPtr[playerIndex].criticalMessageType = CriticalMsg_HangarOrQuitGame;
		PlayerPtr[playerIndex].criticalMessageTargetObjIndex = -1;
		// 8 seconds
		PlayerPtr[playerIndex].criticalMessageTimer = 1888;
	}
	else
	{
		DisplayMessage(InFlightStrings_MSG_YOU_MUST_WAIT, playerIndex);
	}

	params[-1] = 0x4FE9CB;

	return 0;
}

int IFFColorOrangeMessageHook(int* params)
{
	// Change IFF Red message to Orange

	HudMessage* HudReadyMessageText = (HudMessage*)0x7C9DC0;

	params[Params_EDX] = HudReadyMessageText->IFFCode;
	if (g_config.GreenAndRedForIFFColorsOnly)
	{
		params[Params_EAX] = FlightTextColor_MainPal_White;
	}
	else
	{
		if (HudReadyMessageText->IFFCode == IFF_Red)
			params[Params_EAX] = FlightTextColor_LightestRed;
		else
			params[Params_EAX] = MissionMessageColorTable[*missionMessageBufferColor];
	}
	
	params[Params_ReturnAddress] = 0x47A9A0;

	return 0;
}

int IFFColorOrangeMessageLogHook(int* params)
{
	// Change IFF Red message in message log to Orange

	__int16 messageIFF = params[Params_ECX];

	if (g_config.GreenAndRedForIFFColorsOnly)
	{
		params[Params_EAX] = FlightTextColor_MainPal_White;
	}
	else
	{
		if (messageIFF == IFF_Red)
			params[Params_EAX] = FlightTextColor_LightestRed;
		else
			params[Params_EAX] = MissionMessageColorTable[messageIFF];
	}

	params[Params_ReturnAddress] = 0x49A159;

	return 0;
}

int HangarMultiplayerHook(int* params)
{
	int playerIndex = params[Params_EDI];
	Object* ObjectTable = *(Object**)0x7B33C4;
	int playerObjectIndex = PlayerPtr[playerIndex].objectIndex;
	__int16 playerFGIndex = PlayerPtr[playerIndex].playerFG;
	__int16 DepartMothershipFGIndex = GetFlightGroupLeaderCraft(FlightGroupPtr[playerFGIndex].DepartureMothership);

	// If it's multiplayer, it will act like XvT when entering a hangar. 
	// Otherwise it will act like XWA's hangar.

	if (*numberOfPlayersInGame == 1)
	{
		ReEnterHangar(PlayerPtr[playerIndex].criticalMessageTargetObjIndex);
	}
	else if (*numberOfPlayersInGame > 1)
	{
		UpdateFGStatsOfRemovedObject(playerObjectIndex, playerFGIndex, ((DepartMothershipFGIndex - PlayerPtr[playerIndex].criticalMessageTargetObjIndex) < 1u) + 0x12);

		if (*missionType != MissionType_Melee && *missionType != MissionType_Skirmish && FlightGroupPtr[playerFGIndex].numberOfWaves != 99)
		{
			PlayerPtr[playerIndex].score += 40 * CraftDefinitionPtr[GetCraftDefinitionIndex(ObjectTable[playerObjectIndex].objectSpecies)].score;
		}

		if (PlayerPtr[playerIndex].objectIndex != 0xFFFF)
		{
			PlayOrStopBeamSound(0, playerIndex);
			PlayOrStopMissileLockWarningSound(0);
		}

		ObjectTable[playerObjectIndex].objectSpecies = 0;
		SavePlayerCraftSettings(playerIndex);
		ResetUnknownPlayerPtr(ObjectTable[playerObjectIndex].MobileObjectPtr->CraftPtr);
		CheckPlayerFGForNewWave(playerFGIndex);
		if (UnableToSpawnPlayer(playerIndex, -1, 0, 0))
		{
			RemovePlayerFromPlay(playerIndex);
			DisplayRemainingPlayersMessage(playerIndex);
		}
		else if (playerIndex == *thisPlayerIndex)
		{
			// This doesn't display for some reason.
			ActivateSubMessageWithCraftName(InFlightStrings_MSG_PREVIOUS_ENTERED_HANGAR);
		}
	}

	return 0;
}

int UpdateRateHostJoinRoomHook(int* params)
{
	const char* updateRate = 0;

	// Reset UpdateRate if needed
	if (*configUpdateRate != 8 && *configUpdateRate != 29 && *configUpdateRate != 59)
	{
		*configUpdateRate = 8;
	}

	if (*configUpdateRate == 8)
	{
		updateRate = "8";
	}
	else if (*configUpdateRate == 29)
	{
		updateRate = "29";
	}
	else
	{
		updateRate = "59";
	}

	if (UI_CreateClickableText(410 + 93, 303, updateRate, 12, *highlightedTextColor, 28, 0, "settingsound"))
	{
		if (*configUpdateRate == 8)
		{
			*configUpdateRate = 29;
		}
		else if (*configUpdateRate == 29)
		{
			*configUpdateRate = 59;
		}
		else if (*configUpdateRate == 59)
		{
			*configUpdateRate = 8;
		}
	}

	return 0;
}

int UpdateRateOptionsMenuHook(int* params)
{
	const char* updateRate = 0;

	// Reset UpdateRate if needed
	if (*configUpdateRate != 8 && *configUpdateRate != 29 && *configUpdateRate != 59)
	{
		*configUpdateRate = 8;
	}

	if (*configUpdateRate == 8)
	{
		updateRate = "Update Rate   8";
	}
	else if (*configUpdateRate == 29)
	{
		updateRate = "Update Rate   29";
	}
	else
	{
		updateRate = "Update Rate   59";
	}

	if (UI_CreateClickableText(240, 280, updateRate, 15, *highlightedTextColor, 29, 0, "settingsound"))
	{
		if (*configUpdateRate == 8)
		{
			*configUpdateRate = 29;
		}
		else if (*configUpdateRate == 29)
		{
			*configUpdateRate = 59;
		}
		else if (*configUpdateRate == 59)
		{
			*configUpdateRate = 8;
		}
	}

	return 0;
}

int UpdateRateOptionsMenuClientHook(int* params)
{
	const char* updateRate = 0;

	// Reset UpdateRate if needed
	if (*configUpdateRate != 8 && *configUpdateRate != 29 && *configUpdateRate != 59)
	{
		*configUpdateRate = 8;
	}

	if (*configUpdateRate == 8)
	{
		updateRate = "Update Rate   8";
	}
	else if (*configUpdateRate == 29)
	{
		updateRate = "Update Rate   29";
	}
	else
	{
		updateRate = "Update Rate   59";
	}

	UI_DisplayTextWidthHeight(15, (unsigned char*)updateRate, 240, 280, *grayedOutTextColor);

	return 0;
}

int KeyHook1(int* params)
{
	__int16 keyPressed = *keyPressedAfterLocaleAfterMapping;
	params[-4] = keyPressed;

	if (keyPressed == KeyCode_SHIFTM)
	{
		shiftMPressed = true;
		// Do what 'M' key does
		params[-1] = 0x50142B;
	}
	else
	{
		shiftMPressed = false;
	}

	return 0;
}

int MapAIHook(int* params)
{
	int playerIndex = params[0];

	if (shiftMPressed)
	{
		IsCraftJumpPossible(playerIndex, 0, 1);
	}
	else
	{
		IsCraftJumpPossible(playerIndex, 0, 0);
	}

	return 0;
}

int SpecRegionChangeHook(int* params)
{
	// Allows players to change the region on the map if they are spectating.
	// Use "m" or "shift+m" to cycle through regions.

	int playerIndex = params[Params_EDI];

	if (PlayerPtr[playerIndex].participationState == Participation_Spectating)
	{
		if (!shiftMPressed)
		{
			if (PlayerPtr[playerIndex].region + 1 == *latestNonInstantiatedRegion)
			{
				PlayerPtr[playerIndex].region = 0;
			}
			else
			{
				PlayerPtr[playerIndex].region += 1;
			}
		}
		else
		{
			if (PlayerPtr[playerIndex].region - 1 < 0)
			{
				PlayerPtr[playerIndex].region = *latestNonInstantiatedRegion - 1;
			}
			else
			{
				PlayerPtr[playerIndex].region -= 1;
			}
		}

		params[-1] = 0x501884;
	}

	return 0;
}

int TourModeButtonHook(int* params)
{
	const int missionIndex = *(int*)0x0ABC970;

	if (missionIndex != 0 && missionIndex < g_config.TourOfDutyButtonRequiredMissionIndex)
	{
		params[Params_ReturnAddress] = 0x00546F42;
	}

	return 0;
}

int Test1Hook(int* params)
{
	*(int*)0x009F4BC8 = 0x06;

	int& s_V0x09F5E74 = *(int*)0x09F5E74;

	//OutputDebugString(("s_V0x09F5E74 = " + std::to_string(s_V0x09F5E74)).c_str());

	return 0;
}
