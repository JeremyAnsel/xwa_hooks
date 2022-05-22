#pragma once

int KeyboardIsAnyKeyPressedHook(int* params);
int KeyboardGetPressedKeyHook(int* params);
int KeyboardTryUpdateShiftControlAltKeysPressedStateHook(int* params);
int RetrieveKeyboardStateHook(int* params);
int RetrieveMouseStateHook(int* params);
int RegisterClassHook(int* params);
int CreateWindowHook(int* params);
int CursorHook(int* params);
int CursorZeroHook(int* params);

int SplashScreenHook(int* params);

int MissionPausedHook(int* params);
