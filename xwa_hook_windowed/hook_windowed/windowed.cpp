#include "targetver.h"
#include "windowed.h"
#include "config.h"
#include <Windows.h>

class WindowConfig
{
public:
	WindowConfig()
	{
		auto lines = GetFileLines("hook_windowed.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_windowed");
		}

		int x = GetFileKeyValueInt(lines, "X");
		int y = GetFileKeyValueInt(lines, "Y");
		int width = GetFileKeyValueInt(lines, "Width");
		int height = GetFileKeyValueInt(lines, "Height");

		if (x < 0)
		{
			x = 0;
		}

		if (y < 0)
		{
			y = 0;
		}

		if (width < 0)
		{
			width = 0;
		}

		if (height < 0)
		{
			height = 0;
		}

		//int cx = GetSystemMetrics(SM_CXSCREEN);
		//int cy = GetSystemMetrics(SM_CYSCREEN);

		DEVMODE mode{};
		mode.dmSize = sizeof(DEVMODE);
		EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &mode);
		int cx = mode.dmPelsWidth;
		int cy = mode.dmPelsHeight;

		bool isFullscreen;

		if (width == 0 || height == 0)
		{
			x = 0;
			y = 0;
			width = cx;
			height = cy;
			isFullscreen = true;
		}
		else
		{
			if (width < 640)
			{
				width = 640;
			}

			if (height < 480)
			{
				height = 480;
			}

			if (x == 0 && y == 0 && width == cx && height == cy)
			{
				isFullscreen = true;
			}
			else
			{
				isFullscreen = false;
			}
		}

		this->IsFullscreen = isFullscreen;
		this->X = x;
		this->Y = y;
		this->Width = width;
		this->Height = height;
	}

	bool IsFullscreen;
	int X;
	int Y;
	int Width;
	int Height;
};

WindowConfig g_windowConfig;

bool IsWindowInBackground()
{
	static bool initialized = true;

	const auto InitDirectInput = (void(*)())0x0042AF80;
	const auto ReleaseDirectInput = (void(*)())0x0042B4A0;

	HWND mainWindow = *(HWND*)0x009F701A;
	HWND foregroundWindow = GetForegroundWindow();

	if (foregroundWindow == mainWindow)
	{
		if (!initialized)
		{
			initialized = true;
			InitDirectInput();
			BringWindowToTop(mainWindow);
		}

		return false;
	}
	else
	{
		if (initialized)
		{
			initialized = false;
			ReleaseDirectInput();
		}

		return true;
	}
}

int KeyboardIsAnyKeyPressedHook(int* params)
{
	const auto XwaDIKeyboardIsAnyKeyPressed = (int(*)())0x0042B520;

	if (IsWindowInBackground())
	{
		*(int*)0x006343DC = 0;
		*(int*)0x006343E0 = 0;
		*(int*)0x006343E4 = 0;

		return 0;
	}

	return XwaDIKeyboardIsAnyKeyPressed();
}

int KeyboardGetPressedKeyHook(int* params)
{
	const auto XwaDIKeyboardGetPressedKey = (unsigned short(*)())0x0042B740;

	if (IsWindowInBackground())
	{
		*(int*)0x006343DC = 0;
		*(int*)0x006343E0 = 0;
		*(int*)0x006343E4 = 0;

		return 0;
	}

	return XwaDIKeyboardGetPressedKey();
}

int KeyboardTryUpdateShiftControlAltKeysPressedStateHook(int* params)
{
	const auto XwaDIKeyboardTryUpdateShiftControlAltKeysPressedState = (int(*)())0x0042B680;

	if (IsWindowInBackground())
	{
		*(int*)0x006343DC = 0;
		*(int*)0x006343E0 = 0;
		*(int*)0x006343E4 = 0;

		return 0;
	}

	return XwaDIKeyboardTryUpdateShiftControlAltKeysPressedState();
}

int RetrieveKeyboardStateHook(int* params)
{
	const auto XwaDIRetrieveKeyboardState = (int(*)())0x0042B900;

	memset((void*)0x009E9520, 0, 0x100);

	if (IsWindowInBackground())
	{
		return 0;
	}

	return XwaDIRetrieveKeyboardState();
}

int RetrieveMouseStateHook(int* params)
{
	const auto XwaDIRetrieveMouseState = (void(*)())0x0042B920;

	memset((void*)0x009E9620, 0, 0x10);

	if (IsWindowInBackground())
	{
		return 0;
	}

	XwaDIRetrieveMouseState();
	return 0;
}

int CreateWindowHook(int* params)
{
	ATOM atom = (ATOM)params[0];
	HINSTANCE hInstance = (HINSTANCE)params[1];

	HWND hwnd = CreateWindowExA(
		0,
		(LPCSTR)atom,
		"X-Wing Alliance",
		WS_POPUP | WS_VISIBLE,
		g_windowConfig.X, g_windowConfig.Y,
		g_windowConfig.Width, g_windowConfig.Height,
		nullptr,
		nullptr,
		hInstance,
		nullptr
		);

	return (int)hwnd;
}

int CursorHook(int* params)
{
	HWND mainWindow = *(HWND*)0x009F701A;
	int x = *(int*)0x009F65ED;
	int y = *(int*)0x009F65F1;

	if (!g_windowConfig.IsFullscreen || IsWindowInBackground())
	{
		if (x == 0 || y == 0 || x == 640 || y == 480)
		{
			return TRUE;
		}
	}

	RECT rc;

	if (SUCCEEDED(GetWindowRect(mainWindow, &rc)))
	{
		x += rc.left;
		y += rc.top;
	}

	return SetCursorPos(x, y);
}

int CursorZeroHook(int* params)
{
	HWND mainWindow = *(HWND*)0x009F701A;
	int x = 0;
	int y = 0;

	RECT rc;

	if (SUCCEEDED(GetWindowRect(mainWindow, &rc)))
	{
		x += rc.left;
		y += rc.top;
	}

	return SetCursorPos(x, y);
}
