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
		const auto lines = GetFileLines("hook_joystick_ff.cfg");

		this->JoystickDeviceIndex = GetFileKeyValueInt(lines, "JoystickDeviceIndex", 0);
	}

	int JoystickDeviceIndex;
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
