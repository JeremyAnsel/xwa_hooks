#include "targetver.h"
#include "concourse.h"
#include "config.h"
#include <fstream>
#include <queue>
#include <algorithm>
#include <tuple>
#include <sstream>
#include <map>
#include <chrono>

//#define NOMINMAX
#include <Windows.h>
#include <ddraw.h>
#include <d3d.h>
#include <dxgi.h>
#include <d2d1.h>
#include <d2d1effects.h>
#include <d2d1effectauthor.h>
#include <d2d1effecthelpers.h>
#include <dwrite.h>
#include <gdiplus.h>
#include <comdef.h>
#include "ComPtr.h"
#include "SurfaceDC.h"
#include "BitmapEffect.h"

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "Gdiplus")

#include "webm_helpers.h"

#include <string>
#include <sstream>
#include <iomanip>

std::string address_to_hex(int i)
{
	std::stringstream stream;
	stream << std::setfill('0') << std::hex
		<< std::setw(2) << ((i >> 24) & 0xff)
		<< std::setw(2) << ((i >> 16) & 0xff)
		<< std::setw(2) << ((i >> 8) & 0xff)
		<< std::setw(2) << (i & 0xff);
	return stream.str();
}

// Stack contents for function params
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

float GetFileKeyValueFloat(const std::vector<std::string>& lines, const std::string& key, float defaultValue)
{
	std::string value = GetFileKeyValue(lines, key);

	if (value.empty())
	{
		return defaultValue;
	}

	return std::stof(value);
}

std::wstring string_towstring(const char* text)
{
	std::wstringstream path;
	path << text;
	return path.str();
}

unsigned int Color16BppTo32Bpp(unsigned int color)
{
	unsigned int b = (color & 0x1F) << 3;
	unsigned int g = (color & 0x7E0) << 5;
	unsigned int r = (color & 0xF800) << 8;
	return 0xff000000 | b | g | r;
}

bool ends_with(const std::string& first, const std::string& second)
{
	if (second.size() > first.size())
	{
		return false;
	}

	return first.substr(first.size() - second.size()) == second;
}

std::string GetFileNameWithoutExtension(const std::string& str)
{
	auto a = str.find_last_of('\\');

	return a == -1 ? str : str.substr(a + 1, -1);
}

std::vector<std::string> TokennizeSpaces(const std::string& str)
{
	const char* delimiters = " \t\n\r\f\v";
	std::vector<std::string> tokens;

	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	std::string::size_type pos = str.find_first_of(delimiters, lastPos);

	while (std::string::npos != pos || std::string::npos != lastPos)
	{
		std::string value = Trim(str.substr(lastPos, pos - lastPos));

		if (!value.empty())
		{
			tokens.push_back(value);
		}

		lastPos = str.find_first_not_of(delimiters, pos);
		pos = str.find_first_of(delimiters, lastPos);
	}

	return tokens;
}

class FlightModelsList
{
public:
	FlightModelsList()
	{
		for (const auto& line : GetFileLines("FlightModels\\Spacecraft0.LST"))
		{
			this->_spacecraftList.push_back(GetStringWithoutExtension(line));
		}

		for (const auto& line : GetFileLines("FlightModels\\Equipment0.LST"))
		{
			this->_equipmentList.push_back(GetStringWithoutExtension(line));
		}
	}

	std::string GetLstLine(int modelIndex)
	{
		const int xwaObjectStats = 0x05FB240;
		const int dataIndex1 = *(short*)(xwaObjectStats + modelIndex * 0x18 + 0x14);
		const int dataIndex2 = *(short*)(xwaObjectStats + modelIndex * 0x18 + 0x16);

		switch (dataIndex1)
		{
		case 0:
			if ((unsigned int)dataIndex2 < this->_spacecraftList.size())
			{
				return this->_spacecraftList[dataIndex2];
			}

			break;

		case 1:
			if ((unsigned int)dataIndex2 < this->_equipmentList.size())
			{
				return this->_equipmentList[dataIndex2];
			}

			break;
		}

		return std::string();
	}

private:
	std::vector<std::string> _spacecraftList;
	std::vector<std::string> _equipmentList;
};

FlightModelsList g_flightModelsList;

class Config
{
public:
	Config()
	{
		auto lines = GetFileLines("hook_concourse.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_concourse");
		}

		this->SkipFirst7Missions = GetFileKeyValueInt(lines, "SkipFirst7Missions", 0) != 0;
		this->KalidorCrescentPositionX = GetFileKeyValueInt(lines, "KalidorCrescentPositionX", 224);
		this->KalidorCrescentPositionY = GetFileKeyValueInt(lines, "KalidorCrescentPositionY", 168);

		this->FilmPodMovie = GetFileKeyValue(lines, "FilmPodMovie");
		if (this->FilmPodMovie.empty())
		{
			this->FilmPodMovie = "pod";
		}

		this->MultiPodMovie = GetFileKeyValue(lines, "MultiPodMovie");
		if (this->MultiPodMovie.empty())
		{
			this->MultiPodMovie = "pod";
		}

		this->SinglePodMovie = GetFileKeyValue(lines, "SinglePodMovie");
		if (this->SinglePodMovie.empty())
		{
			this->SinglePodMovie = "pod";
		}

		this->FilmRoomBackground = GetFileKeyValue(lines, "FilmRoomBackground");
		if (this->FilmRoomBackground.empty())
		{
			this->FilmRoomBackground = "frontres\\combat\\multiplayer.bmp";
		}

		this->DrawPanelsInSimulator = GetFileKeyValueInt(lines, "DrawPanelsInSimulator", 1) != 0;
		this->DrawPanelsInTourOfDutyBriefing = GetFileKeyValueInt(lines, "DrawPanelsInTourOfDutyBriefing", 1) != 0;
		this->DrawPanelsInTourOfDutyDebriefing = GetFileKeyValueInt(lines, "DrawPanelsInTourOfDutyDebriefing", 1) != 0;
		this->DrawPanelsInFamilyMonitor = GetFileKeyValueInt(lines, "DrawPanelsInFamilyMonitor", 1) != 0;
		this->DrawPanelsInFilmRoom = GetFileKeyValueInt(lines, "DrawPanelsInFilmRoom", 1) != 0;

		this->HDConcourseEnabled = GetFileKeyValueInt(lines, "HDConcourseEnabled", 0) != 0;
		this->HDConcourseTextFont = GetFileKeyValue(lines, "HDConcourseTextFont");

		if (this->HDConcourseTextFont.empty())
		{
			this->HDConcourseTextFont = "consolas";
		}

		this->SupportDatAnimations = GetFileKeyValueInt(lines, "SupportDatAnimations", 0) != 0;
		this->DrawStarfield = GetFileKeyValueInt(lines, "DrawStarfield", 1) != 0;

		this->PlayMovieOutputDebugString = GetFileKeyValueInt(lines, "PlayMovieOutputDebugString", 0) != 0;

		this->SkirmishOptEnabled = GetFileKeyValueInt(lines, "SkirmishOptEnabled", 1) != 0 && this->HDConcourseEnabled;
		this->SkirmishOptScale = GetFileKeyValueFloat(lines, "SkirmishOptScale", 1.0f);
		this->SkirmishOptAnglePitch = GetFileKeyValueFloat(lines, "SkirmishOptAnglePitch", 110.0f);
		this->SkirmishOptAngleYaw = GetFileKeyValueFloat(lines, "SkirmishOptAngleYaw", 225.0f);
		this->SkirmishOptAngleRoll = GetFileKeyValueFloat(lines, "SkirmishOptAngleRoll", 0.0f);
		this->SkirmishOptPositionX = GetFileKeyValueFloat(lines, "SkirmishOptPositionX", 0.5f);
		this->SkirmishOptPositionY = GetFileKeyValueFloat(lines, "SkirmishOptPositionY", 0.0f);
		this->SkirmishOptPositionZ = GetFileKeyValueFloat(lines, "SkirmishOptPositionZ", 0.25f);
	}

	bool SkipFirst7Missions;
	int KalidorCrescentPositionX;
	int KalidorCrescentPositionY;
	std::string FilmPodMovie;
	std::string MultiPodMovie;
	std::string SinglePodMovie;
	std::string FilmRoomBackground;
	bool DrawPanelsInSimulator;
	bool DrawPanelsInTourOfDutyBriefing;
	bool DrawPanelsInTourOfDutyDebriefing;
	bool DrawPanelsInFamilyMonitor;
	bool DrawPanelsInFilmRoom;
	bool HDConcourseEnabled;
	std::string HDConcourseTextFont;
	bool SupportDatAnimations;
	bool DrawStarfield;
	bool PlayMovieOutputDebugString;
	bool SkirmishOptEnabled;
	float SkirmishOptScale;
	float SkirmishOptAnglePitch;
	float SkirmishOptAngleYaw;
	float SkirmishOptAngleRoll;
	float SkirmishOptPositionX;
	float SkirmishOptPositionY;
	float SkirmishOptPositionZ;
};

Config g_config;

class RandomGenerator
{
public:
	RandomGenerator()
	{
		this->seed = (unsigned short)timeGetTime();
	}

	unsigned short Generate()
	{
		this->seed = this->seed * 9421 + 1;
		return this->seed;
	}

	unsigned short Generate(int rangeMin, int rangeMax)
	{
		int range = rangeMax - rangeMin;

		if (range <= 0)
		{
			return 0;
		}

		return rangeMin + (this->Generate() % range);
	}

	unsigned short seed;
};

RandomGenerator g_randomGenerator;

std::vector<std::string> GetMissionConcourseLines(const std::string& mission)
{
	static std::vector<std::string> _lines;
	static std::string _mission;

	if (mission != _mission)
	{
		const std::string path = GetStringWithoutExtension(mission);

		auto lines = GetFileLines(path + "_concourse.txt");

		if (!lines.size())
		{
			lines = GetFileLines(path + ".ini", "concourse");
		}

		_lines = lines;
		_mission = mission;
	}


	return _lines;
}

#pragma pack(push, 1)

struct XwaMissionDescription
{
	char MissionFileName[64];
	char MissionDesciption[128];
	char BattleDescription[128];
	int Id;
	int m144;
};

static_assert(sizeof(XwaMissionDescription) == 328, "size of XwaMissionDescription must be 328");

struct CbmImage
{
	int Width;
	int Height;
	int IsCompressed;
	int DataSize;
	int Left;
	int Top;
	int Right;
	int Bottom;
	char* pData;
	unsigned int ScreenPalette[256];
	unsigned int Palette[256];
};

static_assert(sizeof(CbmImage) == 2084, "size of CbmImage must be 2084");

struct CbmHeader
{
	int ImagesCount;
	int CurrentImage;
	RECT Area;
	int ImageId;
	int GroupId;
	char unk20[4];
};

static_assert(sizeof(CbmHeader) == 36, "size of CbmHeader must be 36");

struct FrontResItem
{
	char Name[64];
	CbmHeader* CbmFile;
};

static_assert(sizeof(FrontResItem) == 68, "size of FrontResItem must be 68");

#pragma pack(pop)

class NetFunctions
{
public:
	typedef void(_cdecl* loadDatImage)(const char*, int, int, int);
	typedef void(_cdecl* loadCbmImage)(const char*, const  char*);
	typedef void(_cdecl* freeImage)(const char*);
	typedef int(_cdecl* getItemImagesCount)(const char*);
	typedef int(_cdecl* getItemImageDimensions)(const char*, int*, int*, int*, int*);
	typedef int(_cdecl* getItemImageRawData)(const char*, int);
	typedef int(_cdecl* frontResDrawItemImage)(const char*, int, int, int, int, int);
	typedef int(_cdecl* frontResDrawItemCurrentImage)(const char*, int, int, int, int, int);
	typedef int(_cdecl* frontResDrawMapIcon)(const char*, int, int, int, int, int, int, int, int, int);
	typedef int(_cdecl* frontResDrawBlendColor)(const char*, int, int, int, int, int, int, int, unsigned int, int, int);
	typedef void(_cdecl* drawBackgroundToScreen)();
	typedef int(_cdecl* frontResGetArea)(const char*, int);
	typedef int(_cdecl* frontResIsBackgroundWide)();
	typedef int(_cdecl* frontResIsBackgroundHD)();

	NetFunctions()
	{
		_module = LoadLibrary("hook_concourse_net.dll");

		if (!_module)
		{
			return;
		}

		_loadDatImage = (loadDatImage)GetProcAddress(_module, "LoadDatImage");
		_loadCbmImage = (loadCbmImage)GetProcAddress(_module, "LoadCbmImage");
		_freeImage = (freeImage)GetProcAddress(_module, "FreeImage");
		_getItemImagesCount = (getItemImagesCount)GetProcAddress(_module, "GetItemImagesCount");
		_getItemImageDimensions = (getItemImageDimensions)GetProcAddress(_module, "GetItemImageDimensions");
		_getItemImageRawData = (getItemImageRawData)GetProcAddress(_module, "GetItemImageRawData");
		_frontResDrawItemImage = (frontResDrawItemImage)GetProcAddress(_module, "FrontResDrawItemImage");
		_frontResDrawItemCurrentImage = (frontResDrawItemCurrentImage)GetProcAddress(_module, "FrontResDrawItemCurrentImage");
		_frontResDrawMapIcon = (frontResDrawMapIcon)GetProcAddress(_module, "FrontResDrawMapIcon");
		_frontResDrawBlendColor = (frontResDrawBlendColor)GetProcAddress(_module, "FrontResDrawBlendColor");
		_drawBackgroundToScreen = (drawBackgroundToScreen)GetProcAddress(_module, "DrawBackgroundToScreen");
		_frontResGetArea = (frontResGetArea)GetProcAddress(_module, "FrontResGetArea");
		_frontResIsBackgroundWide = (frontResIsBackgroundHD)GetProcAddress(_module, "FrontResIsBackgroundWide");
		_frontResIsBackgroundHD = (frontResIsBackgroundHD)GetProcAddress(_module, "FrontResIsBackgroundHD");
	}

	~NetFunctions()
	{
		FreeLibrary(_module);
	}

	HMODULE _module;
	loadDatImage _loadDatImage = nullptr;
	loadCbmImage _loadCbmImage = nullptr;
	freeImage _freeImage = nullptr;
	getItemImagesCount _getItemImagesCount = nullptr;
	getItemImageDimensions _getItemImageDimensions = nullptr;
	getItemImageRawData _getItemImageRawData = nullptr;
	frontResDrawItemImage _frontResDrawItemImage = nullptr;
	frontResDrawItemCurrentImage _frontResDrawItemCurrentImage = nullptr;
	frontResDrawMapIcon _frontResDrawMapIcon = nullptr;
	frontResDrawBlendColor _frontResDrawBlendColor = nullptr;
	drawBackgroundToScreen _drawBackgroundToScreen = nullptr;
	frontResGetArea _frontResGetArea = nullptr;
	frontResIsBackgroundWide _frontResIsBackgroundWide = nullptr;
	frontResIsBackgroundHD _frontResIsBackgroundHD = nullptr;
};

NetFunctions g_netFunctions;

struct ViewConfigurationSettings
{
	float SkirmishOptScale;
	float SkirmishOptAnglePitch;
	float SkirmishOptAngleYaw;
	float SkirmishOptAngleRoll;
	float SkirmishOptPositionX;
	float SkirmishOptPositionY;
	float SkirmishOptPositionZ;
};

ViewConfigurationSettings GetShipViewConfigurationSettings(int modelIndex)
{
	const char* xwaMissionFileName = (const char*)0x06002E8;

	const std::string mission = GetStringWithoutExtension(xwaMissionFileName);
	std::vector<std::string> lines = GetFileLines(mission + "_Objects.txt");

	if (!lines.size())
	{
		lines = GetFileLines(mission + ".ini", "Objects");
	}

	if (!lines.size())
	{
		lines = GetFileLines("FlightModels\\Objects.txt");
	}

	if (!lines.size())
	{
		lines = GetFileLines("FlightModels\\default.ini", "Objects");
	}

	std::string shipPath = g_flightModelsList.GetLstLine(modelIndex);

	const std::string objectValue = GetFileKeyValue(lines, shipPath + ".opt");

	if (!objectValue.empty() && std::ifstream(objectValue))
	{
		shipPath = GetStringWithoutExtension(objectValue);
	}

	ViewConfigurationSettings settings{};

	lines = GetFileLines(shipPath + "ViewConfiguration.txt");

	if (!lines.size())
	{
		lines = GetFileLines(shipPath + ".ini", "ViewConfiguration");
	}

	settings.SkirmishOptScale = GetFileKeyValueFloat(lines, "SkirmishOptScale", g_config.SkirmishOptScale);
	settings.SkirmishOptAnglePitch = GetFileKeyValueFloat(lines, "SkirmishOptAnglePitch", g_config.SkirmishOptAnglePitch);
	settings.SkirmishOptAngleYaw = GetFileKeyValueFloat(lines, "SkirmishOptAngleYaw", g_config.SkirmishOptAngleYaw);
	settings.SkirmishOptAngleRoll = GetFileKeyValueFloat(lines, "SkirmishOptAngleRoll", g_config.SkirmishOptAngleRoll);
	settings.SkirmishOptPositionX = GetFileKeyValueFloat(lines, "SkirmishOptPositionX", g_config.SkirmishOptPositionX);
	settings.SkirmishOptPositionY = GetFileKeyValueFloat(lines, "SkirmishOptPositionY", g_config.SkirmishOptPositionY);
	settings.SkirmishOptPositionZ = GetFileKeyValueFloat(lines, "SkirmishOptPositionZ", g_config.SkirmishOptPositionZ);

	return settings;
}

class ModelIndexSettings
{
public:
	ViewConfigurationSettings GetViewConfiguration(int modelIndex)
	{
		this->Update();

		auto it = this->_viewConfiguration.find(modelIndex);

		if (it != this->_viewConfiguration.end())
		{
			return it->second;
		}
		else
		{
			ViewConfigurationSettings value = GetShipViewConfigurationSettings(modelIndex);
			this->_viewConfiguration.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

private:
	void Update()
	{
		static std::string _mission;
		static int _missionIndex = 0;

		const char* xwaMissionFileName = (const char*)0x06002E8;
		const int missionFileNameIndex = *(int*)0x06002E4;

		if (missionFileNameIndex == 0 ? (_mission != xwaMissionFileName) : (_missionIndex != missionFileNameIndex))
		{
			_mission = xwaMissionFileName;
			_missionIndex = missionFileNameIndex;

			this->_viewConfiguration.clear();
		}
	}

	std::map<int, ViewConfigurationSettings> _viewConfiguration;
};

ModelIndexSettings g_modelIndexSettings;

static std::map<int, ID2D1Bitmap*> g_videoBitmaps;
static std::map<void*, ID2D1Bitmap*> g_dataBitmaps;

const int g_ratioNumerator = 16;
const int g_ratioDenominator = 9;
//const int g_ratioNumerator = 15;
//const int g_ratioDenominator = 10;
static int g_screenWidth = 0;

int MultiplyDivOffsetX(SurfaceDC& dc)
{
	int offsetX = (g_screenWidth - dc.width) / 2;

	bool isBackgroundHD = g_netFunctions._frontResIsBackgroundHD() != 0;

	if (!isBackgroundHD)
	{
		offsetX += dc.width / 2 - (dc.displayWidth * dc.height + dc.displayHeight / 2) / dc.displayHeight / 2;
	}

	return offsetX;
}

int MultiplyDivOffsetXWide(SurfaceDC& dc)
{
	int offsetX = (g_screenWidth - dc.width) / 2;

	bool isWide = g_netFunctions._frontResIsBackgroundWide() != 0;

	if (!isWide)
	{
		offsetX += dc.width / 2 - (dc.displayWidth * dc.height + dc.displayHeight / 2) / dc.displayHeight / 2;
	}

	return offsetX;
}

int MultiplyDivH(int value, bool isWide, SurfaceDC& dc)
{
	if (isWide)
	{
		value = (value * dc.width + dc.displayWidth / 2) / dc.displayWidth;
	}
	else
	{
		value = (value * dc.height + dc.displayHeight / 2) / dc.displayHeight;
	}

	return value;
}

int MultiplyDivHReverse(int value, bool isWide, SurfaceDC& dc)
{
	if (isWide)
	{
		value = (value * dc.displayWidth + dc.width / 2) / dc.width;
	}
	else
	{
		value = (value * dc.displayHeight + dc.height / 2) / dc.height;
	}

	return value;
}

int MultiplyDivV(int value, SurfaceDC& dc)
{
	value = (value * dc.height + dc.displayHeight / 2) / dc.displayHeight;
	return value;
}

static ID2D1Effect* g_d2dBitmapEffect = nullptr;

void InitEffects(ID2D1RenderTarget* d2d1RenderTarget)
{
	if (g_d2dBitmapEffect)
	{
		return;
	}

	HRESULT hr;

	ComPtr<ID2D1Factory> factory;
	d2d1RenderTarget->GetFactory(&factory);

	ComPtr<ID2D1Factory1> factory1;
	hr = factory->QueryInterface(&factory1);
	//OutputDebugString((__FUNCTION__ " ID2D1Factory1: " + std::to_string(SUCCEEDED(hr))).c_str());

	ComPtr<ID2D1DeviceContext> context;
	hr = d2d1RenderTarget->QueryInterface(&context);
	//OutputDebugString((__FUNCTION__ " ID2D1DeviceContext: " + std::to_string(SUCCEEDED(hr))).c_str());

	hr = BitmapEffect::Register(factory1);
	//OutputDebugString((__FUNCTION__ " BitmapEffect::Register: " + std::to_string(hr)).c_str());

	hr = context->CreateEffect(CLSID_BitmapEffect, &g_d2dBitmapEffect);
	//OutputDebugString((__FUNCTION__ " ID2D1Effect: " + std::to_string(SUCCEEDED(hr))).c_str());
}

void CleanEffects()
{
	//OutputDebugString(__FUNCTION__);

	//if (g_d2dBitmapEffect)
	//{
	//	g_d2dBitmapEffect->Release();
	//	g_d2dBitmapEffect = nullptr;
	//}
}

ID2D1Bitmap* CreateBitmap(
	SurfaceDC* dc,
	void* data,
	int dataWidth,
	int dataHeight,
	int imageFormat)
{
	ID2D1Bitmap* dataBitmap;

	if (imageFormat == 28)
	{
		// BC3

		int width = (dataWidth + 3) / 4 * 4;
		int height = (dataHeight + 3) / 4 * 4;

		dc->d2d1RenderTarget->CreateBitmap(
			D2D1::SizeU(width, height),
			data,
			width * 4,
			D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_BC3_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
			&dataBitmap);
	}
	else
	{
		// 32 bpp

		dc->d2d1RenderTarget->CreateBitmap(
			D2D1::SizeU(dataWidth, dataHeight),
			data,
			dataWidth * 4,
			D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
			&dataBitmap);
	}

	return dataBitmap;
}

void UpdateBitmap(
	ID2D1Bitmap* bitmap,
	void* data,
	int dataWidth,
	int dataHeight,
	int imageFormat)
{
	if (imageFormat == 28)
	{
		// BC3

		int width = (dataWidth + 3) / 4 * 4;
		int height = (dataHeight + 3) / 4 * 4;

		bitmap->CopyFromMemory(nullptr, data, width * 4);
	}
	else
	{
		// 32 bpp

		bitmap->CopyFromMemory(nullptr, data, dataWidth * 4);
	}
}

void DrawSurfaceDelegate(
	SurfaceDC* dc,
	void* data,
	int dataWidth,
	int dataHeight,
	int destinationX,
	int destinationY,
	int destinationWidth,
	int destinationHeight,
	int srcLeft,
	int srcTop,
	int srcWidth,
	int srcHeight,
	int rotation,
	unsigned int blendColorParameter,
	int imageFormat)
{
	InitEffects(dc->d2d1RenderTarget);

	if (!g_d2dBitmapEffect)
	{
		return;
	}

	ComPtr<ID2D1DeviceContext> context;
	dc->d2d1RenderTarget->QueryInterface(&context);

	ID2D1Bitmap* dataBitmap;

	if (rotation == -10)
	{
		//dataBitmap = CreateBitmap(dc, data, dataWidth, dataHeight, imageFormat);

		int key = dataHeight * 32768 + dataWidth;
		auto it = g_videoBitmaps.find(key);

		if (it != g_videoBitmaps.end())
		{
			dataBitmap = it->second;
			UpdateBitmap(dataBitmap, data, dataWidth, dataHeight, imageFormat);
		}
		else
		{
			dataBitmap = CreateBitmap(dc, data, dataWidth, dataHeight, imageFormat);

			g_videoBitmaps.insert(std::make_pair(key, dataBitmap));
		}
	}
	else
	{
		auto it = g_dataBitmaps.find(data);

		if (it != g_dataBitmaps.end())
		{
			dataBitmap = it->second;
		}
		else
		{
			dataBitmap = CreateBitmap(dc, data, dataWidth, dataHeight, imageFormat);

			g_dataBitmaps.insert(std::make_pair(data, dataBitmap));
		}
	}

	int offsetX = (g_screenWidth - dc->width) / 2;

	dc->d2d1RenderTarget->BeginDraw();

	D2D1_POINT_2F destinationPoint = D2D1::Point2F(offsetX + destinationX, destinationY);

	D2D1_MATRIX_3X2_F rotationMatrix;

	int deltaX = 0;
	int deltaY = 0;

	switch (rotation)
	{
	case 1:
		rotationMatrix = D2D1::Matrix3x2F::Rotation(-90.0f, destinationPoint);
		deltaX = 0;
		deltaY = destinationWidth - destinationWidth / 8;
		break;

	case 2:
		rotationMatrix = D2D1::Matrix3x2F::Rotation(-180.0f, destinationPoint);
		deltaX = destinationWidth - destinationWidth / 8;
		deltaY = destinationHeight;
		break;

	case 3:
		rotationMatrix = D2D1::Matrix3x2F::Rotation(-270.0f, destinationPoint);
		deltaX = destinationHeight;
		deltaY = -destinationWidth / 8;
		break;

	case 4:
		rotationMatrix = D2D1::Matrix3x2F::Scale(-1.0f, 1.0f, destinationPoint);
		deltaX = destinationWidth - destinationWidth / 8;
		deltaY = 0;
		break;

	case 5:
		rotationMatrix = D2D1::IdentityMatrix();
		deltaX = -destinationWidth / 8;
		deltaY = 0;
		break;

	default:
		rotationMatrix = D2D1::IdentityMatrix();
	}

	bool isBackgroundHD = g_netFunctions._frontResIsBackgroundHD() != 0;
	bool isBackgroundWide = g_netFunctions._frontResIsBackgroundWide() != 0;

	if ((rotation != -1 || !isBackgroundHD) && !isBackgroundWide)
	{
		int offsetLeft = offsetX + (dc->width - (dc->displayWidth * dc->height + dc->displayHeight / 2) / dc->displayHeight) / 2;
		int offsetRight = offsetX + (dc->width + (dc->displayWidth * dc->height + dc->displayHeight / 2) / dc->displayHeight) / 2;

		dc->d2d1RenderTarget->PushAxisAlignedClip(D2D1::RectF(offsetLeft, 0, offsetRight, dc->height), D2D1_ANTIALIAS_MODE_ALIASED);
	}
	else
	{
		dc->d2d1RenderTarget->PushAxisAlignedClip(D2D1::RectF(offsetX, 0, offsetX + dc->width, dc->height), D2D1_ANTIALIAS_MODE_ALIASED);
	}

	float scaleX = (float)destinationWidth / (float)srcWidth;
	float scaleY = (float)destinationHeight / (float)srcHeight;

	D2D1_MATRIX_3X2_F scaleMatrix = D2D1::Matrix3x2F::Scale(scaleX, scaleY, destinationPoint);
	scaleMatrix.dx += deltaX;
	scaleMatrix.dy += deltaY;

	g_d2dBitmapEffect->SetInput(0, dataBitmap, 1);
	g_d2dBitmapEffect->SetValue(0, blendColorParameter);

	context->SetTransform(rotationMatrix * scaleMatrix);

	context->DrawImage(
		g_d2dBitmapEffect,
		D2D1::Point2F(offsetX + destinationX, destinationY),
		D2D1::RectF(srcLeft, srcTop, srcLeft + srcWidth, srcTop + srcHeight));

	dc->d2d1RenderTarget->PopAxisAlignedClip();
	dc->d2d1RenderTarget->SetTransform(D2D1::IdentityMatrix());
	dc->d2d1RenderTarget->EndDraw();

	if (rotation == -10)
	{
		//dataBitmap->Release();
	}
}

bool HasSurfaceDC()
{
	LPDIRECTDRAWSURFACE BackSurface = *(LPDIRECTDRAWSURFACE*)(0x09F60E0 + 0x0F5A);
	HRESULT hr = BackSurface->GetDC(nullptr);
	return SUCCEEDED(hr);
}

bool GetSurfaceDC(SurfaceDC* dc)
{
	*dc = {};

	LPDIRECTDRAWSURFACE BackSurface = *(LPDIRECTDRAWSURFACE*)(0x09F60E0 + 0x0F5A);
	HRESULT hr = BackSurface->GetDC((HDC*)dc);

	if (SUCCEEDED(hr))
	{
		*dc->callback = (void(*)())DrawSurfaceDelegate;
	}

	dc->displayWidth = 640;
	dc->displayHeight = 480;

	g_screenWidth = dc->width;
	dc->width = min(dc->width, (dc->height * g_ratioNumerator + g_ratioDenominator / 2) / g_ratioDenominator);

	dc->ScreenAreaLeft = *(int*)(0x09F60E0 + 0x0FAA);
	dc->ScreenAreaRight = *(int*)(0x09F60E0 + 0x0FAE);
	dc->ScreenAreaTop = *(int*)(0x09F60E0 + 0x0FB2);
	dc->ScreenAreaBottom = *(int*)(0x09F60E0 + 0x0FB6);

	return SUCCEEDED(hr);
}

const CbmHeader* GetCbmHeaderByName(const char* name)
{
	const auto XwaFrontResIndexOf = (int(*)(const char*))0x00537540;

	int resIndex = XwaFrontResIndexOf(name);

	if (resIndex == -1)
	{
		return nullptr;
	}

	const FrontResItem* FrontResItems = *(FrontResItem**)(0x09F60E0 + 0x0501);
	return FrontResItems[resIndex].CbmFile;
}

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

void CheckHResult(const char* step, HRESULT hr)
{
	if (FAILED(hr))
	{
		static bool messageShown = false;

		if (!messageShown)
		{
			char text[512];
			strcpy_s(text, step);
			strcat_s(text, "\n");
			strcat_s(text, _com_error(hr).ErrorMessage());

			MessageBox(nullptr, text, __FUNCTION__, MB_ICONERROR);
		}

		messageShown = true;
	}
}

class FontResources
{
public:
	FontResources();
	void Load();
	void Release();

	IDWriteFactory* GetDWriteFactory()
	{
		return _dwriteFactory;
	}

	IDWriteTextFormat* GetTextFormat(float fontSize);
	int GetTextFormatLineHeight(float fontSize);
	bool IsPlainText(const char* str);

private:
	bool _loaded;
	IDWriteFactory* _dwriteFactory;
	std::map<float, IDWriteTextFormat*> _textFormats;
	std::map<float, int> _textFormatsLineHeight;
};

FontResources::FontResources()
{
	_loaded = false;
	_dwriteFactory = nullptr;
}

void FontResources::Load()
{
	if (_loaded)
	{
		return;
	}

	_loaded = true;

	HRESULT hr = S_OK;

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(_dwriteFactory), (IUnknown**)&_dwriteFactory);
	CheckHResult("DWriteCreateFactory", hr);
}

void FontResources::Release()
{
	for (auto& format : _textFormats)
	{
		if (format.second)
		{
			format.second->Release();
			format.second = nullptr;
		}
	}

	_textFormats.clear();
	_textFormatsLineHeight.clear();

	if (_dwriteFactory)
	{
		_dwriteFactory->Release();
		_dwriteFactory = nullptr;
	}

	_loaded = false;
}

IDWriteTextFormat* FontResources::GetTextFormat(float fontSize)
{
	auto it = _textFormats.find(fontSize);

	if (it != this->_textFormats.end())
	{
		return it->second;
	}

	std::wstring wfont = string_towstring(g_config.HDConcourseTextFont.c_str());

	IDWriteTextFormat* textFormat;
	HRESULT hr = _dwriteFactory->CreateTextFormat(
		wfont.c_str(),
		nullptr,
		DWRITE_FONT_WEIGHT_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fontSize,
		L"en-US",
		&textFormat);
	CheckHResult("CreateTextFormat", hr);

	_textFormats.insert(std::make_pair(fontSize, textFormat));
	return textFormat;
}

int FontResources::GetTextFormatLineHeight(float fontSize)
{
	auto it = _textFormatsLineHeight.find(fontSize);

	if (it != this->_textFormatsLineHeight.end())
	{
		return it->second;
	}

	IDWriteTextFormat* textFormat = GetTextFormat(fontSize);

	ComPtr<IDWriteTextLayout> textLayout;
	HRESULT hr = _dwriteFactory->CreateTextLayout(L"", 0, textFormat, 100, 100, &textLayout);
	CheckHResult("CreateTextLayout", hr);

	DWRITE_TEXT_METRICS m{};
	textLayout->GetMetrics(&m);

	int lineHeight = (int)m.height;

	_textFormatsLineHeight.insert(std::make_pair(fontSize, lineHeight));
	return lineHeight;
}

bool FontResources::IsPlainText(const char* str)
{
	bool isPlain = true;

	for (const char* s = str; *s != 0; s++)
	{
		unsigned char c = *s;

		if (c < 32)
		{
			isPlain = false;
			break;
		}
	}

	return isPlain;
}

FontResources g_fontResources;

int GetMissionFileNameById(int missionId)
{
	const int s_XwaMissionDescriptionsCount = *(int*)0x009F5EC0;
	const XwaMissionDescription* s_XwaMissionDescriptions = *(XwaMissionDescription**)0x009F4B98;

	int index = 0;

	for (; index < s_XwaMissionDescriptionsCount; index++)
	{
		if (s_XwaMissionDescriptions[index].Id == missionId)
		{
			break;
		}
	}

	if (index >= s_XwaMissionDescriptionsCount)
	{
		index = -1;
	}

	return index;
}

std::vector<std::string> g_MedalDetails;
std::vector<int> g_MedalDetailsOrder;
std::vector<std::string> g_FamilyMedalsDescriptions;

void LoadMedalDetails()
{
	static bool _loaded = false;

	if (_loaded)
	{
		return;
	}

	if (std::ifstream("MedalDetailList.txt"))
	{
		const auto lines = GetFileLines("MedalDetailList.txt");

		for (const std::string& line : lines)
		{
			if (line == "null")
			{
				g_MedalDetails.push_back("");
			}
			else
			{
				g_MedalDetails.push_back(line);
			}
		}
	}
	else
	{
		for (int index = 0; index < 53; index++)
		{
			const char* filename = ((const char**)0x006042C8)[index];
			g_MedalDetails.push_back(filename);
		}
	}

	_loaded = true;
}

void LoadMedalDetailsOrder()
{
	static bool _loaded = false;

	if (_loaded)
	{
		return;
	}

	if (std::ifstream("MedalDetailListOrder.txt"))
	{
		const auto lines = GetFileLines("MedalDetailListOrder.txt");

		g_MedalDetailsOrder = GetFileListIntValues(lines);
	}
	else
	{
		for (int index = 0; index < 53; index++)
		{
			g_MedalDetailsOrder.push_back(index);
		}
	}

	_loaded = true;
}

void LoadFamilyMedalsDescriptions()
{
	static bool _loaded = false;

	if (_loaded)
	{
		return;
	}

	std::ifstream file("FamilyMedalsDescriptions.txt");

	if (file)
	{
		std::string line;

		while (std::getline(file, line))
		{
			if (line.empty())
			{
				g_FamilyMedalsDescriptions.push_back("Unknown");
			}
			else
			{
				g_FamilyMedalsDescriptions.push_back(line);
			}
		}
	}

	_loaded = true;
}

int MedalDetailExistHook(int* params)
{
	LoadMedalDetails();

	const int index = params[0];

	params[0] = index < (int)g_MedalDetails.size() && !g_MedalDetails[index].empty() ? 1 : 0;
	return 0;
}

int MedalDetailFilenameHook(int* params)
{
	static const char* s_MedalDetailFilenameEmpty = "";

	LoadMedalDetails();

	const int index = params[0];

	params[0] = index < (int)g_MedalDetails.size() ? (int)g_MedalDetails[index].c_str() : (int)s_MedalDetailFilenameEmpty;
	return 0;
}

int MedalDetailDrawHook(int* params)
{
	LoadMedalDetailsOrder();

	const auto XwaFrontResDraw = (int(*)(const char*, int, int))0x00534A60;

	const int s_XwaMissionDescriptionsCount = *(int*)0x009F5EC0;
	const XwaMissionDescription* s_XwaMissionDescriptions = *(XwaMissionDescription**)0x009F4B98;

	for (int orderIndex = 0; orderIndex < g_MedalDetailsOrder.size(); orderIndex++)
	{
		int missionIndex = g_MedalDetailsOrder[orderIndex];

		if (missionIndex < 0 || missionIndex >= s_XwaMissionDescriptionsCount)
		{
			continue;
		}

		int id = s_XwaMissionDescriptions[missionIndex].Id;

		if (*(int*)(0x00AED76E + id * 0x30) == 0x01)
		{
			char esp20[20];
			sprintf_s(esp20, "medal%d", id);

			XwaFrontResDraw(esp20, 0, 0);
		}
	}

	params[Params_ReturnAddress] = 0x005647AF;

	return 0;
}

int MedalDetailInAreaHook(int* params)
{
	const auto XwaIsPointInRect = (int(*)(const RECT*, int, int))0x005592E0;

	const int result = params[Params_EAX];
	const int cursorPositionX = params[6];
	const int cursorPositionY = params[5];
	const RECT* area = (RECT*)(params + 8);

	if (result == 0)
	{
		if (XwaIsPointInRect(area, cursorPositionX, cursorPositionY))
		{
			params[Params_ReturnAddress] = 0x00563C5B;
		}
		else
		{
			params[Params_ReturnAddress] = 0x00563C3F;
		}
	}

	return 0;
}

int MedalDetailSetAreaHook(int* params)
{
	const auto SetupResourceData = (int(*)(short, short))0x004CDED0;

	const short A4 = (short)params[0];
	const short A8 = (short)params[1];
	int* pDest = (int*)params[3];

	int data = SetupResourceData(A4, A8);

	if (data == 0)
	{
		pDest[0] = 0x7fffffff;
		pDest[1] = 0x7fffffff;
		pDest[2] = 0;
		pDest[3] = 0;
	}

	return data;
}

int MedalDetailLoadImagesHook(int* params)
{
	const auto XwaLoadDatImage = (int(*)(const char*, short, short, short))0x0055E800;

	char key[20];

	for (int index = 0; index < 256; index++)
	{
		sprintf_s(key, "medal%d", index);

		if (index < 100)
		{
			XwaLoadDatImage(key, 15900, 100 + index, 1);
		}
		else
		{
			XwaLoadDatImage(key, 15900, 2000 + index - 100, 1);
		}
	}

	params[Params_ReturnAddress] = 0x0055E671;
	return 0;
}

int FamilyMedalDescriptionHook(int* params)
{
	LoadFamilyMedalsDescriptions();

	const int index = params[0] - 0x402;

	const auto XwaGetFrontTxtString = (const char* (*)(int))0x0055CB50;

	const char* description;

	if (index < (int)g_FamilyMedalsDescriptions.size())
	{
		description = g_FamilyMedalsDescriptions[index].c_str();
	}
	else if (index < 50)
	{
		description = XwaGetFrontTxtString(0x402 + index);
	}
	else
	{
		description = "Unknown";
	}

	return (int)description;
}

std::queue<std::string> g_movieNames;
std::vector<std::pair<int, std::string>> g_CustomMoviesOnPoints;
std::vector<int> g_KalidorCrescentPoints;
std::vector<std::string> g_KalidorCrescentDescriptions;

void LoadCustomPoints()
{
	static bool _loaded = false;

	if (_loaded)
	{
		return;
	}

	if (std::ifstream("CustomMoviesOnPointsList.txt"))
	{
		const auto lines = GetFileLines("CustomMoviesOnPointsList.txt");

		for (const std::string& line : lines)
		{
			if (line.empty())
			{
				continue;
			}

			const auto tokens = Tokennize(line);

			if (tokens.size() < 2)
			{
				continue;
			}

			int points = std::stoi(tokens[0], 0, 0);
			std::string name = tokens[1];

			g_CustomMoviesOnPoints.push_back(std::make_pair(points, name));
		}
	}

	if (std::ifstream("KalidorCrescentPoints.txt"))
	{
		const auto lines = GetFileLines("KalidorCrescentPoints.txt");

		g_KalidorCrescentPoints = GetFileListIntValues(lines);
	}
	else
	{
		const int* s_XwaKalidorCrescentPoints = (int*)0x005B30A8;

		for (int i = 0; i < 6; i++)
		{
			int points = s_XwaKalidorCrescentPoints[i] / 10;
			g_KalidorCrescentPoints.push_back(points);
		}
	}

	std::ifstream file("KalidorCrescentDescriptions.txt");

	if (file)
	{
		std::string line;

		while (std::getline(file, line))
		{
			if (line.empty())
			{
				g_KalidorCrescentDescriptions.push_back("Unknown");
			}
			else
			{
				g_KalidorCrescentDescriptions.push_back(line);
			}
		}
	}

	_loaded = true;
}

std::vector<std::string> g_pilotRatingMovies;
std::vector<std::string> g_pilotRankMovies;

void LoadPilotCustomMovies()
{
	static bool _loaded = false;

	if (_loaded)
	{
		return;
	}

	g_pilotRatingMovies = GetFileLines("PilotRatingCustomMovies.txt");
	g_pilotRankMovies = GetFileLines("RankCustomMovies.txt");

	_loaded = true;
}

int CustomMoviesOnPoints1Hook(int* params)
{
	LoadPilotCustomMovies();

	const int a4 = params[0];
	const int a8 = params[1];

	const auto L0042E750 = (int(*)(int, int))0x0042E750;

	int& XwaPilotData_PilotRating = *(int*)(0x0AE2A60 + 0x10E9E);
	int& XwaPilotData_Rank = *(int*)(0x0AE2A60 + 0x10EA2);

	int currentPilotRating = XwaPilotData_PilotRating;
	int currentRank = XwaPilotData_Rank;

	const char* xwaMissionFileName = (const char*)0x06002E8;
	const int missionFileNameIndex = *(int*)0x06002E4;

	bool isCombatMission = _strnicmp(xwaMissionFileName, "combat", sizeof("combat") - 1) == 0;
	int& campaignMode = *(int*)(0x00AE2A60 + 0x250F3);

	if (isCombatMission)
	{
		int currentCampaignMode = campaignMode;
		campaignMode = 1;
		L0042E750(a4, a8);
		campaignMode = currentCampaignMode;
	}
	else
	{
		L0042E750(a4, a8);
	}

	if (XwaPilotData_PilotRating != currentPilotRating)
	{
		if (XwaPilotData_PilotRating < g_pilotRatingMovies.size())
		{
			for (int i = currentPilotRating + 1; i <= XwaPilotData_PilotRating; i++)
			{
				g_movieNames.push(g_pilotRatingMovies[i]);
				if (g_config.PlayMovieOutputDebugString)
				{
					OutputDebugString(("push pilot rating movie " + g_movieNames.back()).c_str());
				}
			}
		}

		currentPilotRating = XwaPilotData_PilotRating;
	}

	if (currentRank != XwaPilotData_Rank)
	{
		if (XwaPilotData_Rank < g_pilotRankMovies.size())
		{
			for (int i = currentRank + 1; i <= XwaPilotData_Rank; i++)
			{
				g_movieNames.push(g_pilotRankMovies[i]);

				if (g_config.PlayMovieOutputDebugString)
				{
					OutputDebugString(("push pilot rank movie " + g_movieNames.back()).c_str());
				}
			}
		}

		currentRank = XwaPilotData_Rank;
	}

	//g_movieNames.push("TECHROOM");

	return 0;
}

int CustomMoviesOnPointsPlayHook(int* params)
{
	const int A4 = params[0];

	const auto L00558100 = (int(*)(int))0x00558100;
	const auto XwaPlayMovie = (int(*)(const char*, int))0x0055BC20;

	const int s_V0x0ABD1E4 = *(int*)0x0ABD1E4;

	L00558100(s_V0x0ABD1E4);

	std::vector<std::string> movies;

	for (; !g_movieNames.empty(); g_movieNames.pop())
	{
		const std::string& name = g_movieNames.front();

		if (std::find(movies.begin(), movies.end(), name) != movies.end())
		{
			continue;
		}

		if (g_config.PlayMovieOutputDebugString)
		{
			OutputDebugString(("play movie " + name).c_str());
		}

		movies.push_back(name);
		XwaPlayMovie(name.c_str(), 0);
	}

	return 0;
}

int CustomPointsCheckHook(int* params)
{
	LoadCustomPoints();

	const int points = params[0] / 10;

	int& XwaPilotData_m10EA6 = *(int*)0x00AF3906;
	int& XwaPilotData_m10F3A = *(int*)0x00AF399A;
	int& XwaPilotData_m00012 = *(int*)0x00AE2A72;

	if (XwaPilotData_m10EA6 < (int)g_KalidorCrescentPoints.size())
	{
		if (points >= g_KalidorCrescentPoints[XwaPilotData_m10EA6])
		{
			XwaPilotData_m10EA6++;
			XwaPilotData_m10F3A = XwaPilotData_m10EA6;
		}
	}

	while (XwaPilotData_m00012 < (int)g_CustomMoviesOnPoints.size() && points >= g_CustomMoviesOnPoints[XwaPilotData_m00012].first)
	{
		g_movieNames.push(g_CustomMoviesOnPoints[XwaPilotData_m00012].second);

		if (g_config.PlayMovieOutputDebugString)
		{
			OutputDebugString(("push custom points movie " + g_movieNames.back()).c_str());
		}

		XwaPilotData_m00012++;
	}

	return 0;
}

int ConcourseMultiplyDivRound(int value)
{
	return value > 8000 ? (value - 8000 + 5) / 10 : value;
}

class ConcourseDoors
{
public:
	ConcourseDoors()
	{
		auto lines = GetFileLines("ConcourseDoors.txt");

		this->planet_position_left = GetFileKeyValueInt(lines, "planet_position_left", 0);
		this->planet_position_right = GetFileKeyValueInt(lines, "planet_position_right", 640);
		this->planet_position_top = GetFileKeyValueInt(lines, "planet_position_top", 55);
		this->planet_position_bottom = GetFileKeyValueInt(lines, "planet_position_bottom", 125);
		this->tourdoor_posX = GetFileKeyValueInt(lines, "tourdoor_posX", 0xE7);
		this->tourdoor_posY = GetFileKeyValueInt(lines, "tourdoor_posY", 0xAE);
		this->mkanim_posX = GetFileKeyValueInt(lines, "mkanim_posX", 0x6E);
		this->mkanim_posY = GetFileKeyValueInt(lines, "mkanim_posY", 0x166);
		this->traindoor_posX = GetFileKeyValueInt(lines, "traindoor_posX", 0x218);
		this->traindoor_posY = GetFileKeyValueInt(lines, "traindoor_posY", 0xAE);
		this->combatdoor_poxX = GetFileKeyValueInt(lines, "combatdoor_poxX", 0x23);
		this->combatdoor_poxY = GetFileKeyValueInt(lines, "combatdoor_poxY", 0xAE);
		this->filmdoor_posX = GetFileKeyValueInt(lines, "filmdoor_posX", 0x218);
		this->filmdoor_posY = GetFileKeyValueInt(lines, "filmdoor_posY", 0x11E);
		this->techdoor_posX = GetFileKeyValueInt(lines, "techdoor_posX", 0x1AA);
		this->techdoor_posY = GetFileKeyValueInt(lines, "techdoor_posY", 0xDB);
		this->techdoor_posWidth = GetFileKeyValueInt(lines, "techdoor_posWidth", 92);
		this->techdoor_posHeight = GetFileKeyValueInt(lines, "techdoor_posHeight", 93);
		this->multidoor_posX = GetFileKeyValueInt(lines, "multidoor_posX", 0x18);
		this->multidoor_posY = GetFileKeyValueInt(lines, "multidoor_posY", 0x8C);
		this->multidoor_posSelectionX = GetFileKeyValueInt(lines, "multidoor_posSelectionX", 0x24);
		this->multidoor_posSelectionY = GetFileKeyValueInt(lines, "multidoor_posSelectionY", 0x94);
		this->multidoor_posSelectionWidth = GetFileKeyValueInt(lines, "multidoor_posSelectionWidth", 0x11A);
		this->multidoor_posSelectionHeight = GetFileKeyValueInt(lines, "multidoor_posSelectionHeight", 0x6C);
		this->singledoor_posX = GetFileKeyValueInt(lines, "singledoor_posX", 0x175);
		this->singledoor_posY = GetFileKeyValueInt(lines, "singledoor_posY", 0x139);
		this->singledoor_posSelectionX = GetFileKeyValueInt(lines, "singledoor_posSelectionX", 0x180);
		this->singledoor_posSelectionY = GetFileKeyValueInt(lines, "singledoor_posSelectionY", 0xFC);
		this->singledoor_posSelectionWidth = GetFileKeyValueInt(lines, "singledoor_posSelectionWidth", 0xF6);
		this->singledoor_posSelectionHeight = GetFileKeyValueInt(lines, "singledoor_posSelectionHeight", 0xAC);
		this->combatbackdoor_posX = GetFileKeyValueInt(lines, "combatbackdoor_posX", 0x12B);
		this->combatbackdoor_posY = GetFileKeyValueInt(lines, "combatbackdoor_posY", 0x5A);

		this->familyroom_backtoconcourse_posX = GetFileKeyValueInt(lines, "familyroom_backtoconcourse_posX", 0);
		this->familyroom_backtoconcourse_posY = GetFileKeyValueInt(lines, "familyroom_backtoconcourse_posY", 150);
		this->familyroom_backtoconcourse_posWidth = GetFileKeyValueInt(lines, "familyroom_backtoconcourse_posWidth", 120);
		this->familyroom_backtoconcourse_posHeight = GetFileKeyValueInt(lines, "familyroom_backtoconcourse_posHeight", 137);
		this->familyroom_statistics_posX = GetFileKeyValueInt(lines, "familyroom_statistics_posX", 403);
		this->familyroom_statistics_posY = GetFileKeyValueInt(lines, "familyroom_statistics_posY", 228);
		this->familyroom_statistics_posWidth = GetFileKeyValueInt(lines, "familyroom_statistics_posWidth", 96);
		this->familyroom_statistics_posHeight = GetFileKeyValueInt(lines, "familyroom_statistics_posHeight", 79);
		this->familyroom_playmission_posX = GetFileKeyValueInt(lines, "familyroom_playmission_posX", 103);
		this->familyroom_playmission_posY = GetFileKeyValueInt(lines, "familyroom_playmission_posY", 288);
		this->familyroom_playmission_posWidth = GetFileKeyValueInt(lines, "familyroom_playmission_posWidth", 131);
		this->familyroom_playmission_posHeight = GetFileKeyValueInt(lines, "familyroom_playmission_posHeight", 128);
		this->familyroom_mkeye_posX = GetFileKeyValueInt(lines, "familyroom_mkeye_posX", 160);
		this->familyroom_mkeye_posY = GetFileKeyValueInt(lines, "familyroom_mkeye_posY", 200);
		this->familyroom_monitor_posX = GetFileKeyValueInt(lines, "familyroom_monitor_posX", 426);
		this->familyroom_monitor_posY = GetFileKeyValueInt(lines, "familyroom_monitor_posY", 243);
		this->familyroom_monitor_fps = GetFileKeyValueInt(lines, "familyroom_monitor_fps", 6);

		if (this->familyroom_monitor_fps < 1)
		{
			this->familyroom_monitor_fps = 1;
		}
		else if (this->familyroom_monitor_fps > 24)
		{
			this->familyroom_monitor_fps = 24;
		}

		this->familyroom_ladyblue_mission = GetFileKeyValueInt(lines, "familyroom_ladyblue_mission", 23);

		if (this->familyroom_ladyblue_mission < 0 || this->familyroom_ladyblue_mission >= 255)
		{
			this->familyroom_ladyblue_mission = 0;
		}

		this->familyroom_cologne_mission = GetFileKeyValueInt(lines, "familyroom_cologne_mission", 9);

		if (this->familyroom_cologne_mission < 0 || this->familyroom_cologne_mission >= 255)
		{
			this->familyroom_cologne_mission = 0;
		}

		this->familyroom_rating_left = GetFileKeyValueInt(lines, "familyroom_rating_left", 0);
		this->familyroom_rating_top = GetFileKeyValueInt(lines, "familyroom_rating_top", 0);
		this->familyroom_rating_width = GetFileKeyValueInt(lines, "familyroom_rating_width", 0);
		this->familyroom_rating_height = GetFileKeyValueInt(lines, "familyroom_rating_height", 0);

		this->briefing_review_item_left = GetFileKeyValueInt(lines, "briefing_review_item_left", 64);
		this->briefing_review_item_right = GetFileKeyValueInt(lines, "briefing_review_item_right", 573);
		this->briefing_review_item_height = GetFileKeyValueInt(lines, "briefing_review_item_height", 41);

		this->mission_selection_screen_posX = GetFileKeyValueInt(lines, "mission_selection_screen_posX", 230);

		this->battlezoom_image1_posX = GetFileKeyValueInt(lines, "battlezoom_image1_posX", 320);
		this->battlezoom_image1_posY = GetFileKeyValueInt(lines, "battlezoom_image1_posY", 240);
		this->battlezoom_text1_posX = GetFileKeyValueInt(lines, "battlezoom_text1_posX", 300);
		this->battlezoom_text1_posY = GetFileKeyValueInt(lines, "battlezoom_text1_posY", 340);
		this->kalidorzoom_image1_posX = GetFileKeyValueInt(lines, "kalidorzoom_image1_posX", 320);
		this->kalidorzoom_image1_posY = GetFileKeyValueInt(lines, "kalidorzoom_image1_posY", 240);
		this->kalidorzoom_text1_posX = GetFileKeyValueInt(lines, "kalidorzoom_text1_posX", 300);
		this->kalidorzoom_text1_posY = GetFileKeyValueInt(lines, "kalidorzoom_text1_posY", 340);
		this->battlezoom_image2_posX = GetFileKeyValueInt(lines, "battlezoom_image2_posX", 152);
		this->battlezoom_image2_posY = GetFileKeyValueInt(lines, "battlezoom_image2_posY", 360);
		this->battlezoom_text2_posX = GetFileKeyValueInt(lines, "battlezoom_text2_posX", 182);
		this->battlezoom_text2_posY = GetFileKeyValueInt(lines, "battlezoom_text2_posY", 345);
		this->kalidorzoom_image2_posX = GetFileKeyValueInt(lines, "kalidorzoom_image2_posX", 388);
		this->kalidorzoom_image2_posY = GetFileKeyValueInt(lines, "kalidorzoom_image2_posY", 120);
		this->kalidorzoom_text2_posX = GetFileKeyValueInt(lines, "kalidorzoom_text2_posX", 328);
		this->kalidorzoom_text2_posY = GetFileKeyValueInt(lines, "kalidorzoom_text2_posY", 105);
	}

	int planet_position_left;
	int planet_position_right;
	int planet_position_top;
	int planet_position_bottom;
	int tourdoor_posX;
	int tourdoor_posY;
	int mkanim_posX;
	int mkanim_posY;
	int traindoor_posX;
	int traindoor_posY;
	int combatdoor_poxX;
	int combatdoor_poxY;
	int filmdoor_posX;
	int filmdoor_posY;
	int techdoor_posX;
	int techdoor_posY;
	int techdoor_posWidth;
	int techdoor_posHeight;
	int multidoor_posX;
	int multidoor_posY;
	int multidoor_posSelectionX;
	int multidoor_posSelectionY;
	int multidoor_posSelectionWidth;
	int multidoor_posSelectionHeight;
	int singledoor_posX;
	int singledoor_posY;
	int singledoor_posSelectionX;
	int singledoor_posSelectionY;
	int singledoor_posSelectionWidth;
	int singledoor_posSelectionHeight;
	int combatbackdoor_posX;
	int combatbackdoor_posY;

	int familyroom_backtoconcourse_posX;
	int familyroom_backtoconcourse_posY;
	int familyroom_backtoconcourse_posWidth;
	int familyroom_backtoconcourse_posHeight;
	int familyroom_statistics_posX;
	int familyroom_statistics_posY;
	int familyroom_statistics_posWidth;
	int familyroom_statistics_posHeight;
	int familyroom_playmission_posX;
	int familyroom_playmission_posY;
	int familyroom_playmission_posWidth;
	int familyroom_playmission_posHeight;
	int familyroom_mkeye_posX;
	int familyroom_mkeye_posY;
	int familyroom_monitor_posX;
	int familyroom_monitor_posY;
	int familyroom_monitor_fps;
	int familyroom_ladyblue_mission;
	int familyroom_cologne_mission;
	int familyroom_rating_left;
	int familyroom_rating_top;
	int familyroom_rating_width;
	int familyroom_rating_height;

	int briefing_review_item_left;
	int briefing_review_item_right;
	int briefing_review_item_height;

	int mission_selection_screen_posX;

	int battlezoom_image1_posX;
	int battlezoom_image1_posY;
	int battlezoom_text1_posX;
	int battlezoom_text1_posY;
	int kalidorzoom_image1_posX;
	int kalidorzoom_image1_posY;
	int kalidorzoom_text1_posX;
	int kalidorzoom_text1_posY;
	int battlezoom_image2_posX;
	int battlezoom_image2_posY;
	int battlezoom_text2_posX;
	int battlezoom_text2_posY;
	int kalidorzoom_image2_posX;
	int kalidorzoom_image2_posY;
	int kalidorzoom_text2_posX;
	int kalidorzoom_text2_posY;
};

ConcourseDoors g_concourseDoors;

struct ConcourseAnimation
{
	int id;
	std::string room;
	int posX;
	int posY;
	int posWidth;
	int posHeight;
	short nameGroupId;
	std::string name;
	int delayMin;
	int delayMax;
	std::string group;
	std::string missions;
	int frame;

	bool IsRoom(const char* currentRoom) const
	{
		return _stricmp(this->room.c_str(), currentRoom) == 0;
	}

	bool IsGroup(const char* currentGroup) const
	{
		return _stricmp(this->group.c_str(), currentGroup) == 0;
	}

	bool IsInCurrentMission() const
	{
		const int currentMission = *(int*)0x00ABC970;

		std::vector<std::string> missionList = TokennizeSpaces(this->missions);

		if (missionList.empty())
		{
			return true;
		}

		for (const auto& missionEntry : missionList)
		{
			if (missionEntry == "-1")
			{
				return true;
			}
		}

		bool includeMission = false;

		for (const auto& missionEntry : missionList)
		{
			if (missionEntry[0] == '~')
			{
				continue;
			}

			includeMission = true;

			int missionIndex = std::stoi(missionEntry, 0, 0);

			if (missionIndex == currentMission)
			{
				return true;
			}
		}

		for (const auto& missionEntry : missionList)
		{
			if (missionEntry[0] != '~')
			{
				continue;
			}

			int missionIndex = std::stoi(missionEntry.substr(1), 0, 0);

			if (missionIndex == currentMission)
			{
				return false;
			}
		}

		return !includeMission;
	}
};

class ConcourseAnimations
{
public:
	ConcourseAnimations()
	{
		auto lines = GetFileLines("ConcourseAnimations.txt");
		int index = 0;

		for (const std::string& line : lines)
		{
			std::vector<std::string> values = Tokennize(line);

			if (values.size() < 8)
			{
				continue;
			}

			ConcourseAnimation animation;

			animation.id = index;
			index++;

			if (values.size() == 8 || values.size() == 9)
			{
				animation.room = values[0];
				animation.posX = std::stoi(values[1], 0, 0);
				animation.posY = std::stoi(values[2], 0, 0);
				animation.posWidth = 0;
				animation.posHeight = 0;
				animation.nameGroupId = (short)std::stoi(values[3], 0, 0);
				animation.name = "concourse_anim_" + std::to_string(animation.nameGroupId);
				animation.delayMin = std::stoi(values[4], 0, 0);
				animation.delayMax = std::stoi(values[5], 0, 0);
				animation.group = values[6];
				animation.missions = values[7];
				animation.frame = 0;
			}
			else
			{
				animation.room = values[0];
				animation.posX = std::stoi(values[1], 0, 0);
				animation.posY = std::stoi(values[2], 0, 0);
				animation.posWidth = std::stoi(values[3], 0, 0);
				animation.posHeight = std::stoi(values[4], 0, 0);
				animation.nameGroupId = (short)std::stoi(values[5], 0, 0);
				animation.name = "concourse_anim_" + std::to_string(animation.nameGroupId);
				animation.delayMin = std::stoi(values[6], 0, 0);
				animation.delayMax = std::stoi(values[7], 0, 0);
				animation.group = values[8];
				animation.missions = values[9];
				animation.frame = 0;
			}

			this->animations.push_back(animation);
		}
	}

	std::vector<std::string> GetGroups(const char* currentRoom) const
	{
		std::vector<std::string> groups;

		for (const auto& animation : this->animations)
		{
			if (!animation.IsRoom(currentRoom))
			{
				continue;
			}

			if (!animation.IsInCurrentMission())
			{
				continue;
			}

			if (std::find(groups.begin(), groups.end(), animation.group) != groups.end())
			{
				continue;
			}

			groups.push_back(animation.group);
		}

		return groups;
	}

	int GetGroupCount(const char* currentRoom, const char* currentGroup) const
	{
		int count = 0;

		for (const auto& animation : this->animations)
		{
			if (!animation.IsRoom(currentRoom))
			{
				continue;
			}

			if (!animation.IsInCurrentMission())
			{
				continue;
			}

			if (!animation.IsGroup(currentGroup))
			{
				continue;
			}

			count++;
		}

		return count;
	}

	ConcourseAnimation* GetAnimationByIndex(const char* currentRoom, const char* currentGroup, int index)
	{
		int count = 0;

		for (auto& animation : this->animations)
		{
			if (!animation.IsRoom(currentRoom))
			{
				continue;
			}

			if (!animation.IsInCurrentMission())
			{
				continue;
			}

			if (!animation.IsGroup(currentGroup))
			{
				continue;
			}

			if (count == index)
			{
				return &animation;
			}

			count++;
		}

		return nullptr;
	}

	ConcourseAnimation* GetRandomAnimation(const char* currentRoom, const char* currentGroup)
	{
		int count = this->GetGroupCount(currentRoom, currentGroup);
		int index = g_randomGenerator.Generate(0, count);
		return this->GetAnimationByIndex(currentRoom, currentGroup, index);
	}

	void SetGroupFrame(const char* currentRoom, const char* currentGroup, int currentId, int frameIndex)
	{
		for (auto& animation : this->animations)
		{
			if (!animation.IsRoom(currentRoom))
			{
				continue;
			}

			if (!animation.IsInCurrentMission())
			{
				continue;
			}

			if (!animation.IsGroup(currentGroup))
			{
				continue;
			}

			if (animation.id == currentId)
			{
				animation.frame = frameIndex + g_randomGenerator.Generate(animation.delayMin, animation.delayMax);
			}
			else
			{
				animation.frame = 0x7fffffff;
			}
		}
	}

	std::vector<ConcourseAnimation> animations;
};

ConcourseAnimations g_concourseAnimations;

int LoadGameStateHook(int* params)
{
	const auto XwaGetResDataItemImagesCount = (short(*)(short))0x004CF920;
	const auto XwaLoadDatImage = (int(*)(const char*, short, short, short))0x0055E800;

	const int frameCount = *(int*)0x00A21441;
	params[Params_EDX] = frameCount;

	if (frameCount != 0)
	{
		return 0;
	}

	for (const auto& animation : g_concourseAnimations.animations)
	{
		if (g_config.SupportDatAnimations)
		{
			short imageCount = XwaGetResDataItemImagesCount(animation.nameGroupId);
			XwaLoadDatImage(animation.name.c_str(), animation.nameGroupId, 1, imageCount);
		}

		WebmLoadVideo(animation.name);
	}

	TechDoorLoadMovie();

	return 0;
}

int FreeGameStateHook(int* params)
{
	*(int*)0x00A1C08D = 0;

	const auto StopSound = (int(*)(const char*))0x00538C50;
	const auto XwaFrontResFreeItem = (void(*)(const char*))0x00532080;
	const auto XwaFreeResDataItem = (short(*)(short))0x004CDE40;

	FreeSkirmishImage();

	for (const auto& animation : g_concourseAnimations.animations)
	{
		StopSound(animation.name.c_str());

		if (g_config.SupportDatAnimations)
		{
			XwaFrontResFreeItem(animation.name.c_str());
			XwaFreeResDataItem(animation.nameGroupId);
		}
	}

	for (auto& bitmap : g_dataBitmaps)
	{
		bitmap.second->Release();
	}

	g_dataBitmaps.clear();

	for (auto& bitmap : g_videoBitmaps)
	{
		bitmap.second->Release();
	}

	g_videoBitmaps.clear();

	WebmFreeAllVideos();

	TechDoorFreeMovie();

	//CleanEffects();

	return 0;
}

void DrawConcourseAnimations(const char* currentRoom, int frameIndex)
{
	const auto XwaFrontResDraw = (int(*)(const char*, int, int))0x00534A60;
	const auto XwaFrontResMoveToNextImage = (bool(*)(const char*, bool))0x00532230;
	const auto XwaFrontResSetCurrentImage = (bool(*)(const char*, int))0x005322F0;
	const auto XwaFrontResGetCurrentImage = (int(*)(const char*))0x005321F0;
	const auto XwaPlaySound = (int(*)(const char*, int, int, int, int, int))0x00538850;

	if (frameIndex == 0)
	{
		if (g_config.SupportDatAnimations)
		{
			for (const auto& animation : g_concourseAnimations.animations)
			{
				XwaFrontResSetCurrentImage(animation.name.c_str(), 0);
			}
		}

		WebmResetAllTimecode();

		for (const auto& group : g_concourseAnimations.GetGroups(currentRoom))
		{
			ConcourseAnimation* randomAnimation = g_concourseAnimations.GetRandomAnimation(currentRoom, group.c_str());

			if (randomAnimation != nullptr)
			{
				g_concourseAnimations.SetGroupFrame(currentRoom, randomAnimation->group.c_str(), randomAnimation->id, frameIndex);
			}
		}
	}

	for (auto& animation : g_concourseAnimations.animations)
	{
		if (!animation.IsRoom(currentRoom))
		{
			continue;
		}

		if (!animation.IsInCurrentMission())
		{
			continue;
		}

		if (frameIndex < animation.frame)
		{
			continue;
		}

		if (animation.frame == frameIndex)
		{
			int volume = *(unsigned char*)0x00B0C887 * 12;
			XwaPlaySound(animation.name.c_str(), 1, 0, 0xff, volume, 0x3f);
		}

		SurfaceDC dc;
		bool hasDC = GetSurfaceDC(&dc);

		uint8_t* image = 0;
		unsigned int imageWidth = 0;
		unsigned int imageHeight = 0;
		int r = -1;

		if (hasDC)
		{
			r = WebmReadVideoFrame(animation.name, &image, &imageWidth, &imageHeight);

			if (image)
			{
				int animPosX = animation.posX;
				int animPosY = animation.posY;
				int animWidth = animation.posWidth != 0 ? animation.posWidth : imageWidth;
				int animHeight = animation.posHeight != 0 ? animation.posHeight : imageHeight;

				bool isBackgroundHD = g_netFunctions._frontResIsBackgroundWide() != 0;

				int left = MultiplyDivH(animPosX, isBackgroundHD, dc);
				int top = MultiplyDivV(animPosY, dc);
				int width = MultiplyDivH(animWidth, isBackgroundHD, dc);
				int height = MultiplyDivV(animHeight, dc);

				DrawSurfaceDelegate(&dc, image, imageWidth, imageHeight, left, top, width, height, 0, 0, imageWidth, imageHeight, -10, 0, 25);
			}
		}

		if (!image && g_config.SupportDatAnimations)
		{
			XwaFrontResDraw(animation.name.c_str(), animation.posX, animation.posY);
			XwaFrontResMoveToNextImage(animation.name.c_str(), true);
		}

		bool setImage = false;

		if (image)
		{
			if (WebmGetTimecode(animation.name) == -1 && r == 0)
			{
				setImage = true;
			}
		}
		else if (g_config.SupportDatAnimations)
		{
			if (XwaFrontResGetCurrentImage(animation.name.c_str()) == 0)
			{
				setImage = true;
			}
		}

		if (setImage)
		{
			ConcourseAnimation* randomAnimation;

			if (animation.group[0] == '+')
			{
				randomAnimation = &animation;
			}
			else
			{
				randomAnimation = g_concourseAnimations.GetRandomAnimation(currentRoom, animation.group.c_str());
			}

			if (randomAnimation != nullptr)
			{
				g_concourseAnimations.SetGroupFrame(currentRoom, randomAnimation->group.c_str(), randomAnimation->id, frameIndex + 1);
			}
		}
	}
}

static int g_planetImageLeft = 0;
static int g_planetImageTop = 0;
static int g_planetImageWidthOver2 = 0;
static int g_planetImageHeightOver2 = 0;

int DrawConcoursePlanetHook(int* params)
{
	const auto Xwa_rand = (int(*)())0x0059BEB0;

	int planetWidthOver2 = params[Params_ESI];
	int planetHeightOver2 = params[Params_EDI];

	int positionX = g_concourseDoors.planet_position_left;
	int positionY = g_concourseDoors.planet_position_top;

	int randX = Xwa_rand();

	if (g_concourseDoors.planet_position_right > g_concourseDoors.planet_position_left)
	{
		positionX += randX % (g_concourseDoors.planet_position_right - g_concourseDoors.planet_position_left);
	}

	int randY = Xwa_rand();

	if (g_concourseDoors.planet_position_bottom > g_concourseDoors.planet_position_top)
	{
		positionY += randY % (g_concourseDoors.planet_position_bottom - g_concourseDoors.planet_position_top);
	}

	static int _mission = -1;
	static int _planetPositionX;
	static int _planetPositionY;

	const int currentMission = *(int*)0x00ABC970;
	const XwaMissionDescription* s_XwaMissionDescriptions = *(XwaMissionDescription**)0x009F4B98;
	const int missionDirectoryId = *(int*)(0x00AE2A60 + 0x002A);

	if (currentMission != _mission)
	{
		_mission = currentMission;
		_planetPositionX = -1;
		_planetPositionY = -1;

		int currentMissionIndex = GetMissionFileNameById(currentMission);

		if (currentMissionIndex != -1)
		{
			const char* missionDirectory = ((const char**)0x00603168)[missionDirectoryId];
			const char* missionFileName = s_XwaMissionDescriptions[currentMissionIndex].MissionFileName;

			char path[256];
			sprintf_s(path, "%s\\%s", missionDirectory, missionDirectoryId == 3 ? "temp.tie" : missionFileName);

			const auto lines = GetMissionConcourseLines(path);

			_planetPositionX = GetFileKeyValueInt(lines, "FrontPlanetPositionX", -1);
			_planetPositionY = GetFileKeyValueInt(lines, "FrontPlanetPositionY", -1);
		}
	}

	params[Params_EBX] = (_planetPositionX != -1 ? _planetPositionX : positionX) - planetWidthOver2;
	params[Params_EDX] = (_planetPositionY != -1 ? _planetPositionY : positionY) - planetHeightOver2;

	g_planetImageLeft = params[Params_EBX];
	g_planetImageTop = params[Params_EDX];
	g_planetImageWidthOver2 = planetWidthOver2;
	g_planetImageHeightOver2 = planetHeightOver2;

	return 0;
}

int DrawConcourseDoorsHook(int* params)
{
	const int frameIndex = params[Params_EBP];

	const auto XwaFrontResDraw = (int(*)(const char*, int, int))0x00534A60;
	const auto XwaFrontResDrawCurrentImage = (int(*)(const char*, int, int))0x00535470;
	const auto XwaFrontResMoveToNextImage = (bool(*)(const char*, bool))0x00532230;

	if (HasSurfaceDC())
	{
		DrawConcourseStarfieldHD(frameIndex);
		XwaFrontResDraw("planet", g_planetImageLeft, g_planetImageTop);
		XwaFrontResDraw("concourse", 0, 0);
	}

	XwaFrontResDrawCurrentImage("tourdoor", g_concourseDoors.tourdoor_posX, g_concourseDoors.tourdoor_posY);
	XwaFrontResDraw("mkanim", g_concourseDoors.mkanim_posX, g_concourseDoors.mkanim_posY);
	XwaFrontResDrawCurrentImage("traindoor", g_concourseDoors.traindoor_posX, g_concourseDoors.traindoor_posY);
	XwaFrontResDrawCurrentImage("combatdoor", g_concourseDoors.combatdoor_poxX, g_concourseDoors.combatdoor_poxY);
	XwaFrontResDrawCurrentImage("filmdoor", g_concourseDoors.filmdoor_posX, g_concourseDoors.filmdoor_posY);

	if (IsTechDoorMovie())
	{
		TechDoorDrawImage(g_concourseDoors.techdoor_posX, g_concourseDoors.techdoor_posY, g_concourseDoors.techdoor_posWidth, g_concourseDoors.techdoor_posHeight);
	}
	else
	{
		XwaFrontResDrawCurrentImage("techdoor", g_concourseDoors.techdoor_posX, g_concourseDoors.techdoor_posY);
	}

	DrawConcourseAnimations("concourse", frameIndex);

	return 0;
}

int TrainDoorRectHook(int* params)
{
	int A4 = params[0];

	const auto XwaRectMove = (void(*)(int, int, int))0x00558CD0;

	XwaRectMove(A4, ConcourseMultiplyDivRound(g_concourseDoors.traindoor_posX), ConcourseMultiplyDivRound(g_concourseDoors.traindoor_posY));

	return 0;
}

int CombatDoorRectHook(int* params)
{
	int A4 = params[0];

	const auto XwaRectMove = (void(*)(int, int, int))0x00558CD0;

	XwaRectMove(A4, ConcourseMultiplyDivRound(g_concourseDoors.combatdoor_poxX), ConcourseMultiplyDivRound(g_concourseDoors.combatdoor_poxY));

	return 0;
}

int TourDoorRectHook(int* params)
{
	int A4 = params[0];

	const auto XwaRectMove = (void(*)(int, int, int))0x00558CD0;

	XwaRectMove(A4, ConcourseMultiplyDivRound(g_concourseDoors.tourdoor_posX), ConcourseMultiplyDivRound(g_concourseDoors.tourdoor_posY));

	return 0;
}

int MkanimRectHook(int* params)
{
	int A4 = params[0];
	int x1 = params[1];
	int y1 = params[2];
	int x2 = params[3];
	int y2 = params[4];
	int posX = ConcourseMultiplyDivRound(g_concourseDoors.mkanim_posX);
	int posY = ConcourseMultiplyDivRound(g_concourseDoors.mkanim_posY);

	const auto XwaRectSet = (void(*)(int, int, int, int, int))0x00558C90;

	XwaRectSet(A4, posX, posY, posX + x2 - x1, posY + y2 - y1);

	return 0;
}

int FilmDoorRectHook(int* params)
{
	int A4 = params[0];

	const auto XwaRectMove = (void(*)(int, int, int))0x00558CD0;

	XwaRectMove(A4, ConcourseMultiplyDivRound(g_concourseDoors.filmdoor_posX), ConcourseMultiplyDivRound(g_concourseDoors.filmdoor_posY));

	return 0;
}

int TechDoorRectHook(int* params)
{
	int A4 = params[0];

	const auto XwaRectMove = (void(*)(int, int, int))0x00558CD0;

	XwaRectMove(A4, ConcourseMultiplyDivRound(g_concourseDoors.techdoor_posX), ConcourseMultiplyDivRound(g_concourseDoors.techdoor_posY));

	return 0;
}

std::vector<std::pair<int, int>> g_MedalsPositions;
std::vector<std::string> g_MedalsDescriptions;

void LoadMedalsPositions()
{
	static bool _loaded = false;

	if (_loaded)
	{
		return;
	}

	if (std::ifstream("MedalsPositions.txt"))
	{
		const auto lines = GetFileLines("MedalsPositions.txt");

		for (const std::string& line : lines)
		{
			if (line.empty())
			{
				continue;
			}

			const auto tokens = Tokennize(line);

			if (tokens.size() < 2)
			{
				continue;
			}

			int positionX = std::stoi(tokens[0], 0, 0);
			int positionY = std::stoi(tokens[1], 0, 0);

			g_MedalsPositions.push_back(std::make_pair(positionX, positionY));
		}
	}
	else
	{
		g_MedalsPositions.push_back(std::make_pair(190, 70));
		g_MedalsPositions.push_back(std::make_pair(260, 90));
		g_MedalsPositions.push_back(std::make_pair(330, 70));
		g_MedalsPositions.push_back(std::make_pair(158, 170));
		g_MedalsPositions.push_back(std::make_pair(378, 168));
		g_MedalsPositions.push_back(std::make_pair(196, 300));
		g_MedalsPositions.push_back(std::make_pair(328, 296));
	}

	_loaded = true;
}

void LoadMedalsDescriptions()
{
	static bool _loaded = false;

	if (_loaded)
	{
		return;
	}

	std::ifstream file("MedalsDescriptions.txt");

	if (file)
	{
		std::string line;

		while (std::getline(file, line))
		{
			if (line.empty())
			{
				g_MedalsDescriptions.push_back("Unknown");
			}
			else
			{
				g_MedalsDescriptions.push_back(line);
			}
		}
	}

	_loaded = true;
}

int MedalPositionHook(int* params)
{
	LoadMedalsPositions();

	const int index = params[0];

	if (index < (int)g_MedalsPositions.size())
	{
		params[0] = ConcourseMultiplyDivRound(g_MedalsPositions[index].first);
		params[1] = ConcourseMultiplyDivRound(g_MedalsPositions[index].second);
	}
	else
	{
		params[0] = 0;
		params[1] = 0;
	}

	return 0;
}

int MedalDescriptionHook(int* params)
{
	LoadMedalsDescriptions();

	const int index = params[0];

	const auto XwaGetFrontTxtString = (const char* (*)(int))0x0055CB50;

	const char* description;

	if (index < (int)g_MedalsDescriptions.size())
	{
		description = g_MedalsDescriptions[index].c_str();
	}
	else if (index < 7)
	{
		description = XwaGetFrontTxtString(0x3AD + index);
	}
	else
	{
		description = "Unknown";
	}

	return (int)description;
}

int KalidorCrescentPosition1Hook(int* params)
{
	const int A4 = params[0];

	const auto L00558CD0 = (void(*)(int, int, int))0x00558CD0;

	L00558CD0(A4, ConcourseMultiplyDivRound(g_config.KalidorCrescentPositionX), ConcourseMultiplyDivRound(g_config.KalidorCrescentPositionY));

	return 0;
}

int KalidorCrescentPosition2Hook(int* params)
{
	const int A4 = params[0];

	const auto L00534A60 = (void(*)(int, int, int))0x00534A60;

	L00534A60(A4, g_config.KalidorCrescentPositionX, g_config.KalidorCrescentPositionY);

	return 0;
}

std::vector<int> g_RankPoints;
std::vector<int> g_PilotRatingPoints;

void LoadRankPoints()
{
	static bool _loaded = false;

	if (_loaded)
	{
		return;
	}

	if (std::ifstream("RankPoints.txt"))
	{
		const auto lines = GetFileLines("RankPoints.txt");

		g_RankPoints = GetFileListIntValues(lines);
	}
	else
	{
		const int* s_RankPoints = (int*)0x005B30C0;

		for (int i = 0; i < 8; i++)
		{
			int points = s_RankPoints[i];
			g_RankPoints.push_back(points);
		}
	}

	if (std::ifstream("PilotRatingPoints.txt"))
	{
		const auto lines = GetFileLines("PilotRatingPoints.txt");

		g_PilotRatingPoints = GetFileListIntValues(lines);
	}
	else
	{
		const int* s_PilotRatingPoints = (int*)0x005B2FD4;

		for (int i = 0; i < 30; i++)
		{
			int points = s_PilotRatingPoints[i];
			g_PilotRatingPoints.push_back(points);
		}
	}

	_loaded = true;
}

int GetRankPointsArrayPtr()
{
	LoadRankPoints();

	return (int)g_RankPoints.data();
}

int GetPilotRatingPointsArrayPtr()
{
	LoadRankPoints();

	return (int)g_PilotRatingPoints.data();
}

int MedalAndKalidorCrescentDescriptionHook(int* params)
{
	int index = params[0];

	const auto XwaGetFrontTxtString = (const char* (*)(int))0x0055CB50;

	const char* description;

	if (index > 100)
	{
		// Medal Description
		LoadMedalsDescriptions();

		index = index - 0x3AD;

		if (index < (int)g_MedalsDescriptions.size())
		{
			description = g_MedalsDescriptions[index].c_str();
		}
		else if (index < 7)
		{
			description = XwaGetFrontTxtString(0x3AD + index);
		}
		else
		{
			description = "Unknown";
		}
	}
	else
	{
		// Kalidor Crescent Description
		LoadCustomPoints();

		index = index - 1;

		if (index < (int)g_KalidorCrescentDescriptions.size())
		{
			description = g_KalidorCrescentDescriptions[index].c_str();
		}
		else if (index - 1 < 6)
		{
			description = XwaGetFrontTxtString(0x3A7 + index);
		}
		else
		{
			description = "Unknown";
		}
	}

	return (int)description;
}

int BattlesMissionsImagesCountHook(int* params)
{
	const auto XwaLoadResDataItem = (short(*)(short))0x004CD6C0;
	const auto XwaLoadDatImage = (int(*)(const char*, short, short, short))0x0055E800;

	XwaLoadResDataItem(0x3B6A);
	XwaLoadDatImage("mislarge", 0x3B6A, 1, 256);
	XwaLoadDatImage("missmall", 0x3B6A, 0x3E8, 256);
	XwaLoadDatImage("battleback", 0x3B6A, 0x7D0, 100);
	XwaLoadDatImage("battle00", 0x3B6A, 0x834, 1);

	params[Params_ReturnAddress] = 0x0055DDCE;
	return 0;
}

bool DoesMovieExist(const std::string& name)
{
	if (g_config.PlayMovieOutputDebugString)
	{
		OutputDebugString(("try to play " + name).c_str());
	}

	const char* extensions[] =
	{
		".avi",
		".wmv",
		".mp4",
		".znm",
		".snm"
	};

	for (const char* ext : extensions)
	{
		std::string filename = std::string("movies\\") + name + ext;

		if (std::ifstream(filename))
		{
			return true;
		}
	}

	return false;
}

std::string GetPerMissionMovieName(const std::string& A4, int* params)
{
	static auto _lines = GetFileLines("Movies\\Movies.txt");

	const auto XwaLoadMissionDescriptions = (void(*)(int))0x00547800;

	XwaLoadMissionDescriptions(4);

	const XwaMissionDescription* s_XwaMissionDescriptions = *(XwaMissionDescription**)0x009F4B98;
	const int s_V0x09F5E74 = *(int*)0x009F5E74;

	std::string missionFilename = GetStringWithoutExtension(s_XwaMissionDescriptions[s_V0x09F5E74].MissionFileName);

	std::string name;

	if (name.empty() && A4 == "ceremony" && params != nullptr)
	{
		name = A4 + "_" + (const char*)(params + 13);
		std::vector<std::string> values = Tokennize(GetFileKeyValue(_lines, name));

		if (!values.empty())
		{
			unsigned short rand = g_randomGenerator.Generate(0, values.size());
			name = values[rand];
		}

		if (!DoesMovieExist(name))
		{
			name = std::string();
		}
	}

	if (name.empty())
	{
		name = missionFilename + "_" + A4;
		std::vector<std::string> values = Tokennize(GetFileKeyValue(_lines, name));

		if (!values.empty())
		{
			unsigned short rand = g_randomGenerator.Generate(0, values.size());
			name = values[rand];
		}

		if (!DoesMovieExist(name))
		{
			name = std::string();
		}
	}

	if (name.empty())
	{
		name = A4;
		std::vector<std::string> values = Tokennize(GetFileKeyValue(_lines, name));

		if (!values.empty())
		{
			unsigned short rand = g_randomGenerator.Generate(0, values.size());
			name = values[rand];
		}

		if (!DoesMovieExist(name))
		{
			name = std::string();
		}
	}

	return name;
}

std::string g_perMissionMovieFileName;
std::string g_perMissionMovieBeforeBackground;
std::string g_perMissionMovieAfterBackground;

int PlayPerMissionMovieHook(int* params)
{
	const char* A4 = (const char*)params[0];
	const int A8 = params[1];

	const auto XwaPlayMovie = (int(*)(const char*, int))0x0055BC20;

	std::string name;

	if (ends_with(A4, "briefroom"))
	{
		name = g_perMissionMovieFileName;
	}
	else
	{
		name = GetPerMissionMovieName(A4, params);
	}

	if (g_config.PlayMovieOutputDebugString)
	{
		OutputDebugString(("play per-mission movie " + name).c_str());
	}

	if (DoesMovieExist(name))
	{
		XwaPlayMovie(name.c_str(), A8);
	}
	else
	{
		XwaPlayMovie(A4, A8);
	}

	return 0;
}

int LoadPerMissionMovieBeforeBackgroundHook(int* params)
{
	// "frontres\combat\briefoff.bmp"

	const char* filename = (const char*)params[0];
	const char* key = (const char*)params[1];

	const auto XwaFrontResLoad = (int(*)(const char*, const char*))0x00531D70;

	if (ends_with(filename, "briefoff.bmp"))
	{
		std::string name = GetPerMissionMovieName("briefroom", nullptr);

		g_perMissionMovieFileName = name;
		g_perMissionMovieBeforeBackground = "movies\\" + name + "_start.cbm";
		g_perMissionMovieAfterBackground = "movies\\" + name + "_end.cbm";

		if (std::ifstream(g_perMissionMovieBeforeBackground))
		{
			XwaFrontResLoad(g_perMissionMovieBeforeBackground.c_str(), key);
		}
		else
		{
			XwaFrontResLoad(filename, key);
		}
	}
	else
	{
		XwaFrontResLoad(filename, key);
	}

	return 0;
}

int LoadPerMissionMovieAfterBackgroundHook(int* params)
{
	// "frontres\family\markoholo.bmp" or "frontres\combat\solo.bmp"

	const char* filename = (const char*)params[0];
	const char* key = (const char*)params[1];

	const auto XwaFrontResLoad = (int(*)(const char*, const char*))0x00531D70;

	if (ends_with(filename, "solo.bmp"))
	{
		if (std::ifstream(g_perMissionMovieAfterBackground))
		{
			XwaFrontResLoad(g_perMissionMovieAfterBackground.c_str(), key);
		}
		else
		{
			XwaFrontResLoad(filename, key);
		}
	}
	else
	{
		XwaFrontResLoad(filename, key);
	}

	return 0;
}

std::vector<unsigned char> g_emailsStatus;

void UpdateEmailsStatusPointers()
{
	*(int*)(0x00539F3B + 0x02) = (int)g_emailsStatus.data();
	*(int*)(0x00539F59 + 0x02) = (int)g_emailsStatus.data();
	*(int*)(0x00560048 + 0x02) = (int)g_emailsStatus.data();
	*(int*)(0x0056005D + 0x02) = (int)g_emailsStatus.data();
	*(int*)(0x00560C06 + 0x02) = (int)g_emailsStatus.data();
	*(int*)(0x00560C9A + 0x02) = (int)g_emailsStatus.data();
	*(int*)(0x00560E45 + 0x02) = (int)g_emailsStatus.data();
	*(int*)(0x00563937 + 0x02) = (int)g_emailsStatus.data();
	*(int*)(0x00563949 + 0x02) = (int)g_emailsStatus.data();
	*(int*)(0x0056395E + 0x02) = (int)g_emailsStatus.data();
}

void ResizeEmailsStatusBuffer(int newSize)
{
	std::vector<unsigned char> buffer;
	buffer.reserve(g_emailsStatus.capacity());
	memcpy_s(buffer.data(), g_emailsStatus.capacity(), g_emailsStatus.data(), g_emailsStatus.capacity());

	g_emailsStatus.clear();
	g_emailsStatus.reserve(newSize);
	memset(g_emailsStatus.data(), 0, g_emailsStatus.capacity());

	memcpy_s(g_emailsStatus.data(), min(g_emailsStatus.capacity(), buffer.capacity()), buffer.data(), min(g_emailsStatus.capacity(), buffer.capacity()));

	UpdateEmailsStatusPointers();
}

int WritePilotHook(int* params)
{
	const char* pilotName = (const char*)0x00ABD680;

	if (std::ofstream file{ std::string(pilotName) + "_emails.bin", std::ofstream::trunc | std::ofstream::binary })
	{
		file.write((const char*)g_emailsStatus.data(), g_emailsStatus.capacity());
	}

	return 1;
}

int ReadPilotHook(int* params)
{
	const char* pilotName = (const char*)0x00ABD680;

	g_emailsStatus.clear();

	if (std::ifstream file{ std::string(pilotName) + "_emails.bin", std::ifstream::binary })
	{
		file.seekg(0, std::ios::end);
		int count = (int)file.tellg();
		file.seekg(0, std::ios::beg);
		ResizeEmailsStatusBuffer(count);
		file.read((char*)g_emailsStatus.data(), count);
	}
	else
	{
		int offset = 0x00AE2A60 + 0x2508E;
		int count = 100;
		ResizeEmailsStatusBuffer(count);
		memcpy_s(g_emailsStatus.data(), g_emailsStatus.capacity(), (const char*)offset, count);
	}

	return 1;
}

int CreatePilotHook(int* params)
{
	const char* A4 = (const char*)params[0];
	const char* A8 = (const char*)params[1];

	const auto Xwa_fopen = (int(*)(const char*, const char*))0x0059AE10;

	int result = Xwa_fopen(A4, A8);

	if (result != 0)
	{
		return result;
	}

	const char* pilotName = (const char*)params[14];

	g_emailsStatus.clear();
	ResizeEmailsStatusBuffer(100);

	if (std::ofstream file{ std::string(pilotName) + "_emails.bin", std::ofstream::trunc | std::ofstream::binary })
	{
		file.write((const char*)g_emailsStatus.data(), g_emailsStatus.capacity());
	}

	return 0;
}

int ReadEmailTxtHook(int* params)
{
	int& s_XwaEmailsCount = *(int*)0x009F4B10;

	s_XwaEmailsCount = params[Params_EDI];
	ResizeEmailsStatusBuffer(s_XwaEmailsCount);

	return 0;
}

int RandInteger(int min, int max)
{
	const auto Xwa_rand = (int(*)())0x0059BEB0;

	int value = ((Xwa_rand() & 0x7fff) << 15) | (Xwa_rand() & 0x7fff);
	return min + (value % (max - min));
}

static bool g_isPlanetImageLoaded = false;
static unsigned short g_concourseStarfieldColors[31];
static unsigned int g_concourseStarfieldColors32[31];
static std::vector<int> g_concourseStarfieldPositionsX;
static std::vector<int> g_concourseStarfieldPositionsY;
static std::vector<int> g_concourseStarfieldFrames;
static std::vector<int> g_concourseStarfieldTypes;

int SetupConcourseStarfieldHook(int* params)
{
	bool isBackgroundHD = g_netFunctions._frontResIsBackgroundHD() != 0;

	g_concourseStarfieldPositionsX.clear();
	g_concourseStarfieldPositionsY.clear();
	g_concourseStarfieldFrames.clear();
	g_concourseStarfieldTypes.clear();

	const auto XwaGetCurrentSurfacePitch = (int(*)())0x0053FA60;
	const auto XwaGetColor = (unsigned int(*)(unsigned char, unsigned char, unsigned char))0x0055D270;
	const auto Xwa_rand = (int(*)())0x0059BEB0;
	const auto XwaFrontResDraw = (int(*)(const char*, short, short))0x00534A60;
	const auto XwaSaveDrawingSurfaceToFile = (int(*)(const char*, void*, int, int, int, int, int, void*))0x005375E0;

	void*& s_pXwaCurrentSurfaceData = *(void**)0x009F60D4;
	int& XwaGlobalVariables_CurrentSurfacePitch = *(int*)(0x09F60E0 + 0x0F1A);

	for (int i = 0; i < 16; i++)
	{
		unsigned char al = (unsigned char)(0x2F + i * 0x0B);
		unsigned char bl = (unsigned char)(0x40 + i * 0x0C);

		g_concourseStarfieldColors[i] = (unsigned short)XwaGetColor(al, al, bl);
		g_concourseStarfieldColors32[i] = (al << 16) | (al << 8) | bl;
	}

	for (int i = 0; i < 15; i++)
	{
		g_concourseStarfieldColors[16 + i] = g_concourseStarfieldColors[14 - i];
		g_concourseStarfieldColors32[16 + i] = g_concourseStarfieldColors32[14 - i];
	}

	unsigned short* surfaceData = *(unsigned short**)0x009F60D4;
	int surfacePitch = XwaGetCurrentSurfacePitch() / 2;
	int width = 640;
	int height = 480;

	std::vector<std::tuple<int, int>> pixels;
	pixels.reserve(width * height);

	void* currentSurfaceData = s_pXwaCurrentSurfaceData;
	int currentSurfacePitch = XwaGlobalVariables_CurrentSurfacePitch;
	std::vector<unsigned short> surface;
	surface.reserve(width * height);
	memset(surface.data(), 0x80, width * height * 2);
	s_pXwaCurrentSurfaceData = surface.data();
	XwaGlobalVariables_CurrentSurfacePitch = width * 2;

	XwaFrontResDraw("planet", g_planetImageLeft, g_planetImageTop);
	XwaFrontResDraw("concourse", 0, 0);
	//XwaSaveDrawingSurfaceToFile("concourse.bmp", surface.data(), width, height, width * 2, 16, 0, nullptr);

	if (g_config.HDConcourseEnabled && isBackgroundHD)
	{
		memset(surfaceData, 0, width * height * 2);
		memset(surface.data(), 0x80, width * height * 2);

		int surfaceConcourseWidth;
		int surfaceConcourseHeight;
		bool hasConcourse = g_netFunctions._getItemImageDimensions("concourse", &surfaceConcourseWidth, &surfaceConcourseHeight, nullptr, nullptr) == 0;

		if (hasConcourse)
		{
			std::vector<unsigned int> surfaceConcourse;
			surfaceConcourse.reserve(surfaceConcourseWidth * surfaceConcourseHeight);

			g_netFunctions._getItemImageRawData("concourse", (int)surfaceConcourse.data());

			for (int y = 0; y < surfaceConcourseHeight; y++)
			{
				if (y < 0 || y >= 480)
				{
					continue;
				}

				for (int x = 0; x < surfaceConcourseWidth; x++)
				{
					if (x < 0 || x >= 640)
					{
						continue;
					}

					int iSource = y * surfaceConcourseWidth + x;
					int iDestination = y * 640 + x;
					char* data = (char*)surfaceConcourse.data();

					if (data[iSource * 4 + 3] != 0)
					{
						surface.data()[iDestination] = 0xffff;
					}
				}
			}
		}

		//XwaSaveDrawingSurfaceToFile("concourse.bmp", surfaceData, width, height, width * 2, 16, 0, nullptr);
	}

	int starfieldHeight = (g_isPlanetImageLoaded || (g_config.HDConcourseEnabled && isBackgroundHD)) ? height : min(height, 125);

	for (int y = 0; y < starfieldHeight; y++)
	{
		for (int x = 0; x < width; x++)
		{
			//if (surfaceData[surfacePitch * y + x] != 0)
			//{
			//	continue;
			//}

			if (surface.data()[width * y + x] != 0x8080)
			{
				continue;
			}

			pixels.push_back(std::make_tuple(x, y));
		}
	}

	surface.reserve(0);
	s_pXwaCurrentSurfaceData = currentSurfaceData;
	XwaGlobalVariables_CurrentSurfacePitch = currentSurfacePitch;

	int count = pixels.size() * 2048 / 50000;

	g_concourseStarfieldPositionsX.resize(count);
	g_concourseStarfieldPositionsY.resize(count);
	g_concourseStarfieldFrames.resize(count);
	g_concourseStarfieldTypes.resize(count);

	for (int i = 0; i < count; i++)
	{
		int index = RandInteger(0, pixels.size());
		const auto& pixel = pixels[index];

		int x = std::get<0>(pixel);
		int y = std::get<1>(pixel);

		g_concourseStarfieldPositionsX[i] = x;
		g_concourseStarfieldPositionsY[i] = y;
		g_concourseStarfieldFrames[i] = Xwa_rand() % 31;
		g_concourseStarfieldTypes[i] = 2 + (Xwa_rand() % 8);
	}

	return 1;
}

int DrawConcourseStarfieldHook(int* params)
{
	if (HasSurfaceDC() || !g_config.DrawStarfield)
	{
		return 1;
	}

	const int frameIndex = params[0];

	const auto XwaGetCurrentSurfacePitch = (int(*)())0x0053FA60;

	unsigned short* surfaceData = *(unsigned short**)0x009F60D4;
	int surfacePitch = XwaGetCurrentSurfacePitch() / 2;

	for (int i = 0; i < (int)g_concourseStarfieldTypes.size(); i++)
	{
		int x = g_concourseStarfieldPositionsX[i];
		int y = g_concourseStarfieldPositionsY[i];
		int frame = g_concourseStarfieldFrames[i];
		int type = g_concourseStarfieldTypes[i];

		surfaceData[surfacePitch * y + x] = g_concourseStarfieldColors[frame];

		if (type == 9)
			continue;

		if (2 + (frameIndex % 7) != type)
			continue;

		frame++;

		if (frame >= 31)
		{
			frame = 0;
		}

		g_concourseStarfieldFrames[i] = frame;
	}

	return 1;
}

void DrawConcourseStarfieldHD(int frameIndex)
{
	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	if (!hasDC)
	{
		return;
	}

	if (!g_config.DrawStarfield)
	{
		dc.d2d1RenderTarget->BeginDraw();
		dc.d2d1RenderTarget->Clear(D2D1::ColorF(0));
		dc.d2d1RenderTarget->EndDraw();
		return;
	}

	bool isBackgroundHD = g_netFunctions._frontResIsBackgroundHD() != 0;
	int offsetX = MultiplyDivOffsetX(dc);
	int w = MultiplyDivH(1, isBackgroundHD, dc);
	int h = MultiplyDivV(1, dc);

	ComPtr<ID2D1SolidColorBrush> brush;
	dc.d2d1RenderTarget->CreateSolidColorBrush(D2D1::ColorF(0), &brush);

	dc.d2d1RenderTarget->BeginDraw();

	dc.d2d1RenderTarget->Clear(D2D1::ColorF(0));

	for (int i = 0; i < (int)g_concourseStarfieldTypes.size(); i++)
	{
		int x = g_concourseStarfieldPositionsX[i];
		int y = g_concourseStarfieldPositionsY[i];
		int frame = g_concourseStarfieldFrames[i];
		int type = g_concourseStarfieldTypes[i];
		unsigned int color = g_concourseStarfieldColors[frame];

		x = MultiplyDivH(x, isBackgroundHD, dc) + offsetX;
		y = MultiplyDivV(y, dc);

		brush->SetColor(D2D1::ColorF(g_concourseStarfieldColors32[frame]));
		dc.d2d1RenderTarget->FillRectangle(D2D1::RectF(x, y, x + w, y + h), brush);

		if (type == 9)
			continue;

		if (2 + (frameIndex % 7) != type)
			continue;

		frame++;

		if (frame >= 31)
		{
			frame = 0;
		}

		g_concourseStarfieldFrames[i] = frame;
	}

	dc.d2d1RenderTarget->EndDraw();
}

int GetMissionFrontPlanetImageIndex(const std::string& mission)
{
	const auto lines = GetMissionConcourseLines(mission);

	return GetFileKeyValueInt(lines, "FrontPlanetIndex", -1);
}

int SwitchFrontPlanetImageHook(int* params)
{
	const auto XwaLoadPlanetImage = (int(*)(int))0x0055DCA0;

	const XwaMissionDescription* s_XwaMissionDescriptions = *(XwaMissionDescription**)0x009F4B98;
	const int missionDirectoryId = *(int*)(0x00AE2A60 + 0x002A);
	const int currentMission = *(int*)0x00ABC970;

	int currentMissionIndex = GetMissionFileNameById(currentMission);

	if (currentMissionIndex != -1)
	{
		const char* missionDirectory = ((const char**)0x00603168)[missionDirectoryId];
		const char* missionFileName = s_XwaMissionDescriptions[currentMissionIndex].MissionFileName;

		char path[256];
		sprintf_s(path, "%s\\%s", missionDirectory, missionDirectoryId == 3 ? "temp.tie" : missionFileName);

		int missionPlanetImageIndex = GetMissionFrontPlanetImageIndex(path);

		if (missionPlanetImageIndex != -1)
		{
			g_isPlanetImageLoaded = true;
			XwaLoadPlanetImage(missionPlanetImageIndex - 0x3AA1);
		}
	}

	int edi = params[Params_EDI];

	if (edi == 0)
	{
		params[Params_ReturnAddress] = 0x0053A005;
	}

	return 0;
}

int LoadFrontPlanetImageHook(int* params)
{
	int planetImageIndex = params[0];

	const auto XwaLoadPlanetImage = (int(*)(int))0x0055DCA0;

	if (!g_isPlanetImageLoaded)
	{
		XwaLoadPlanetImage(planetImageIndex);
	}

	return 0;
}

int DrawMultiDoorHook(int* params)
{
	const auto XwaFrontResDraw = (int(*)(const char*, int, int))0x00534A60;

	XwaFrontResDraw("multi", g_concourseDoors.multidoor_posX, g_concourseDoors.multidoor_posY);

	return 0;
}

int DrawSingleDoorHook(int* params)
{
	const auto XwaFrontResDraw = (int(*)(const char*, int, int))0x00534A60;

	XwaFrontResDraw("single", g_concourseDoors.singledoor_posX, g_concourseDoors.singledoor_posY);

	return 0;
}

int DrawCombatBackDoorHook(int* params)
{
	const auto XwaFrontResDraw = (int(*)(const char*, int, int))0x00534A60;

	XwaFrontResDraw("doors", g_concourseDoors.combatbackdoor_posX, g_concourseDoors.combatbackdoor_posY);

	return 0;
}

int MultiDoorRectHook(int* params)
{
	const auto XwaRectSet = (void(*)(RECT*, int, int, int, int))0x00558C90;

	RECT* lpRect = (RECT*)params[0];
	int left = ConcourseMultiplyDivRound(g_concourseDoors.multidoor_posSelectionX);
	int top = ConcourseMultiplyDivRound(g_concourseDoors.multidoor_posSelectionY);
	int right = left + ConcourseMultiplyDivRound(g_concourseDoors.multidoor_posSelectionWidth);
	int bottom = top + ConcourseMultiplyDivRound(g_concourseDoors.multidoor_posSelectionHeight);

	XwaRectSet(lpRect, left, top, right, bottom);

	return 0;
}

int SingleDoorRectHook(int* params)
{
	const auto XwaRectSet = (void(*)(RECT*, int, int, int, int))0x00558C90;

	RECT* lpRect = (RECT*)params[0];
	int left = ConcourseMultiplyDivRound(g_concourseDoors.singledoor_posSelectionX);
	int top = ConcourseMultiplyDivRound(g_concourseDoors.singledoor_posSelectionY);
	int right = left + ConcourseMultiplyDivRound(g_concourseDoors.singledoor_posSelectionWidth);
	int bottom = top + ConcourseMultiplyDivRound(g_concourseDoors.singledoor_posSelectionHeight);

	XwaRectSet(lpRect, left, top, right, bottom);

	return 0;
}

int CombatBackDoorRectHook(int* params)
{
	const auto XwaRectSet = (void(*)(RECT*, int, int, int, int))0x00558C90;

	RECT* lpRect = (RECT*)params[0];
	int left = params[1] - 0x12B + ConcourseMultiplyDivRound(g_concourseDoors.combatbackdoor_posX);
	int top = params[2] - 0x5A + ConcourseMultiplyDivRound(g_concourseDoors.combatbackdoor_posY);
	int right = params[3] - 0x12B + ConcourseMultiplyDivRound(g_concourseDoors.combatbackdoor_posX);
	int bottom = params[4] - 0x5A + ConcourseMultiplyDivRound(g_concourseDoors.combatbackdoor_posY);

	XwaRectSet(lpRect, left, top, right, bottom);

	return 0;
}

int PlayFilmPodMovieHook(int* params)
{
	const auto XwaPlayMovie = (int(*)(const char*, int))0x0055BC20;

	XwaPlayMovie(g_config.FilmPodMovie.c_str(), 1);

	return 0;
}

int PlayMultiPodMovieHook(int* params)
{
	const auto XwaPlayMovie = (int(*)(const char*, int))0x0055BC20;

	XwaPlayMovie(g_config.MultiPodMovie.c_str(), 1);

	return 0;
}

int PlaySinglePodMovieHook(int* params)
{
	const auto XwaPlayMovie = (int(*)(const char*, int))0x0055BC20;

	XwaPlayMovie(g_config.SinglePodMovie.c_str(), 1);

	return 0;
}

int LoadFilmRoomBackgroundHook(int* params)
{
	const auto XwaFrontResLoad = (int(*)(const char*, const char*))0x00531D70;

	XwaFrontResLoad(g_config.FilmRoomBackground.c_str(), (const char*)params[1]);

	return 0;
}

bool IsUpdateCallback(int gameStateUpdate)
{
	const int currentGameState = *(int*)(0x009F60E0 + 0x25FA9);
	const int updateCallback = *(int*)(0x009F60E0 + 0x25FB1 + 0x850 * currentGameState + 0x0844);
	return updateCallback == gameStateUpdate;
}

bool IsPreviousUpdateCallback(int gameStateUpdate)
{
	const int currentGameState = *(int*)(0x009F60E0 + 0x25FA9) - 1;
	const int updateCallback = *(int*)(0x009F60E0 + 0x25FB1 + 0x850 * currentGameState + 0x0844);
	return updateCallback == gameStateUpdate;
}

bool AreDrawPanelsRemoved()
{
	const auto XwaRectSet = (void(*)(RECT*, int, int, int, int))0x00558C90;
	const auto XwaRetrieveCursorPos = (void(*)(int*, int*))0x0055BA50;
	const auto XwaIsPointInRect = (BOOL(*)(LPRECT, LONG, LONG))0x005592E0;

	bool isTourOfDutyRoomGameStateUpdate = IsUpdateCallback(0x005775E0);
	bool isCombatSingleGameStateUpdate = IsUpdateCallback(0x005438B0);
	bool isCombatMultiGameStateUpdate = IsUpdateCallback(0x0053C120);
	bool isBriefingGameStateUpdate = IsUpdateCallback(0x00564E90);
	bool isDebriefingGameStateUpdate = IsUpdateCallback(0x0057ECE0);
	bool isFamilyRoomGameGameStateUpdate = IsUpdateCallback(0x0055FF30);
	int s_V0x0783B64 = *(int*)0x00783B64;
	int s_V0x0783B0C = *(int*)0x00783B0C;
	bool isFamilyRoomMonitor = isFamilyRoomGameGameStateUpdate && s_V0x0783B64 == 1;
	bool isFilmRoomGameGameStateUpdate = IsUpdateCallback(0x00573020);

	const bool isCampaign = *(int*)0x0B07B53 == 1;

	// DrawPanelsInSimulator
	if (isCombatSingleGameStateUpdate
		|| isCombatMultiGameStateUpdate
		|| (isBriefingGameStateUpdate && !isCampaign)
		|| (isDebriefingGameStateUpdate && !isCampaign))
	{
		if (!g_config.DrawPanelsInSimulator)
		{
			return true;
		}
	}

	// DrawPanelsInTourOfDutyBriefing
	if (isBriefingGameStateUpdate && isCampaign)
	{
		if (!g_config.DrawPanelsInTourOfDutyBriefing)
		{
			return true;
		}
	}

	// DrawPanelsInTourOfDutyDebriefing
	if (isDebriefingGameStateUpdate && isCampaign)
	{
		if (!g_config.DrawPanelsInTourOfDutyDebriefing)
		{
			return true;
		}
	}

	// DrawPanelsInFamilyMonitor
	if (isFamilyRoomMonitor)
	{
		if (!g_config.DrawPanelsInFamilyMonitor)
		{
			return true;
		}
	}

	// DrawPanelsInFilmRoom
	if (isFilmRoomGameGameStateUpdate)
	{
		if (!g_config.DrawPanelsInFilmRoom)
		{
			return true;
		}
	}

	return false;
}

int PlaySoundHook(int* params)
{
	const char* name = (const char*)params[1];

	if (name == nullptr)
	{
		return 0;
	}

	if (AreDrawPanelsRemoved())
	{
		if (strcmp(name, "panelarm") == 0)
		{
			return 0;
		}
	}

	return *(int*)0x009F7F0F;
}

void FreeSkirmishImage()
{
	const auto L0050EC00 = (bool(*)())0x0050EC00;

	L0050EC00();
}

void DrawSkirmishImage(const SurfaceDC& dc, int craftId)
{
	const auto XwaOffScreenSurfaceLock = (int(*)())0x0053F830;
	const auto XwaOffScreenSurfaceUnlock = (int(*)(int))0x0053F8D0;
	const auto XwaGetCraftOptFilename = (const char* (*)(int))0x004341B0;
	const auto LoadOpt = (bool(*)(const char*, int))0x0050E5C0;
	const auto XwaSetOptAngle = (void(*)(float, float, float))0x0050FB10;
	const auto L0050FB80 = (void(*)(float))0x0050FB80;
	const auto XwaSetOptPosition = (void(*)(int, int, int))0x0050FB60;
	const auto XwaSetGlobalLight = (void(*)(int, int, int))0x0050FA50;
	const auto L0050EC70 = (bool(*)(int, int, int, int, void*, int, int))0x0050EC70;
	const auto XwaResetScreenArea = (void(*)())0x0053F760;

	const char* optFilename = XwaGetCraftOptFilename(craftId);

	if (optFilename == 0)
	{
		return;
	}

	bool isBackgroundHD = g_netFunctions._frontResIsBackgroundHD() != 0;
	bool isBackgroundWide = g_netFunctions._frontResIsBackgroundWide() != 0;

	const unsigned short* xwaExeSpecies = (unsigned short*)0x005B0F70;
	const int modelIndex = xwaExeSpecies[craftId];

	ViewConfigurationSettings settings = g_modelIndexSettings.GetViewConfiguration(modelIndex);

	XwaOffScreenSurfaceUnlock(1);

	// L0050EC70
	*(unsigned char*)0x0050EC90 = 0xEB;
	*(unsigned char*)0x0050ECA0 = 0xEB;
	*(unsigned char*)0x0050ECA1 = 0x44;

	float scale = (isBackgroundHD || isBackgroundWide) ? 1536.0f : 512.0f;
	*(float*)(0x0050EDCA + 0x06) = scale * settings.SkirmishOptScale * dc.displayHeight / dc.height;

	unsigned short& engineGlowsSetting = *(unsigned short*)0x00B0C7C2;
	unsigned short currentEngineGlowsSetting = engineGlowsSetting;
	engineGlowsSetting = 0;

	LoadOpt(optFilename, craftId);

	XwaResetScreenArea();

	*(int*)0x0783B8C = 0;
	*(float*)0x0783C0C = settings.SkirmishOptAnglePitch;
	*(float*)0x0783BD0 = settings.SkirmishOptAngleYaw;
	*(float*)0x0783BFC = settings.SkirmishOptAngleRoll;
	*(int*)0x0783B98 = 0x01;
	*(int*)0x0783BD8 = 0x01;
	*(int*)0x0783BF4 = 0x01;
	*(float*)0x0783B9C = 0;
	*(int*)0x0783BF0 = 0x258;

	XwaSetGlobalLight(*(int*)0x0783B98, *(int*)0x0783BD8, *(int*)0x0783BF4);
	XwaSetOptAngle(*(float*)0x0783C0C, *(float*)0x0783BD0, *(float*)0x0783BFC);
	L0050FB80(*(float*)0x0783B9C);
	XwaSetOptPosition((int)(settings.SkirmishOptPositionX * dc.displayHeight), (int)(settings.SkirmishOptPositionY * dc.displayHeight), (int)(settings.SkirmishOptPositionZ * dc.displayHeight));

	// lod distance
	((unsigned char*)0x0050F04D)[0] = 0x90;
	((unsigned char*)0x0050F04D)[1] = 0x90;
	((unsigned char*)0x0050F04D)[2] = 0x90;
	((unsigned char*)0x0050F04D)[3] = 0x90;
	((unsigned char*)0x0050F04D)[4] = 0x90;
	*(int*)0x007D4F8C = 0;

	int& s_XwaOptCurrentSwitchIndex = *(int*)0x007827C0;
	s_XwaOptCurrentSwitchIndex = 0;

	float position_left = (isBackgroundHD || isBackgroundWide) ? 400.0f : 100.0f;
	L0050EC70((int)(position_left * dc.displayHeight / dc.height), 0, 640, 480, nullptr, 0, 0);

	((unsigned char*)0x0050F04D)[0] = 0xA3;
	((unsigned char*)0x0050F04D)[1] = 0x8C;
	((unsigned char*)0x0050F04D)[2] = 0x90;
	((unsigned char*)0x0050F04D)[3] = 0x7D;
	((unsigned char*)0x0050F04D)[4] = 0x00;

	*(float*)(0x0050EDCA + 0x06) = 512.0f;
	engineGlowsSetting = currentEngineGlowsSetting;

	FreeSkirmishImage();

	XwaOffScreenSurfaceLock();
}

int LoadDatImageHook(int* params)
{
	const char* name = (const char*)params[11];
	const short groupId = (short)(params[12] & 0xffff);
	const short imageId = (short)(params[13] & 0xffff);
	const short count = (short)(params[14] & 0xffff);

	//OutputDebugString((std::string("Load DAT Image: ") + name + " " + std::to_string(groupId) + "-" + std::to_string(imageId) + " " + std::to_string(count)).c_str());

	if (g_config.HDConcourseEnabled)
	{
		for (auto& bitmap : g_dataBitmaps)
		{
			bitmap.second->Release();
		}

		g_dataBitmaps.clear();

		g_netFunctions._loadDatImage(name, groupId, imageId, count);
	}

	return 0x7FFFFFFF;
}

int LoadCbmImageHook(int* params)
{
	const char* fileName = (const char*)params[0];
	const char* fileMode = (const char*)params[1];
	const char* name = (const char*)params[92];

	const auto XwaFileOpen = (int(*)(const char*, const char*))0x0052AD30;

	if (g_config.HDConcourseEnabled)
	{
		for (auto& bitmap : g_dataBitmaps)
		{
			bitmap.second->Release();
		}

		g_dataBitmaps.clear();

		g_netFunctions._loadCbmImage(name, fileName);
	}

	return XwaFileOpen(fileName, fileMode);
}

int FrontResFreeItemHook(int* params)
{
	const char* name = (const char*)params[0];

	const auto XwaFrontResIndexOf = (int(*)(const char*))0x00537540;

	if (g_config.HDConcourseEnabled)
	{
		for (auto& bitmap : g_dataBitmaps)
		{
			bitmap.second->Release();
		}

		g_dataBitmaps.clear();

		g_netFunctions._freeImage(name);
	}

	return XwaFrontResIndexOf(name);
}

int FrontResDrawHook(int* params)
{
	const int sourceAddress = params[1];
	const char* name = (const char*)params[2];
	const short positionX = (short)params[3];
	const short positionY = (short)params[4];

	if (name == nullptr)
	{
		return -1;
	}

	const auto XwaFrontResIndexOf = (int(*)(const char*))0x00537540;

	const auto XwaDrawFillRectangle = (void(*)(const RECT*, int, int, unsigned int))0x00558D90;
	const auto XwaDrawRectangleBorder = (void(*)(const RECT*, int, int, unsigned int))0x005590D0;
	const auto XwaFrontResGetArea = (bool(*)(const char*, RECT*))0x00532180;

	const int frameIndex = *(int*)(0x009F60E0 + 0x2B361);

	if (g_config.HDConcourseEnabled && frameIndex == 0)
	{
		if (strcmp(name, "planet") == 0 || strcmp(name, "concourse") == 0)
		{
			return -1;
		}
	}

	if (AreDrawPanelsRemoved())
	{
		if (strstr(name, "leftbar") == name || strstr(name, "rightbar") == name)
		{
			return -1;
		}
	}

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	if (g_config.SkirmishOptEnabled && strcmp(name, "shipbmp") == 0)
	{
		FreeSkirmishImage();

		int craftId = *(int*)0x007838A0 - 0x4E20;

		if (craftId <= 0 || !hasDC || frameIndex == 0)
		{
			return -1;
		}

		DrawSkirmishImage(dc, craftId);

		return -1;
	}

	const CbmHeader* cbmHeader = GetCbmHeaderByName(name);

	if (hasDC && cbmHeader)
	{
		bool isDrawScrollBar = sourceAddress >= 0x005555D0 && sourceAddress < 0x00555CF0;

		// todo
		if (strcmp(name, "dialogbox") == 0)
		{
			dc.d2d1RenderTarget->BeginDraw();
			//dc.d2d1RenderTarget->Clear(D2D1::ColorF(0x00C450));
			dc.d2d1RenderTarget->Clear(D2D1::ColorF(0));
			dc.d2d1RenderTarget->EndDraw();
		}

		int originReturnAddress = params[Params_ReturnAddress];

		if (isDrawScrollBar)
		{
			originReturnAddress = 0x10;
		}

		int result = g_netFunctions._frontResDrawItemImage(name, positionX, positionY, cbmHeader->CurrentImage, (int)&dc, originReturnAddress);

		// todo
		if (strcmp(name, "dialogbox") == 0)
		{
			RECT area;
			XwaFrontResGetArea("dialogbox", &area);

			RECT rc;
			unsigned int color = 0x0000;

			rc = { 0, 0, 640, area.top };
			XwaDrawFillRectangle(&rc, 0, 0, color);
			rc = { 0, area.bottom, 640, 480 };
			XwaDrawFillRectangle(&rc, 0, 0, color);
			rc = { 0, 0, area.left, 480 };
			XwaDrawFillRectangle(&rc, 0, 0, color);
			rc = { area.right, 0, 640, 480 };
			XwaDrawFillRectangle(&rc, 0, 0, color);
		}

		if (result == 1)
		{
			return -1;
		}
	}

	return XwaFrontResIndexOf(name);
}

int FrontResDrawCurrentImageHook(int* params)
{
	const char* name = (const char*)params[2];
	const short positionX = (short)params[3];
	const short positionY = (short)params[4];

	if (name == nullptr)
	{
		return -1;
	}

	const auto XwaFrontResIndexOf = (int(*)(const char*))0x00537540;

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	const CbmHeader* cbmHeader = GetCbmHeaderByName(name);

	if (hasDC && cbmHeader)
	{
		if (strcmp(name, "background") == 0 || strcmp(name, "backconfig") == 0)
		{
			dc.d2d1RenderTarget->BeginDraw();
			dc.d2d1RenderTarget->Clear(D2D1::ColorF(0));
			dc.d2d1RenderTarget->EndDraw();
		}

		int result = g_netFunctions._frontResDrawItemCurrentImage(name, positionX, positionY, cbmHeader->CurrentImage, (int)&dc, params[Params_ReturnAddress]);

		if (result == 1)
		{
			return -1;
		}
	}

	return XwaFrontResIndexOf(name);
}

int FrontResDrawCurrentImageAddHook(int* params)
{
	const char* name = (const char*)params[2];
	const short positionX = (short)params[3];
	const short positionY = (short)params[4];

	if (name == nullptr)
	{
		return -1;
	}

	const auto XwaFrontResIndexOf = (int(*)(const char*))0x00537540;

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	const CbmHeader* cbmHeader = GetCbmHeaderByName(name);

	if (hasDC && cbmHeader)
	{
		int result = g_netFunctions._frontResDrawItemCurrentImage(name, positionX, positionY, cbmHeader->CurrentImage, (int)&dc, params[Params_ReturnAddress]);

		if (result == 1)
		{
			return -1;
		}
	}

	return XwaFrontResIndexOf(name);
}

void RectRotation(RECT* rc, const RECT* srcRect, short positionX, short positionY, short rotation)
{
	const auto XwaRectMove = (void(*)(RECT*, int, int))0x00558CD0;

	*rc = *srcRect;
	XwaRectMove(rc, -rc->left, -rc->top);

	switch (rotation)
	{
	case 1:
	{
		int w = rc->right - rc->left;
		int h = rc->bottom - rc->top;
		rc->right = rc->left + h;
		rc->bottom = rc->top + w;
		break;
	}

	case 3:
	{
		int w = rc->right - rc->left;
		int h = rc->bottom - rc->top;
		rc->right = rc->left + h;
		rc->bottom = rc->top + w;
		break;
	}
	}

	XwaRectMove(rc, positionX, positionY);
}

void RectRotationBack(RECT* rc, const RECT* rc2, const RECT* srcRect, short& positionX, short& positionY, short rotation)
{
	const auto XwaRectMove = (void(*)(RECT*, int, int))0x00558CD0;

	switch (rotation)
	{
	case 1:
	{
		int w = rc->right - rc->left;
		int h = rc->bottom - rc->top;
		rc->right = rc->left + h;
		rc->bottom = rc->top + w;
		break;
	}

	case 3:
	{
		int w = rc->right - rc->left;
		int h = rc->bottom - rc->top;
		rc->right = rc->left + h;
		rc->bottom = rc->top + w;
		break;
	}
	}

	int deltaX = (srcRect->right - srcRect->left) - (rc->right - rc->left);
	int deltaY = (srcRect->bottom - srcRect->top) - (rc->bottom - rc->top);

	if (rc->left != rc2->left)
	{
		switch (rotation)
		{
		case 5:
			// 0°
			XwaRectMove(rc, deltaX, 0);
			positionX += deltaX;
			break;

		case 1:
			// -90°
			XwaRectMove(rc, 0, deltaY);
			positionX += deltaY;
			break;

		case 2:
			// -180°
			positionX += deltaX;
			break;

		case 3:
			// -270°
			positionX += deltaY;
			break;

		case 4:
			// flip right left
			positionX += deltaX;
			break;
		}
	}
	else if (rc->right != rc2->right)
	{
		switch (rotation)
		{
		case 5:
			// 0°
			break;

		case 1:
			// -90°
			break;

		case 2:
			// -180°
			XwaRectMove(rc, deltaX, 0);
			break;

		case 3:
			// -270°
			XwaRectMove(rc, 0, deltaY);
			break;

		case 4:
			// flip right left
			XwaRectMove(rc, deltaX, 0);
			break;
		}
	}
	else if (rc->top != rc2->top)
	{
		switch (rotation)
		{
		case 5:
			// 0°
			XwaRectMove(rc, 0, deltaY);
			positionY += deltaY;
			break;

		case 1:
			// -90°
			positionY += deltaX;
			break;

		case 2:
			// -180°
			positionY += deltaY;
			break;

		case 3:
			// -270°
			XwaRectMove(rc, deltaX, 0);
			positionY += deltaX;
			break;

		case 4:
			// flip right left
			XwaRectMove(rc, 0, deltaY);
			positionY += deltaY;
			break;
		}
	}
	else if (rc->bottom != rc2->bottom)
	{
		switch (rotation)
		{
		case 5:
			// 0°
			break;

		case 1:
			// -90°
			XwaRectMove(rc, deltaX, 0);
			break;

		case 2:
			// -180°
			XwaRectMove(rc, 0, deltaY);
			break;

		case 3:
			// -270°
			break;

		case 4:
			// flip right left
			XwaRectMove(rc, deltaX, 0);
			break;
		}
	}

	XwaRectMove(rc, -positionX, -positionY);
	XwaRectMove(rc, srcRect->left, srcRect->top);
}

int FrontResDrawMapIconHook(int* params)
{
	const char* name = (const char*)params[2];
	const RECT* srcRect = (RECT*)params[3];
	short positionX = (short)params[4];
	short positionY = (short)params[5];
	short rotation = (short)params[6];

	if (name == nullptr)
	{
		return -1;
	}

	if (rotation == 0)
	{
		rotation = 5;
	}

	const auto XwaFrontResIndexOf = (int(*)(const char*))0x00537540;
	const auto XwaRectRestrictToScreenArea = (int(*)(RECT*))0x00558D10;
	const auto XwaRectMove = (void(*)(RECT*, int, int))0x00558CD0;

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	const CbmHeader* cbmHeader = GetCbmHeaderByName(name);

	if (hasDC && cbmHeader)
	{
		RECT rc;
		RectRotation(&rc, srcRect, positionX, positionY, rotation);
		RECT rc2 = rc;
		int r = XwaRectRestrictToScreenArea(&rc);
		RECT rc1 = rc;

		if (rc.right - rc.left <= 0 || rc.bottom - rc.top <= 0)
		{
			return -1;
		}

		RectRotationBack(&rc, &rc2, srcRect, positionX, positionY, rotation);

		int result = g_netFunctions._frontResDrawMapIcon(name, rc.left, rc.top, rc.right, rc.bottom, positionX, positionY, rotation, (int)&dc, params[Params_ReturnAddress]);

		if (result == 1)
		{
			return -1;
		}
	}

	return XwaFrontResIndexOf(name);
}

int FrontResDrawGreyMapIconHook(int* params)
{
	const char* name = (const char*)params[2];
	const RECT* srcRect = (RECT*)params[3];
	short positionX = (short)params[4];
	short positionY = (short)params[5];
	unsigned int blendColor = (unsigned int)params[6];
	short rotation = (short)params[7];

	if (name == nullptr)
	{
		return -1;
	}

	if (rotation == 0)
	{
		rotation = 5;
	}

	const auto XwaFrontResIndexOf = (int(*)(const char*))0x00537540;
	const auto XwaRectRestrictToScreenArea = (int(*)(RECT*))0x00558D10;

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	const CbmHeader* cbmHeader = GetCbmHeaderByName(name);

	if (hasDC && cbmHeader)
	{
		RECT rc;
		RectRotation(&rc, srcRect, positionX, positionY, rotation);
		RECT rc2 = rc;
		int r = XwaRectRestrictToScreenArea(&rc);
		RECT rc1 = rc;

		if (rc.right - rc.left <= 0 || rc.bottom - rc.top <= 0)
		{
			return -1;
		}

		RectRotationBack(&rc, &rc2, srcRect, positionX, positionY, rotation);

		blendColor = Color16BppTo32Bpp(blendColor);

		int result = g_netFunctions._frontResDrawBlendColor(name, rc.left, rc.top, rc.right, rc.bottom, positionX, positionY, rotation, blendColor, (int)&dc, params[Params_ReturnAddress]);

		if (result == 1)
		{
			return -1;
		}
	}

	return XwaFrontResIndexOf(name);
}

int FrontResDrawIconHook(int* params)
{
	const char* name = (const char*)params[2];
	const RECT* srcRect = (RECT*)params[3];
	const short positionX = (short)params[4];
	const short positionY = (short)params[5];
	unsigned int blendColor = (unsigned short)params[6];

	if (name == nullptr)
	{
		return -1;
	}

	const auto XwaFrontResIndexOf = (int(*)(const char*))0x00537540;

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	const CbmHeader* cbmHeader = GetCbmHeaderByName(name);

	if (hasDC && cbmHeader)
	{
		blendColor = Color16BppTo32Bpp(blendColor);

		int result = g_netFunctions._frontResDrawBlendColor(name, srcRect->left, srcRect->top, srcRect->right, srcRect->bottom, positionX, positionY, 0, blendColor, (int)&dc, params[Params_ReturnAddress]);

		if (result == 1)
		{
			return -1;
		}
	}

	return XwaFrontResIndexOf(name);
}

int FrontResIconRectHook(int* params)
{
	RECT* pRect = (RECT*)params[0];
	RECT* A4 = (RECT*)params[1];
	const char* A8 = (const char*)params[17];

	const auto XwaRectCopy = (void(*)(RECT*, RECT*))0x00558CB0;
	const auto XwaFrontResGetArea = (bool(*)(const char*, RECT*))0x00532180;

	XwaRectCopy(pRect, A4);

	RECT area;
	XwaFrontResGetArea(A8, &area);

	pRect->right = pRect->left + (area.right - area.left);
	pRect->bottom = pRect->top + (area.bottom - area.top);

	return 0;
}

int DrawCursorMessageHook(int* params)
{
	int A4 = params[0];
	const char* A8 = (const char*)params[1];
	LPRECT AC = (LPRECT)params[2];
	unsigned int A10 = (unsigned int)params[3];

	const auto XwaUILabel = (int(*)(int, const char*, LPRECT, unsigned int))0x005575A0;

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	if (!hasDC)
	{
		return XwaUILabel(A4, A8, AC, A10);
	}

	bool isCombatMultiGameStateUpdate = IsUpdateCallback(0x0053C120);

	bool isBackgroundHD = g_netFunctions._frontResIsBackgroundWide() != 0;
	int offsetX = MultiplyDivOffsetXWide(dc);
	IDWriteTextFormat* textFormat = g_fontResources.GetTextFormat(MultiplyDivV(A4, dc) * (isBackgroundHD ? 1.0f : 0.75f));

	std::wstring textW = string_towstring(A8);

	float left = MultiplyDivH(AC->left + 2, isBackgroundHD, dc);
	float top = MultiplyDivV(AC->top, dc);
	float right = MultiplyDivH(AC->right, isBackgroundHD, dc);
	float bottom = MultiplyDivV(AC->bottom, dc);

	if (!isCombatMultiGameStateUpdate)
	{
		right = left + (right - left) * 0.75f;
	}

	D2D1_RECT_F layoutRect
	{
		left + offsetX,
		top,
		right + offsetX,
		bottom,
	};

	ComPtr<ID2D1SolidColorBrush> brush;
	dc.d2d1RenderTarget->CreateSolidColorBrush(D2D1::ColorF(0xFFFF00), &brush);

	dc.d2d1RenderTarget->BeginDraw();

	dc.d2d1RenderTarget->DrawTextA(
		textW.c_str(),
		textW.size(),
		textFormat,
		layoutRect,
		brush,
		D2D1_DRAW_TEXT_OPTIONS_NONE);

	dc.d2d1RenderTarget->EndDraw();

	return 0;
}

int DrawFillRectangleHook(int* params)
{
	RECT* A4 = (RECT*)params[1];
	int A8 = params[2];
	int AC = params[3];
	unsigned int A10 = (unsigned int)params[4];

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	if (!hasDC)
	{
		params[Params_EAX] = (int)A4;
		params[Params_ECX] = A4->right;
		return 0;
	}

	params[Params_ReturnAddress] = 0x00558EC4;

	const auto XwaRectRestrictToScreenArea = (int(*)(RECT*))0x00558D10;
	XwaRectRestrictToScreenArea(A4);

	if (A4->right - A4->left <= 0 || A4->bottom - A4->top <= 0)
	{
		return 0;
	}

	bool isBackgroundHD = g_netFunctions._frontResIsBackgroundHD() != 0;
	bool isBackgroundWide = g_netFunctions._frontResIsBackgroundWide() != 0;
	int offsetX = MultiplyDivOffsetX(dc);

	D2D1_RECT_F layoutRect
	{
		MultiplyDivH(A8 + A4->left, isBackgroundHD, dc) + offsetX,
		MultiplyDivV(AC + A4->top, dc),
		MultiplyDivH(A8 + A4->right, isBackgroundHD, dc) + offsetX,
		MultiplyDivV(AC + A4->bottom, dc),
	};

	if (isBackgroundHD)
	{
		bool isFamilyRoomGameGameStateUpdate = IsUpdateCallback(0x0055FF30);
		int s_V0x0783B64 = *(int*)0x00783B64;
		bool isFamilyAwardDetail = isFamilyRoomGameGameStateUpdate && s_V0x0783B64 == 0x02;
		bool isBriefingGameStateUpdate = IsUpdateCallback(0x00564E90);
		bool isEnterFilmFilenameGameStateUpdate = IsUpdateCallback(0x00541260);
		bool isCombatRoomGameStateUpdate = IsUpdateCallback(0x0053B500);
		bool isShowMedalGameStateUpdate = IsUpdateCallback(0x0057ECE0) && *(int*)0x0784BCC != 0;

		if (isEnterFilmFilenameGameStateUpdate)
		{
			layoutRect.right = layoutRect.left + (layoutRect.right - layoutRect.left) * 1.1f;
		}
		else if (isCombatRoomGameStateUpdate)
		{
			layoutRect.left = layoutRect.left + (layoutRect.right - layoutRect.left) * 0.2f;
			layoutRect.right = layoutRect.left + (layoutRect.right - layoutRect.left) * 0.8f;
		}
		else if (isShowMedalGameStateUpdate)
		{
			layoutRect.right = layoutRect.left + (layoutRect.right - layoutRect.left) * 0.8f;
			layoutRect.left = layoutRect.left + (layoutRect.right - layoutRect.left) * 0.2f;
		}
		else if (!isFamilyAwardDetail && !isBriefingGameStateUpdate)
		{
			layoutRect.right = layoutRect.left + (layoutRect.right - layoutRect.left) * 0.8f;

			if (!isBackgroundWide)
			{
				layoutRect.left = layoutRect.left + (layoutRect.right - layoutRect.left) * 0.2f;
			}
		}
	}

	unsigned int color = Color16BppTo32Bpp(A10);

	ComPtr<ID2D1SolidColorBrush> brush;
	dc.d2d1RenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, 0.7f), &brush);

	dc.d2d1RenderTarget->BeginDraw();

	dc.d2d1RenderTarget->FillRectangle(layoutRect, brush);

	dc.d2d1RenderTarget->EndDraw();

	return 0;
}

int DrawRectangleHook(int* params)
{
	RECT* A4 = (RECT*)params[9];
	int A8 = params[10];
	int AC = params[11];
	unsigned int A10 = (unsigned int)params[12];
	const int callAddress = params[8];

	const auto XwaGetColor = (unsigned int(*)(unsigned char, unsigned char, unsigned char))0x0055D270;

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	if (!hasDC)
	{
		params[Params_EAX] = (int)A4;
		params[Params_ECX] = A4->right;
		return 0;
	}

	params[Params_ReturnAddress] = 0x005590C4;

	const auto XwaRectRestrictToScreenArea = (int(*)(RECT*))0x00558D10;
	XwaRectRestrictToScreenArea(A4);

	if (A4->right - A4->left <= 0 || A4->bottom - A4->top <= 0)
	{
		return 0;
	}

	bool isBackgroundHD = g_netFunctions._frontResIsBackgroundWide() != 0;
	int offsetX = MultiplyDivOffsetXWide(dc);

	int width = A4->right - A4->left;

	D2D1_RECT_F layoutRect;

	if (width == 1)
	{
		layoutRect.left = MultiplyDivH(A8 + A4->left + 2, isBackgroundHD, dc) + offsetX;
		layoutRect.top = MultiplyDivV(AC + A4->top, dc);
		layoutRect.right = MultiplyDivH(A8 + A4->right + 2, isBackgroundHD, dc) + offsetX;
		layoutRect.bottom = MultiplyDivV(AC + A4->bottom, dc);
	}
	else
	{
		unsigned int cursorMessageBorderColor = XwaGetColor(0x60, 0x60, 0x60);
		bool isBriefingGameStateUpdate = IsUpdateCallback(0x00564E90);
		bool isBriefingMapIcons = IsUpdateCallback(0x00564E90) && A10 != cursorMessageBorderColor;
		bool isFamilyRoomCursorMessage = callAddress == 0x0055B9AD;
		bool isCombatMultiGameStateUpdate = IsUpdateCallback(0x0053C120);

		layoutRect.left = MultiplyDivH(A8 + A4->left, isBackgroundHD, dc) + offsetX;
		layoutRect.top = MultiplyDivV(AC + A4->top, dc);
		layoutRect.right = MultiplyDivH(A8 + A4->right, isBackgroundHD, dc) + offsetX;
		layoutRect.bottom = MultiplyDivV(AC + A4->bottom, dc);

		if (isCombatMultiGameStateUpdate)
		{
		}
		else if (isFamilyRoomCursorMessage)
		{
			layoutRect.right = layoutRect.left + (layoutRect.right - layoutRect.left) * 0.75f;
		}
		else if (!isBriefingGameStateUpdate)
		{
			layoutRect.right = layoutRect.left + (layoutRect.right - layoutRect.left) * 0.8f;
		}
		else if (isBriefingMapIcons)
		{
			if (isBackgroundHD)
			{
				layoutRect.right = layoutRect.left + (layoutRect.right - layoutRect.left) * 0.8f;
			}
		}
	}

	unsigned int color = Color16BppTo32Bpp(A10);

	ComPtr<ID2D1SolidColorBrush> brush;
	dc.d2d1RenderTarget->CreateSolidColorBrush(D2D1::ColorF(color), &brush);

	dc.d2d1RenderTarget->BeginDraw();

	dc.d2d1RenderTarget->FillRectangle(layoutRect, brush);

	dc.d2d1RenderTarget->EndDraw();

	return 0;
}

int DrawRectangleBorderHook(int* params)
{
	RECT* A4 = (RECT*)params[13];
	int A8 = params[14];
	int AC = params[15];
	unsigned int A10 = (unsigned short)params[16];

	const auto XwaGetColor = (unsigned int(*)(unsigned char, unsigned char, unsigned char))0x0055D270;

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	if (!hasDC)
	{
		params[Params_EAX] = (int)A4;
		params[Params_ECX] = A4->right;
		return 0;
	}

	params[Params_EAX] = 0;
	params[Params_ReturnAddress] = 0x0055924A;

	bool isConfigMenuGameStateUpdate = IsUpdateCallback(0x0051D100);
	bool isConfigMenuGameStateUpdate_ViewCutscenes = isConfigMenuGameStateUpdate && *(int*)0x007829B8 == 15;

	if (!isConfigMenuGameStateUpdate || isConfigMenuGameStateUpdate_ViewCutscenes)
	{
		const auto XwaRectRestrictToScreenArea = (int(*)(RECT*))0x00558D10;
		XwaRectRestrictToScreenArea(A4);
	}

	if (A4->right - A4->left <= 0 || A4->bottom - A4->top <= 0)
	{
		return 0;
	}

	bool isBackgroundHD = g_netFunctions._frontResIsBackgroundWide() != 0;
	int offsetX = MultiplyDivOffsetXWide(dc);

	D2D1_RECT_F layoutRect
	{
		MultiplyDivH(A8 + A4->left, isBackgroundHD, dc) + offsetX,
		MultiplyDivV(AC + A4->top, dc),
		MultiplyDivH(A8 + A4->right, isBackgroundHD, dc) + offsetX,
		MultiplyDivV(AC + A4->bottom, dc),
	};

	bool isTechLibraryGameStateUpdate = IsUpdateCallback(0x00574D70);
	unsigned int cursorMessageBorderColor = XwaGetColor(0x60, 0x60, 0x60);
	bool isBriefingGameStateUpdate = IsUpdateCallback(0x00564E90) && A10 == cursorMessageBorderColor;
	bool isBriefingMapIcons = IsUpdateCallback(0x00564E90) && A10 != cursorMessageBorderColor;
	bool isDebriefingGameStateUpdate = IsUpdateCallback(0x0057ECE0);
	bool isCombatSingleGameStateUpdate = IsUpdateCallback(0x005438B0);
	bool isCombatMultiGameStateUpdate = IsUpdateCallback(0x0053C120);
	bool isFamilyRoomGameGameStateUpdate = IsUpdateCallback(0x0055FF30);
	int s_V0x0783B64 = *(int*)0x00783B64;
	int s_V0x0783B0C = *(int*)0x00783B0C;
	bool isFamilyRoomEmails = isFamilyRoomGameGameStateUpdate && s_V0x0783B64 == 1 && s_V0x0783B0C == 3;
	bool isFamilyRoomMonitor = isFamilyRoomGameGameStateUpdate && s_V0x0783B64 == 1 && *(unsigned int*)0x0ABD234 != A10;
	bool isFamilyRoomMain = isFamilyRoomGameGameStateUpdate && s_V0x0783B64 == 0;
	bool isFamilyRoomAward = isFamilyRoomGameGameStateUpdate && s_V0x0783B64 == 2;
	bool isFamilyRoomMedalCase = isFamilyRoomGameGameStateUpdate && s_V0x0783B64 == 3;
	bool isFamilyRoomRank = isFamilyRoomGameGameStateUpdate && s_V0x0783B64 == 4;
	bool isMessageBoxGameStateUpdate = IsUpdateCallback(0x005595A0);
	bool isFilmRoomGameGameStateUpdate = IsUpdateCallback(0x00573020);
	bool isTourOfDutyRoomGameStateUpdate = IsUpdateCallback(0x005775E0);

	if (isFamilyRoomMain || isFamilyRoomRank || isFilmRoomGameGameStateUpdate)
	{
		layoutRect.right = layoutRect.left + (layoutRect.right - layoutRect.left) * 0.75f;
	}
	else if (isMessageBoxGameStateUpdate
		|| isTechLibraryGameStateUpdate
		|| isBriefingGameStateUpdate
		|| isDebriefingGameStateUpdate
		|| isCombatSingleGameStateUpdate
		|| isFamilyRoomMonitor
		|| isFamilyRoomAward
		|| isFamilyRoomMedalCase
		|| isTourOfDutyRoomGameStateUpdate)
	{
		layoutRect.right = layoutRect.left + (layoutRect.right - layoutRect.left) * 0.75f;
	}

	if (isBriefingMapIcons)
	{
		if (isBackgroundHD)
		{
			layoutRect.right = layoutRect.left + (layoutRect.right - layoutRect.left) * 0.8f;
		}
	}

	unsigned int color = Color16BppTo32Bpp(A10);

	ComPtr<ID2D1SolidColorBrush> brush;
	dc.d2d1RenderTarget->CreateSolidColorBrush(D2D1::ColorF(color), &brush);

	dc.d2d1RenderTarget->BeginDraw();

	dc.d2d1RenderTarget->DrawRectangle(layoutRect, brush);

	dc.d2d1RenderTarget->EndDraw();

	return 0;
}

int DrawBriefingGridVerticalHook(int* params)
{
	int top = params[0];
	int bottom = params[1];
	int x = params[2];
	unsigned short color = (unsigned short)params[3];

	const auto L005435E0 = (void(*)(int, int, int, unsigned short))0x005435E0;
	const auto XwaRetrieveScreenArea = (void(*)(RECT*))0x0053F730;

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	if (!hasDC)
	{
		L005435E0(top, bottom, x, color);
		return 0;
	}

	RECT area;
	XwaRetrieveScreenArea(&area);

	if (top > area.bottom || bottom < area.top || x < area.left || x > area.right)
	{
		return 0;
	}

	bool isBackgroundHD = g_netFunctions._frontResIsBackgroundWide() != 0;
	int offsetX = MultiplyDivOffsetXWide(dc);

	top = MultiplyDivV(top, dc);
	bottom = MultiplyDivV(bottom, dc);
	x = MultiplyDivH(x, isBackgroundHD, dc) + offsetX;

	ComPtr<ID2D1SolidColorBrush> brush;
	dc.d2d1RenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0.0f, 0.0f), &brush);

	dc.d2d1RenderTarget->BeginDraw();

	dc.d2d1RenderTarget->DrawLine(D2D1::Point2F(x, top), D2D1::Point2F(x, bottom), brush);

	dc.d2d1RenderTarget->EndDraw();

	return 0;
}

int DrawBriefingGridHorizontalHook(int* params)
{
	int left = params[0];
	int right = params[1];
	int y = params[2];
	unsigned short color = (unsigned short)params[3];

	const auto L00543490 = (void(*)(int, int, int, unsigned short))0x00543490;
	const auto XwaRetrieveScreenArea = (void(*)(RECT*))0x0053F730;

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	if (!hasDC)
	{
		L00543490(left, right, y, color);
		return 0;
	}

	RECT area;
	XwaRetrieveScreenArea(&area);

	if (left > area.right || right < area.left || y < area.top || y > area.bottom)
	{
		return 0;
	}

	bool isBackgroundHD = g_netFunctions._frontResIsBackgroundWide() != 0;
	int offsetX = MultiplyDivOffsetXWide(dc);

	left = MultiplyDivH(left, isBackgroundHD, dc) + offsetX;
	right = MultiplyDivH(right, isBackgroundHD, dc) + offsetX;
	y = MultiplyDivV(y, dc);

	ComPtr<ID2D1SolidColorBrush> brush;
	dc.d2d1RenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0.0f, 0.0f), &brush);

	dc.d2d1RenderTarget->BeginDraw();

	dc.d2d1RenderTarget->DrawLine(D2D1::Point2F(left, y), D2D1::Point2F(right, y), brush);

	dc.d2d1RenderTarget->EndDraw();

	return 0;
}

int DrawLineHook(int* params)
{
	int left = params[9];
	int top = params[10];
	int right = params[11];
	int bottom = params[12];
	unsigned int color = (unsigned short)params[13];

	const auto XwaRetrieveScreenArea = (void(*)(RECT*))0x0053F730;

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	if (!hasDC)
	{
		*(int*)(params[Params_EBP] - 8) = 0x800;
		return 0;
	}

	params[Params_ReturnAddress] = 0x00542CC3;

	RECT area;
	XwaRetrieveScreenArea(&area);

	if (left > area.right || right < area.left || top > area.bottom || bottom < area.top)
	{
		return 0;
	}

	bool isBackgroundHD = g_netFunctions._frontResIsBackgroundWide() != 0;
	int offsetX = MultiplyDivOffsetXWide(dc);

	left = MultiplyDivH(left, isBackgroundHD, dc) + offsetX;
	top = MultiplyDivV(top, dc);
	right = MultiplyDivH(right, isBackgroundHD, dc) + offsetX;
	bottom = MultiplyDivV(bottom, dc);

	color = Color16BppTo32Bpp(color);

	ComPtr<ID2D1SolidColorBrush> brush;
	dc.d2d1RenderTarget->CreateSolidColorBrush(D2D1::ColorF(color), &brush);

	dc.d2d1RenderTarget->BeginDraw();

	dc.d2d1RenderTarget->DrawLine(D2D1::Point2F(left, top), D2D1::Point2F(right, bottom), brush);

	dc.d2d1RenderTarget->EndDraw();

	return 0;
}

void BriefingZoom1(int A4, int A8, int AC)
{
	const auto L005665F0 = (void(*)(int, int, int))0x005665F0;
	L005665F0(A4, A8, AC);

	const auto XwaBriefingZoomEnd = (void(*)())0x00448660;
	XwaBriefingZoomEnd();
}

int DrawBriefingBackgroundHook(int* params)
{
	if (g_config.HDConcourseEnabled)
	{
		// :00565A9F E84C0B0000              call 005665F0
		*(unsigned char*)(0x00565A9F + 0x00) = 0xE8;
		*(unsigned int*)(0x00565A9F + 0x01) = (int)BriefingZoom1 - (0x00565A9F + 0x05);

		//// 0050EC70 -  render opt
		//*(unsigned short*)(0x0050EC70 + 0x00) = 0xC033;
		//*(unsigned char*)(0x0050EC70 + 0x02) = 0xC3;

		const auto XwaBriefingZoomBegin = (void(*)(int))0x00489310;
		XwaBriefingZoomBegin(1);

		const auto XwaFrontResDraw = (int(*)(const char*, int, int))0x00534A60;
		const auto XwaFrontResDrawCurrentImage = (int(*)(const char*, int, int))0x00535470;

		XwaFrontResDrawCurrentImage("background", 0, 0);

		int xwaPilot_campaignMode = *(int*)(0x0AE2A60 + 0x250F3);
		int TieMissionType_Family = 0x07;

		unsigned char missionType = *(unsigned char*)(*(int*)0x009EB8E0 + 0x0B1B82);

		if (xwaPilot_campaignMode != 0)
		{
			if (missionType == TieMissionType_Family)
			{
				XwaFrontResDraw("briefbarfam", 0x3C, 0x174);
			}
			else
			{
				XwaFrontResDraw("briefbartour", 0x3C, 0x174);
			}
		}
		else
		{
			XwaFrontResDraw("briefbarcom", 0x3C, 0x174);
		}
	}

	return *(unsigned char*)0x00B0C7BC;
}

int DrawBriefingBottomTextHook(int* params)
{
	RECT* A4 = (RECT*)params[0];
	const char* A8 = (const char*)params[1];
	const int AC = params[2];

	const auto L00569BE0 = (void(*)(const RECT*, const char*, int))0x00569BE0;

	L00569BE0(A4, A8, AC);

	return 0;
}

int DrawConfigMenuBackHook(int* params)
{
	const RECT* pArea = (RECT*)params[0];

	const auto XwaSetScreenArea = (void(*)(const RECT*))0x0053F790;
	const auto XwaDrawBackConfig = (int(*)())0x00526CB0;

	XwaSetScreenArea(pArea);

	if (g_config.HDConcourseEnabled)
	{
		XwaDrawBackConfig();
	}

	return 0;
}

int DrawMessageBoxBackgroundHook(int* params)
{
	const auto L005599D0 = (int(*)())0x005599D0;
	const auto XwaOffScreenSurfaceLock = (int(*)())0x0053F830;
	const auto XwaOffScreenSurfaceUnlock = (int(*)(int))0x0053F8D0;
	const auto XwaFrontResDraw = (int(*)(const char*, short, short))0x00534A60;
	const auto XwaFrontResGetArea = (bool(*)(const char*, RECT*))0x00532180;
	const auto XwaRectMove = (void(*)(RECT*, int, int))0x00558CD0;
	const auto XwaFrontResDrawCurrentImage = (void(*)(const char*, int, int))0x00535470;

	bool isPreviousCombatMultiGameStateUpdate = IsPreviousUpdateCallback(0x0053C120);

	if (g_config.HDConcourseEnabled)
	{
		if (isPreviousCombatMultiGameStateUpdate)
		{
			XwaOffScreenSurfaceLock();
			//XwaFrontResDrawCurrentImage("background", 0, 0);
			XwaFrontResDraw("dialogbox", 0, 0);
			RECT area;
			XwaFrontResGetArea("igz", &area);
			int x = 0x140 - (area.right - area.left + 1) / 2;
			XwaRectMove(&area, x, 0x154);
			XwaFrontResDraw("igz", x, 0x168);
			XwaOffScreenSurfaceUnlock(0x01);
		}
		else
		{
			XwaOffScreenSurfaceLock();
			XwaFrontResDraw("dialogbox", 0, 0);
			XwaOffScreenSurfaceUnlock(0x01);
		}
	}

	return L005599D0();
}

int DrawTechLibraryBackgroundHook(int* params)
{
	const auto L00540D30 = (int(*)())0x00540D30;
	const auto XwaFrontResDraw = (int(*)(const char*, int, int))0x00534A60;
	const auto XwaFrontResDrawCurrentImage = (int(*)(const char*, int, int))0x00535470;

	if (g_config.HDConcourseEnabled)
	{
		XwaFrontResDrawCurrentImage("globe", 0, 0);
		XwaFrontResDraw("techroom", 0, 0);
	}

	return L00540D30();
}

int DrawFilmRoomBackgroundHook(int* params)
{
	const auto XwaFrontResDraw = (int(*)(const char*, int, int))0x00534A60;
	const auto XwaFrontResDrawCurrentImage = (int(*)(const char*, int, int))0x00535470;

	if (g_config.HDConcourseEnabled)
	{
		XwaFrontResDrawCurrentImage("background", 0, 0);
		XwaFrontResDraw("settingbar", 0x39, 0x15C);
	}

	if (*(int*)0x00783FEC == 0)
	{
		params[Params_ReturnAddress] = 0x005733EF;
	}

	return 0;
}

int DrawCreatePilotBackgroundHook(int* params)
{
	RECT* rc = (RECT*)params[0];
	int left = params[1];
	int top = params[2];
	int right = params[3];
	int bottom = params[4];

	const auto XwaRectSet = (void(*)(RECT*, int, int, int, int))0x00558C90;
	const auto XwaFrontResDrawCurrentImage = (int(*)(const char*, int, int))0x00535470;

	if (g_config.HDConcourseEnabled)
	{
		XwaFrontResDrawCurrentImage("backname", 0, 0);

		//int left = 0x000000FA;
		//int top = 0x000000F5;
		//int right = 0x000001B8;
		//int bottom = 0x00000109;

		int left = 640 / 2 - 15;
		int right = left + 120;
		int top = 250 + 10;
		int bottom = top + 20;

		*(int*)(0x00559C0C + 0x01) = left;
		*(int*)(0x00559C03 + 0x01) = top;
		*(int*)(0x00559BFE + 0x01) = right;
		*(int*)(0x00559BF9 + 0x01) = bottom;
	}

	XwaRectSet(rc, left, top, right, bottom);

	return 0;
}

int DrawCombatSingleGameStateBackgroundHook(int* params)
{
	const auto XwaFrontResDraw = (int(*)(const char*, int, int))0x00534A60;
	const auto XwaFrontResDrawCurrentImage = (int(*)(const char*, int, int))0x00535470;
	const auto L00545E70 = (void(*)())0x00545E70;

	if (g_config.HDConcourseEnabled)
	{
		XwaFrontResDraw("settingbar", 0x39, 0x15C);

		L00545E70();
	}

	if (*(int*)0x007830B0 == 0)
	{
		params[Params_ReturnAddress] = 0x00544557;
	}
	else
	{
		if (g_config.HDConcourseEnabled)
		{
			params[Params_ReturnAddress] = 0x0054444B;
		}
	}

	if (*(int*)0x007830B0 == 0)
	{
		params[Params_ReturnAddress] = 0x00544557;
	}

	return 0;
}

int DrawSkirmishShipbmpBackgroundHook(int* params)
{
	const auto L0054CF90 = (void(*)(int))0x0054CF90;
	const auto XwaFreeShipImage = (int(*)())0x0055DC70;

	XwaFreeShipImage();
	L0054CF90(0);

	return 0;
}

int FamilyRoomBacktoconcoursePositionHook(int* params)
{
	const auto XwaRectSet = (void(*)(RECT*, int, int, int, int))0x00558C90;

	RECT* lpRect = (RECT*)params[0];
	int left = ConcourseMultiplyDivRound(g_concourseDoors.familyroom_backtoconcourse_posX);
	int top = ConcourseMultiplyDivRound(g_concourseDoors.familyroom_backtoconcourse_posY);
	int right = ConcourseMultiplyDivRound(g_concourseDoors.familyroom_backtoconcourse_posX) + g_concourseDoors.familyroom_backtoconcourse_posWidth;
	int bottom = ConcourseMultiplyDivRound(g_concourseDoors.familyroom_backtoconcourse_posY) + g_concourseDoors.familyroom_backtoconcourse_posHeight;

	XwaRectSet(lpRect, left, top, right, bottom);

	return 0;
}

int FamilyRoomStatisticsPositionHook(int* params)
{
	const auto XwaRectSet = (void(*)(RECT*, int, int, int, int))0x00558C90;

	RECT* lpRect = (RECT*)params[0];
	int left = ConcourseMultiplyDivRound(g_concourseDoors.familyroom_statistics_posX);
	int top = ConcourseMultiplyDivRound(g_concourseDoors.familyroom_statistics_posY);
	int right = ConcourseMultiplyDivRound(g_concourseDoors.familyroom_statistics_posX) + g_concourseDoors.familyroom_statistics_posWidth;
	int bottom = ConcourseMultiplyDivRound(g_concourseDoors.familyroom_statistics_posY) + g_concourseDoors.familyroom_statistics_posHeight;

	XwaRectSet(lpRect, left, top, right, bottom);

	return 0;
}

int FamilyRoomPlaymissionPositionHook(int* params)
{
	const auto XwaRectSet = (void(*)(RECT*, int, int, int, int))0x00558C90;

	RECT* lpRect = (RECT*)params[0];
	int left = ConcourseMultiplyDivRound(g_concourseDoors.familyroom_playmission_posX);
	int top = ConcourseMultiplyDivRound(g_concourseDoors.familyroom_playmission_posY);
	int right = ConcourseMultiplyDivRound(g_concourseDoors.familyroom_playmission_posX) + g_concourseDoors.familyroom_playmission_posWidth;
	int bottom = ConcourseMultiplyDivRound(g_concourseDoors.familyroom_playmission_posY) + g_concourseDoors.familyroom_playmission_posHeight;

	XwaRectSet(lpRect, left, top, right, bottom);

	return 0;
}

int FamilyRoomMkeyePositionHook(int* params)
{
	const char* key = (const char*)params[0];

	const auto XwaFrontResDraw = (int(*)(const char*, short, short))0x00534A60;

	return XwaFrontResDraw(key, (short)g_concourseDoors.familyroom_mkeye_posX, (short)g_concourseDoors.familyroom_mkeye_posY);
}

int FamilyRoomMonitorPositionHook(int* params)
{
	const char* key = (const char*)params[0];

	const auto XwaFrontResDraw = (int(*)(const char*, short, short))0x00534A60;
	const auto XwaFrontResSetCurrentImage = (int(*)(const char*, int))0x005322F0;

	const int frame = *(int*)(0x009F60E0 + 0x2B361);
	const bool hasNewEmail = *(int*)0x009F4B18 != 0;

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	if (hasDC)
	{
		int imagesCount = g_netFunctions._getItemImagesCount(key);
		int fps = 24 / g_concourseDoors.familyroom_monitor_fps;

		if (imagesCount >= 2)
		{
			int framesCount = (imagesCount - 1) * fps;
			int index = (frame % framesCount) / fps;
			XwaFrontResSetCurrentImage(key, index);
		}
	}

	return XwaFrontResDraw(key, (short)g_concourseDoors.familyroom_monitor_posX, (short)g_concourseDoors.familyroom_monitor_posY);
}

int FamilyRoomLadyblueMission1Hook(int* params)
{
	int mission = g_concourseDoors.familyroom_ladyblue_mission;
	int value = mission <= 0 ? 1 : *(int*)(0xAED76E + (mission - 1) * 0x30);

	if (value == 0)
	{
		params[Params_ReturnAddress] = 0x00563FDA;
	}

	return 0;
}

int FamilyRoomLadyblueMission2Hook(int* params)
{
	int mission = g_concourseDoors.familyroom_ladyblue_mission;
	int* value = (int*)params[Params_EDX];

	if (*value == mission)
	{
		params[Params_ReturnAddress] = 0x00563FD5;
	}

	return 0;
}

int FamilyRoomLadyblueMission3Hook(int* params)
{
	int mission = g_concourseDoors.familyroom_ladyblue_mission;
	int value = mission <= 0 ? 1 : *(int*)(0xAED76E + (mission - 1) * 0x30);

	return value;
}

int FamilyRoomCologneMission1Hook(int* params)
{
	int mission = g_concourseDoors.familyroom_cologne_mission;
	int value = mission <= 0 ? 1 : *(int*)(0xAED76E + (mission - 1) * 0x30);

	if (value == 0)
	{
		params[Params_ReturnAddress] = 0x00564235;
	}

	return 0;
}

int FamilyRoomCologneMission2Hook(int* params)
{
	int mission = g_concourseDoors.familyroom_cologne_mission;
	int* value = (int*)params[Params_EDX];

	if (*value == mission)
	{
		params[Params_ReturnAddress] = 0x00564104;
	}

	return 0;
}

int FamilyRoomCologneMission3Hook(int* params)
{
	int mission = g_concourseDoors.familyroom_cologne_mission;
	int value = mission <= 0 ? 1 : *(int*)(0xAED76E + (mission - 1) * 0x30);

	return value;
}

int FrontResGetAreaHook(int* params)
{
	const char* name = (const char*)params[Params_EAX];
	RECT* pArea = (RECT*)params[2];

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);
	int result = 0;

	const CbmHeader* cbmHeader = GetCbmHeaderByName(name);

	bool isBackgroundHD = g_netFunctions._frontResIsBackgroundHD() != 0;
	bool isBackgroundWide = g_netFunctions._frontResIsBackgroundWide() != 0;

	if (g_config.SkirmishOptEnabled && _stricmp(name, "shipbmp") == 0)
	{
		SetRect(pArea, 0, 0, 256, 256);

		result = 1;
	}
	else if (_stricmp(name, "hud") == 0)
	{
		if (cbmHeader)
		{
			*pArea = cbmHeader->Area;
		}
		else
		{
			SetRect(pArea, 0, 0, 0, 0);
		}

		result = 1;
	}
	else if (_stricmp(name, "squadlogo") == 0)
	{
		int width = 180;

		if (cbmHeader)
		{
			width = max(width, (int)cbmHeader->Area.right);
		}

		SetRect(pArea, 0, 0, width, 180);
		result = 1;
	}
	else if (hasDC && cbmHeader)
	{
		result = g_netFunctions._frontResGetArea(name, (int)pArea);
	}

	if (_strnicmp(name, "rating", sizeof("rating") - 1) == 0)
	{
		LONG width = pArea->right - pArea->left;
		LONG height = pArea->bottom - pArea->top;
		pArea->left -= width * 3 / 2;
		pArea->right += width;
		pArea->top -= height / 2;

		if (g_concourseDoors.familyroom_rating_width != 0 && g_concourseDoors.familyroom_rating_height != 0)
		{
			SetRect(
				pArea,
				g_concourseDoors.familyroom_rating_left,
				g_concourseDoors.familyroom_rating_top,
				g_concourseDoors.familyroom_rating_left + g_concourseDoors.familyroom_rating_width,
				g_concourseDoors.familyroom_rating_top + g_concourseDoors.familyroom_rating_height);
		}
	}

	if (result == 1)
	{
		params[Params_ReturnAddress] = 0x005321E8;
	}
	else
	{
		params[Params_ReturnAddress] = 0x005321A1;
	}

	return 0;
}

int FrontResSetCurrentImageHook(int* params)
{
	const char* name = (const char*)params[1];

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	int imagesCount;

	if (!hasDC)
	{
		imagesCount = params[Params_ECX];
	}
	else
	{
		imagesCount = g_netFunctions._getItemImagesCount(name);
	}

	if (params[Params_EAX] >= imagesCount)
	{
		params[Params_EAX] = imagesCount - 1;
	}

	return 0;
}

int LoadFontHook(int* params)
{
	if (g_config.HDConcourseEnabled)
	{
		g_fontResources.Load();
	}

	params[Params_EAX] = ((int*)0x009FBC69)[params[Params_EDI]];

	return 0;
}

int FreeFontsHook(int* params)
{
	if (g_config.HDConcourseEnabled)
	{
		g_fontResources.Release();
	}

	params[Params_ESI] = 0x009F7FFB;

	return 0;
}

int FontGetLineHeightHook(int* params)
{
	const int fontSize = params[Params_EAX];

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	if (!hasDC)
	{
		params[Params_EAX] = ((int*)0x009FBC69)[fontSize];
		return 0;
	}

	params[Params_ECX] = g_fontResources.GetTextFormatLineHeight((float)fontSize);
	params[Params_ReturnAddress] = 0x00557F02;

	return 0;
}

int FontGetStringWidthHook(int* params)
{
	const char* str = (const char*)params[Params_ECX];
	const int fontSize = params[Params_EAX];
	const int callAddress = params[2];

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	if (!hasDC)
	{
		params[Params_ESI] = ((int*)0x009FBC69)[fontSize];
		return 0;
	}

	// L00555CF0 - XwaUITextBox
	bool isXwaUITextBox = callAddress == 0x00555E99 || callAddress == 0x00555F54;
	bool isBriefingGameStateUpdate = IsUpdateCallback(0x00564E90);
	bool isBriefingBottomText = isBriefingGameStateUpdate && callAddress == 0x00569D29;
	bool isCombatSingleGameStateUpdate = IsUpdateCallback(0x005438B0);
	bool isCombatMultiGameStateUpdate = IsUpdateCallback(0x0053C120);
	bool isFamilyRoomCursorMessage = callAddress == 0x0055b931;
	bool isEnterFilmFilenameGameStateUpdate = IsUpdateCallback(0x00541260);
	bool isConfigMenuGameStateUpdate = IsUpdateCallback(0x0051D100);

	HRESULT hr;

	IDWriteTextFormat* textFormat = g_fontResources.GetTextFormat((float)fontSize);

	float width = 0;

	if (g_fontResources.IsPlainText(str))
	{
		std::wstring wtext = string_towstring(str);

		ComPtr<IDWriteTextLayout> textLayout;
		hr = g_fontResources.GetDWriteFactory()->CreateTextLayout(wtext.c_str(), wtext.length(), textFormat, 640 * 4 / 3, 480, &textLayout);
		CheckHResult("CreateTextLayout", hr);

		DWRITE_TEXT_METRICS m{};
		textLayout->GetMetrics(&m);

		width = m.widthIncludingTrailingWhitespace;
	}
	else
	{
		width = 0;

		for (const char* s = str; *s != 0; s++)
		{
			if ((unsigned char)*s < 32)
			{
				continue;
			}

			char text[2];
			text[0] = *s;
			text[1] = 0;
			std::wstring wtext = string_towstring(text);

			ComPtr<IDWriteTextLayout> textLayout;
			hr = g_fontResources.GetDWriteFactory()->CreateTextLayout(wtext.c_str(), wtext.length(), textFormat, 100, 100, &textLayout);
			CheckHResult("CreateTextLayout", hr);

			DWRITE_TEXT_METRICS m{};
			textLayout->GetMetrics(&m);

			width += *s == ' ' ? m.widthIncludingTrailingWhitespace : m.width;
		}
	}

	if (isEnterFilmFilenameGameStateUpdate)
	{
		*(unsigned char*)0x00555EB1 = 0xEB;
	}
	else
	{
		*(unsigned char*)0x00555EB1 = 0x7E;
	}

	if (isXwaUITextBox)
	{
		bool isBackgroundHD = g_netFunctions._frontResIsBackgroundWide() != 0;

		if (isBackgroundHD)
		{
			width *= 0.75f;
		}
	}
	else if (isBriefingBottomText)
	{
		bool isBackgroundHD = g_netFunctions._frontResIsBackgroundWide() != 0;

		if (isBackgroundHD)
		{
			width *= 0.75f;
		}

		width += fontSize * 2;
	}
	else if (isFamilyRoomCursorMessage)
	{
		if ((float)g_screenWidth / dc.height < (float)g_ratioNumerator / g_ratioDenominator)
		{
			width += fontSize + fontSize / 2;
		}
	}
	else if (isConfigMenuGameStateUpdate)
	{
		bool isBackgroundHD = g_netFunctions._frontResIsBackgroundWide() != 0;

		if (isBackgroundHD)
		{
			width *= 0.75f;
		}
	}
	else if (isCombatMultiGameStateUpdate)
	{
		bool isBackgroundHD = g_netFunctions._frontResIsBackgroundWide() != 0;

		if (!isBackgroundHD)
		{
			width *= 1.3f;
			width += fontSize;
		}
		else
		{
			width *= 0.75f;
		}
	}
	else if (isCombatSingleGameStateUpdate)
	{
		bool isBackgroundHD = g_netFunctions._frontResIsBackgroundWide() != 0;

		if (!isBackgroundHD)
		{
			width *= 1.3f;
		}

		width += fontSize;
	}
	else
	{
		width += fontSize;
	}

	params[Params_EAX] = (int)width;
	params[Params_ReturnAddress] = 0x00557F6C;

	return 0;
}

int L00557B10Hook(int* params)
{
	const int fontSize = params[Params_EAX];

	const char* str = (const char*)params[609];
	const RECT* pRect = (RECT*)params[610];
	const unsigned int fontColor = (unsigned short)params[611];
	const int A14 = params[612];
	const int A18 = params[613];
	const bool A1C = params[614] != 0;
	const int A20 = params[615];

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	if (!hasDC)
	{
		params[Params_EDI] = ((int*)0x009FBC69)[fontSize];
		return 0;
	}

	params[Params_EAX] = 0;
	params[Params_ReturnAddress] = 0x00557EC6;

	const auto XwaFontGetLineHeight = (unsigned char(*)(int))0x00557EE0;
	const auto XwaFontGetStringWidth = (int(*)(const char*, int))0x00557F10;
	const auto XwaRectCopy = (void(*)(RECT*, const RECT*))0x00558CB0;
	const auto XwaRetrieveScreenArea = (void(*)(RECT*))0x0053F730;
	const auto XwaSetScreenArea = (void(*)(const RECT*))0x0053F790;
	const auto XwaRectRestrictToScreenArea = (int(*)(RECT*))0x00558D10;

	HRESULT hr;

	bool isBackgroundHD = g_netFunctions._frontResIsBackgroundWide() != 0;
	int offsetX = MultiplyDivOffsetXWide(dc);
	IDWriteTextFormat* textFormat = g_fontResources.GetTextFormat(MultiplyDivV(fontSize, dc));

	bool isTechLibraryGameStateUpdate = IsUpdateCallback(0x00574D70);
	bool isBriefingGameStateUpdate = IsUpdateCallback(0x00564E90);
	bool isTourOfDutyRoomGameStateUpdate = IsUpdateCallback(0x005775E0);

	int& linesCount = params[Params_EAX];
	linesCount = 0;

	RECT rect = *pRect;
	rect.left = MultiplyDivH(rect.left, isBackgroundHD, dc) + offsetX;
	rect.top = MultiplyDivV(rect.top, dc);
	rect.right = MultiplyDivH(rect.right, isBackgroundHD, dc) + offsetX;
	rect.bottom = MultiplyDivV(rect.bottom, dc);

	int esp10 = 0;
	int esp18 = 0;
	int esp1C = 1;
	unsigned int esp28 = fontColor;
	int esp2C = 0;
	int esi = rect.left;
	int esp14 = rect.top;
	char esp58[256];

	RECT esp48;

	if (A1C != 0)
	{
		XwaRetrieveScreenArea(&esp48);
		XwaSetScreenArea(pRect);

		RECT esp38;
		XwaRectCopy(&esp38, pRect);

		int eax = esp38.top + A20 * 2;

		if (eax < esp38.bottom)
		{
			esp38.bottom = eax;
		}

		XwaRectRestrictToScreenArea(&esp38);
		XwaSetScreenArea(&esp38);
	}

	ComPtr<ID2D1SolidColorBrush> brush;
	dc.d2d1RenderTarget->CreateSolidColorBrush(D2D1::ColorF(0), &brush);

	dc.d2d1RenderTarget->BeginDraw();

	for (int esp20 = 0; str[esp20] != 0; esp20++)
	{
		unsigned char esp30 = (unsigned char)str[esp20];
		unsigned int esp24 = esp28;

		if (esp30 == 0x01)
		{
			esp24 = fontColor;
		}
		else if (esp30 >= 0x02 && esp30 <= 0x07)
		{
			esp24 = ((unsigned int*)0x009F7EF7)[esp30 - 1];
		}
		else if (esp30 == ' ' || esp30 == 0x0A || str[esp20 + 1] == 0 || esp30 == '$')
		{
			if (esp1C == 1 && esp30 == ' ')
			{
				continue;
			}

			if (esp30 != 0x0A && esp30 != '$')
			{
				esp58[esp18] = esp30;
				esp18++;
			}

			esp58[esp18] = 0;

			int stringWidth = XwaFontGetStringWidth(esp58, fontSize);

			if (isTechLibraryGameStateUpdate)
			{
				stringWidth = stringWidth * 6 / 10;
			}
			else if (isBriefingGameStateUpdate || isTourOfDutyRoomGameStateUpdate)
			{
				stringWidth = stringWidth * 5 / 10;
			}

			if (esi + MultiplyDivH(stringWidth, isBackgroundHD, dc) > rect.right - MultiplyDivH(fontSize, isBackgroundHD, dc))
			{
				if (esp10 >= A18)
				{
					esp14 += MultiplyDivV(A14 + fontSize, dc);
				}

				esp10++;
				esp1C = 0x01;

				esi = rect.left;
			}

			for (int ebx = 0; ebx < esp18; ebx++)
			{
				char text[2];
				text[0] = esp58[ebx];
				text[1] = 0;
				std::wstring wtext = string_towstring(text);

				ComPtr<IDWriteTextLayout> textLayout;
				hr = g_fontResources.GetDWriteFactory()->CreateTextLayout(wtext.c_str(), wtext.length(), textFormat, 100, 100, &textLayout);
				CheckHResult("CreateTextLayout", hr);

				DWRITE_TEXT_METRICS m{};
				textLayout->GetMetrics(&m);

				float mWidth = esp58[ebx] == ' ' ? m.widthIncludingTrailingWhitespace : m.width;

				if (esp10 < A18)
				{
					esi += mWidth;

					if (esi > rect.right)
					{
						esi = rect.left;

						esp10++;
					}

					continue;
				}

				D2D1_RECT_F layoutRect
				{
					esi,
					esp14,
					esi + mWidth,
					esp14 + m.height
				};

				unsigned int colorRgb = Color16BppTo32Bpp(esp28);

				brush->SetColor(D2D1::ColorF(colorRgb));

				if (esp14 >= rect.top && esp14 < rect.bottom)
				{
					dc.d2d1RenderTarget->DrawTextA(
						wtext.c_str(),
						wtext.size(),
						textFormat,
						layoutRect,
						brush,
						D2D1_DRAW_TEXT_OPTIONS_NONE);
				}

				esi += mWidth;

				if (esi > rect.right - MultiplyDivH(fontSize, isBackgroundHD, dc))
				{
					esp14 += MultiplyDivV(A14 + fontSize, dc);
					esp10++;

					esi = rect.left;
				}
			}

			if (esp30 == 0x0A || esp30 == '$')
			{
				if (esp10 >= A18)
				{
					esp14 += MultiplyDivV(A14 + fontSize, dc);
				}

				esi = rect.left;
				esp10++;

				esp1C = 1;
			}

			esp18 = 0;
		}
		else
		{
			esp58[esp18] = esp30;
			esp18++;

			esp1C = 0;
		}

		esp28 = esp24;
	}

	dc.d2d1RenderTarget->EndDraw();

	if (A1C != 0)
	{
		XwaSetScreenArea(&esp48);
	}

	linesCount = esp10;

	return 0;
}

int L00557910Hook(int* params)
{
	const int fontSize = params[Params_EBP];

	const char* str = (const char*)params[530];
	const RECT* pRect = (RECT*)params[531];
	const bool alignHorizontal = params[532] != 0;
	const bool alignVertical = params[533] != 0;
	unsigned int fontColor = (unsigned int)params[534];

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	if (!hasDC)
	{
		params[Params_ESI] = ((int*)0x009FBC69)[fontSize];
		return 0;
	}

	params[Params_EAX] = 0;
	params[Params_ReturnAddress] = 0x00557AA0;

	if (pRect->left < 0 || pRect->right > dc.displayWidth || pRect->top < 0 || pRect->bottom > dc.displayHeight)
	{
		return 0;
	}

	const auto XwaFontGetLineHeight = (unsigned char(*)(int))0x00557EE0;
	const auto XwaFontGetStringWidth = (int(*)(const char*, int))0x00557F10;

	HRESULT hr;

	bool isBackgroundHD = g_netFunctions._frontResIsBackgroundWide() != 0;
	int offsetX = MultiplyDivOffsetXWide(dc);
	IDWriteTextFormat* textFormat = g_fontResources.GetTextFormat(MultiplyDivV(fontSize, dc));

	unsigned int color = fontColor;

	bool isBriefingGameStateUpdate = IsUpdateCallback(0x00564E90);

	int lineHeight = XwaFontGetLineHeight(fontSize);
	int strWidth = XwaFontGetStringWidth(str, fontSize);

	float currentX = MultiplyDivH(alignHorizontal ? (pRect->left + (pRect->right - pRect->left) / 2 - strWidth / 2) : pRect->left, isBackgroundHD, dc) + offsetX;
	float currentY = MultiplyDivV(alignVertical ? (pRect->top + (pRect->bottom - pRect->top) / 2 - lineHeight / 2) : pRect->top, dc);
	float width = MultiplyDivH(pRect->right - pRect->left, isBackgroundHD, dc) + 1;
	float height = MultiplyDivV(pRect->bottom - pRect->top, dc) + 1;

	ComPtr<ID2D1SolidColorBrush> brush;
	dc.d2d1RenderTarget->CreateSolidColorBrush(D2D1::ColorF(0), &brush);

	dc.d2d1RenderTarget->BeginDraw();

	if (g_fontResources.IsPlainText(str))
	{
		std::wstring wtext = string_towstring(str);

		D2D1_RECT_F layoutRect
		{
			currentX,
			currentY,
			currentX + width,
			currentY + height
		};

		unsigned int colorRgb = Color16BppTo32Bpp(color);

		brush->SetColor(D2D1::ColorF(colorRgb));

		dc.d2d1RenderTarget->DrawTextA(
			wtext.c_str(),
			wtext.size(),
			textFormat,
			layoutRect,
			brush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);
	}
	else
	{
		for (const char* s = str; *s != 0; s++)
		{
			unsigned char c = *s;

			if (c == 1)
			{
				color = fontColor;
				continue;
			}

			if (c >= 2 && c <= 6)
			{
				color = ((unsigned int*)0x009F7EF7)[c - 1];
				continue;
			}

			if (c < 32)
			{
				continue;
			}

			char text[2];
			text[0] = *s;
			text[1] = 0;
			std::wstring wtext = string_towstring(text);

			ComPtr<IDWriteTextLayout> textLayout;
			hr = g_fontResources.GetDWriteFactory()->CreateTextLayout(wtext.c_str(), wtext.length(), textFormat, 100, 100, &textLayout);
			CheckHResult("CreateTextLayout", hr);

			DWRITE_TEXT_METRICS m{};
			textLayout->GetMetrics(&m);

			float mWidth = *s == ' ' ? m.widthIncludingTrailingWhitespace : m.width;

			D2D1_RECT_F layoutRect
			{
				currentX,
				currentY,
				currentX + mWidth,
				currentY + m.height
			};

			unsigned int colorRgb = Color16BppTo32Bpp(color);

			brush->SetColor(D2D1::ColorF(colorRgb));

			dc.d2d1RenderTarget->DrawTextA(
				wtext.c_str(),
				wtext.size(),
				textFormat,
				layoutRect,
				brush,
				D2D1_DRAW_TEXT_OPTIONS_NONE);

			currentX += mWidth;
		}
	}

	dc.d2d1RenderTarget->EndDraw();

	return 0;
}

int L00557720Hook(int* params)
{
	const int fontSize = params[Params_ESI];

	const char* str = (const char*)params[532];
	RECT* pRect = (RECT*)params[533];
	unsigned int fontColor = (unsigned int)params[534];
	const int charCount = params[535];

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	if (!hasDC)
	{
		params[Params_EBP] = ((int*)0x009FBC69)[fontSize];
		return 0;
	}

	params[Params_EAX] = 0;
	params[Params_ReturnAddress] = 0x005578F8;

	const auto XwaFontGetLineHeight = (unsigned char(*)(int))0x00557EE0;
	const auto XwaFontGetStringWidth = (int(*)(const char*, int))0x00557F10;

	bool isTourOfDutyRoomGameStateUpdate = IsUpdateCallback(0x005775E0);
	bool isBriefingGameStateUpdate = IsUpdateCallback(0x00564E90);
	bool isDebriefingGameStateUpdate = IsUpdateCallback(0x0057ECE0);

	HRESULT hr;

	bool isBackgroundHD = g_netFunctions._frontResIsBackgroundWide() != 0;
	int offsetX = MultiplyDivOffsetXWide(dc);
	IDWriteTextFormat* textFormat = g_fontResources.GetTextFormat(MultiplyDivV(fontSize, dc));

	unsigned int color = fontColor;

	int lineHeight = XwaFontGetLineHeight(fontSize);
	int strWidth = XwaFontGetStringWidth(str, fontSize);
	int strLength = strlen(str);

	if (isTourOfDutyRoomGameStateUpdate || isBriefingGameStateUpdate || isDebriefingGameStateUpdate)
	{
		strWidth = (strWidth - fontSize) * 3 / 4;
	}

	int strStart = strLength / 2 - charCount;
	int strEnd = strLength / 2 + charCount;

	float currentX = MultiplyDivH(pRect->left + (pRect->right - pRect->left) / 2 - strWidth / 2, isBackgroundHD, dc) + offsetX;
	float currentY = MultiplyDivV(pRect->top + (pRect->bottom - pRect->top) / 2 - lineHeight / 2, dc);
	float width = MultiplyDivH(640, isBackgroundHD, dc) + 1;
	float height = MultiplyDivV(480, dc) + 1;

	ComPtr<ID2D1SolidColorBrush> brush;
	dc.d2d1RenderTarget->CreateSolidColorBrush(D2D1::ColorF(0), &brush);

	dc.d2d1RenderTarget->BeginDraw();

	for (const char* s = str; *s != 0; s++)
	{
		int strPosition = s - str;

		if (currentX >= width)
		{
			break;
		}

		unsigned char c = *s;

		if (c == 1)
		{
			color = fontColor;
			continue;
		}

		if (c >= 2 && c <= 6)
		{
			color = ((unsigned int*)0x009F7EF7)[c - 1];
			continue;
		}

		if (c < 32)
		{
			continue;
		}

		char text[2];
		text[0] = *s;
		text[1] = 0;
		std::wstring wtext = string_towstring(text);

		ComPtr<IDWriteTextLayout> textLayout;
		hr = g_fontResources.GetDWriteFactory()->CreateTextLayout(wtext.c_str(), wtext.length(), textFormat, 100, 100, &textLayout);
		CheckHResult("CreateTextLayout", hr);

		DWRITE_TEXT_METRICS m{};
		textLayout->GetMetrics(&m);

		float mWidth = *s == ' ' ? m.widthIncludingTrailingWhitespace : m.width;

		D2D1_RECT_F layoutRect
		{
			currentX,
			currentY,
			currentX + mWidth,
			currentY + m.height
		};

		if (strPosition >= strStart && strPosition <= strEnd)
		{
			unsigned int colorRgb = Color16BppTo32Bpp(color);

			if (strPosition == strStart || strPosition == strEnd)
			{
				colorRgb = Color16BppTo32Bpp(((unsigned int*)0x009F7EF7)[4]);
			}

			brush->SetColor(D2D1::ColorF(colorRgb));

			dc.d2d1RenderTarget->DrawTextA(
				wtext.c_str(),
				wtext.size(),
				textFormat,
				layoutRect,
				brush,
				D2D1_DRAW_TEXT_OPTIONS_NONE);
		}

		currentX += mWidth;
	}

	dc.d2d1RenderTarget->EndDraw();

	return 0;
}

int L005575A0Hook(int* params)
{
	const int fontSize = params[Params_EBX];

	const int sourceAddress = params[528];
	const char* str = (const char*)params[530];
	const RECT* pRect0 = (RECT*)params[531];
	unsigned int fontColor = (unsigned int)params[532];

	RECT rc = *pRect0;
	RECT* pRect = &rc;

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	if (!hasDC)
	{
		params[Params_ESI] = ((int*)0x009FBC69)[fontSize];
		return 0;
	}

	params[Params_EAX] = 0;
	params[Params_ReturnAddress] = 0x0055770F;

	bool isConfigMenuGameStateUpdate_ViewCutscenes = IsUpdateCallback(0x0051D100) && *(int*)0x007829B8 == 15;

	if (isConfigMenuGameStateUpdate_ViewCutscenes)
	{
		const auto XwaRectRestrictToScreenArea = (int(*)(RECT*))0x00558D10;
		XwaRectRestrictToScreenArea(pRect);

		if (pRect->right - pRect->left <= 0 || pRect->bottom - pRect->top <= 0)
		{
			return 0;
		}
	}

	if (pRect->left < 0 || pRect->right > dc.displayWidth || pRect->top < 0 || pRect->bottom > dc.displayHeight)
	{
		return 0;
	}

	// missmall text
	if (sourceAddress == 0x00549BBB)
	{
		int x = pRect->left;
		int w = pRect->right - pRect->left + 1;

		if ((x < dc.ScreenAreaLeft) || (x + w > dc.ScreenAreaRight))
		{
			return 0;
		}
	}

	const auto XwaFontGetLineHeight = (unsigned char(*)(int))0x00557EE0;
	const auto XwaFontGetStringWidth = (int(*)(const char*, int))0x00557F10;

	HRESULT hr;

	bool isMainRoomGameStateUpdate = IsUpdateCallback(0x005397D0);
	bool isConfigMenuGameStateUpdate = IsUpdateCallback(0x0051D100);
	bool isCreatePilotGameStateUpdate = IsUpdateCallback(0x00559B50);
	bool isFilmRoomGameGameStateUpdate = IsUpdateCallback(0x00573020);
	bool isCombatRoomGameStateUpdate = IsUpdateCallback(0x0053B500);
	bool isCombatMultiGameStateUpdate = IsUpdateCallback(0x0053C120);
	bool isCombatMultiGameStateUpdate_Main = IsUpdateCallback(0x0053C120) && *(int*)0x09F4BC8 == 0;
	bool isMessageBoxGameStateUpdate = IsUpdateCallback(0x005595A0);
	bool isCombatSingleMissionTitle = sourceAddress == 0x00544e83;
	bool isCombatSingleGameStateMissionsNavigator = sourceAddress >= 0x00549760 && sourceAddress < 0x0054CE80;
	bool isFamilyRoomGameGameStateUpdate = IsUpdateCallback(0x0055FF30);
	bool isTechLibraryGameStateUpdate = IsUpdateCallback(0x00574D70);
	bool isBriefingGameStateUpdate = IsUpdateCallback(0x00564E90);
	bool isDebriefingGameStateUpdate = IsUpdateCallback(0x0057ECE0);
	bool isTourOfDutyRoomGameStateUpdate = IsUpdateCallback(0x005775E0);
	bool isTourOfDutyDeathStarGameStateUpdate = IsUpdateCallback(0x0055AF90);

	bool isBackgroundHD = g_netFunctions._frontResIsBackgroundWide() != 0;
	int offsetX = MultiplyDivOffsetXWide(dc);
	IDWriteTextFormat* textFormat = g_fontResources.GetTextFormat(MultiplyDivV(fontSize, dc));

	unsigned int color = fontColor;

	int lineHeight = XwaFontGetLineHeight(fontSize);
	int strWidth = XwaFontGetStringWidth(str, fontSize);

	if (isBriefingGameStateUpdate
		|| isDebriefingGameStateUpdate
		|| isCombatSingleGameStateMissionsNavigator
		|| isCombatSingleMissionTitle
		|| isFilmRoomGameGameStateUpdate
		|| isCombatMultiGameStateUpdate_Main
		|| isCreatePilotGameStateUpdate
		|| isTechLibraryGameStateUpdate
		|| isTourOfDutyDeathStarGameStateUpdate)
	{
		strWidth = strWidth * 3 / 4;
	}

	float currentX = MultiplyDivH(pRect->left + (pRect->right - pRect->left) / 2 - strWidth / 2, isBackgroundHD, dc) + offsetX;
	float currentY = MultiplyDivV(pRect->top + (pRect->bottom - pRect->top) / 2 - lineHeight / 2, dc);
	float width = MultiplyDivH(pRect->right - pRect->left, isBackgroundHD, dc) / 0.75f + 1;
	float height = MultiplyDivV(pRect->bottom - pRect->top, dc) + 1;

	if (isCombatRoomGameStateUpdate)
	{
		pRect->left = pRect->left + (pRect->right - pRect->left) * 0.2f;
	}
	else if (isTechLibraryGameStateUpdate)
	{
		int strLength = strlen(str);
		int strIndex = -1;

		for (int i = 0; i < strLength; i++)
		{
			if (str[i] == ':')
			{
				strIndex = i;
				break;
			}
		}

		if (strIndex != -1)
		{
			float length = (float)MultiplyDivH(strWidth, isBackgroundHD, dc);

			float centerH = currentX + length * 0.5f;
			float percent = (float)strIndex / strLength;
			currentX = centerH - length * percent;
		}
	}

	ComPtr<ID2D1SolidColorBrush> brush;
	dc.d2d1RenderTarget->CreateSolidColorBrush(D2D1::ColorF(0), &brush);

	dc.d2d1RenderTarget->BeginDraw();

	if (g_fontResources.IsPlainText(str))
	{
		bool alignText = isMainRoomGameStateUpdate
			|| isFamilyRoomGameGameStateUpdate
			|| isCombatRoomGameStateUpdate
			|| isCombatMultiGameStateUpdate
			|| isFilmRoomGameGameStateUpdate
			|| isCreatePilotGameStateUpdate
			|| isMessageBoxGameStateUpdate;

		std::wstring wtext = string_towstring(str);

		// simulator bottom icons
		if (sourceAddress >= 0x00549760 && sourceAddress <= 0x0054CE80)
		{
			for (int x = -1; x <= 2; x++)
			{
				for (int y = -1; y <= 2; y++)
				{
					D2D1_RECT_F layoutRectShadow
					{
						currentX + x,
						currentY + y,
						currentX + x + width,
						currentY + y + height
					};

					brush->SetColor(D2D1::ColorF(0));

					dc.d2d1RenderTarget->DrawTextA(
						wtext.c_str(),
						wtext.size(),
						textFormat,
						layoutRectShadow,
						brush,
						D2D1_DRAW_TEXT_OPTIONS_NONE);
				}
			}
		}

		if (alignText)
		{
			float widthScale = 1.0f;

			if (!isCombatMultiGameStateUpdate && !isFilmRoomGameGameStateUpdate && !isTechLibraryGameStateUpdate && !isMessageBoxGameStateUpdate)
			{
				widthScale = 0.8f;
			}

			currentX = MultiplyDivH(pRect->left, isBackgroundHD, dc) + offsetX;
			currentY = MultiplyDivV(pRect->top + (pRect->bottom - pRect->top) / 2 - lineHeight / 2, dc);
			width = MultiplyDivH(pRect->right - pRect->left, isBackgroundHD, dc) * widthScale + 1;
			height = MultiplyDivV(pRect->bottom - pRect->top, dc) + 1;
		}

		D2D1_RECT_F layoutRect
		{
			currentX,
			currentY,
			currentX + width,
			currentY + height
		};

		unsigned int colorRgb = Color16BppTo32Bpp(color);

		brush->SetColor(D2D1::ColorF(colorRgb));

		if (alignText)
		{
			textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		}

		dc.d2d1RenderTarget->DrawTextA(
			wtext.c_str(),
			wtext.size(),
			textFormat,
			layoutRect,
			brush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);

		if (alignText)
		{
			textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		}
	}
	else
	{
		for (const char* s = str; *s != 0; s++)
		{
			unsigned char c = *s;

			if (c == 1)
			{
				color = fontColor;
				continue;
			}

			if (c >= 2 && c <= 6)
			{
				color = ((unsigned int*)0x009F7EF7)[c - 1];
				continue;
			}

			if (c < 32)
			{
				continue;
			}

			char text[2];
			text[0] = *s;
			text[1] = 0;
			std::wstring wtext = string_towstring(text);

			ComPtr<IDWriteTextLayout> textLayout;
			hr = g_fontResources.GetDWriteFactory()->CreateTextLayout(wtext.c_str(), wtext.length(), textFormat, 100, 100, &textLayout);
			CheckHResult("CreateTextLayout", hr);

			DWRITE_TEXT_METRICS m{};
			textLayout->GetMetrics(&m);

			float mWidth = *s == ' ' ? m.widthIncludingTrailingWhitespace : m.width;

			D2D1_RECT_F layoutRect
			{
				currentX,
				currentY,
				currentX + mWidth,
				currentY + m.height
			};

			unsigned int colorRgb = Color16BppTo32Bpp(color);

			brush->SetColor(D2D1::ColorF(colorRgb));

			dc.d2d1RenderTarget->DrawTextA(
				wtext.c_str(),
				wtext.size(),
				textFormat,
				layoutRect,
				brush,
				D2D1_DRAW_TEXT_OPTIONS_NONE);

			currentX += mWidth;
		}
	}

	dc.d2d1RenderTarget->EndDraw();

	return 0;
}

int L00557450Hook(int* params)
{
	const int fontSize = params[Params_EAX];

	const char* str = (const char*)params[529];
	const int positionX = params[530];
	const int positionY = params[531];
	unsigned int fontColor = (unsigned int)params[532];

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	if (!hasDC)
	{
		params[Params_ESI] = ((int*)0x009FBC69)[fontSize];
		return 0;
	}

	params[Params_EAX] = 0;
	params[Params_ReturnAddress] = 0x00557590;

	const auto XwaFontGetStringWidth = (int(*)(const char*, int))0x00557F10;

	HRESULT hr;

	bool isCombatSingleGameStateUpdate = IsUpdateCallback(0x005438B0);

	bool isBackgroundHD = g_netFunctions._frontResIsBackgroundWide() != 0;
	int offsetX = MultiplyDivOffsetXWide(dc);
	IDWriteTextFormat* textFormat = g_fontResources.GetTextFormat(MultiplyDivV(fontSize, dc));

	unsigned int color = fontColor;

	int strWidth = XwaFontGetStringWidth(str, fontSize);

	if (isCombatSingleGameStateUpdate)
	{
		strWidth = strWidth * 3 / 4;
	}

	float currentX = MultiplyDivH(positionX - strWidth, isBackgroundHD, dc) + offsetX;
	float currentY = MultiplyDivV(positionY, dc);
	float width = MultiplyDivH(640, isBackgroundHD, dc) + 1;
	float height = MultiplyDivV(480, dc) + 1;

	ComPtr<ID2D1SolidColorBrush> brush;
	dc.d2d1RenderTarget->CreateSolidColorBrush(D2D1::ColorF(0), &brush);

	dc.d2d1RenderTarget->BeginDraw();

	if (g_fontResources.IsPlainText(str))
	{
		std::wstring wtext = string_towstring(str);

		D2D1_RECT_F layoutRect
		{
			currentX,
			currentY,
			offsetX + width,
			height
		};

		unsigned int colorRgb = Color16BppTo32Bpp(color);

		brush->SetColor(D2D1::ColorF(colorRgb));

		dc.d2d1RenderTarget->DrawTextA(
			wtext.c_str(),
			wtext.size(),
			textFormat,
			layoutRect,
			brush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);
	}
	else
	{
		for (const char* s = str; *s != 0; s++)
		{
			if (currentX >= width)
			{
				break;
			}

			unsigned char c = *s;

			if (c == 1)
			{
				color = fontColor;
				continue;
			}

			if (c >= 2 && c <= 6)
			{
				color = ((unsigned int*)0x009F7EF7)[c - 1];
				continue;
			}

			if (c < 32)
			{
				continue;
			}

			char text[2];
			text[0] = *s;
			text[1] = 0;
			std::wstring wtext = string_towstring(text);

			ComPtr<IDWriteTextLayout> textLayout;
			hr = g_fontResources.GetDWriteFactory()->CreateTextLayout(wtext.c_str(), wtext.length(), textFormat, 100, 100, &textLayout);
			CheckHResult("CreateTextLayout", hr);

			DWRITE_TEXT_METRICS m{};
			textLayout->GetMetrics(&m);

			float mWidth = *s == ' ' ? m.widthIncludingTrailingWhitespace : m.width;

			D2D1_RECT_F layoutRect
			{
				currentX,
				currentY,
				currentX + mWidth,
				currentY + m.height
			};

			unsigned int colorRgb = Color16BppTo32Bpp(color);

			brush->SetColor(D2D1::ColorF(colorRgb));

			dc.d2d1RenderTarget->DrawTextA(
				wtext.c_str(),
				wtext.size(),
				textFormat,
				layoutRect,
				brush,
				D2D1_DRAW_TEXT_OPTIONS_NONE);

			currentX += mWidth;
		}
	}

	dc.d2d1RenderTarget->EndDraw();

	return 0;
}

int L00557310Hook(int* params)
{
	const int fontSize = params[Params_EAX];

	const char* str = (const char*)params[529];
	const int positionX = params[530];
	const int positionY = params[531];
	unsigned int fontColor = (unsigned int)params[532];

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	if (!hasDC)
	{
		params[Params_ESI] = ((int*)0x009FBC69)[fontSize];
		return 0;
	}

	params[Params_EAX] = 0;
	params[Params_ReturnAddress] = 0x00557443;

	const auto XwaRectRestrictToScreenArea = (int(*)(RECT*))0x00558D10;
	RECT rc{ positionX , positionY, positionX + 1 , positionY + 1 };
	XwaRectRestrictToScreenArea(&rc);

	if (rc.right - rc.left <= 0 || rc.bottom - rc.top <= 0)
	{
		return 0;
	}

	HRESULT hr;

	bool isBackgroundHD = g_netFunctions._frontResIsBackgroundWide() != 0;
	int offsetX = MultiplyDivOffsetXWide(dc);
	IDWriteTextFormat* textFormat = g_fontResources.GetTextFormat(MultiplyDivV(fontSize, dc));

	unsigned int color = fontColor;

	float currentX = MultiplyDivH(positionX, isBackgroundHD, dc) + offsetX;
	float currentY = MultiplyDivV(positionY, dc);
	float width = MultiplyDivH(640, isBackgroundHD, dc) + 1;
	float height = MultiplyDivV(480, dc) + 1;

	ComPtr<ID2D1SolidColorBrush> brush;
	dc.d2d1RenderTarget->CreateSolidColorBrush(D2D1::ColorF(0), &brush);

	dc.d2d1RenderTarget->BeginDraw();

	if (g_fontResources.IsPlainText(str))
	{
		std::wstring wtext = string_towstring(str);

		D2D1_RECT_F layoutRect
		{
			currentX,
			currentY,
			offsetX + width,
			height
		};

		unsigned int colorRgb = Color16BppTo32Bpp(color);

		brush->SetColor(D2D1::ColorF(colorRgb));

		dc.d2d1RenderTarget->DrawTextA(
			wtext.c_str(),
			wtext.size(),
			textFormat,
			layoutRect,
			brush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);
	}
	else
	{
		for (const char* s = str; *s != 0; s++)
		{
			if (currentX >= width)
			{
				break;
			}

			unsigned char c = *s;

			if (c == 1)
			{
				color = fontColor;
				continue;
			}

			if (c >= 2 && c <= 6)
			{
				color = ((unsigned int*)0x009F7EF7)[c - 1];
				continue;
			}

			if (c < 32)
			{
				continue;
			}

			char text[2];
			text[0] = *s;
			text[1] = 0;
			std::wstring wtext = string_towstring(text);

			ComPtr<IDWriteTextLayout> textLayout;
			hr = g_fontResources.GetDWriteFactory()->CreateTextLayout(wtext.c_str(), wtext.length(), textFormat, 100, 100, &textLayout);
			CheckHResult("CreateTextLayout", hr);

			DWRITE_TEXT_METRICS m{};
			textLayout->GetMetrics(&m);

			float mWidth = *s == ' ' ? m.widthIncludingTrailingWhitespace : m.width;

			D2D1_RECT_F layoutRect
			{
				currentX,
				currentY,
				currentX + mWidth,
				currentY + m.height
			};

			unsigned int colorRgb = Color16BppTo32Bpp(color);

			brush->SetColor(D2D1::ColorF(colorRgb));

			dc.d2d1RenderTarget->DrawTextA(
				wtext.c_str(),
				wtext.size(),
				textFormat,
				layoutRect,
				brush,
				D2D1_DRAW_TEXT_OPTIONS_NONE);

			currentX += mWidth;
		}
	}

	dc.d2d1RenderTarget->EndDraw();

	return 0;
}

int L005571A0Hook(int* params)
{
	const int fontSize = params[Params_EAX];

	const char* str = (const char*)params[530];
	const int positionX = params[531];
	const int positionY = params[532];
	unsigned int fontColor = (unsigned int)params[533];
	const int maxLength = params[534];

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	if (!hasDC)
	{
		params[Params_ESI] = ((int*)0x009FBC69)[fontSize];
		return 0;
	}

	params[Params_EAX] = 0;
	params[Params_ReturnAddress] = 0x005572F9;

	HRESULT hr;

	bool isBackgroundHD = g_netFunctions._frontResIsBackgroundWide() != 0;
	int offsetX = MultiplyDivOffsetXWide(dc);
	IDWriteTextFormat* textFormat = g_fontResources.GetTextFormat(MultiplyDivV(fontSize, dc));

	unsigned int color = fontColor;

	float currentX = MultiplyDivH(positionX, isBackgroundHD, dc) + offsetX;
	float currentY = MultiplyDivV(positionY, dc);
	float width = MultiplyDivH(640, isBackgroundHD, dc) + 1;
	float height = MultiplyDivV(480, dc) + 1;

	char text[256];
	strcpy_s(text, str);

	if (maxLength >= 0 && maxLength < 256)
	{
		text[maxLength] = 0;
	}

	ComPtr<ID2D1SolidColorBrush> brush;
	dc.d2d1RenderTarget->CreateSolidColorBrush(D2D1::ColorF(0), &brush);

	dc.d2d1RenderTarget->BeginDraw();

	if (g_fontResources.IsPlainText(str))
	{
		std::wstring wtext = string_towstring(str);

		D2D1_RECT_F layoutRect
		{
			currentX,
			currentY,
			offsetX + width,
			height
		};

		unsigned int colorRgb = Color16BppTo32Bpp(color);

		brush->SetColor(D2D1::ColorF(colorRgb));

		dc.d2d1RenderTarget->DrawTextA(
			wtext.c_str(),
			wtext.size(),
			textFormat,
			layoutRect,
			brush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);
	}
	else
	{
		for (const char* s = str; *s != 0; s++)
		{
			if (currentX >= width)
			{
				break;
			}

			unsigned char c = *s;

			if (c == 1)
			{
				color = fontColor;
				continue;
			}

			if (c >= 2 && c <= 6)
			{
				color = ((unsigned int*)0x009F7EF7)[c - 1];
				continue;
			}

			if (c < 32)
			{
				continue;
			}

			char text[2];
			text[0] = *s;
			text[1] = 0;
			std::wstring wtext = string_towstring(text);

			ComPtr<IDWriteTextLayout> textLayout;
			hr = g_fontResources.GetDWriteFactory()->CreateTextLayout(wtext.c_str(), wtext.length(), textFormat, 100, 100, &textLayout);
			CheckHResult("CreateTextLayout", hr);

			DWRITE_TEXT_METRICS m{};
			textLayout->GetMetrics(&m);

			float mWidth = *s == ' ' ? m.widthIncludingTrailingWhitespace : m.width;

			D2D1_RECT_F layoutRect
			{
				currentX,
				currentY,
				currentX + mWidth,
				currentY + m.height
			};

			unsigned int colorRgb = Color16BppTo32Bpp(color);

			brush->SetColor(D2D1::ColorF(colorRgb));

			dc.d2d1RenderTarget->DrawTextA(
				wtext.c_str(),
				wtext.size(),
				textFormat,
				layoutRect,
				brush,
				D2D1_DRAW_TEXT_OPTIONS_NONE);

			currentX += mWidth;
		}
	}

	dc.d2d1RenderTarget->EndDraw();

	return 0;
}

int ConfigMenuSetVersionStringHook(int* params)
{
	strcpy_s((char*)(params + 7), 8, "2.02");

	return 0;
}

int ReviewBriefingDrawProgressBarHook(int* params)
{
	const int step = params[Params_EBP];

	const auto XwaFrontResDraw = (int(*)(const char*, short, short))0x00534A60;

	int length = (g_concourseDoors.briefing_review_item_right - g_concourseDoors.briefing_review_item_left) * 8 / 10 - 14;
	int currentLength = length * step / 8;

	for (int i = 0; i < currentLength; i++)
	{
		XwaFrontResDraw("sbarcenter", g_concourseDoors.briefing_review_item_left + 6 + i, 0x4B);
	}

	XwaFrontResDraw("sbarstart", g_concourseDoors.briefing_review_item_left + 1, 0x4B);

	if (currentLength > 0)
	{
		XwaFrontResDraw("sbarspark", g_concourseDoors.briefing_review_item_left + 6 + currentLength, 0x4B);
	}

	return 0;
}

int ReviewBriefingDrawMissionItemHook(int* params)
{
	RECT* pRect = (RECT*)params[0];
	int left = params[1];
	int top = params[2];
	int right = params[3];
	int bottom = params[4];

	const auto XwaRectSet = (void(*)(RECT*, int, int, int, int))0x00558C90;

	left = g_concourseDoors.briefing_review_item_left;
	right = g_concourseDoors.briefing_review_item_right;
	bottom = top + g_concourseDoors.briefing_review_item_height;

	XwaRectSet(pRect, left, top, right, bottom);

	return 0;
}

int ReviewBriefingDrawMissionTitleHook(int* params)
{
	RECT* pRect = (RECT*)params[0];
	int left = params[1];
	int top = params[2];
	int right = params[3];
	int bottom = params[4];

	const auto XwaRectSet = (void(*)(RECT*, int, int, int, int))0x00558C90;

	left = g_concourseDoors.briefing_review_item_left + 1;
	right = g_concourseDoors.briefing_review_item_right + 2;

	XwaRectSet(pRect, left, top, right, bottom);

	return 0;
}

int ReviewBriefingDrawSkirmishTitleHook(int* params)
{
	RECT* pRect = (RECT*)params[0];
	int left = params[1];
	int top = params[2];
	int right = params[3];
	int bottom = params[4];

	const auto XwaRectSet = (void(*)(RECT*, int, int, int, int))0x00558C90;

	left = g_concourseDoors.briefing_review_item_left + 1;
	right = g_concourseDoors.briefing_review_item_right + 2;

	XwaRectSet(pRect, left, top, right, bottom);

	return 0;
}

int ReviewBriefingDrawSkirmishItem1Hook(int* params)
{
	int fontSize = params[0];
	int text = params[1];
	int positionX = params[2];
	int positionY = params[3];
	int color = params[4];

	const auto L00557310 = (int(*)(int, int, int, int, int))0x00557310;

	positionX = g_concourseDoors.briefing_review_item_left + 6;

	L00557310(fontSize, text, positionX, positionY, color);

	return 0;
}

int ReviewBriefingDrawSkirmishItem2Hook(int* params)
{
	int fontSize = params[0];
	int text = params[1];
	int positionX = params[2];
	int positionY = params[3];
	int color = params[4];

	const auto L00557310 = (int(*)(int, int, int, int, int))0x00557310;

	positionX = g_concourseDoors.briefing_review_item_left + 6;

	L00557310(fontSize, text, positionX, positionY, color);

	return 0;
}

int ReviewBriefingDrawSkirmishItem3Hook(int* params)
{
	int fontSize = params[0];
	int text = params[1];
	int positionX = params[2];
	int positionY = params[3];
	int color = params[4];

	const auto L00557310 = (int(*)(int, int, int, int, int))0x00557310;

	positionX = positionX - 0x55 + g_concourseDoors.briefing_review_item_left + 21;

	L00557310(fontSize, text, positionX, positionY, color);

	return 0;
}

int MissionSelectionScreenPositionHook(int* params)
{
	int missionDirectoryId = params[Params_EAX];

	int left = 0;

	if (missionDirectoryId == 4 || missionDirectoryId == 1)
	{
		left = g_concourseDoors.mission_selection_screen_posX;
	}
	else
	{
		left = 0;
	}

	params[Params_EBX] = left;

	return 0;
}

int DrawBattleZoomImage1Hook(int* params)
{
	const char* A4 = (const char*)params[0];
	int A8 = params[1];
	int AC = params[2];

	const auto L00534A60 = (void(*)(const char*, int, int))0x00534A60;

	L00534A60(A4, A8 - 0x140 + g_concourseDoors.battlezoom_image1_posX, AC - 0xF0 + g_concourseDoors.battlezoom_image1_posY);

	return 0;
}

int DrawKalidorZoomImage1Hook(int* params)
{
	const char* A4 = (const char*)params[0];
	int A8 = params[1];
	int AC = params[2];

	const auto L00534A60 = (void(*)(const char*, int, int))0x00534A60;

	L00534A60(A4, A8 - 0x140 + g_concourseDoors.kalidorzoom_image1_posX, AC - 0xF0 + g_concourseDoors.kalidorzoom_image1_posY);

	return 0;
}

int DrawBattleKalidorZoomText1Hook(int* params)
{
	RECT* A4 = (RECT*)params[0];
	int A8 = params[1];
	int AC = params[2];
	int A10 = params[3];

	const auto L00558D90 = (void(*)(RECT*, int, int, int))0x00558D90;

	unsigned int esi = *(unsigned int*)0x0784BCC & 0x7FFFFFFFU;

	if (esi != 0)
	{
		A4->left = A4->left - 0x12C + g_concourseDoors.battlezoom_text1_posX;
		A4->top = A4->top - 0x190 + g_concourseDoors.battlezoom_text1_posY;
		A4->right = A4->right - 0x12C + g_concourseDoors.battlezoom_text1_posX;
		A4->bottom = A4->bottom - 0x190 + g_concourseDoors.battlezoom_text1_posY;
	}
	else
	{
		A4->left = A4->left - 0x12C + g_concourseDoors.kalidorzoom_text1_posX;
		A4->top = A4->top - 0x190 + g_concourseDoors.kalidorzoom_text1_posY;
		A4->right = A4->right - 0x12C + g_concourseDoors.kalidorzoom_text1_posX;
		A4->bottom = A4->bottom - 0x190 + g_concourseDoors.kalidorzoom_text1_posY;
	}

	L00558D90(A4, A8, AC, A10);

	return 0;
}

int DrawBattleZoomImage2Hook(int* params)
{
	const char* A4 = (const char*)params[0];
	int A8 = params[1];
	int AC = params[2];

	const auto L00534A60 = (void(*)(const char*, int, int))0x00534A60;

	L00534A60(A4, A8 - 0x98 + g_concourseDoors.battlezoom_image2_posX, AC - 0x168 + g_concourseDoors.battlezoom_image2_posY);

	return 0;
}

int DrawBattleZoomText2Hook(int* params)
{
	RECT* A4 = (RECT*)params[0];
	int A8 = params[1];
	int AC = params[2];
	int A10 = params[3];

	const auto L00558D90 = (void(*)(RECT*, int, int, int))0x00558D90;

	A4->left = A4->left - 0xB6 + g_concourseDoors.battlezoom_text2_posX;
	A4->top = A4->top - 0x159 + g_concourseDoors.battlezoom_text2_posY;
	A4->right = A4->right - 0xB6 + g_concourseDoors.battlezoom_text2_posX;
	A4->bottom = A4->bottom - 0x159 + g_concourseDoors.battlezoom_text2_posY;

	L00558D90(A4, A8, AC, A10);

	return 0;
}

int DrawKalidorZoomImage2Hook(int* params)
{
	const char* A4 = (const char*)params[0];
	int A8 = params[1];
	int AC = params[2];

	const auto L00534A60 = (void(*)(const char*, int, int))0x00534A60;

	L00534A60(A4, A8 - 0x184 + g_concourseDoors.kalidorzoom_image2_posX, AC - 0x78 + g_concourseDoors.kalidorzoom_image2_posY);

	return 0;
}

int DrawKalidorZoomText2Hook(int* params)
{
	int A4 = params[0];
	const char* A8 = (const char*)params[1];
	RECT* AC = (RECT*)params[2];
	int A10 = params[3];
	int A14 = params[4];

	const auto L00557720 = (void(*)(int, const char*, RECT*, int, int))0x00557720;

	AC->left = AC->left - 0x148 + g_concourseDoors.kalidorzoom_text2_posX;
	AC->top = AC->top - 0x69 + g_concourseDoors.kalidorzoom_text2_posY;
	AC->right = AC->right - 0x148 + g_concourseDoors.kalidorzoom_text2_posX;
	AC->bottom = AC->bottom - 0x69 + g_concourseDoors.kalidorzoom_text2_posY;

	L00557720(A4, A8, AC, A10, A14);

	return 0;
}

bool g_dsbrief_webm;
uint8_t* g_dsbrief_image;
unsigned int g_dsbrief_imageWidth;
unsigned int g_dsbrief_imageHeight;

int DSBriefLoadMovieHook(int* params)
{
	const char* A4 = (const char*)params[0];
	const char* A8 = (const char*)params[1];

	const auto XwaFrontResLoad = (int(*)(const char*, const char*))0x00531D70;

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	if (hasDC && std::ifstream("Resdata\\dsbrief.webm"))
	{
		g_dsbrief_webm = true;
		g_dsbrief_image = nullptr;
		g_dsbrief_imageWidth = 0;
		g_dsbrief_imageHeight = 0;

		WebmLoadVideo("dsbrief");
	}
	else
	{
		g_dsbrief_webm = false;

		XwaFrontResLoad(A4, A8);
	}

	if (g_dsbrief_webm)
	{
		*(unsigned short*)0x0056849B = 0x9090;
	}
	else
	{
		*(unsigned short*)0x0056849B = 0x0F75;
	}

	return 0;
}

int DSBriefFreeMovieHook(int* params)
{
	const char* A4 = (const char*)params[0];

	const auto XwaFrontResFreeItem = (void(*)(const char*))0x00532080;

	if (g_dsbrief_webm)
	{
		WebmFreeVideo("dsbrief");
	}
	else
	{
		XwaFrontResFreeItem(A4);
	}

	return 0;
}

int DSBriefMoveToNextImageHook(int* params)
{
	const char* A4 = (const char*)params[0];
	const bool A8 = (bool)params[1];

	const auto XwaFrontResMoveToNextImage = (bool(*)(const char*, bool))0x00532230;

	if (g_dsbrief_webm)
	{
		WebmReadVideoFrame("dsbrief", &g_dsbrief_image, &g_dsbrief_imageWidth, &g_dsbrief_imageHeight);
	}
	else
	{
		XwaFrontResMoveToNextImage(A4, A8);
	}

	return 0;
}

int DSBriefDrawImageHook(int* params)
{
	const char* A4 = (const char*)params[0];
	const int A8 = params[1];
	const int AC = params[2];

	const auto XwaFrontResDraw = (int(*)(const char*, short, short))0x00534A60;

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	if (g_dsbrief_webm)
	{
		if (g_dsbrief_image)
		{
			int posX = A8 + 5;
			int posY = AC + 3;

			bool isBackgroundHD = g_netFunctions._frontResIsBackgroundWide() != 0;

			int left = MultiplyDivH(posX, isBackgroundHD, dc);
			int top = MultiplyDivV(posY, dc);
			int width = MultiplyDivH(272, isBackgroundHD, dc);
			int height = MultiplyDivV(270, dc);

			DrawSurfaceDelegate(&dc, g_dsbrief_image, g_dsbrief_imageWidth, g_dsbrief_imageHeight, left, top, width, height, 0, 0, g_dsbrief_imageWidth, g_dsbrief_imageHeight, -10, 0, 25);
		}
	}
	else
	{
		XwaFrontResDraw(A4, A8, AC);
	}

	return 0;
}

struct SquadlogoSettings
{
	int PositionX;
	int PositionY;
	int Width;
	int Height;

	bool IsDefaultValues() const
	{
		return this->Width == 0 || this->Height == 0;
	}
};

SquadlogoSettings GetSquadlogoSettings(const std::string& name)
{
	SquadlogoSettings settings{};

	auto lines = GetFileLines("Resdata\\" + name + ".txt");

	settings.PositionX = GetFileKeyValueInt(lines, "PositionX", 0);
	settings.PositionY = GetFileKeyValueInt(lines, "PositionY", 0);
	settings.Width = GetFileKeyValueInt(lines, "Width", 0);
	settings.Height = GetFileKeyValueInt(lines, "Height", 0);

	return settings;
}

bool g_squadlogo_webm;
std::string g_squadlogo_name;
SquadlogoSettings g_squadlogo_settings;
uint8_t* g_squadlogo_image;
unsigned int g_squadlogo_imageWidth;
unsigned int g_squadlogo_imageHeight;

int SquadlogoLoadMovieHook(int* params)
{
	const char* A4 = (const char*)params[0];
	const char* A8 = (const char*)params[1];

	const auto XwaFrontResLoad = (int(*)(const char*, const char*))0x00531D70;

	XwaFrontResLoad(A4, A8);

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	g_squadlogo_webm = false;

	std::string squadlogoName = GetStringWithoutExtension(GetFileNameWithoutExtension(A4));

	if (std::ifstream("Resdata\\" + squadlogoName + ".webm"))
	{
		g_squadlogo_webm = true;
		g_squadlogo_name = squadlogoName;
		g_squadlogo_settings = GetSquadlogoSettings(squadlogoName);
		g_squadlogo_image = nullptr;
		g_squadlogo_imageWidth = 0;
		g_squadlogo_imageHeight = 0;

		WebmLoadVideo(g_squadlogo_name);
	}

	return 0;
}

int SquadlogoFreeMovieHook(int* params)
{
	const char* A4 = (const char*)params[0];

	const auto XwaFrontResFreeItem = (void(*)(const char*))0x00532080;

	XwaFrontResFreeItem(A4);

	if (g_squadlogo_webm)
	{
		WebmFreeVideo(g_squadlogo_name);
	}

	return 0;
}

int SquadlogoMoveToNextImageHook(int* params)
{
	const char* A4 = (const char*)params[0];
	const bool A8 = (bool)params[1];

	const auto XwaFrontResMoveToNextImage = (bool(*)(const char*, bool))0x00532230;

	XwaFrontResMoveToNextImage(A4, A8);

	if (g_squadlogo_webm)
	{
		WebmReadVideoFrame(g_squadlogo_name, &g_squadlogo_image, &g_squadlogo_imageWidth, &g_squadlogo_imageHeight);
	}

	return 0;
}

int SquadlogoDrawImageHook(int* params)
{
	const char* A4 = (const char*)params[0];
	int A8 = params[1];
	int AC = params[2];

	const auto XwaFrontResDraw = (int(*)(const char*, short, short))0x00534A60;
	const auto XwaFrontResGetArea = (bool(*)(const char*, LPRECT))0x00532180;

	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	if (g_squadlogo_webm)
	{
		if (g_squadlogo_image)
		{
			//bool isBackgroundHD = g_netFunctions._frontResIsBackgroundWide() != 0;
			bool isBackgroundHD = g_netFunctions._frontResIsBackgroundHD() != 0;

			unsigned char missionType = *(unsigned char*)(*(char**)0x009EB8E0 + 0x000B1B82);

			RECT rc{};
			XwaFrontResGetArea(A4, &rc);

			if (missionType != 0x07 && !g_squadlogo_settings.IsDefaultValues())
			{
				A8 = g_squadlogo_settings.PositionX;
				AC = g_squadlogo_settings.PositionY;
				rc.left = 0;
				rc.top = 0;
				rc.right = g_squadlogo_settings.Width;
				rc.bottom = g_squadlogo_settings.Height;

				int newWidth = MultiplyDivHReverse(MultiplyDivH(rc.right, false, dc), isBackgroundHD, dc);
				rc.right = newWidth;

				*(RECT*)(params + 11) = rc;
				*(int*)0x00784984 = A8;
				*(int*)0x00784980 = AC;
				params[Params_ECX] = *(int*)0x00784984;
				params[Params_EBX] = *(int*)0x00784980;
			}

			int posX = A8 + 0;
			int posY = AC + 0;

			int left = MultiplyDivH(posX, isBackgroundHD, dc);
			int top = MultiplyDivV(posY, dc);
			int width = MultiplyDivH(rc.right - rc.left, isBackgroundHD, dc);
			int height = MultiplyDivV(rc.bottom - rc.top, dc);

			DrawSurfaceDelegate(&dc, g_squadlogo_image, g_squadlogo_imageWidth, g_squadlogo_imageHeight, left, top, width, height, 0, 0, g_squadlogo_imageWidth, g_squadlogo_imageHeight, -10, 0, 25);
		}
	}
	else
	{
		XwaFrontResDraw(A4, A8, AC);
	}

	return 0;
}

bool g_techdoor_webm = false;
uint8_t* g_techdoor_image;
unsigned int g_techdoor_imageWidth;
unsigned int g_techdoor_imageHeight;

bool IsTechDoorMovie()
{
	return g_techdoor_webm;
}

void TechDoorLoadMovie()
{
	TechDoorFreeMovie();
	g_techdoor_webm = false;

	if (std::ifstream("Resdata\\techdoor.webm"))
	{
		g_techdoor_webm = true;
		g_techdoor_image = nullptr;
		g_techdoor_imageWidth = 0;
		g_techdoor_imageHeight = 0;

		WebmLoadVideo("techdoor");

		*(unsigned short*)0x0053A939 = 0x9090;
		*(int*)(0x0053A942 + 0x01) = (int)TechDoorMoveToNextImage - (0x0053A942 + 0x05);
	}
}

void TechDoorFreeMovie()
{
	if (g_techdoor_webm)
	{
		g_techdoor_webm = false;
		WebmFreeVideo("techdoor");

		*(unsigned short*)0x0053A939 = 0x0F75;
		*(int*)(0x0053A942 + 0x01) = 0x00532230 - (0x0053A942 + 0x05);
	}
}

void TechDoorMoveToNextImage()
{
	const auto XwaFrontResMoveToNextImage = (bool(*)(const char*, bool))0x00532230;

	if (g_techdoor_webm)
	{
		WebmReadVideoFrame("techdoor", &g_techdoor_image, &g_techdoor_imageWidth, &g_techdoor_imageHeight);
	}
}

void TechDoorDrawImage(int x, int y, int w, int h)
{
	SurfaceDC dc;
	bool hasDC = GetSurfaceDC(&dc);

	if (g_techdoor_webm)
	{
		if (g_techdoor_image)
		{
			bool isBackgroundHD = g_netFunctions._frontResIsBackgroundWide() != 0;

			int left = MultiplyDivH(x, isBackgroundHD, dc);
			int top = MultiplyDivV(y, dc);
			int width = MultiplyDivH(w, isBackgroundHD, dc);
			int height = MultiplyDivV(h, dc);

			DrawSurfaceDelegate(&dc, g_techdoor_image, g_techdoor_imageWidth, g_techdoor_imageHeight, left, top, width, height, 0, 0, g_techdoor_imageWidth, g_techdoor_imageHeight, -10, 0, 25);
		}
	}
}

int DrawMultiHook(int* params)
{
	((int(*)())0x0053F970)();

	return 0;
}

int DrawMulti2Hook(int* params)
{
	((void(*)())0x0053F010)();

	if (g_config.HDConcourseEnabled)
	{
		g_netFunctions._drawBackgroundToScreen();
	}

	return 0;
}

int ViewCustscenesConfigMenuSetPositionHook(int* params)
{
	RECT* pRect = (RECT*)params[0];
	pRect->left = params[1];
	pRect->top = params[2];
	pRect->right = params[3];
	pRect->bottom = params[4];

	bool isBackgroundHD = g_netFunctions._frontResIsBackgroundHD() != 0;

	if (g_config.HDConcourseEnabled && isBackgroundHD)
	{
		int width = pRect->right - pRect->left;
		pRect->left = 0;
		pRect->right = pRect->left + width;
	}

	return 0;
}

int DefineMedalCaseHook(int* params)
{
	unsigned int& s_V0x0784BCC = *(unsigned int*)0x0784BCC;
	unsigned int& s_XwaPilot_KalidorCresent = *(unsigned int*)0x00AF3906;

	auto lines = GetFileLines("hook_concourse.cfg");

	if (lines.empty())
	{
		lines = GetFileLines("hooks.ini", "hook_concourse");
	}

	int test_Overwrite = GetFileKeyValueInt(lines, "TestMedalCase_Overwrite", 0) != 0;
	int test_MedalCaseDuration = GetFileKeyValueInt(lines, "TestMedalCase_Duration", 10);
	int test_BattleZoomMedalIndex = GetFileKeyValueInt(lines, "TestMedalCase_BattleZoomMedalIndex", 1);
	int test_KalidorZoomMedalIndex = GetFileKeyValueInt(lines, "TestMedalCase_KalidorZoomMedalIndex", 1);

	if (test_Overwrite)
	{
		*(int*)(0x00584902 + 0x06) = 24 * test_MedalCaseDuration;
		s_V0x0784BCC = test_BattleZoomMedalIndex | (test_KalidorZoomMedalIndex == 0 ? 0 : 0x80000000);
		s_XwaPilot_KalidorCresent = test_KalidorZoomMedalIndex;
	}

	params[Params_EAX] = *(int*)0x00784BCC;
	return 0;
}
