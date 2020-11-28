#include "targetver.h"
#include "joystick.h"
#include "config.h"
#include <array>
#include <map>
#include <utility>

#define INITGUID
#include <dinput.h>

#pragma comment(lib, "Winmm.lib")

enum KeyEnum : unsigned short
{
	Key_None = 0,
	Key_NUMPAD0 = 178,
	Key_NUMPAD1 = 179,
	Key_NUMPAD2 = 180,
	Key_NUMPAD3 = 181,
	Key_NUMPAD4 = 182,
	Key_NUMPAD5 = 183,
	Key_NUMPAD6 = 184,
	Key_NUMPAD7 = 185,
	Key_NUMPAD8 = 186,
	Key_NUMPAD9 = 187,
};

class Config
{
public:
	Config()
	{
		this->Load();
	}

	void Load()
	{
		auto lines = GetFileLines("JoystickConfig.txt");

		this->JoystickFFDeviceIndex = GetFileKeyValueInt(lines, "JoystickFFDeviceIndex", 0);
		this->EnableSmallMovement = GetFileKeyValueInt(lines, "EnableSmallMovement", 1) != 0;
		this->MainControllerIndex = GetFileKeyValueInt(lines, "MainControllerIndex", 0);
		this->ThrottleControllerIndex = GetFileKeyValueInt(lines, "ThrottleControllerIndex", 0);
		this->ThrottleControllerAxisIndex = GetFileKeyValueInt(lines, "ThrottleControllerAxisIndex", 2);
		this->RudderControllerIndex = GetFileKeyValueInt(lines, "RudderControllerIndex", 0);
		this->RudderControllerAxisIndex = GetFileKeyValueInt(lines, "RudderControllerAxisIndex", 3);
		this->UsePovControllerAsButtons = GetFileKeyValueInt(lines, "UsePovControllerAsButtons", 0) != 0;
	}

	int JoystickFFDeviceIndex;
	bool EnableSmallMovement;
	int MainControllerIndex;
	int ThrottleControllerIndex;
	int ThrottleControllerAxisIndex;
	int RudderControllerIndex;
	int RudderControllerAxisIndex;
	bool UsePovControllerAsButtons;
};

Config g_config;

class ButtonsConfig
{
public:
	ButtonsConfig()
	{
		this->Load();
	}

	void Load()
	{
		this->_buttons.clear();
		this->_buttonsIsPressed.clear();

		auto lines = GetFileLines("JoystickConfig.txt");

		int deviceCount = joyGetNumDevs();
		int controllerIndex = -1;

		int buttonIndex = 0;
		int povIndex = 0;

		for (int deviceIndex = 0; deviceIndex < deviceCount; deviceIndex++)
		{
			JOYCAPS caps{};

			if (joyGetDevCapsA(deviceIndex, &caps, sizeof(JOYCAPS)) != JOYERR_NOERROR)
			{
				continue;
			}

			controllerIndex++;

			std::array<short, 32 + 4> buttons;

			for (int index = 0; index < 32; index++)
			{
				std::string key = std::string("joybutton_") + std::to_string(controllerIndex) + std::string("_") + std::to_string(index + 1);
				int defaultValue = GetDefaultConfigButton(buttonIndex + 1);
				buttonIndex++;

				buttons[index] = (short)GetFileKeyValueInt(lines, key, defaultValue);
			}

			for (int index = 0; index < 4; index++)
			{
				std::string key = std::string("joybutton_") + std::to_string(controllerIndex) + std::string("_pov") + std::to_string(index + 1);
				int defaultValue = GetDefaultConfigPov(povIndex + 1);
				povIndex++;

				buttons[32 + index] = (short)GetFileKeyValueInt(lines, key, defaultValue);
			}

			this->_buttons.insert(std::make_pair(controllerIndex, buttons));

			std::array<bool, 32 + 4> buttonsIsPressed;
			this->_buttonsIsPressed.insert(std::make_pair(controllerIndex, buttonsIsPressed));
		}
	}

	static int GetDefaultConfigButton(int index)
	{
		int value;

		switch (index)
		{
		case 1:
			value = 156;
			break;

		case 2:
			value = 157;
			break;

		case 3:
			value = 114;
			break;

		case 4:
			value = 108;
			break;

		case 5:
			value = 101;
			break;

		case 6:
			value = 105;
			break;

		case 7:
			value = 91;
			break;

		case 8:
			value = 8;
			break;

		case 9:
			value = 13;
			break;

		case 10:
			value = 93;
			break;

		default:
			value = 0;
			break;
		}

		return value;
	}

	static int GetDefaultConfigPov(int index)
	{
		int value;

		switch (index)
		{
		case 1:
			value = 186;
			break;

		case 2:
			value = 184;
			break;

		case 3:
			value = 180;
			break;

		case 4:
			value = 182;
			break;

		default:
			value = 0;
			break;
		}

		return value;
	}

	short GetKey(int controllerIndex, int buttonIndex)
	{
		auto it = this->_buttons.find(controllerIndex);

		if (it != this->_buttons.end())
		{
			return it->second[buttonIndex];
		}

		return 0;
	}

	bool IsPressed(int controllerIndex, int buttonIndex)
	{
		auto it = this->_buttonsIsPressed.find(controllerIndex);

		if (it != this->_buttonsIsPressed.end())
		{
			return it->second[buttonIndex];
		}

		return false;
	}

	void SetIsPressed(int controllerIndex, int buttonIndex, bool isPressed)
	{
		auto it = this->_buttonsIsPressed.find(controllerIndex);

		if (it != this->_buttonsIsPressed.end())
		{
			it->second[buttonIndex] = isPressed;
		}
	}

	void ClearIsPressed()
	{
		for (auto& buttons : this->_buttonsIsPressed)
		{
			buttons.second.fill(false);
		}
	}

private:
	std::map<int, std::array<short, 32 + 4>> _buttons;
	std::map<int, std::array<bool, 32 + 4>> _buttonsIsPressed;
};

ButtonsConfig& GetGlobalButtonsConfig()
{
	static ButtonsConfig g_buttonsConfig;
	return g_buttonsConfig;
}

int JoystickFFEnumHook(int* params)
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

	bool next = g_config.JoystickFFDeviceIndex == -1 || index != g_config.JoystickFFDeviceIndex;

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

int JoystickSmallMovement1Hook(int* params)
{
	if (g_config.EnableSmallMovement)
	{
		return 0;
	}

	short& V0x008C1CC2 = *(short*)0x008C1CC2;
	short& V0x008C1CC0 = *(short*)0x008C1CC0;
	short& V0x008C1CC4 = *(short*)0x008C1CC4;

	if (std::abs(V0x008C1CC2) <= 0x40)
	{
		V0x008C1CC2 = 0;
	}

	if (std::abs(V0x008C1CC0) <= 0x18)
	{
		V0x008C1CC0 = 0;
	}

	if (std::abs(V0x008C1CC4) <= 0x40)
	{
		V0x008C1CC4 = 0;
	}

	return 0;
}

int JoystickSmallMovement2Hook(int* params)
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

int MainControllerIndexHook(int* params)
{
	return g_config.MainControllerIndex + 1;
}

int InitControllerHook(int* params)
{
	const int A4 = params[0];
	JOYCAPS& esp44 = *(JOYCAPS*)&params[18];

	const auto XwaGetConnectedJoysticksCount = (int(*)())0x00541030;
	const auto XwaJoysticksGetDevId = (int(*)(int))0x00541240;

	if (XwaGetConnectedJoysticksCount() == 0)
	{
		return 0;
	}

	int throttleControllerIndex = g_config.ThrottleControllerIndex;
	int throttleControllerAxisIndex = g_config.ThrottleControllerAxisIndex;
	int rudderControllerIndex = g_config.RudderControllerIndex;
	int rudderControllerAxisIndex = g_config.RudderControllerAxisIndex;

	int deviceCount = joyGetNumDevs();
	int controllerIndex = -1;

	for (int deviceIndex = 0; deviceIndex < deviceCount; deviceIndex++)
	{
		JOYCAPS caps{};

		if (joyGetDevCapsA(deviceIndex, &caps, sizeof(JOYCAPS)) != JOYERR_NOERROR)
		{
			continue;
		}

		controllerIndex++;

		if (controllerIndex == throttleControllerIndex)
		{
			esp44.wCaps |= JOYCAPS_HASZ;

			switch (throttleControllerAxisIndex)
			{
			case 0:
				esp44.wZmin = caps.wXmin;
				esp44.wZmax = caps.wXmax;
				break;

			case 1:
				esp44.wZmin = caps.wYmin;
				esp44.wZmax = caps.wYmax;
				break;

			case 2:
				esp44.wZmin = caps.wZmin;
				esp44.wZmax = caps.wZmax;
				break;

			case 3:
				esp44.wZmin = caps.wRmin;
				esp44.wZmax = caps.wRmax;
				break;

			case 4:
				esp44.wZmin = caps.wUmin;
				esp44.wZmax = caps.wUmax;
				break;

			case 5:
				esp44.wZmin = caps.wVmin;
				esp44.wZmax = caps.wVmax;
				break;
			}
		}

		if (controllerIndex == rudderControllerIndex)
		{
			esp44.wCaps |= JOYCAPS_HASR;

			switch (rudderControllerAxisIndex)
			{
			case 0:
				esp44.wRmin = caps.wXmin;
				esp44.wRmax = caps.wXmax;
				break;

			case 1:
				esp44.wRmin = caps.wYmin;
				esp44.wRmax = caps.wYmax;
				break;

			case 2:
				esp44.wRmin = caps.wZmin;
				esp44.wRmax = caps.wZmax;
				break;

			case 3:
				esp44.wRmin = caps.wRmin;
				esp44.wRmax = caps.wRmax;
				break;

			case 4:
				esp44.wRmin = caps.wUmin;
				esp44.wRmax = caps.wUmax;
				break;

			case 5:
				esp44.wRmin = caps.wVmin;
				esp44.wRmax = caps.wVmax;
				break;
			}
		}
	}

	return XwaJoysticksGetDevId(A4);
}

int UpdateControllerHook(int* params)
{
	JOYINFOEX& esp10 = *(JOYINFOEX*)&params[4];

	const auto XwaGetConnectedJoysticksCount = (int(*)())0x00541030;

	if (XwaGetConnectedJoysticksCount() == 0)
	{
		// not connected
		params[-1] = 0x0050BBD7;
		return 0;
	}

	int throttleControllerIndex = g_config.ThrottleControllerIndex;
	int throttleControllerAxisIndex = g_config.ThrottleControllerAxisIndex;
	int rudderControllerIndex = g_config.RudderControllerIndex;
	int rudderControllerAxisIndex = g_config.RudderControllerAxisIndex;

	int deviceCount = joyGetNumDevs();
	int controllerIndex = -1;

	for (int deviceIndex = 0; deviceIndex < deviceCount; deviceIndex++)
	{
		JOYINFOEX info{};
		info.dwSize = sizeof(JOYINFOEX);
		info.dwFlags = JOY_RETURNX | JOY_RETURNY | JOY_RETURNZ | JOY_RETURNR | JOY_RETURNU | JOY_RETURNV | JOY_RETURNPOV | JOY_RETURNBUTTONS | JOY_RETURNCENTERED;

		if (joyGetPosEx(deviceIndex, &info) != JOYERR_NOERROR)
		{
			continue;
		}

		controllerIndex++;

		if (controllerIndex == throttleControllerIndex)
		{
			switch (throttleControllerAxisIndex)
			{
			case 0:
				esp10.dwZpos = info.dwXpos;
				break;

			case 1:
				esp10.dwZpos = info.dwYpos;
				break;

			case 2:
				esp10.dwZpos = info.dwZpos;
				break;

			case 3:
				esp10.dwZpos = info.dwRpos;
				break;

			case 4:
				esp10.dwZpos = info.dwUpos;
				break;

			case 5:
				esp10.dwZpos = info.dwVpos;
				break;
			}
		}

		if (controllerIndex == rudderControllerIndex)
		{
			switch (rudderControllerAxisIndex)
			{
			case 0:
				esp10.dwRpos = info.dwXpos;
				break;

			case 1:
				esp10.dwRpos = info.dwYpos;
				break;

			case 2:
				esp10.dwRpos = info.dwZpos;
				break;

			case 3:
				esp10.dwRpos = info.dwRpos;
				break;

			case 4:
				esp10.dwRpos = info.dwUpos;
				break;

			case 5:
				esp10.dwRpos = info.dwVpos;
				break;
			}
		}
	}

	return 0;
}

int SetControllerPressedButtonHook(int* params)
{
	int& pressedButton = params[0];
	int& esp10 = params[5];
	int& esp14 = params[6];
	int& esp3C = params[16];

	bool isJoystickConnected = *(short*)0x091AB84 != 0;
	int& s_V0x063CF70 = *(int*)0x063CF70;

	esp3C = 0;

	if (pressedButton != 0)
	{
		esp14 = isJoystickConnected ? 0 : 1;
		GetGlobalButtonsConfig().ClearIsPressed();
		return 0;
	}

	if (!isJoystickConnected)
	{
		esp14 = 1;
		pressedButton = 186;
		return 0;
	}

	int deviceCount = joyGetNumDevs();
	int controllerIndex = -1;
	bool buttonSet = false;

	for (int deviceIndex = 0; deviceIndex < deviceCount; deviceIndex++)
	{
		JOYINFOEX info{};
		info.dwSize = sizeof(JOYINFOEX);
		info.dwFlags = JOY_RETURNPOV | JOY_RETURNBUTTONS | JOY_RETURNCENTERED;

		if (joyGetPosEx(deviceIndex, &info) != JOYERR_NOERROR)
		{
			continue;
		}

		controllerIndex++;

		for (int i = 0; i < 32; i++)
		{
			short button = GetGlobalButtonsConfig().GetKey(controllerIndex, i);

			if (button == 0)
			{
				continue;
			}

			if ((info.dwButtons & (1U << i)) != 0)
			{
				if (!GetGlobalButtonsConfig().IsPressed(controllerIndex, i))
				{
					if (!buttonSet)
					{
						buttonSet = true;
						pressedButton = button;
					}
				}

				switch (button)
				{
				case 156:
					// Key_ALT_2
					esp10 = 1;
					break;

				case 157:
					// Key_ALT_3
					esp14 = 1;
					break;
				}
			}

			GetGlobalButtonsConfig().SetIsPressed(controllerIndex, i, (info.dwButtons & (1U << i)) != 0);
		}

		if (g_config.UsePovControllerAsButtons)
		{
			int povIndex;

			if (info.dwPOV != JOY_POVCENTERED)
			{
				povIndex = info.dwPOV / 9000;
			}
			else
			{
				povIndex = -1;
			}

			if (povIndex != -1)
			{
				short button = GetGlobalButtonsConfig().GetKey(controllerIndex, 32 + povIndex);

				if (button != 0)
				{
					if (!GetGlobalButtonsConfig().IsPressed(controllerIndex, 32 + povIndex))
					{
						if (!buttonSet)
						{
							buttonSet = true;
							pressedButton = button;
						}
					}
				}
			}

			for (int i = 0; i < 4; i++)
			{
				GetGlobalButtonsConfig().SetIsPressed(controllerIndex, 32 + i, false);
			}

			if (povIndex != -1)
			{
				GetGlobalButtonsConfig().SetIsPressed(controllerIndex, 32 + povIndex, true);
			}
		}
		else
		{
			if (info.dwPOV != JOY_POVCENTERED)
			{
				int povIndex = info.dwPOV / 9000;
				short button = GetGlobalButtonsConfig().GetKey(controllerIndex, 32 + povIndex);

				if (button != 0 && !buttonSet)
				{
					buttonSet = true;
					pressedButton = button;
				}
			}
		}
	}

	return 0;
}

/*int SetControllerPressedButtonLogHook(int* params)
{
	const int A4 = params[0];

	const auto L00434570 = (short(*)(int))0x00434570;

	short& s_V0x08D93D4 = *(short*)0x08D93D4;
	short& s_V0x08D93DC = *(short*)0x08D93DC;
	short& s_V0x08D6BAE = *(short*)0x08D6BAE;
	short& s_V0x07D4B6A = *(short*)0x07D4B6A;
	short& s_V0x091AB86 = *(short*)0x091AB86;

	L00434570(A4);

	OutputDebugString((
		std::to_string(A4)
		+ " " + std::to_string(s_V0x08D93D4) + ";" + std::to_string(s_V0x08D93DC) + ";" + std::to_string(s_V0x08D6BAE)
		+ " " + std::to_string(s_V0x07D4B6A)
		+ " " + std::to_string(s_V0x091AB86)
		).c_str());

	return 0;
}*/

int ControllerRemapTextHook(int* params)
{
	return (int)"Use XwaJoystickConfig to Remap Buttons";
}

int ControllerRemapRunHook(int* params)
{
	const auto XwaCreateProcess = (int(*)(const char*))0x0050DCA0;

	//const bool s_XwaIsInConcourse = *(int*)0x005FFD9C != 0;

	//if (!s_XwaIsInConcourse)
	//{
	//	return 0;
	//}

	XwaCreateProcess("XwaJoystickConfig.exe");

	return 0;
}

int ControllerRemapReloadHook(int* params)
{
	//s_XwaConfigMenuIndex = ConfigMenuIndex_Main;
	*(int*)0x007829B8 = 0;

	g_config.Load();
	GetGlobalButtonsConfig().Load();

	return 0;
}

int NumPadLookMoveHook(int* params)
{
	const auto XwaGetConnectedJoysticksCount = (int(*)())0x00541030;

	int moveDelta = *(int*)0x005AA000;
	KeyEnum joyKey = *(KeyEnum*)0x008053C0;

	moveDelta = moveDelta / 2;

	if (XwaGetConnectedJoysticksCount() == 0)
	{
		switch (joyKey)
		{
		case Key_NUMPAD4:
		case Key_NUMPAD6:
		case Key_NUMPAD8:
		case Key_NUMPAD2:
		case Key_NUMPAD5:
			joyKey = Key_None;
			break;
		}
	}

	params[0] = joyKey;

	return moveDelta;
}
