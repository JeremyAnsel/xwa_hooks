#include "targetver.h"
#include "resolution.h"
#include "config.h"
#include <sstream>
#include <iomanip>
#include <Windows.h>

class Config
{
public:
	Config()
	{
		auto lines = GetFileLines("hook_resolution.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_resolution");
		}

		this->IsAutoResolutionEnabled = GetFileKeyValueInt(lines, "IsAutoResolutionEnabled", 0) != 0;
		this->ResolutionWidth = GetFileKeyValueInt(lines, "ResolutionWidth", 0);
		this->ResolutionHeight = GetFileKeyValueInt(lines, "ResolutionHeight", 0);

		if (this->ResolutionWidth <= 0 || this->ResolutionHeight <= 0)
		{
			this->ResolutionWidth = GetSystemMetrics(SM_CXSCREEN);
			this->ResolutionHeight = GetSystemMetrics(SM_CYSCREEN);
		}

		DEVMODE mode{};
		mode.dmSize = sizeof(DEVMODE);
		EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &mode);

		this->DisplayWidth = mode.dmPelsWidth;
		this->DisplayHeight = mode.dmPelsHeight;

		std::string hudScaleValue = GetFileKeyValue(lines, "HudScale");
		if (hudScaleValue.empty())
		{
			this->HudScale = 0;
		}
		else
		{
			this->HudScale = std::stof(hudScaleValue);
		}

		this->Fov = GetFileKeyValueInt(lines, "Fov", 0);
	}

	bool IsAutoResolutionEnabled;
	int ResolutionWidth;
	int ResolutionHeight;
	int DisplayWidth;
	int DisplayHeight;
	float HudScale;
	int Fov;
};

Config g_config;

std::string g_screenResolution;

int ResolutionHook(int* params)
{
	const int stringIndex = params[0];

	// STR_SCREEN640X480, STR_SCREEN1600X1200
	if (stringIndex < 123 || stringIndex > 128)
	{
		const auto GetFrontTxtString = (const char*(*)(int))0x0055CB50;

		return (int)GetFrontTxtString(stringIndex);
	}

	// FrontTxt_STR_SCREEN640X480
	const int screenIndex = stringIndex - 123;

	int width = 0;
	int height = 0;
	float hudScale = 1.0f;

	switch (screenIndex)
	{
	case 0: // STR_SCREEN640X480
		width = *(int*)0x0050AFDE;
		height = *(int*)0x0050AFE3;
		hudScale = *(float*)(0x00510022 + 6);
		break;

	case 1: // STR_SCREEN800X600
		width = *(int*)0x0050AF35;
		height = *(int*)0x0050AF3A;
		hudScale = *(float*)(0x0051004F + 6);
		break;

	case 2: // STR_SCREEN1024X768
		width = 1024;
		height = 768;
		hudScale = *(float*)(0x0051007C + 6);
		break;

	case 3: // STR_SCREEN1152X864
		width = *(int*)0x0050AF7D;
		height = *(int*)0x0050AF82;
		hudScale = *(float*)(0x005100A6 + 6);
		break;

	case 4: // STR_SCREEN1280X1024
		width = 1280;
		height = 1024;
		hudScale = *(float*)(0x005100D0 + 6);
		break;

	case 5: // STR_SCREEN1600X1200
		width = *(int*)0x0050AFBC;
		height = *(int*)0x0050AFC1;
		hudScale = *(float*)(0x005100FA + 6);
		break;
	}

	if (g_config.IsAutoResolutionEnabled)
	{
		width = g_config.ResolutionWidth;
		height = g_config.ResolutionHeight;
		hudScale = g_config.ResolutionHeight / 600.0f;

		if (g_config.HudScale != 0.0f)
		{
			hudScale = g_config.HudScale;
		}
	}

	std::stringstream stream;
	stream << std::fixed << std::setprecision(2);
	stream << screenIndex << ":" << width << "x" << height;
	stream << " HUD:" << hudScale;

	if (g_config.IsAutoResolutionEnabled)
	{
		stream << " DPI:" << (100 * g_config.DisplayWidth / g_config.ResolutionWidth) << "%";
	}

	g_screenResolution = stream.str();

	return (int)g_screenResolution.c_str();
}

int FlightRunLoopHook(int* params)
{
	const auto L0050D070 = (void(*)())0x0050D070;

	if (g_config.IsAutoResolutionEnabled)
	{
		float cy = (float)g_config.ResolutionHeight;

		*(float*)0x06002B8 = cy / 600.0f;

		if (g_config.HudScale != 0.0f)
		{
			*(float*)0x06002B8 = g_config.HudScale;
		}

		*(int*)0x091AB6C = (int)(cy * 1.0666f + 0.5f);

		if (g_config.Fov != 0)
		{
			*(int*)0x091AB6C = g_config.Fov;
		}

		*(int*)0x07B33CC = *(int*)0x091AB6C / 2;
		*(float*)0x07D4B78 = *(float*)0x06002B8 * 1.25f;

		*(float*)0x08B94CC = (float)*(int*)0x091AB6C;
		*(float*)0x08B94BC = *(int*)0x091AB6C * 0.001953125f;
	}

	L0050D070();
	return 0;
}

int FlightMainHook(int* params)
{
	const auto L0050BC20 = (char(*)())0x0050BC20;

	if (g_config.IsAutoResolutionEnabled)
	{
		int cx = g_config.ResolutionWidth;
		int cy = g_config.ResolutionHeight;

		*(int*)0x091AD34 = cx;
		*(int*)0x091AD3C = cy;
		*(int*)0x06002B0 = cx;
		*(int*)0x06002B4 = cy;
		*(int*)0x05FFDB4 = cx;
	}

	return L0050BC20();
}
