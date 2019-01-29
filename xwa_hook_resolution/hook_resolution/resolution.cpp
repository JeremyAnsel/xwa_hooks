#include "targetver.h"
#include "resolution.h"
#include <string>
#include <sstream>
#include <iomanip>

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

	std::stringstream stream;
	stream << screenIndex << ": ";
	stream << width << " x " << height;
	stream << " HUD: ";
	stream << std::fixed << std::setprecision(2) << hudScale;

	g_screenResolution = stream.str();

	return (int)g_screenResolution.c_str();
}
