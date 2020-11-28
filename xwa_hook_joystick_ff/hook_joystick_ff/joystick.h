#pragma once

int JoystickFFEnumHook(int* params);

int JoystickSmallMovement1Hook(int* params);
int JoystickSmallMovement2Hook(int* params);

int MainControllerIndexHook(int* params);
int InitControllerHook(int* params);
int UpdateControllerHook(int* params);

int SetControllerPressedButtonHook(int* params);
//int SetControllerPressedButtonLogHook(int* params);

int ControllerRemapTextHook(int* params);
int ControllerRemapRunHook(int* params);
int ControllerRemapReloadHook(int* params);

int NumPadLookMoveHook(int* params);
