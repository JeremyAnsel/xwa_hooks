#include "targetver.h"
#include "weapon.h"
#include "config.h"
#include <map>
#include <utility>

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
	char Unk01AF[16];
	short m1BF[3];
	int m1C5[3];
	char Unk01D1[270];
	XwaCraftWeaponRack WeaponRacks[16];
	char Unk03BF[58];
};

static_assert(sizeof(XwaCraft) == 1017, "size of XwaCraft must be 1017");

struct XwaObject
{
	char Unk0000[2];
	unsigned short ModelIndex;
	char Unk0004[35];
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

#pragma pack(pop)

int GetWeaponDechargeRate(int modelIndex)
{
	const std::string ship = g_flightModelsList.GetLstLine(modelIndex);

	auto lines = GetFileLines(ship + "WeaponRate.txt");

	if (!lines.size())
	{
		lines = GetFileLines(ship + ".ini", "WeaponRate");
	}

	int rate = GetFileKeyValueInt(lines, "DechargeRate", -1);

	if (rate == -1)
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
	const std::string ship = g_flightModelsList.GetLstLine(modelIndex);

	auto lines = GetFileLines(ship + "WeaponRate.txt");

	if (!lines.size())
	{
		lines = GetFileLines(ship + ".ini", "WeaponRate");
	}

	int rate = GetFileKeyValueInt(lines, "RechargeRate", -1);

	if (rate == -1)
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

int GetWeaponCooldownTime(int modelIndex)
{
	const std::string ship = g_flightModelsList.GetLstLine(modelIndex);

	auto lines = GetFileLines(ship + "WeaponRate.txt");

	if (!lines.size())
	{
		lines = GetFileLines(ship + ".ini", "WeaponRate");
	}

	int rate = GetFileKeyValueInt(lines, "CooldownTimeFactor", 0x2F);

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

	int GetCooldownTime(int modelIndex)
	{
		auto it = this->_weaponCooldownTime.find(modelIndex);

		if (it != this->_weaponCooldownTime.end())
		{
			return it->second;
		}
		else
		{
			int value = GetWeaponCooldownTime(modelIndex);
			this->_weaponCooldownTime.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

private:
	std::map<int, int> _weaponDechargeRate;
	std::map<int, int> _weaponRechargeRate;
	std::map<int, int> _weaponCooldownTime;
};

ModelIndexWeapon g_modelIndexWeapon;

int WeaponDechargeHook(int* params)
{
	const int objectIndex = params[12];
	const int weaponRackIndex = (unsigned short)params[14];

	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	XwaCraft* XwaCurrentCraft = *(XwaCraft**)0x00910DFC;

	if (XwaCurrentCraft->WeaponRacks[weaponRackIndex].Sequence < 4)
	{
		unsigned short modelIndex = XwaObjects[objectIndex].ModelIndex;
		int dechargeRate = g_modelIndexWeapon.GetDechargeRate(modelIndex);

		XwaCurrentCraft->WeaponRacks[weaponRackIndex].Charge -= dechargeRate;
	}

	return 0;
}

int WeaponRechargeHook(int* params)
{
	unsigned short modelIndex = (unsigned short)params[0];

	int rechargeRate = g_modelIndexWeapon.GetRechargeRate(modelIndex);
	params[0] = rechargeRate;

	return 0;
}

int WeaponCooldownTimeHook(int* params)
{
	const int esp10 = params[4];
	const int A4 = params[12];
	const int A8 = params[13];
	const int AC = params[Params_EBX];

	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	XwaCraft* XwaCurrentCraft = *(XwaCraft**)0x00910DFC;

	unsigned short modelIndex = XwaObjects[A4].ModelIndex;
	int rate = g_modelIndexWeapon.GetCooldownTime(modelIndex);

	if (AC != -1)
	{
		XwaCurrentCraft->m1BF[0] += esp10 * rate + 0x02;
		XwaCurrentCraft->m1C5[0] += esp10 * rate + 0x02;
	}
	else
	{
		XwaCurrentCraft->m1BF[A8] += esp10 * rate + 0x02;
		XwaCurrentCraft->m1C5[A8] += esp10 * rate + 0x02;
	}

	return 0;
}
