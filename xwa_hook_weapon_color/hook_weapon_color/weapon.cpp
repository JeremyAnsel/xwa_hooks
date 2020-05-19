#include "targetver.h"
#include "weapon.h"
#include "config.h"
#include <array>
#include <map>
#include <utility>

unsigned int GetFileKeyValueUnsignedInt(const std::vector<std::string>& lines, const std::string& key, unsigned int defaultValue = 0)
{
	std::string value = GetFileKeyValue(lines, key);

	if (value.empty())
	{
		return defaultValue;
	}

	return std::stoul(value, 0, 16);
}

class Config
{
public:
	Config()
	{
		const auto lines = GetFileLines("hook_weapon_color.cfg");

		this->WeaponSwitchBasedOnIff = GetFileKeyValueInt(lines, "WeaponSwitchBasedOnIff", 0) != 0;

		this->WeaponImpactColor_3100 = GetFileKeyValueUnsignedInt(lines, "WeaponImpactColor_3100", 0xFF0000FF); // blue
		this->WeaponImpactColor_3200 = GetFileKeyValueUnsignedInt(lines, "WeaponImpactColor_3200", 0xFFFF0000); // red
		this->WeaponImpactColor_3300 = GetFileKeyValueUnsignedInt(lines, "WeaponImpactColor_3300", 0xFF00FF00); // green
		this->WeaponImpactColor_3400 = GetFileKeyValueUnsignedInt(lines, "WeaponImpactColor_3400", 0xFFFF00FF); // purple
		this->WeaponImpactColor_3500 = GetFileKeyValueUnsignedInt(lines, "WeaponImpactColor_3500", 0xFFFFFF00); // yellow

		for (int i = 0; i < 28; i++)
		{
			const std::string key = std::string("WeaponLightColor") + std::to_string(280 + i);

			this->WeaponLightColor[i] = GetFileKeyValueUnsignedInt(lines, key);
		}
	}

	bool WeaponSwitchBasedOnIff;

	unsigned int WeaponImpactColor_3100;
	unsigned int WeaponImpactColor_3200;
	unsigned int WeaponImpactColor_3300;
	unsigned int WeaponImpactColor_3400;
	unsigned int WeaponImpactColor_3500;

	std::array<unsigned int, 28> WeaponLightColor;
};

Config g_config;

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
	char Unk0000[147];
	short ObjectIndex;
	unsigned short ModelIndex;
	unsigned char Iff;
	char Unk0098[1];
	unsigned char Markings;
	char Unk009A[75];
};

static_assert(sizeof(XwaMobileObject) == 229, "size of XwaMobileObject must be 229");

struct XwaObject
{
	char Unk0000[2];
	unsigned short ModelIndex;
	char Unk0004[23];
	short m1B;
	char Unk001D[6];
	XwaMobileObject* pMobileObject;
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

struct XwaPlayer
{
	int ObjectIndex;
	char Unk0004[3019];
};

static_assert(sizeof(XwaPlayer) == 3023, "size of XwaPlayer must be 3023");

struct XwaCraft
{
	char Unk0000[432];
	unsigned short LaserTypeId[3];
	char Unk01B6[579];
};

static_assert(sizeof(XwaCraft) == 1017, "size of XwaCraft must be 1017");

struct XwaKnockoutData
{
	char Unk0000[712];
	unsigned int Color;
	char Unk02CC[24];
};

static_assert(sizeof(XwaKnockoutData) == 740, "size of XwaKnockoutData must be 740");

struct S0x0761E70
{
	int S0x0761E70_m00;
	int S0x0761E70_m04;
	unsigned short S0x0761E70_m08;
	char Unk000A[50];
	unsigned short S0x0761E70_m3C;
	char Unk003E[13];
};

static_assert(sizeof(S0x0761E70) == 75, "size of S0x0761E70 must be 75");

struct S0x07FA360
{
	int PositionX;
	int PositionY;
	int PositionZ;
	char unk0C[12];
	float m18;
	int m1C;
	float m20;
	float ColorR;
	float ColorB;
	float ColorG;
};

static_assert(sizeof(S0x07FA360) == 48, "size of S0x07FA360 must be 48");

#pragma pack(pop)

int GetWeaponSwitch(int modelIndex)
{
	const std::string ship = g_flightModelsList.GetLstLine(modelIndex);

	auto lines = GetFileLines(ship + "WeaponColor.txt");

	if (!lines.size())
	{
		lines = GetFileLines(ship + ".ini", "WeaponColor");
	}

	int weaponSwitch = -1;

	if (lines.size())
	{
		weaponSwitch = GetFileKeyValueInt(lines, "WeaponSwitchBasedOnIff", -1);
	}

	return weaponSwitch;
}

std::string GetMarkingsKey(std::string key, int markings)
{
	if (markings == -1)
	{
		return key;
	}

	return key + "_fg" + std::to_string(markings) + "_";
}

std::array<int, 28> GetWeaponColor(int modelIndex, int markings)
{
	const std::string ship = g_flightModelsList.GetLstLine(modelIndex);

	auto lines = GetFileLines(ship + "WeaponColor.txt");

	if (!lines.size())
	{
		lines = GetFileLines(ship + ".ini", "WeaponColor");
	}

	std::array<int, 28> color;

	if (markings != -1)
	{
		color = GetWeaponColor(modelIndex, -1);
	}

	if (lines.size())
	{
		const std::string key = GetMarkingsKey("WeaponColor", markings);

		int defaultValue = GetFileKeyValueInt(lines, key, -1);

		if (defaultValue != -1)
		{
			color.fill(defaultValue);
		}
		else
		{
			if (markings == -1)
			{
				color.fill(0);
			}
		}

		for (int i = 0; i < 28; i++)
		{
			const std::string valueKey = key + std::to_string(280 + i);
			int value = GetFileKeyValueInt(lines, valueKey, -1);

			if (value != -1)
			{
				color[i] = value;
			}
		}
	}
	else
	{
		if (markings == -1)
		{
			color.fill(0);
		}
	}

	return color;
}

std::array<unsigned int, 28> GetWeaponImpactColor(int modelIndex, int markings)
{
	const std::string ship = g_flightModelsList.GetLstLine(modelIndex);

	auto lines = GetFileLines(ship + "WeaponColor.txt");

	if (!lines.size())
	{
		lines = GetFileLines(ship + ".ini", "WeaponColor");
	}

	std::array<unsigned int, 28> color;

	if (markings != -1)
	{
		color = GetWeaponImpactColor(modelIndex, -1);
	}

	if (markings == -1)
	{
		unsigned int ResData_Animations_3100 = GetFileKeyValueUnsignedInt(lines, GetMarkingsKey("WeaponImpactColor_3100", markings), g_config.WeaponImpactColor_3100); // blue
		unsigned int ResData_Animations_3200 = GetFileKeyValueUnsignedInt(lines, GetMarkingsKey("WeaponImpactColor_3200", markings), g_config.WeaponImpactColor_3200); // red
		unsigned int ResData_Animations_3300 = GetFileKeyValueUnsignedInt(lines, GetMarkingsKey("WeaponImpactColor_3300", markings), g_config.WeaponImpactColor_3300); // green
		unsigned int ResData_Animations_3400 = GetFileKeyValueUnsignedInt(lines, GetMarkingsKey("WeaponImpactColor_3400", markings), g_config.WeaponImpactColor_3400); // purple
		unsigned int ResData_Animations_3500 = GetFileKeyValueUnsignedInt(lines, GetMarkingsKey("WeaponImpactColor_3500", markings), g_config.WeaponImpactColor_3500); // yellow

		color[0] = ResData_Animations_3200;
		color[1] = ResData_Animations_3200;
		color[2] = ResData_Animations_3300;
		color[3] = ResData_Animations_3300;
		color[4] = ResData_Animations_3100;
		color[5] = ResData_Animations_3100;
		color[6] = ResData_Animations_3500;
		color[7] = ResData_Animations_3500;
		color[8] = ResData_Animations_3200;
		color[9] = ResData_Animations_3300;
		color[10] = ResData_Animations_3100;
		color[11] = ResData_Animations_3500;
		color[12] = ResData_Animations_3500;
		color[13] = ResData_Animations_3500;
		color[14] = ResData_Animations_3500;
		color[15] = ResData_Animations_3400;
		color[16] = ResData_Animations_3100;
		color[17] = ResData_Animations_3200;
		color[18] = ResData_Animations_3400;
		color[19] = ResData_Animations_3500;
		color[20] = ResData_Animations_3500;
		color[21] = ResData_Animations_3200;
		color[22] = ResData_Animations_3200;
		color[23] = ResData_Animations_3300;
		color[24] = ResData_Animations_3300;
		color[25] = ResData_Animations_3300;
		color[26] = ResData_Animations_3300;
		color[27] = ResData_Animations_3500;
	}
	else
	{
		unsigned int ResData_Animations_3100 = GetFileKeyValueUnsignedInt(lines, GetMarkingsKey("WeaponImpactColor_3100", markings), 0); // blue
		unsigned int ResData_Animations_3200 = GetFileKeyValueUnsignedInt(lines, GetMarkingsKey("WeaponImpactColor_3200", markings), 0); // red
		unsigned int ResData_Animations_3300 = GetFileKeyValueUnsignedInt(lines, GetMarkingsKey("WeaponImpactColor_3300", markings), 0); // green
		unsigned int ResData_Animations_3400 = GetFileKeyValueUnsignedInt(lines, GetMarkingsKey("WeaponImpactColor_3400", markings), 0); // purple
		unsigned int ResData_Animations_3500 = GetFileKeyValueUnsignedInt(lines, GetMarkingsKey("WeaponImpactColor_3500", markings), 0); // yellow

		if (ResData_Animations_3100 != 0)
		{
			color[4] = ResData_Animations_3100;
			color[5] = ResData_Animations_3100;
			color[10] = ResData_Animations_3100;
			color[16] = ResData_Animations_3100;
		}

		if (ResData_Animations_3200 != 0)
		{
			color[0] = ResData_Animations_3200;
			color[1] = ResData_Animations_3200;
			color[8] = ResData_Animations_3200;
			color[17] = ResData_Animations_3200;
			color[21] = ResData_Animations_3200;
			color[22] = ResData_Animations_3200;
		}

		if (ResData_Animations_3300 != 0)
		{
			color[2] = ResData_Animations_3300;
			color[3] = ResData_Animations_3300;
			color[9] = ResData_Animations_3300;
			color[23] = ResData_Animations_3300;
			color[24] = ResData_Animations_3300;
			color[25] = ResData_Animations_3300;
			color[26] = ResData_Animations_3300;
		}

		if (ResData_Animations_3400 != 0)
		{
			color[15] = ResData_Animations_3400;
			color[18] = ResData_Animations_3400;
		}

		if (ResData_Animations_3500 != 0)
		{
			color[6] = ResData_Animations_3500;
			color[7] = ResData_Animations_3500;
			color[11] = ResData_Animations_3500;
			color[12] = ResData_Animations_3500;
			color[13] = ResData_Animations_3500;
			color[14] = ResData_Animations_3500;
			color[19] = ResData_Animations_3500;
			color[20] = ResData_Animations_3500;
			color[27] = ResData_Animations_3500;
		}
	}

	if (lines.size())
	{
		for (int i = 0; i < 28; i++)
		{
			const std::string key = GetMarkingsKey("WeaponImpactColor", markings) + std::to_string(280 + i);
			unsigned int value = GetFileKeyValueUnsignedInt(lines, key);

			if (value != 0)
			{
				color[i] = value;
			}
		}
	}

	return color;
}

struct WeaponEnergyBarColor
{
	unsigned int LaserHigh;
	unsigned int LaserLow;
	unsigned int IonHigh;
	unsigned int IonLow;
};

WeaponEnergyBarColor GetWeaponEnergyBarColor(int modelIndex, int markings)
{
	const std::string ship = g_flightModelsList.GetLstLine(modelIndex);

	auto lines = GetFileLines(ship + "WeaponColor.txt");

	if (!lines.size())
	{
		lines = GetFileLines(ship + ".ini", "WeaponColor");
	}

	WeaponEnergyBarColor color{};

	if (markings != -1)
	{
		color = GetWeaponEnergyBarColor(modelIndex, -1);
	}

	if (lines.size())
	{
		unsigned int value;

		value = GetFileKeyValueUnsignedInt(lines, GetMarkingsKey("EnergyBarColorLaserHigh", markings));

		if (value != 0)
		{
			color.LaserHigh = value;
		}

		value = GetFileKeyValueUnsignedInt(lines, GetMarkingsKey("EnergyBarColorLaserLow", markings));

		if (value != 0)
		{
			color.LaserLow = value;
		}

		value = GetFileKeyValueUnsignedInt(lines, GetMarkingsKey("EnergyBarColorIonHigh", markings));

		if (value != 0)
		{
			color.IonHigh = value;
		}

		value = GetFileKeyValueUnsignedInt(lines, GetMarkingsKey("EnergyBarColorIonLow", markings));

		if (value != 0)
		{
			color.IonLow = value;
		}
	}

	return color;
}

std::array<unsigned int, 28> GetWeaponLightColor(int modelIndex, int markings)
{
	const std::string ship = g_flightModelsList.GetLstLine(modelIndex);

	auto lines = GetFileLines(ship + "WeaponColor.txt");

	if (!lines.size())
	{
		lines = GetFileLines(ship + ".ini", "WeaponColor");
	}

	std::array<unsigned int, 28> color = g_config.WeaponLightColor;

	if (markings != -1)
	{
		color = GetWeaponLightColor(modelIndex, -1);
	}

	if (lines.size())
	{
		for (int i = 0; i < 28; i++)
		{
			const std::string key = GetMarkingsKey("WeaponLightColor", markings) + std::to_string(280 + i);
			unsigned int value = GetFileKeyValueUnsignedInt(lines, key);

			if (value != 0)
			{
				color[i] = value;
			}
		}
	}

	return color;
}

class ModelIndexWeapon
{
public:
	int GetSwitchBasedOnIff(int modelIndex)
	{
		auto it = this->_weaponSwitch.find(modelIndex);

		if (it != this->_weaponSwitch.end())
		{
			return it->second;
		}
		else
		{
			auto value = GetWeaponSwitch(modelIndex);
			this->_weaponSwitch.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

	int GetColor(int modelIndex, int weaponModelIndex, int markings)
	{
		if (weaponModelIndex < 280 || weaponModelIndex >= 280 + 28)
		{
			return 0;
		}

		auto it = this->_weaponColor.find(std::make_pair(modelIndex, markings));

		if (it != this->_weaponColor.end())
		{
			return it->second[weaponModelIndex - 280];
		}
		else
		{
			auto value = GetWeaponColor(modelIndex, markings);
			this->_weaponColor.insert(std::make_pair(std::make_pair(modelIndex, markings), value));
			return value[weaponModelIndex - 280];
		}
	}

	unsigned int GetImpactColor(int modelIndex, int weaponModelIndex, int markings)
	{
		if (weaponModelIndex < 280 || weaponModelIndex >= 280 + 28)
		{
			return 0xFFFFFFFF;
		}

		auto it = this->_weaponImpactColor.find(std::make_pair(modelIndex, markings));

		if (it != this->_weaponImpactColor.end())
		{
			return it->second[weaponModelIndex - 280];
		}
		else
		{
			auto value = GetWeaponImpactColor(modelIndex, markings);
			this->_weaponImpactColor.insert(std::make_pair(std::make_pair(modelIndex, markings), value));
			return value[weaponModelIndex - 280];
		}
	}

	WeaponEnergyBarColor GetEnergyBarColor(int modelIndex, int markings)
	{
		auto it = this->_weaponEnergyBarColor.find(std::make_pair(modelIndex, markings));

		if (it != this->_weaponEnergyBarColor.end())
		{
			return it->second;
		}
		else
		{
			auto value = GetWeaponEnergyBarColor(modelIndex, markings);
			this->_weaponEnergyBarColor.insert(std::make_pair(std::make_pair(modelIndex, markings), value));
			return value;
		}
	}

	unsigned int GetLightColor(int modelIndex, int weaponModelIndex, int markings)
	{
		if (weaponModelIndex < 280 || weaponModelIndex >= 280 + 28)
		{
			return 0;
		}

		auto it = this->_weaponLightColor.find(std::make_pair(modelIndex, markings));

		if (it != this->_weaponLightColor.end())
		{
			return it->second[weaponModelIndex - 280];
		}
		else
		{
			auto value = GetWeaponLightColor(modelIndex, markings);
			this->_weaponLightColor.insert(std::make_pair(std::make_pair(modelIndex, markings), value));
			return value[weaponModelIndex - 280];
		}
	}

private:
	std::map<int, int> _weaponSwitch;
	std::map<std::pair<int, int>, std::array<int, 28>> _weaponColor;
	std::map<std::pair<int, int>, std::array<unsigned int, 28>> _weaponImpactColor;
	std::map<std::pair<int, int>, WeaponEnergyBarColor> _weaponEnergyBarColor;
	std::map<std::pair<int, int>, std::array<unsigned int, 28>> _weaponLightColor;
};

ModelIndexWeapon g_modelIndexWeapon;

bool GetConfigWeaponSwitchBasedOnIff(int modelIndex)
{
	int value = g_modelIndexWeapon.GetSwitchBasedOnIff(modelIndex);

	if (value == -1)
	{
		return g_config.WeaponSwitchBasedOnIff;
	}

	return value != 0;
}

int WeaponColorHook(int* params)
{
	const int objectIndex = params[54];
	unsigned short& weaponModelIndex = (unsigned short&)params[56];
	const int weaponObjectIndex = params[28];

	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	unsigned char* ExeWeaponSide = (unsigned char*)0x005B6660;
	unsigned short* ExeWeaponSideModel = (unsigned short*)0x005B6680;
	unsigned char* s_V0x05FE758 = (unsigned char*)0x005FE758;
	unsigned short objectModelIndex = XwaObjects[objectIndex].ModelIndex;
	unsigned char objectMarkings = XwaObjects[objectIndex].pMobileObject->Markings;

	if (GetConfigWeaponSwitchBasedOnIff(objectModelIndex))
	{
		unsigned char iff = XwaObjects[objectIndex].pMobileObject->Iff;
		unsigned char side = ExeWeaponSide[weaponModelIndex - 280];

		if (side != 0xff)
		{
			if (s_V0x05FE758[iff] != side)
			{
				weaponModelIndex = ExeWeaponSideModel[weaponModelIndex - 280];
			}
		}
	}

	int color = g_modelIndexWeapon.GetColor(objectModelIndex, weaponModelIndex, objectMarkings);

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
	unsigned char objectMarkings = XwaObjects[objectIndex].pMobileObject->Markings;

	int color = g_modelIndexWeapon.GetColor(objectModelIndex, weaponModelIndex, objectMarkings);

	XwaObjects[weaponObjectIndex].pMobileObject->Markings = color;

	return *(int*)0x007B33C4;
}

int WeaponAIColorHook(int* params)
{
	unsigned short& weaponModelIndex = (unsigned short&)params[0];
	const XwaObject* object = (XwaObject*)params[1];

	unsigned char* ExeWeaponSide = (unsigned char*)0x005B6660;
	unsigned short* ExeWeaponSideModel = (unsigned short*)0x005B6680;
	unsigned char* s_V0x05FE758 = (unsigned char*)0x005FE758;

	if (GetConfigWeaponSwitchBasedOnIff(object->ModelIndex))
	{
		unsigned char iff = object->pMobileObject->Iff;
		unsigned char side = ExeWeaponSide[weaponModelIndex - 280];

		if (side != 0xff)
		{
			if (s_V0x05FE758[iff] != side)
			{
				weaponModelIndex = ExeWeaponSideModel[weaponModelIndex - 280];
			}
		}
	}

	return 0;
}

int WeaponImpactColorHook(int* params)
{
	S0x0761E70* esi = (S0x0761E70*)params[0];
	unsigned short resdataModelIndex = (unsigned short)params[1];

	const auto L004E77C0 = (XwaKnockoutData* (*)(int, int))0x004E77C0;
	const auto L004E7DF0 = (XwaKnockoutData* (*)(int, int))0x004E7DF0;

	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;

	unsigned short weaponModelIndex;
	unsigned short modelIndex;
	int modelMarkings = -1;

	if (resdataModelIndex == 0x228 || resdataModelIndex == 0x117)
	{
		int weaponObjectIndex = esi->S0x0761E70_m08;
		weaponModelIndex = -XwaObjects[weaponObjectIndex].ModelIndex;
		modelIndex = XwaObjects[weaponObjectIndex].pMobileObject->ModelIndex;

		short objectIndex = XwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

		if (objectIndex != -1 && XwaObjects[objectIndex].pMobileObject != nullptr)
		{
			modelMarkings = XwaObjects[objectIndex].pMobileObject->Markings;
		}

		esi->S0x0761E70_m08 = weaponModelIndex;

		if (XwaObjects[weaponObjectIndex].ModelIndex >= 0x8000)
		{
			XwaObjects[weaponObjectIndex].ModelIndex = 0;
		}
	}
	else
	{
		weaponModelIndex = esi->S0x0761E70_m08;
		modelIndex = 0;
	}

	XwaKnockoutData* knockout;

	if (esi->S0x0761E70_m3C == 0x228)
	{
		knockout = L004E77C0(esi->S0x0761E70_m08, 0);
	}
	else
	{
		knockout = L004E7DF0(esi->S0x0761E70_m08, esi->S0x0761E70_m3C);
	}

	if (knockout == nullptr)
	{
		return 0;
	}

	if (modelIndex != 0)
	{
		knockout->Color = g_modelIndexWeapon.GetImpactColor(modelIndex, weaponModelIndex, modelMarkings);
	}

	return (int)knockout;
}

int WeaponImpactColorSetIndexHook(int* params)
{
	int A4 = (unsigned short)params[0];
	int A8 = (unsigned short)params[1];
	int AC = (unsigned short)params[2];
	float A10 = *(float*)&params[3];
	float A14 = *(float*)&params[4];

	const auto L004E9440 = (S0x0761E70* (*)(int, int, int, float, float))0x004E9440;

	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;

	int weaponObjectIndex = params[29];

	if (XwaObjects[weaponObjectIndex].ModelIndex == 0)
	{
		XwaObjects[weaponObjectIndex].ModelIndex = -A8;
	}

	S0x0761E70* eax = L004E9440(A4, A8, AC, A10, A14);

	if (eax != nullptr)
	{
		eax->S0x0761E70_m08 = weaponObjectIndex;
	}

	return 0;
}

int WeaponEnergyBarLaserColorHook(int* params)
{
	unsigned int& colorHigh = (unsigned int&)params[9];
	unsigned int& colorLow = (unsigned int&)params[6];
	XwaCraft* craft = (XwaCraft*)params[4];

	XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;

	int playerId = *(int*)0x008C1CC8;
	int playerObjectIndex = XwaPlayers[playerId].ObjectIndex;
	unsigned short playerModelIndex = XwaObjects[playerObjectIndex].ModelIndex;
	unsigned char playerMarkings = XwaObjects[playerObjectIndex].pMobileObject->Markings;

	WeaponEnergyBarColor color = g_modelIndexWeapon.GetEnergyBarColor(playerModelIndex, playerMarkings);

	if (color.LaserHigh == 0)
	{
		if (craft->LaserTypeId[0] == 0x118)
		{
			colorHigh = 0xFFFF0000;
		}
		else
		{
			colorHigh = 0xFF00FC0F;
		}
	}
	else
	{
		colorHigh = color.LaserHigh;
	}

	if (color.LaserLow == 0)
	{
		if (craft->LaserTypeId[0] == 0x118)
		{
			colorLow = 0xFF9B1E00;
		}
		else
		{
			colorLow = 0xFF008C00;
		}
	}
	else
	{
		colorLow = color.LaserLow;
	}

	return 0;
}

int WeaponEnergyBarIonColorHook(int* params)
{
	unsigned int& colorHigh = (unsigned int&)params[9];
	unsigned int& colorLow = (unsigned int&)params[6];
	XwaCraft* craft = (XwaCraft*)params[4];

	XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;

	int playerId = *(int*)0x008C1CC8;
	int playerObjectIndex = XwaPlayers[playerId].ObjectIndex;
	unsigned short playerModelIndex = XwaObjects[playerObjectIndex].ModelIndex;
	unsigned char playerMarkings = XwaObjects[playerObjectIndex].pMobileObject->Markings;

	WeaponEnergyBarColor color = g_modelIndexWeapon.GetEnergyBarColor(playerModelIndex, playerMarkings);

	if (color.IonHigh == 0)
	{
		colorHigh = 0xFF00B4FF;
	}
	else
	{
		colorHigh = color.IonHigh;
	}

	if (color.IonLow == 0)
	{
		colorLow = 0xFF006992;
	}
	else
	{
		colorLow = color.IonLow;
	}

	return 0;
}

int WeaponLightColorHook(int* params)
{
	const XwaObject* A4 = (XwaObject*)params[0];
	const unsigned short weaponModelIndex = A4->ModelIndex;

	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;

	unsigned int color = 0;

	if (weaponModelIndex >= 280 && weaponModelIndex < 280 + 28)
	{
		const unsigned short modelIndex = A4->pMobileObject->ModelIndex;

		if (modelIndex != 0)
		{
			short objectIndex = A4->pMobileObject->ObjectIndex;

			int modelMarkings = -1;

			if (objectIndex != -1 && XwaObjects[objectIndex].pMobileObject != nullptr)
			{
				modelMarkings = XwaObjects[objectIndex].pMobileObject->Markings;
			}

			color = g_modelIndexWeapon.GetLightColor(modelIndex, weaponModelIndex, modelMarkings);
		}
	}

	S0x07FA360* lights = (S0x07FA360*)0x007FA360;
	int& lightsCount = *(int*)0x00782840;

	if (color != 0)
	{
		switch (weaponModelIndex)
		{
		case 280: // ModelIndex_280_1_17_LaserRebel
		case 281: // ModelIndex_281_1_18_LaserRebelTurbo
		case 288: // ModelIndex_288_1_18_LaserRebelTurbo
		case 297: // ModelIndex_297_1_18_LaserRebelTurbo
		case 301: // ModelIndex_301_1_18_LaserRebelTurbo
		case 302: // ModelIndex_302_1_18_LaserRebelTurbo
			lights[lightsCount].m18 = 200.0f;
			break;

		case 282: // ModelIndex_282_1_19_LaserImp
		case 283: // ModelIndex_283_1_20_LaserImpTurbo
		case 289: // ModelIndex_289_1_20_LaserImpTurbo
		case 303: // ModelIndex_303_1_20_LaserImpTurbo
		case 304: // ModelIndex_304_1_20_LaserImpTurbo
		case 305: // ModelIndex_305_1_20_LaserImpTurbo
			lights[lightsCount].m18 = 200.0f;
			break;

		case 284: // ModelIndex_284_1_22_LaserIon
		case 285: // ModelIndex_285_1_23_LaserIonTurbo
		case 290: // ModelIndex_290_1_23_LaserIonTurbo
			lights[lightsCount].m18 = 200.0f;
			break;

		case 286: // ModelIndex_286_1_24_ProtonTorpedo
		case 291: // ModelIndex_291_1_24_ProtonTorpedo
			lights[lightsCount].m18 = 250.0f;
			break;

		case 287: // ModelIndex_287_1_25_ConcussionMissile
		case 292: // ModelIndex_292_1_25_ConcussionMissile
		case 293: // ModelIndex_293_1_26_SpaceBomb
		case 294: // ModelIndex_294_1_27_HeavyRocket
			lights[lightsCount].m18 = 250.0f;
			break;

		case 295: // ModelIndex_295_1_28_MagPulse
			lights[lightsCount].m18 = 250.0f;
			break;

		case 296: // ModelIndex_296_1_28_MagPulse
			lights[lightsCount].m18 = 250.0f;
			break;

		case 298: // ModelIndex_298_1_29_Flare
			lights[lightsCount].m18 = 200.0f;
			break;

		case 306: // ModelIndex_306_1_21_LaserImpDS
			lights[lightsCount].m18 = 50000.0f;
			lights[lightsCount].m1C = 0x4000;
			break;
		}

		unsigned int R = (color >> 16) & 0xFFU;
		unsigned int G = (color >> 8) & 0xFFU;
		unsigned int B = color & 0xFFU;

		lights[lightsCount].ColorR = R / 255.0f;
		lights[lightsCount].ColorG = G / 255.0f;
		lights[lightsCount].ColorB = B / 255.0f;
		lightsCount++;
	}
	else
	{
		switch (weaponModelIndex)
		{
		case 59: // ModelIndex_059_0_46_MilleniumFalcon2
			lights[lightsCount].m18 = 2048.0f;
			lights[lightsCount].m1C = 0x100;
			lights[lightsCount].ColorR = 0.5f;
			lights[lightsCount].ColorG = 0.5f;
			lights[lightsCount].ColorB = 0.5f;
			lightsCount++;
			break;

		case 280: // ModelIndex_280_1_17_LaserRebel
		case 281: // ModelIndex_281_1_18_LaserRebelTurbo
		case 288: // ModelIndex_288_1_18_LaserRebelTurbo
		case 297: // ModelIndex_297_1_18_LaserRebelTurbo
		case 301: // ModelIndex_301_1_18_LaserRebelTurbo
		case 302: // ModelIndex_302_1_18_LaserRebelTurbo
			lights[lightsCount].m18 = 200.0f;
			lights[lightsCount].ColorR = 1.0f;
			lights[lightsCount].ColorG = 0.2f;
			lights[lightsCount].ColorB = 0.0f;
			lightsCount++;
			break;

		case 282: // ModelIndex_282_1_19_LaserImp
		case 283: // ModelIndex_283_1_20_LaserImpTurbo
		case 289: // ModelIndex_289_1_20_LaserImpTurbo
		case 303: // ModelIndex_303_1_20_LaserImpTurbo
		case 304: // ModelIndex_304_1_20_LaserImpTurbo
		case 305: // ModelIndex_305_1_20_LaserImpTurbo
			lights[lightsCount].m18 = 200.0f;
			lights[lightsCount].ColorR = 0.0f;
			lights[lightsCount].ColorG = 1.0f;
			lights[lightsCount].ColorB = 0.2f;
			lightsCount++;
			break;

		case 284: // ModelIndex_284_1_22_LaserIon
		case 285: // ModelIndex_285_1_23_LaserIonTurbo
		case 290: // ModelIndex_290_1_23_LaserIonTurbo
			lights[lightsCount].m18 = 200.0f;
			lights[lightsCount].ColorR = 0.2f;
			lights[lightsCount].ColorG = 0.2f;
			lights[lightsCount].ColorB = 1.0f;
			lightsCount++;
			break;

		case 286: // ModelIndex_286_1_24_ProtonTorpedo
		case 291: // ModelIndex_291_1_24_ProtonTorpedo
			lights[lightsCount].m18 = 250.0f;
			lights[lightsCount].ColorR = 0.4f;
			lights[lightsCount].ColorG = 0.2f;
			lights[lightsCount].ColorB = 1.0f;
			lightsCount++;
			break;

		case 287: // ModelIndex_287_1_25_ConcussionMissile
		case 292: // ModelIndex_292_1_25_ConcussionMissile
		case 293: // ModelIndex_293_1_26_SpaceBomb
		case 294: // ModelIndex_294_1_27_HeavyRocket
			lights[lightsCount].m18 = 250.0f;
			lights[lightsCount].ColorR = 1.0f;
			lights[lightsCount].ColorG = 0.4f;
			lights[lightsCount].ColorB = 0.2f;
			lightsCount++;
			break;

		case 295: // ModelIndex_295_1_28_MagPulse
			lights[lightsCount].m18 = 250.0f;
			lights[lightsCount].ColorR = 1.0f;
			lights[lightsCount].ColorG = 0.2f;
			lights[lightsCount].ColorB = 1.0f;
			lightsCount++;
			break;

		case 296: // ModelIndex_296_1_28_MagPulse
			lights[lightsCount].m18 = 250.0f;
			lights[lightsCount].ColorR = 0.2f;
			lights[lightsCount].ColorG = 0.2f;
			lights[lightsCount].ColorB = 1.0f;
			lightsCount++;
			break;

		case 298: // ModelIndex_298_1_29_Flare
			lights[lightsCount].m18 = 200.0f;
			lights[lightsCount].ColorR = 1.0f;
			lights[lightsCount].ColorG = 0.4f;
			lights[lightsCount].ColorB = 1.0f;
			lightsCount++;
			break;

		case 306: // ModelIndex_306_1_21_LaserImpDS
			lights[lightsCount].m18 = 50000.0f;
			lights[lightsCount].m1C = 0x4000;
			lights[lightsCount].ColorR = 0.0f;
			lights[lightsCount].ColorG = 1.0f;
			lights[lightsCount].ColorB = 0.2f;
			lightsCount++;
			break;

		case 324: // ModelIndex_324_1_61_DSReactorCylinder
			lights[lightsCount].m18 = A4->m1B;
			lights[lightsCount].m1C = 0x20000;
			lights[lightsCount].ColorR = 0.3f;
			lights[lightsCount].ColorG = 0.5f;
			lights[lightsCount].ColorB = 0.9f;
			lightsCount++;
			break;

		case 349: // ModelIndex_349_1_86_DSAccelChamber
			lights[lightsCount].m18 = A4->m1B;

			if ((A4->m1B & 0x01) != 0)
			{
				lights[lightsCount].m1C = 0x8000;
				lights[lightsCount].ColorR = 0.2f;
				lights[lightsCount].ColorG = 0.0f;
				lights[lightsCount].ColorB = 0.8f;
			}
			else
			{
				lights[lightsCount].m1C = 0x400;
				lights[lightsCount].ColorR = 0.8f;
				lights[lightsCount].ColorG = 0.0f;
				lights[lightsCount].ColorB = 0.4f;
			}

			lightsCount++;
			break;
		}
	}

	return 0;
}
