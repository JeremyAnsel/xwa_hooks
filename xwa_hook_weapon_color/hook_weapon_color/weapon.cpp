#include "targetver.h"
#include "weapon.h"
#include "config.h"
#include <array>
#include <map>
#include <utility>

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
	unsigned short ModelIndex;
	char Unk0004[31];
	XwaMobileObject* pMobileObject;
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

#pragma pack(pop)

std::array<int, 28> GetWeaponColor(int modelIndex)
{
	const std::string ship = g_flightModelsList.GetLstLine(modelIndex);

	auto lines = GetFileLines(ship + "WeaponColor.txt");

	if (!lines.size())
	{
		lines = GetFileLines(ship + ".ini", "WeaponColor");
	}

	std::array<int, 28> color;

	if (lines.size())
	{
		int defaultValue = GetFileKeyValueInt(lines, "WeaponColor", -1);

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
			int value = GetFileKeyValueInt(lines, key, -1);

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

		auto it = this->_weaponColor.find(modelIndex);

		if (it != this->_weaponColor.end())
		{
			return it->second[weaponModelIndex - 280];
		}
		else
		{
			auto value = GetWeaponColor(modelIndex);
			this->_weaponColor.insert(std::make_pair(modelIndex, value));
			return value[weaponModelIndex - 280];
		}
	}

private:
	std::map<int, std::array<int, 28>> _weaponColor;
};

ModelIndexWeapon g_modelIndexWeapon;

int WeaponColorHook(int* params)
{
	const int objectIndex = params[54];
	const unsigned short weaponModelIndex = (unsigned short)params[56];
	const int weaponObjectIndex = params[28];

	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;

	unsigned short objectModelIndex = XwaObjects[objectIndex].ModelIndex;
	int color = g_modelIndexWeapon.GetColor(objectModelIndex, weaponModelIndex);

	XwaObjects[weaponObjectIndex].pMobileObject->Markings = color;

	return *(int*)0x007B33C4;
}

int WeaponColorCapitalShipHook(int* params)
{
	const int objectIndex = (unsigned short)params[278];
	const int weaponObjectIndex = params[272];

	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;

	unsigned short weaponModelIndex = XwaObjects[weaponObjectIndex].ModelIndex;
	unsigned short objectModelIndex = XwaObjects[objectIndex].ModelIndex;
	int color = g_modelIndexWeapon.GetColor(objectModelIndex, weaponModelIndex);

	XwaObjects[weaponObjectIndex].pMobileObject->Markings = color;

	return *(int*)0x007B33C4;
}
