#include "targetver.h"
#include "weapon.h"
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <vector>
#include <array>
#include <map>
#include <utility>

std::string GetStringWithoutExtension(const std::string& str)
{
	auto b = str.find_last_of('.');

	return str.substr(0, b);
}

std::string GetFileKeyValue(const std::string& path, const std::string& key)
{
	std::ifstream file(path);

	if (file)
	{
		std::string line;

		while (std::getline(file, line))
		{
			if (!line.length())
			{
				continue;
			}

			if (line[0] == '#' || line[0] == ';' || (line[0] == '/' && line[1] == '/'))
			{
				continue;
			}

			int pos = line.find("=");

			if (pos == -1)
			{
				continue;
			}

			std::string name = line.substr(0, pos);
			name.erase(std::remove_if(name.begin(), name.end(), std::isspace), name.end());

			std::string value = line.substr(pos + 1);
			value.erase(std::remove_if(value.begin(), value.end(), std::isspace), value.end());

			if (!name.length() || !value.length())
			{
				continue;
			}

			if (_stricmp(name.c_str(), key.c_str()) == 0)
			{
				return value;
			}
		}
	}

	return std::string();
}

int GetFileKeyValueInt(const std::string& path, const std::string& key)
{
	std::string value = GetFileKeyValue(path, key);

	if (value.empty())
	{
		return -1;
	}

	return std::stoi(value, 0, 0);
}

std::vector<std::string> GetFileLines(const std::string& path)
{
	std::vector<std::string> values;

	std::ifstream file(path);

	if (file)
	{
		std::string line;

		while (std::getline(file, line))
		{
			if (!line.length())
			{
				continue;
			}

			if (line[0] == '#' || line[0] == ';' || (line[0] == '/' && line[1] == '/'))
			{
				continue;
			}

			values.push_back(line);
		}
	}

	return values;
}

class FlightModelsList
{
public:
	FlightModelsList()
	{
		for (auto& line : GetFileLines("FlightModels\\Spacecraft0.LST"))
		{
			this->_spacecraftList.push_back(GetStringWithoutExtension(line));
		}

		for (auto& line : GetFileLines("FlightModels\\Equipment0.LST"))
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

std::array<int, 28> GetWeaponColor(int modelIndex)
{
	std::string pilotPath = g_flightModelsList.GetLstLine(modelIndex);

	if (!pilotPath.empty())
	{
		pilotPath.append("WeaponColor.txt");
	}

	std::array<int, 28> color;

	if (!pilotPath.empty() && std::ifstream(pilotPath))
	{
		int defaultValue = GetFileKeyValueInt(pilotPath, "WeaponColor");

		if (defaultValue != -1)
		{
			color.fill(defaultValue);
		}
		else
		{
			color.fill(0);
		}

		for (int i = 0; i < 28; i++)
		{
			const std::string key = std::string("WeaponColor") + std::to_string(280 + i);
			int value = GetFileKeyValueInt(pilotPath, key);

			if (value != -1)
			{
				color[i] = value;
			}
		}
	}
	else
	{
		color.fill(0);
	}

	return color;
}

class ModelIndexWeapon
{
public:
	int GetColor(int modelIndex, int weaponModelIndex)
	{
		if (weaponModelIndex < 280 || weaponModelIndex >= 280 + 28)
		{
			return 0;
		}

		const auto it = this->_weaponColor.find(modelIndex);

		if (it != this->_weaponColor.end())
		{
			return it->second[weaponModelIndex - 280];
		}
		else
		{
			const auto value = GetWeaponColor(modelIndex);
			this->_weaponColor.insert(std::make_pair(modelIndex, value));
			return value[weaponModelIndex - 280];
		}
	}

private:
	std::map<int, std::array<int, 28>> _weaponColor;
};

ModelIndexWeapon g_modelIndexWeapon;

#pragma pack(push, 1)

struct XwaMobileObject
{
	char Unk0000[153];
	unsigned char Markings;
	char Unk009A[75];
};

static_assert(sizeof(XwaMobileObject) == 229, "size of XwaMobileObject must be 229");

struct XwaObject
{
	char Unk0000[2];
	short ModelIndex;
	char Unk0004[31];
	XwaMobileObject* pMobileObject;
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

#pragma pack(pop)

int WeaponColorHook(int* params)
{
	int objectIndex = params[54];
	short weaponModelIndex = params[56] & 0xFFFF;
	int weaponObjectIndex = params[28];

	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;

	short objectModelIndex = XwaObjects[objectIndex].ModelIndex;
	int color = g_modelIndexWeapon.GetColor(objectModelIndex, weaponModelIndex);

	XwaObjects[weaponObjectIndex].pMobileObject->Markings = color;

	return *(int*)0x007B33C4;
}
