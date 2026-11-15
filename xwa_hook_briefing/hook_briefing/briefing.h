#pragma once

int CustomBriefingHook(int* params);
int CustomToHangarTextHook(int* params);
int SwitchToCustomBriefingHook(int* params);
int SwitchToCustomDebriefingHook(int* params);
int CheckFGGoalStateHook(int* params);

int MissionSetupLoadMissionDescTextHook(int* params);
int MissionSetupBuildBriefingTextHook(int* params);
int MissionDebriefBuildTextHook(int* params);
