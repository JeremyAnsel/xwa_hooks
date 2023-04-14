#pragma once

int WeaponDechargeHook(int* params);
int WeaponRechargeHook(int* params);
int WeaponCooldownTimeHook(int* params);

int WeaponRackHook(int* params);
int WeaponHardpointHook(int* params);
int GetNearestWeaponMeshHook(int* params);
int MirrorHardpointHook(int* params);

int PlayerFireHook(int* params);
