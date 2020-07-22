#include "targetver.h"
#include "joystick.h"
#include "config.h"

#define INITGUID
#include <dinput.h>

class Config
{
public:
	Config()
	{
		auto lines = GetFileLines("hooks.ini", "hook_joystick_ff");

		if (lines.empty())
		{
			lines = GetFileLines("hook_joystick_ff.cfg");
		}

		this->JoystickDeviceIndex = GetFileKeyValueInt(lines, "JoystickDeviceIndex", 0);
		this->EnableSmallMovement = GetFileKeyValueInt(lines, "EnableSmallMovement", 1) != 0;
	}

	int JoystickDeviceIndex;
	bool EnableSmallMovement;
};

Config g_config;

int JoystickEnumHook(int* params)
{
	static int index = 0;

	LPCDIDEVICEINSTANCEA lpddi = (LPCDIDEVICEINSTANCEA)params[1];

	LPDIRECTINPUTA &s_XwaForceFeedbackIDirectInput = *(LPDIRECTINPUTA*)0x0063CF78;
	LPDIRECTINPUTDEVICE2A &s_XwaForceFeedbackIDirectInput2 = *(LPDIRECTINPUTDEVICE2A*)0x0063CF7C;

	if (s_XwaForceFeedbackIDirectInput2 == nullptr)
	{
		index = 0;
	}
	else
	{
		index++;
	}

	bool next = g_config.JoystickDeviceIndex == -1 || index != g_config.JoystickDeviceIndex;

	if (s_XwaForceFeedbackIDirectInput2 != nullptr)
	{
		s_XwaForceFeedbackIDirectInput2->Release();
		s_XwaForceFeedbackIDirectInput2 = nullptr;
	}

	LPDIRECTINPUTDEVICEA lpDID;

	if (FAILED(s_XwaForceFeedbackIDirectInput->CreateDevice(lpddi->guidInstance, &lpDID, nullptr)))
	{
		s_XwaForceFeedbackIDirectInput2 = nullptr;
		return 0;
	}

	HRESULT hr = lpDID->QueryInterface(IID_IDirectInputDevice2A, (LPVOID*)&s_XwaForceFeedbackIDirectInput2);

	lpDID->Release();

	if (FAILED(hr))
	{
		s_XwaForceFeedbackIDirectInput2 = nullptr;
		return 0;
	}

	return next ? 1 : 0;
}

int JoystickSmallMovementHook(int* params)
{
	int s_V0x05FFDF0 = *(int*)0x05FFDF0;
	int s_V0x05FFDF4 = *(int*)0x05FFDF4;
	int s_V0x05FFDF8 = *(int*)0x05FFDF8;
	int s_V0x05FFDFC = *(int*)0x05FFDFC;

	int& s_V0x07733B8 = *(int*)0x07733B8;
	int& s_V0x07733BC = *(int*)0x07733BC;
	int& s_V0x07733C0 = *(int*)0x07733C0;
	int& s_V0x07733C4 = *(int*)0x07733C4;

	if (g_config.EnableSmallMovement)
	{
		s_V0x07733B8 = 0;
		s_V0x07733BC = 0;
		s_V0x07733C0 = 0;
		s_V0x07733C4 = 0;
	}
	else
	{
		s_V0x07733B8 = s_V0x05FFDF0 / 0x14;
		s_V0x07733BC = s_V0x05FFDF4 / 0x14;
		s_V0x07733C0 = s_V0x05FFDF8 / 0x14;
		s_V0x07733C4 = s_V0x05FFDFC / 0x0A;
	}

	return 0;
}
