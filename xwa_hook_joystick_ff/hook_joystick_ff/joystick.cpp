#include "targetver.h"
#include "joystick.h"
#include "config.h"
#include <array>
#include <map>
#include <utility>

#define INITGUID
#include <dinput.h>

#pragma comment(lib, "Winmm.lib")

const auto& g_joyGetNumDevs = *(decltype(joyGetNumDevs)**)0x005A92B4;
const auto& g_joyGetDevCapsA = *(decltype(joyGetDevCapsA)**)0x005A92A8;
const auto& g_joyGetPosEx = *(decltype(joyGetPosEx)**)0x005A92A4;

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
		this->YawControllerIndex = GetFileKeyValueInt(lines, "YawControllerIndex", 0);
		this->YawControllerAxisIndex = GetFileKeyValueInt(lines, "YawControllerAxisIndex", 0);
		this->InvertYaw = GetFileKeyValueInt(lines, "InvertYaw", 0) != 0;
		this->PitchControllerIndex = GetFileKeyValueInt(lines, "PitchControllerIndex", 0);
		this->PitchControllerAxisIndex = GetFileKeyValueInt(lines, "PitchControllerAxisIndex", 1);
		this->InvertPitch = GetFileKeyValueInt(lines, "InvertPitch", 0) != 0;
		this->ThrottleControllerIndex = GetFileKeyValueInt(lines, "ThrottleControllerIndex", 0);
		this->ThrottleControllerAxisIndex = GetFileKeyValueInt(lines, "ThrottleControllerAxisIndex", 2);
		this->InvertThrottle = GetFileKeyValueInt(lines, "InvertThrottle", 0) != 0;
		this->RudderControllerIndex = GetFileKeyValueInt(lines, "RudderControllerIndex", 0);
		this->RudderControllerAxisIndex = GetFileKeyValueInt(lines, "RudderControllerAxisIndex", 3);
		this->InvertRudder = GetFileKeyValueInt(lines, "InvertRudder", 0) != 0;
		this->UsePovControllerAsButtons = GetFileKeyValueInt(lines, "UsePovControllerAsButtons", 0) != 0;
	}

	int JoystickFFDeviceIndex;
	bool EnableSmallMovement;
	int MainControllerIndex;
	int YawControllerIndex;
	int YawControllerAxisIndex;
	bool InvertYaw;
	int PitchControllerIndex;
	int PitchControllerAxisIndex;
	bool InvertPitch;
	int ThrottleControllerIndex;
	int ThrottleControllerAxisIndex;
	bool InvertThrottle;
	int RudderControllerIndex;
	int RudderControllerAxisIndex;
	bool InvertRudder;
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
		this->_deviceCount = 0;

		auto lines = GetFileLines("JoystickConfig.txt");

		int deviceCount = g_joyGetNumDevs();
		int controllerIndex = -1;
		int buttonIndex = 0;
		int povIndex = 0;

		for (int deviceIndex = 0; deviceIndex < deviceCount; deviceIndex++)
		{
			JOYCAPS caps{};

			if (g_joyGetDevCapsA(deviceIndex, &caps, sizeof(JOYCAPS)) != JOYERR_NOERROR)
			{
				this->_isConnected[deviceIndex] = false;
				continue;
			}

			this->_isConnected[deviceIndex] = true;
			controllerIndex++;
			int controllerId = (int)((unsigned int)caps.wMid << 16 | (unsigned int)caps.wPid);

			std::array<short, 32 + 4> buttons{};

			int numButtons = min(caps.wNumButtons, 32);

			for (int index = 0; index < numButtons; index++)
			{
				std::string key1 = std::string("joybutton_") + std::to_string(controllerIndex) + std::string("_") + std::to_string(index + 1);
				std::string key2 = std::string("joybutton_") + std::to_string(controllerId) + std::string("_") + std::to_string(index + 1);

				int value = GetDefaultConfigButton(buttonIndex + 1);
				value = GetFileKeyValueInt(lines, key1, value);
				value = GetFileKeyValueInt(lines, key2, value);

				buttons[index] = (short)value;
				buttonIndex++;
			}

			if (caps.wCaps & JOYCAPS_HASPOV)
			{
				for (int index = 0; index < 4; index++)
				{
					std::string key1 = std::string("joybutton_") + std::to_string(controllerIndex) + std::string("_pov") + std::to_string(index + 1);
					std::string key2 = std::string("joybutton_") + std::to_string(controllerId) + std::string("_pov") + std::to_string(index + 1);

					int value = GetDefaultConfigPov(povIndex + 1);
					value = GetFileKeyValueInt(lines, key1, value);
					value = GetFileKeyValueInt(lines, key2, value);

					buttons[32 + index] = (short)value;
					povIndex++;
				}
			}

			this->_buttons[deviceIndex] = buttons;
			this->_buttonsIsPressed[deviceIndex].fill(false);
			this->_caps[deviceIndex] = caps;

			if (g_config.MainControllerIndex == controllerId)
			{
				g_config.MainControllerIndex = controllerIndex;
			}

			if (g_config.YawControllerIndex == controllerId)
			{
				g_config.YawControllerIndex = controllerIndex;
			}

			if (g_config.PitchControllerIndex == controllerId)
			{
				g_config.PitchControllerIndex = controllerIndex;
			}

			if (g_config.ThrottleControllerIndex == controllerId)
			{
				g_config.ThrottleControllerIndex = controllerIndex;
			}

			if (g_config.RudderControllerIndex == controllerId)
			{
				g_config.RudderControllerIndex = controllerIndex;
			}
		}

		this->_deviceCount = deviceCount;
	}

private:
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

public:
	int GetDeviceCount()
	{
		return this->_deviceCount;
	}

	bool IsConnected(int deviceIndex)
	{
		return this->_isConnected[deviceIndex];
	}

	short GetKey(int deviceIndex, int buttonIndex)
	{
		return this->_buttons[deviceIndex][buttonIndex];
	}

	bool IsPressed(int deviceIndex, int buttonIndex)
	{
		return this->_buttonsIsPressed[deviceIndex][buttonIndex];
	}

	void SetIsPressed(int deviceIndex, int buttonIndex, bool isPressed)
	{
		this->_buttonsIsPressed[deviceIndex][buttonIndex] = isPressed;
	}

	void ClearIsPressed()
	{
		for (auto& buttons : this->_buttonsIsPressed)
		{
			buttons.fill(false);
		}
	}

	const JOYCAPS& GetCaps(int deviceIndex)
	{
		return this->_caps[deviceIndex];
	}

private:
	int _deviceCount;
	std::array<bool, 16> _isConnected;
	std::array<std::array<short, 32 + 4>, 16> _buttons;
	std::array<std::array<bool, 32 + 4>, 16> _buttonsIsPressed;
	std::array<JOYCAPS, 16> _caps;
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

	LPDIRECTINPUTA& s_XwaForceFeedbackIDirectInput = *(LPDIRECTINPUTA*)0x0063CF78;
	LPDIRECTINPUTDEVICE2A& s_XwaForceFeedbackIDirectInput2 = *(LPDIRECTINPUTDEVICE2A*)0x0063CF7C;

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
	int deviceCount = GetGlobalButtonsConfig().GetDeviceCount();

	if (deviceCount == 0)
	{
		return 1;
	}

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

	int deviceCount = GetGlobalButtonsConfig().GetDeviceCount();

	int yawControllerIndex = g_config.YawControllerIndex;
	int yawControllerAxisIndex = g_config.YawControllerAxisIndex;
	int pitchControllerIndex = g_config.PitchControllerIndex;
	int pitchControllerAxisIndex = g_config.PitchControllerAxisIndex;
	int throttleControllerIndex = g_config.ThrottleControllerIndex;
	int throttleControllerAxisIndex = g_config.ThrottleControllerAxisIndex;
	int rudderControllerIndex = g_config.RudderControllerIndex;
	int rudderControllerAxisIndex = g_config.RudderControllerAxisIndex;

	int controllerIndex = -1;

	for (int deviceIndex = 0; deviceIndex < deviceCount; deviceIndex++)
	{
		if (!GetGlobalButtonsConfig().IsConnected(deviceIndex))
		{
			continue;
		}

		const JOYCAPS& caps = GetGlobalButtonsConfig().GetCaps(deviceIndex);

		controllerIndex++;

		if (controllerIndex == yawControllerIndex)
		{
			switch (yawControllerAxisIndex)
			{
			case 0:
				esp44.wXmin = caps.wXmin;
				esp44.wXmax = caps.wXmax;
				break;

			case 1:
				esp44.wXmin = caps.wYmin;
				esp44.wXmax = caps.wYmax;
				break;

			case 2:
				esp44.wXmin = caps.wZmin;
				esp44.wXmax = caps.wZmax;
				break;

			case 3:
				esp44.wXmin = caps.wRmin;
				esp44.wXmax = caps.wRmax;
				break;

			case 4:
				esp44.wXmin = caps.wUmin;
				esp44.wXmax = caps.wUmax;
				break;

			case 5:
				esp44.wXmin = caps.wVmin;
				esp44.wXmax = caps.wVmax;
				break;
			}
		}

		if (controllerIndex == pitchControllerIndex)
		{
			switch (pitchControllerAxisIndex)
			{
			case 0:
				esp44.wYmin = caps.wXmin;
				esp44.wYmax = caps.wXmax;
				break;

			case 1:
				esp44.wYmin = caps.wYmin;
				esp44.wYmax = caps.wYmax;
				break;

			case 2:
				esp44.wYmin = caps.wZmin;
				esp44.wYmax = caps.wZmax;
				break;

			case 3:
				esp44.wYmin = caps.wRmin;
				esp44.wYmax = caps.wRmax;
				break;

			case 4:
				esp44.wYmin = caps.wUmin;
				esp44.wYmax = caps.wUmax;
				break;

			case 5:
				esp44.wYmin = caps.wVmin;
				esp44.wYmax = caps.wVmax;
				break;
			}
		}

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

	int deviceCount = GetGlobalButtonsConfig().GetDeviceCount();

	int yawControllerIndex = g_config.YawControllerIndex;
	int yawControllerAxisIndex = g_config.YawControllerAxisIndex;
	int pitchControllerIndex = g_config.PitchControllerIndex;
	int pitchControllerAxisIndex = g_config.PitchControllerAxisIndex;
	int throttleControllerIndex = g_config.ThrottleControllerIndex;
	int throttleControllerAxisIndex = g_config.ThrottleControllerAxisIndex;
	int rudderControllerIndex = g_config.RudderControllerIndex;
	int rudderControllerAxisIndex = g_config.RudderControllerAxisIndex;

	int controllerIndex = -1;

	for (int deviceIndex = 0; deviceIndex < deviceCount; deviceIndex++)
	{
		if (!GetGlobalButtonsConfig().IsConnected(deviceIndex))
		{
			continue;
		}

		JOYINFOEX info{};
		info.dwSize = sizeof(JOYINFOEX);
		info.dwFlags = JOY_RETURNX | JOY_RETURNY | JOY_RETURNZ | JOY_RETURNR | JOY_RETURNU | JOY_RETURNV | JOY_RETURNPOV | JOY_RETURNBUTTONS | JOY_RETURNCENTERED;

		if (g_joyGetPosEx(deviceIndex, &info) != JOYERR_NOERROR)
		{
			continue;
		}

		controllerIndex++;

		const auto& caps = GetGlobalButtonsConfig().GetCaps(deviceIndex);
		bool isYawInverted = g_config.InvertYaw;
		bool isPitchInverted = g_config.InvertPitch;
		bool isThrottleInverted = g_config.InvertThrottle;
		bool isRudderInverted = g_config.InvertRudder;

		if (controllerIndex == yawControllerIndex)
		{
			switch (yawControllerAxisIndex)
			{
			case 0:
				if (isYawInverted)
				{
					esp10.dwXpos = caps.wXmax + caps.wXmin - info.dwXpos;
				}
				else
				{
					esp10.dwXpos = info.dwXpos;
				}

				break;

			case 1:
				if (isYawInverted)
				{
					esp10.dwXpos = caps.wYmax + caps.wYmin - info.dwYpos;
				}
				else
				{
					esp10.dwXpos = info.dwYpos;
				}

				break;

			case 2:
				if (isYawInverted)
				{
					esp10.dwXpos = caps.wZmax + caps.wZmin - info.dwZpos;
				}
				else
				{
					esp10.dwXpos = info.dwZpos;
				}

				break;

			case 3:
				if (isYawInverted)
				{
					esp10.dwXpos = caps.wRmax + caps.wRmin - info.dwRpos;
				}
				else
				{
					esp10.dwXpos = info.dwRpos;
				}

				break;

			case 4:
				if (isYawInverted)
				{
					esp10.dwXpos = caps.wUmax + caps.wUmin - info.dwUpos;
				}
				else
				{
					esp10.dwXpos = info.dwUpos;
				}

				break;

			case 5:
				if (isYawInverted)
				{
					esp10.dwXpos = caps.wVmax + caps.wVmin - info.dwVpos;
				}
				else
				{
					esp10.dwXpos = info.dwVpos;
				}

				break;
			}
		}

		if (controllerIndex == pitchControllerIndex)
		{
			switch (pitchControllerAxisIndex)
			{
			case 0:
				if (isPitchInverted)
				{
					esp10.dwYpos = caps.wXmax + caps.wXmin - info.dwXpos;
				}
				else
				{
					esp10.dwYpos = info.dwXpos;
				}

				break;

			case 1:
				if (isPitchInverted)
				{
					esp10.dwYpos = caps.wYmax + caps.wYmin - info.dwYpos;
				}
				else
				{
					esp10.dwYpos = info.dwYpos;
				}

				break;

			case 2:
				if (isPitchInverted)
				{
					esp10.dwYpos = caps.wZmax + caps.wZmin - info.dwZpos;
				}
				else
				{
					esp10.dwYpos = info.dwZpos;
				}

				break;

			case 3:
				if (isPitchInverted)
				{
					esp10.dwYpos = caps.wRmax + caps.wRmin - info.dwRpos;
				}
				else
				{
					esp10.dwYpos = info.dwRpos;
				}

				break;

			case 4:
				if (isPitchInverted)
				{
					esp10.dwYpos = caps.wUmax + caps.wUmin - info.dwUpos;
				}
				else
				{
					esp10.dwYpos = info.dwUpos;
				}

				break;

			case 5:
				if (isPitchInverted)
				{
					esp10.dwYpos = caps.wVmax + caps.wVmin - info.dwVpos;
				}
				else
				{
					esp10.dwYpos = info.dwVpos;
				}

				break;
			}
		}

		if (controllerIndex == throttleControllerIndex)
		{
			switch (throttleControllerAxisIndex)
			{
			case 0:
				if (isThrottleInverted)
				{
					esp10.dwZpos = caps.wXmax + caps.wXmin - info.dwXpos;
				}
				else
				{
					esp10.dwZpos = info.dwXpos;
				}

				break;

			case 1:
				if (isThrottleInverted)
				{
					esp10.dwZpos = caps.wYmax + caps.wYmin - info.dwYpos;
				}
				else
				{
					esp10.dwZpos = info.dwYpos;
				}

				break;

			case 2:
				if (isThrottleInverted)
				{
					esp10.dwZpos = caps.wZmax + caps.wZmin - info.dwZpos;
				}
				else
				{
					esp10.dwZpos = info.dwZpos;
				}

				break;

			case 3:
				if (isThrottleInverted)
				{
					esp10.dwZpos = caps.wRmax + caps.wRmin - info.dwRpos;
				}
				else
				{
					esp10.dwZpos = info.dwRpos;
				}

				break;

			case 4:
				if (isThrottleInverted)
				{
					esp10.dwZpos = caps.wUmax + caps.wUmin - info.dwUpos;
				}
				else
				{
					esp10.dwZpos = info.dwUpos;
				}

				break;

			case 5:
				if (isThrottleInverted)
				{
					esp10.dwZpos = caps.wVmax + caps.wVmin - info.dwVpos;
				}
				else
				{
					esp10.dwZpos = info.dwVpos;
				}

				break;
			}
		}

		if (controllerIndex == rudderControllerIndex)
		{
			switch (rudderControllerAxisIndex)
			{
			case 0:
				if (isRudderInverted)
				{
					esp10.dwRpos = caps.wXmax + caps.wXmin - info.dwXpos;
				}
				else
				{
					esp10.dwRpos = info.dwXpos;
				}

				break;

			case 1:
				if (isRudderInverted)
				{
					esp10.dwRpos = caps.wYmax + caps.wYmin - info.dwYpos;
				}
				else
				{
					esp10.dwRpos = info.dwYpos;
				}

				break;

			case 2:
				if (isRudderInverted)
				{
					esp10.dwRpos = caps.wZmax + caps.wZmin - info.dwZpos;
				}
				else
				{
					esp10.dwRpos = info.dwZpos;
				}

				break;

			case 3:
				if (isRudderInverted)
				{
					esp10.dwRpos = caps.wRmax + caps.wRmin - info.dwRpos;
				}
				else
				{
					esp10.dwRpos = info.dwRpos;
				}

				break;

			case 4:
				if (isRudderInverted)
				{
					esp10.dwRpos = caps.wUmax + caps.wUmin - info.dwUpos;
				}
				else
				{
					esp10.dwRpos = info.dwUpos;
				}

				break;

			case 5:
				if (isRudderInverted)
				{
					esp10.dwRpos = caps.wVmax + caps.wVmin - info.dwVpos;
				}
				else
				{
					esp10.dwRpos = info.dwVpos;
				}

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

	int deviceCount = GetGlobalButtonsConfig().GetDeviceCount();
	int controllerIndex = -1;
	bool buttonSet = false;

	for (int deviceIndex = 0; deviceIndex < deviceCount; deviceIndex++)
	{
		if (!GetGlobalButtonsConfig().IsConnected(deviceIndex))
		{
			continue;
		}

		JOYINFOEX info{};
		info.dwSize = sizeof(JOYINFOEX);
		info.dwFlags = JOY_RETURNPOV | JOY_RETURNBUTTONS | JOY_RETURNCENTERED;

		if (g_joyGetPosEx(deviceIndex, &info) != JOYERR_NOERROR)
		{
			continue;
		}

		controllerIndex++;

		for (int i = 0; i < 32; i++)
		{
			short button = GetGlobalButtonsConfig().GetKey(deviceIndex, i);

			if (button == 0)
			{
				continue;
			}

			if ((info.dwButtons & (1U << i)) != 0)
			{
				if (!GetGlobalButtonsConfig().IsPressed(deviceIndex, i))
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

			GetGlobalButtonsConfig().SetIsPressed(deviceIndex, i, (info.dwButtons & (1U << i)) != 0);
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
				short button = GetGlobalButtonsConfig().GetKey(deviceIndex, 32 + povIndex);

				if (button != 0)
				{
					if (!GetGlobalButtonsConfig().IsPressed(deviceIndex, 32 + povIndex))
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
				GetGlobalButtonsConfig().SetIsPressed(deviceIndex, 32 + i, false);
			}

			if (povIndex != -1)
			{
				GetGlobalButtonsConfig().SetIsPressed(deviceIndex, 32 + povIndex, true);
			}
		}
		else
		{
			if (info.dwPOV != JOY_POVCENTERED)
			{
				int povIndex = info.dwPOV / 9000;
				short button = GetGlobalButtonsConfig().GetKey(deviceIndex, 32 + povIndex);

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
