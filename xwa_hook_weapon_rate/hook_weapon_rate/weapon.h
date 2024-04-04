#pragma once

int WeaponDechargeHook(int* params);
int WeaponRechargeHook(int* params);
int WeaponCooldownTimeHook(int* params);
int ModelEnergyTransfer1Hook(int* params);
int ModelEnergyTransfer2Hook(int* params);
int ModelEnergyTransfer3Hook(int* params);
int ModelEnergyTransferShieldToLaser1Hook(int* params);
int ModelEnergyTransferShieldToLaser2Hook(int* params);
int ModelEnergyTransferLaserToShield1Hook(int* params);
int ModelEnergyTransferLaserToShield2Hook(int* params);
int ModelAIMaxTorpedoCountHook(int* params);
int ModelIsImpactSpinningEnabledHook(int* params);
int ModelImpactSpinningFactorHook(int* params);

int WeaponDurationOffsetHook(int* params);
int WeaponFireRatioHook(int* params);
int WeaponRange_004A8D42_Hook(int* params);
int WeaponRange_004A8FB6_Hook(int* params);
int WeaponRange_004A91B2_Hook(int* params);
int WeaponRange_004A9B15_Hook(int* params);
int WeaponRange_004E2119_Hook(int* params);
int WeaponSpreadHook(int* params);

int WeaponFireRateHook(int* params);
int GetWeaponDurationHook(int* params);
int WeaponDuration_0040D54C_Hook(int* params);
int WeaponDuration_0040D570_Hook(int* params);
int WeaponDuration_004922DB_Hook(int* params);
int WeaponDuration_004922E3_Hook(int* params);
int WeaponDuration_0049312E_Hook(int* params);
int WeaponDuration_00493136_Hook(int* params);
int WeaponDuration_00493616_Hook(int* params);
int WeaponDuration_00493628_Hook(int* params);
int WeaponScore_00497454_Hook(int* params);
int WeaponScore_004D9B16_Hook(int* params);
int WeaponScore_004D9B4E_Hook(int* params);
int WeaponSpeed_0040D553_Hook(int* params);
int WeaponSpeed_0049221A_Hook(int* params);
int WeaponSpeed_00492235_Hook(int* params);
int WeaponSpeed_00492EE5_Hook(int* params);
int WeaponSpeed_004930F3_Hook(int* params);
int WeaponSpeed_0049359A_Hook(int* params);
int WeaponSpeed_004935C3_Hook(int* params);
int WeaponSpeed_0049424E_Hook(int* params);
int WeaponSpeed_004942CF_Hook(int* params);
int WeaponSpeed_00496903_Hook(int* params);
int WeaponSpeed_004B5F76_Hook(int* params);
int WeaponSpeed_004E22F7_Hook(int* params);
int WeaponSpeed_004E2678_Hook(int* params);
int WeaponSpeed_004E26A2_Hook(int* params);
int WeaponSpeed_004E4D75_Hook(int* params);
int WeaponPower_0040F511_Hook(int* params);
int WeaponPower_00492279_Hook(int* params);
int WeaponPower_004922A2_Hook(int* params);
int WeaponPower_004922BA_Hook(int* params);
int WeaponPower_00493121_Hook(int* params);
int WeaponPower_004935E1_Hook(int* params);
int WeaponPower_004935FE_Hook(int* params);
int WeaponPower_00494265_Hook(int* params);
int WeaponPower_004A7983_Hook(int* params);
int WeaponPower_004A7D68_Hook(int* params);
int WeaponPower_004A7E46_Hook(int* params);
int WeaponPower_004E2691_Hook(int* params);
int WeaponPower_004E4DAF_Hook(int* params);
int WeaponPower_00519C36_Hook(int* params);

int WeaponRackHook(int* params);
int WeaponHardpointHook(int* params);
int GetNearestWeaponMeshHook(int* params);
int MirrorHardpointHook(int* params);

int PlayerFireHook(int* params);

int WarheadCapacity_0041BE55_Hook(int* params);
int WarheadCapacity_0045CBFD_Hook(int* params);
int WarheadCapacity_00460904_Hook(int* params);
int WarheadCapacity_004B1426_Hook(int* params);

int EnergySpeedFactorHook(int* params);

int LasersLinkAIFighterHook(int* params);
int LasersLinkPlayerFighterHook(int* params);
int LasersLinkPlayerKeyXHook(int* params);
int PreventAILasersIonWhenDisablingHook(int* params);
int PreventAIIonWhenNotDisablingHook(int* params);
int PreventPlayerLinkedLasersIonHook(int* params);

int WarheadLockDistanceHook(int* params);

int ProjectileTurretThreatHook(int* params);

void EnergyLowHighSeparationFunction1(int* params);
bool EnergyLowHighSeparationFunction2(int* params);

int DrawEnergyBarHook(int* params);
int SetupLaserChargePositionsHook(int* params);

int SetWeaponsCount1Hook(int* params);
int SetWeaponsCount2Hook(int* params);

int HangarWarheadReloadHook(int* params);
int HangarWarheadCountHook(int* params);
