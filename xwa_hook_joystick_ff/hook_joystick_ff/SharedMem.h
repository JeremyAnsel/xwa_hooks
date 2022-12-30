#pragma once

#include <windows.h>
#include "SharedMemTemplate.h"

struct SharedMemDataJoystick {
	// Joystick's position, written by the joystick hook, or by the joystick emulation code.
	// These values are normalized in the range -1..1
	float JoystickYaw, JoystickPitch, JoystickRoll;
	// If the gimbal lock fix is active, the following will disable the joystick (default: false)
	// Set by ddraw
	bool GimbalLockFixActive;
	// Set by ddraw when joystick emulation is enabled
	bool JoystickEmulationEnabled;
	// Set by the joystick hook
	bool JoystickHookPresent;

	SharedMemDataJoystick() {
		JoystickYaw = 0.0f;
		JoystickPitch = 0.0f;
		JoystickRoll = 0.0f;
		GimbalLockFixActive = false;
		JoystickEmulationEnabled = false;
		JoystickHookPresent = true;
	}
};

void InitSharedMem();

const auto SHARED_MEM_NAME = L"Local\\JoystickFfHook";

extern SharedMemDataJoystick* g_pSharedData;
extern SharedMem<SharedMemDataJoystick> g_SharedMem;
