#pragma once

int TieHook(int* params);
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
