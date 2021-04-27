#pragma once

int GetSoundEffectNamesArrayCount();
int GetSoundEffectNamesArraySize();
int GetSoundEffectNamesArrayPtr();
int GetSfxVolumesArrayCount();
int GetSfxVolumesArraySize();
int GetSfxVolumesArrayPtr();
int GetSfxDistancesArrayCount();
int GetSfxDistancesArraySize();
int GetSfxDistancesArrayPtr();
int GetSoundEffectIdsArrayCount();
int GetSoundEffectIdsArraySize();
int GetSoundEffectIdsArrayPtr();
int GetSoundEffectBufferArrayCount();
int GetSoundEffectBufferArraySize();
int GetSoundEffectBufferArrayPtr();

int ResetSoundEffectNamesHook(int* params);
int ResetSoundEffectIdsHook(int* params);
int LoadEffectSoundsHook(int* params);
