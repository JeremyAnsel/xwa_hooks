#pragma once

int MedalDetailExistHook(int* params);
int MedalDetailFilenameHook(int* params);
int MedalDetailDrawHook(int* params);
int MedalDetailInAreaHook(int* params);
int MedalDetailSetAreaHook(int* params);
//int MedalDetailInteractiveHook(int* params);
int MedalDetailLoadImagesHook(int* params);
int FamilyMedalDescriptionHook(int* params);
int CustomMoviesOnPoints1Hook(int* params);
int CustomMoviesOnPointsPlayHook(int* params);
int CustomPointsCheckHook(int* params);
int LoadGameStateHook(int* params);
int FreeGameStateHook(int* params);
int DrawConcoursePlanetHook(int* params);
int DrawConcourseDoorsHook(int* params);
int TrainDoorRectHook(int* params);
int CombatDoorRectHook(int* params);
int TourDoorRectHook(int* params);
int MkanimRectHook(int* params);
int FilmDoorRectHook(int* params);
int TechDoorRectHook(int* params);
int MedalPositionHook(int* params);
int MedalDescriptionHook(int* params);
int KalidorCrescentPosition1Hook(int* params);
int KalidorCrescentPosition2Hook(int* params);
int GetRankPointsArrayPtr();
int GetPilotRatingPointsArrayPtr();
int MedalAndKalidorCrescentDescriptionHook(int* params);
int BattlesMissionsImagesCountHook(int* params);
int PlayPerMissionMovieHook(int* params);
int LoadPerMissionMovieBeforeBackgroundHook(int* params);
int LoadPerMissionMovieAfterBackgroundHook(int* params);
int WritePilotHook(int* params);
int ReadPilotHook(int* params);
int CreatePilotHook(int* params);
int ReadEmailTxtHook(int* params);
int SetupConcourseStarfieldHook(int* params);
int DrawConcourseStarfieldHook(int* params);
void DrawConcourseStarfieldHD(int frameIndex);
int SwitchFrontPlanetImageHook(int* params);
int LoadFrontPlanetImageHook(int* params);
int DrawMultiDoorHook(int* params);
int DrawSingleDoorHook(int* params);
int DrawCombatBackDoorHook(int* params);
int MultiDoorRectHook(int* params);
int SingleDoorRectHook(int* params);
int CombatBackDoorRectHook(int* params);
int PlayFilmPodMovieHook(int* params);
int PlayMultiPodMovieHook(int* params);
int PlaySinglePodMovieHook(int* params);
int LoadFilmRoomBackgroundHook(int* params);
int PlaySoundHook(int* params);

int LoadDatImageHook(int* params);
int LoadCbmImageHook(int* params);
int FrontResFreeItemHook(int* params);
int FrontResDrawHook(int* params);
int FrontResDrawCurrentImageHook(int* params);
int FrontResDrawCurrentImageAddHook(int* params);
int FrontResDrawMapIconHook(int* params);
int FrontResDrawGreyMapIconHook(int* params);
int FrontResDrawIconHook(int* params);
int FrontResIconRectHook(int* params);
int DrawCursorMessageHook(int* params);
int DrawFillRectangleHook(int* params);
int DrawRectangleHook(int* params);
int DrawRectangleBorderHook(int* params);
int DrawBriefingGridVerticalHook(int* params);
int DrawBriefingGridHorizontalHook(int* params);
int DrawLineHook(int* params);
int DrawBriefingBackgroundHook(int* params);
int DrawBriefingBottomTextHook(int* params);
int DrawConfigMenuBackHook(int* params);
int DrawMessageBoxBackgroundHook(int* params);
int DrawTechLibraryBackgroundHook(int* params);
int DrawFilmRoomBackgroundHook(int* params);
int DrawCreatePilotBackgroundHook(int* params);
int DrawCombatSingleGameStateBackgroundHook(int* params);
int DrawSkirmishShipbmpBackgroundHook(int* params);
void FreeSkirmishImage();

int FamilyRoomBacktoconcoursePositionHook(int* params);
int FamilyRoomStatisticsPositionHook(int* params);
int FamilyRoomPlaymissionPositionHook(int* params);
int FamilyRoomMkeyePositionHook(int* params);
int FamilyRoomMonitorPositionHook(int* params);
int FamilyRoomLadyblueMission1Hook(int* params);
int FamilyRoomLadyblueMission2Hook(int* params);
int FamilyRoomLadyblueMission3Hook(int* params);
int FamilyRoomCologneMission1Hook(int* params);
int FamilyRoomCologneMission2Hook(int* params);
int FamilyRoomCologneMission3Hook(int* params);
int FrontResGetAreaHook(int* params);
int FrontResSetCurrentImageHook(int* params);

int LoadFontHook(int* params);
int FreeFontsHook(int* params);
int FontGetLineHeightHook(int* params);
int FontGetStringWidthHook(int* params);
int L00557B10Hook(int* params);
int L00557910Hook(int* params);
int L00557720Hook(int* params);
int L005575A0Hook(int* params);
int L00557450Hook(int* params);
int L00557310Hook(int* params);
int L005571A0Hook(int* params);
int ConfigMenuSetVersionStringHook(int* params);

int ReviewBriefingDrawProgressBarHook(int* params);
int ReviewBriefingDrawMissionItemHook(int* params);
int ReviewBriefingDrawMissionTitleHook(int* params);
int ReviewBriefingDrawSkirmishTitleHook(int* params);
int ReviewBriefingDrawSkirmishItem1Hook(int* params);
int ReviewBriefingDrawSkirmishItem2Hook(int* params);
int ReviewBriefingDrawSkirmishItem3Hook(int* params);

int MissionSelectionScreenPositionHook(int* params);

int DrawBattleZoomImage1Hook(int* params);
int DrawKalidorZoomImage1Hook(int* params);
int DrawBattleKalidorZoomText1Hook(int* params);
int DrawBattleZoomImage2Hook(int* params);
int DrawBattleZoomText2Hook(int* params);
int DrawKalidorZoomImage2Hook(int* params);
int DrawKalidorZoomText2Hook(int* params);
int DrawMedalZoomRect1Hook(int* params);
int DrawMedalZoomRect2Hook(int* params);

int DSBriefLoadMovieHook(int* params);
int DSBriefFreeMovieHook(int* params);
int DSBriefMoveToNextImageHook(int* params);
int DSBriefDrawImageHook(int* params);

int SquadlogoLoadMovieHook(int* params);
int SquadlogoFreeMovieHook(int* params);
int SquadlogoMoveToNextImageHook(int* params);
int SquadlogoDrawImageHook(int* params);

bool IsTechDoorMovie();
void TechDoorLoadMovie();
void TechDoorFreeMovie();
void TechDoorMoveToNextImage();
void TechDoorDrawImage(int x, int y, int w, int h);

int DrawMultiHook(int* params);
int DrawMulti2Hook(int* params);

int ViewCustscenesConfigMenuSetPositionHook(int* params);

int DefineMedalCaseHook(int* params);

int RenderBug01Hook(int* params);
