#pragma once

#include <windows.h>
#include "SharedMemTemplate.h"

struct SharedMemDataJoystick {
	// Joystick's position, written by the joystick hook, or by the joystick emulation code.
	// These values are normalized in the range -1..1
	float JoystickYaw, JoystickPitch;

	SharedMemDataJoystick() {
		JoystickYaw = 0.0f;
		JoystickPitch = 0.0f;
	}
};

void InitSharedMem();

const auto SHARED_MEM_NAME = L"Local\\JoystickFfHook";

extern SharedMemDataJoystick* g_pSharedData;
extern SharedMem<SharedMemDataJoystick> g_SharedMem;
