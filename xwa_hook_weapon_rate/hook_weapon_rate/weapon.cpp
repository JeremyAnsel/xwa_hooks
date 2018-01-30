#include "targetver.h"
#include "weapon.h"
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <vector>
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
		return 0;
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

int GetWeaponDechargeRate(int modelIndex)
{
	std::string pilotPath = g_flightModelsList.GetLstLine(modelIndex);

	if (!pilotPath.empty())
	{
		pilotPath.append("WeaponRate.txt");
	}

	int rate = 0;

	if (!pilotPath.empty() && std::ifstream(pilotPath))
	{
		rate = GetFileKeyValueInt(pilotPath, "DechargeRate");
	}
	else
	{
		const int xwaObjectStats = 0x05FB240;
		const char category = *(char*)(xwaObjectStats + modelIndex * 0x18 + 0x03);

		// ShipCategory_Starfighter
		if (category == 0)
		{
			switch (modelIndex)
			{
			case 5: // ModelIndex_005_0_4_TieFighter
			case 7: // ModelIndex_007_0_6_TieBomber
				rate = 3;
				break;

			default:
				rate = 4;
				break;
			}
		}
		else
		{
			rate = 3;
		}
	}

	return rate;
}

int GetWeaponRechargeRate(int modelIndex)
{
	std::string pilotPath = g_flightModelsList.GetLstLine(modelIndex);

	if (!pilotPath.empty())
	{
		pilotPath.append("WeaponRate.txt");
	}

	int rate = 0;

	if (!pilotPath.empty() && std::ifstream(pilotPath))
	{
		rate = GetFileKeyValueInt(pilotPath, "RechargeRate");
	}
	else
	{
		switch (modelIndex)
		{
		case 5: // ModelIndex_005_0_4_TieFighter
		case 7: // ModelIndex_007_0_6_TieBomber
			rate = 3;
			break;

		default:
			rate = 2;
			break;
		}
	}

	return rate;
}

class ModelIndexWeapon
{
public:
	int GetDechargeRate(int modelIndex)
	{
		auto it = this->_weaponDechargeRate.find(modelIndex);

		if (it != this->_weaponDechargeRate.end())
		{
			return it->second;
		}
		else
		{
			int value = GetWeaponDechargeRate(modelIndex);
			this->_weaponDechargeRate.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

	int GetRechargeRate(int modelIndex)
	{
		auto it = this->_weaponRechargeRate.find(modelIndex);

		if (it != this->_weaponRechargeRate.end())
		{
			return it->second;
		}
		else
		{
			int value = GetWeaponRechargeRate(modelIndex);
			this->_weaponRechargeRate.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

private:
	std::map<int, int> _weaponDechargeRate;
	std::map<int, int> _weaponRechargeRate;
};

ModelIndexWeapon g_modelIndexWeapon;

#pragma pack(push, 1)

struct XwaCraftWeaponRack
{
	char Unk0000[2];
	char Sequence;
	char Unk0003[1];
	char Charge;
	char Unk0005[9];
};

static_assert(sizeof(XwaCraftWeaponRack) == 14, "size of XwaCraftWeaponRack must be 14");

struct XwaCraft
{
	char Unk0000[430];
	char WeaponRacksCount;
	char Unk01AF[304];
	XwaCraftWeaponRack WeaponRacks[16];
	char Unk03BF[58];
};

static_assert(sizeof(XwaCraft) == 1017, "size of XwaCraft must be 1017");

struct XwaObject
{
	char Unk0000[2];
	short ModelIndex;
	char Unk0004[35];
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

#pragma pack(pop)

int WeaponDechargeHook(int* params)
{
	int objectIndex = params[12];
	int weaponRackIndex = params[14] & 0xFFFF;

	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	XwaCraft* XwaCurrentCraft = *(XwaCraft**)0x00910DFC;

	if (XwaCurrentCraft->WeaponRacks[weaponRackIndex].Sequence < 4)
	{
		short modelIndex = XwaObjects[objectIndex].ModelIndex;
		int dechargeRate = g_modelIndexWeapon.GetDechargeRate(modelIndex);

		XwaCurrentCraft->WeaponRacks[weaponRackIndex].Charge -= dechargeRate;
	}

	return 0;
}

int WeaponRechargeHook(int* params)
{
	int modelIndex = params[0] & 0xFFFF;

	int rechargeRate = g_modelIndexWeapon.GetRechargeRate(modelIndex);
	params[0] = rechargeRate;

	return 0;
}
