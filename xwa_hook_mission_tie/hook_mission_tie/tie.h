#pragma once

int TieHook(int* params);
int MissionFreeHook(int* params);
int CreateObjectHook(int* params);

int StatsProfiles_L0041FA00_Speed_Hook(int* params);
int StatsProfiles_L0042CF90_Acceleration_Hook(int* params);
int StatsProfiles_L0042CF90_Deceleration_Hook(int* params);
int StatsProfiles_L0042CF90_SpeedIncrement_Hook(int* params);
int StatsProfiles_L0042CF90_SpeedDecrement_Hook(int* params);
int StatsProfiles_L0040B4F0_HullStrength_Hook(int* params);
int StatsProfiles_SystemStrength_Hook(int* params);
int StatsProfiles_ShieldStrength_Hook1(int* params);
int StatsProfiles_ShieldStrength_Hook2(int* params);
int StatsProfiles_L004D5AE0_ShieldStrength_Hook(int* params);
int StatsProfiles_HasHyperdrive_Hook(int* params);

int MissionIdRedAlertFilterHook(int* params);
int MissionIdRedAlertHook(int* params);
int MissionIdSkipHyperspacedMessagesHook(int* params);
int MissionIdSkipEnteringAreaMessagesHook(int* params);
int MissionIdForcePlayerInTurret1Hook(int* params);
int MissionIdForcePlayerInTurret2Hook(int* params);
int MissionPlayerShootHook(int* params);
int MissionPlayerTypeShootHook(int* params);
int MissionDrawLaserCharge3DHook(int* params);
int WarheadCollisionDamagesHook(int* params);

int OrderOfBattleHook(int* params);
