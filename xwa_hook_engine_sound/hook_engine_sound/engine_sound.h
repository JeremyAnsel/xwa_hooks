#pragma once

int ReplaceMissionSoundsHook(int* params);

int InteriorSoundHook(int* params);
int StopInteriorSoundHook(int* params);
int FlyBySoundHook(int* params);
int LaunchSoundHook(int* params);
int WashSoundHook(int* params);
int WashSoundStopHook(int* params);
int TakeOffSoundHook(int* params);
int EngineSlowingSoundHook(int* params);
int CanopyOpeningSoundHook(int* params);
int HangarShuttleSoundsHook(int* params);

int WeaponSoundHook(int* params);

int HyperStartSoundHook(int* params);
int HyperZoomSoundHook(int* params);
int HyperEndSoundHook(int* params);
