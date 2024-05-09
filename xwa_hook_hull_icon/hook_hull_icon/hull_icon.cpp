#include "targetver.h"
#include "hull_icon.h"
#include "config.h"
#include <fstream>

std::string GetFileNameWithoutExtension(const std::string& str)
{
	auto a = str.find_last_of('\\');

	return a == -1 ? str : str.substr(a + 1, -1);
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
		auto lines = GetFileLines("hook_hull_icon.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_hull_icon");
		}

		this->MapIconScale = GetFileKeyValueInt(lines, "MapIconScale", 256);
		this->HullIconScale = GetFileKeyValueInt(lines, "HullIconScale", 256);
	}

	int MapIconScale;
	int HullIconScale;
};

Config g_config;

#pragma pack(push, 1)

struct XwaObject
{
	char Unk0000[2];
	unsigned short ModelIndex;
	char Unk0004[35];
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

struct XwaPlayer
{
	int ObjectIndex;
	char Unk0004[3019];
};

static_assert(sizeof(XwaPlayer) == 3023, "size of XwaPlayer must be 3023");

struct S0x042BD80
{
	int S0x042BD80_m00;
	int S0x042BD80_m04;
	int S0x042BD80_m08;
	short S0x042BD80_m0C;
	short S0x042BD80_m0E;
};

static_assert(sizeof(S0x042BD80) == 16, "size of S0x042BD80 must be 16");

#pragma pack(pop)

std::vector<unsigned short> GetDefaultHullIconValues()
{
	const auto hudIcons = (const unsigned short*)0x005A9608;

	std::vector<unsigned short> values;

	for (int i = 0; i < 72; i++)
	{
		values.push_back(hudIcons[i]);
	}

	return values;
}

class HullIconValues
{
public:
	HullIconValues()
	{
		if (std::ifstream("HullIconList.txt"))
		{
			const auto lines = GetFileLines("HullIconList.txt");
			this->_hullIcons = GetFileListUnsignedShortValues(lines);
		}
		else
		{
			this->_hullIcons = GetDefaultHullIconValues();
		}
	}

	std::vector<unsigned short> _hullIcons;
};

HullIconValues g_hullIconValues;

std::vector<unsigned short> GetDefaultMapIconValues()
{
	const auto mapIcons = (const unsigned short*)0x005A9AD8;

	std::vector<unsigned short> values;

	for (int i = 0; i < 222; i++)
	{
		values.push_back(mapIcons[i]);
	}

	return values;
}

class MapIconValues
{
public:
	MapIconValues()
	{
		if (std::ifstream("MapIconList.txt"))
		{
			const auto lines = GetFileLines("MapIconList.txt");
			this->_mapIcons = GetFileListUnsignedShortValues(lines);
		}
		else
		{
			this->_mapIcons = GetDefaultMapIconValues();
		}
	}

	std::vector<unsigned short> _mapIcons;
};

MapIconValues g_mapIconValues;

unsigned short GetPlayerCraftHullIcon()
{
	static std::string _mission;
	static int _missionIndex = 0;
	static unsigned short _playerHullIcon;

	const char* xwaMissionFileName = (const char*)0x06002E8;
	const int missionFileNameIndex = *(int*)0x06002E4;

	if (missionFileNameIndex == 0 ? (_mission != xwaMissionFileName) : (_missionIndex != missionFileNameIndex))
	{
		_mission = xwaMissionFileName;
		_missionIndex = missionFileNameIndex;

		const std::string mission = GetStringWithoutExtension(xwaMissionFileName);
		std::vector<std::string> lines = GetFileLines(mission + "_HullIcon.txt");

		if (!lines.size())
		{
			lines = GetFileLines(mission + ".ini", "HullIcon");
		}

		_playerHullIcon = (unsigned short)GetFileKeyValueInt(lines, "PlayerHullIcon", 0);
	}

	return _playerHullIcon;
}

unsigned short GetCraftHullIcon()
{
	static std::string _mission;
	static int _missionIndex = 0;
	static unsigned short _hullIcon;

	const char* xwaMissionFileName = (const char*)0x06002E8;
	const int missionFileNameIndex = *(int*)0x06002E4;

	if (missionFileNameIndex == 0 ? (_mission != xwaMissionFileName) : (_missionIndex != missionFileNameIndex))
	{
		_mission = xwaMissionFileName;
		_missionIndex = missionFileNameIndex;

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

		const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
		const XwaPlayer* xwaPlayers = (XwaPlayer*)0x008B94E0;
		const int currentPlayerId = *(int*)0x008C1CC8;

		int modelIndex = xwaObjects[xwaPlayers[currentPlayerId].ObjectIndex].ModelIndex;
		std::string shipPath = g_flightModelsList.GetLstLine(modelIndex);

		const std::string objectValue = GetFileKeyValue(lines, shipPath + ".opt");

		if (!objectValue.empty() && std::ifstream(objectValue))
		{
			shipPath = GetStringWithoutExtension(objectValue);
		}

		_hullIcon = (unsigned short)GetFileKeyValueInt(lines, shipPath + "_HullIcon", 0);
	}

	return _hullIcon;
}

int HullIconHook(int* params)
{
	const unsigned short playerHullIcon = GetPlayerCraftHullIcon();

	if (playerHullIcon != 0)
	{
		return playerHullIcon;
	}

	const unsigned short craftHullIcon = GetCraftHullIcon();

	if (craftHullIcon != 0)
	{
		return craftHullIcon;
	}

	const unsigned short modelIndex = (unsigned short)params[0];

	if (modelIndex < g_hullIconValues._hullIcons.size())
	{
		return g_hullIconValues._hullIcons[modelIndex];
	}

	return 0;
}

int MapIconHook(int* params)
{
	const unsigned short modelIndex = (unsigned short)params[0];
	int& icon = params[0];

	if (modelIndex < g_mapIconValues._mapIcons.size())
	{
		icon = g_mapIconValues._mapIcons[modelIndex];
	}
	else
	{
		icon = 0;
	}

	return 0;
}

int RenderMapIconHook(int* params)
{
	const int A4 = params[0];
	const S0x042BD80* A8 = (S0x042BD80*)params[1];
	const unsigned int AC = (unsigned int)params[2];

	const auto L0042BD80 = (void(*)(int, const S0x042BD80*, unsigned int))0x0042BD80;

	S0x042BD80 settings = *A8;
	settings.S0x042BD80_m0E = g_config.MapIconScale;

	L0042BD80(A4, &settings, AC);

	return 0;
}

int RenderHullIconHook(int* params)
{
	const int A4 = params[0];
	const S0x042BD80* A8 = (S0x042BD80*)params[1];
	const unsigned int AC = (unsigned int)params[2];

	const auto L0042BD80 = (void(*)(int, const S0x042BD80*, unsigned int))0x0042BD80;

	S0x042BD80 settings = *A8;
	settings.S0x042BD80_m0E = g_config.HullIconScale;

	L0042BD80(A4, &settings, AC);

	return 0;
}
