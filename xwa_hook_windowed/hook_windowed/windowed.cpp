#include "targetver.h"
#include "windowed.h"
#include "config.h"
#include <fstream>
#include <Windows.h>
#include <gdiplus.h>

#pragma comment(lib, "Gdiplus")

enum ParamsEnum
{
	Params_ReturnAddress = -1,
	Params_EAX = -3,
	Params_ECX = -4,
	Params_EDX = -5,
	Params_EBX = -6,
	Params_EBP = -8,
	Params_ESI = -9,
	Params_EDI = -10,
};

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

		this->ShowSplashScreen = GetFileKeyValueInt(lines, "ShowSplashScreen", 0) != 0;
	}

	bool IsFullscreen;
	int X;
	int Y;
	int Width;
	int Height;
	bool ShowSplashScreen;
};

WindowConfig g_windowConfig;

class GdiInitializer
{
public:
	GdiInitializer()
	{
		this->status = Gdiplus::GdiplusStartup(&token, &gdiplusStartupInput, nullptr);
	}

	~GdiInitializer()
	{
		if (this->status == 0)
		{
			Gdiplus::GdiplusShutdown(token);
		}
	}

	bool hasError()
	{
		return this->status != 0;
	}

	ULONG_PTR token;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;

	Gdiplus::Status status;
};

static GdiInitializer g_gdiInitializer;

void scaleSurface(char* dest, DWORD destWidth, DWORD destHeight, DWORD destBpp, char* src, DWORD srcWidth, DWORD srcHeight, DWORD srcBpp, bool aspectRatioPreserved)
{
	if (g_gdiInitializer.hasError())
		return;

	std::unique_ptr<Gdiplus::Bitmap> bitmap(new Gdiplus::Bitmap(destWidth, destHeight, destBpp == 2 ? PixelFormat16bppRGB565 : PixelFormat32bppRGB));
	std::unique_ptr<Gdiplus::Bitmap> bitmapSrc(new Gdiplus::Bitmap(srcWidth, srcHeight, srcWidth * srcBpp, srcBpp == 2 ? PixelFormat16bppRGB565 : PixelFormat32bppRGB, (BYTE*)src));

	{
		std::unique_ptr<Gdiplus::Graphics> graphics(new Gdiplus::Graphics(bitmap.get()));

		Gdiplus::Rect rc(0, 0, destWidth, destHeight);

		Gdiplus::Rect srcRc;

		if (aspectRatioPreserved)
		{
			if (srcHeight * destWidth <= srcWidth * destHeight)
			{
				srcRc.Width = srcHeight * destWidth / destHeight;
				srcRc.Height = srcHeight;
			}
			else
			{
				srcRc.Width = srcWidth;
				srcRc.Height = srcWidth * destHeight / destWidth;
			}
		}
		else
		{
			srcRc.Width = srcWidth;
			srcRc.Height = srcHeight;
		}

		srcRc.X = (srcWidth - srcRc.Width) / 2;
		srcRc.Y = (srcHeight - srcRc.Height) / 2;

		if (graphics->DrawImage(bitmapSrc.get(), rc, srcRc.X, srcRc.Y, srcRc.Width, srcRc.Height, Gdiplus::UnitPixel) == 0)
		{
			Gdiplus::BitmapData data;

			if (bitmap->LockBits(&rc, Gdiplus::ImageLockModeRead, bitmap->GetPixelFormat(), &data) == 0)
			{
				int rowLength = destWidth * destBpp;

				if (rowLength == data.Stride)
				{
					memcpy(dest, data.Scan0, destHeight * rowLength);
				}
				else
				{
					char* srcBuffer = (char*)data.Scan0;
					char* destBuffer = dest;

					for (DWORD y = 0; y < destHeight; y++)
					{
						memcpy(destBuffer, srcBuffer, rowLength);

						srcBuffer += data.Stride;
						destBuffer += rowLength;
					}
				}

				bitmap->UnlockBits(&data);
			}
		}
	}
}

void GdiLoadImage(const WCHAR* filename, char* dest, DWORD destWidth, DWORD destHeight, DWORD destBpp, bool aspectRatioPreserved)
{
	if (destBpp != 2)
	{
		return;
	}

	if (g_gdiInitializer.hasError())
		return;

	std::unique_ptr<Gdiplus::Bitmap> bitmap(new Gdiplus::Bitmap(filename));
	Gdiplus::Rect rect{ 0, 0, (INT)bitmap->GetWidth(), (INT)bitmap->GetHeight() };

	Gdiplus::BitmapData bmData;
	bitmap->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat16bppRGB565, &bmData);
	scaleSurface(dest, destWidth, destHeight, 2, (char*)bmData.Scan0, bmData.Width, bmData.Height, 2, aspectRatioPreserved);
	bitmap->UnlockBits(&bmData);
}

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

	if (IsWindowInBackground() || *(int*)0x006343CC == 0)
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

	if (IsWindowInBackground() || *(int*)0x006343CC == 0)
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

	if (IsWindowInBackground() || *(int*)0x006343CC == 0)
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

	if (IsWindowInBackground() || *(int*)0x006343CC == 0)
	{
		return 0;
	}

	return XwaDIRetrieveKeyboardState();
}

int RetrieveMouseStateHook(int* params)
{
	const auto XwaDIRetrieveMouseState = (void(*)())0x0042B920;

	memset((void*)0x009E9620, 0, 0x10);

	if (IsWindowInBackground() || *(int*)0x006343D0 == 0)
	{
		return 0;
	}

	XwaDIRetrieveMouseState();
	return 0;
}

int RegisterClassHook(int* params)
{
	HINSTANCE hInstance = (HINSTANCE)params[Params_ESI];

	HBRUSH brush = (HBRUSH)GetStockObject(g_windowConfig.ShowSplashScreen ? GRAY_BRUSH : BLACK_BRUSH);

	return (int)brush;
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

	if (GetWindowRect(mainWindow, &rc))
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

	if (GetWindowRect(mainWindow, &rc))
	{
		x += rc.left;
		y += rc.top;
	}

	return SetCursorPos(x, y);
}

int SplashScreenHook(int* params)
{
	const auto XwaFrontSurfaceLock = (char* (*)())0x0053EF80;
	const auto XwaFrontSurfaceUnlock = (void(*)())0x0053F010;

	char* s_pXwaCurrentSurfaceData = *(char**)0x009F60D4;
	bool isLocked = *(char*)0x009F6FF4 != 0;

	if (isLocked)
	{
		XwaFrontSurfaceUnlock();
	}

	s_pXwaCurrentSurfaceData = XwaFrontSurfaceLock();

	if (g_windowConfig.ShowSplashScreen)
	{
		if (std::ifstream("Splash.jpg"))
		{
			GdiLoadImage(L"Splash.jpg", s_pXwaCurrentSurfaceData, 640, 480, 2, false);
		}
		else if (std::ifstream("Alliance.jpg"))
		{
			GdiLoadImage(L"Alliance.jpg", s_pXwaCurrentSurfaceData, 640, 480, 2, false);
		}
		else
		{
			memset(s_pXwaCurrentSurfaceData, 0xff, 640 * 480 * 2);
		}
	}
	else
	{
		memset(s_pXwaCurrentSurfaceData, 0, 640 * 480 * 2);

	}

	if (!isLocked)
	{
		XwaFrontSurfaceUnlock();
	}

	return 0;
}

int MissionPausedHook(int* params)
{
	return ((int(*)())0x0050B680)();
}
