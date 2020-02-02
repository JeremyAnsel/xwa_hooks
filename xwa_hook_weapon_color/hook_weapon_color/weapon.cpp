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
	char Unk0000[149];
	unsigned short ModelIndex;
	char Unk0097[2];
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

std::array<unsigned int, 28> GetWeaponImpactColor(int modelIndex)
{
	const std::string ship = g_flightModelsList.GetLstLine(modelIndex);

	auto lines = GetFileLines(ship + "WeaponColor.txt");

	if (!lines.size())
	{
		lines = GetFileLines(ship + ".ini", "WeaponColor");
	}

	unsigned int ResData_Animations_3100 = GetFileKeyValueUnsignedInt(lines, "WeaponImpactColor_3100", g_config.WeaponImpactColor_3100); // blue
	unsigned int ResData_Animations_3200 = GetFileKeyValueUnsignedInt(lines, "WeaponImpactColor_3200", g_config.WeaponImpactColor_3200); // red
	unsigned int ResData_Animations_3300 = GetFileKeyValueUnsignedInt(lines, "WeaponImpactColor_3300", g_config.WeaponImpactColor_3300); // green
	unsigned int ResData_Animations_3400 = GetFileKeyValueUnsignedInt(lines, "WeaponImpactColor_3400", g_config.WeaponImpactColor_3400); // purple
	unsigned int ResData_Animations_3500 = GetFileKeyValueUnsignedInt(lines, "WeaponImpactColor_3500", g_config.WeaponImpactColor_3500); // yellow

	std::array<unsigned int, 28> color;

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

	if (lines.size())
	{
		for (int i = 0; i < 28; i++)
		{
			const std::string key = std::string("WeaponImpactColor") + std::to_string(280 + i);
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

WeaponEnergyBarColor GetWeaponEnergyBarColor(int modelIndex)
{
	const std::string ship = g_flightModelsList.GetLstLine(modelIndex);

	auto lines = GetFileLines(ship + "WeaponColor.txt");

	if (!lines.size())
	{
		lines = GetFileLines(ship + ".ini", "WeaponColor");
	}

	WeaponEnergyBarColor color{};

	if (lines.size())
	{
		unsigned int value;

		value = GetFileKeyValueUnsignedInt(lines, "EnergyBarColorLaserHigh");

		if (value != 0)
		{
			color.LaserHigh = value;
		}

		value = GetFileKeyValueUnsignedInt(lines, "EnergyBarColorLaserLow");

		if (value != 0)
		{
			color.LaserLow = value;
		}

		value = GetFileKeyValueUnsignedInt(lines, "EnergyBarColorIonHigh");

		if (value != 0)
		{
			color.IonHigh = value;
		}

		value = GetFileKeyValueUnsignedInt(lines, "EnergyBarColorIonLow");

		if (value != 0)
		{
			color.IonLow = value;
		}
	}

	return color;
}

std::array<unsigned int, 28> GetWeaponLightColor(int modelIndex)
{
	const std::string ship = g_flightModelsList.GetLstLine(modelIndex);

	auto lines = GetFileLines(ship + "WeaponColor.txt");

	if (!lines.size())
	{
		lines = GetFileLines(ship + ".ini", "WeaponColor");
	}

	std::array<unsigned int, 28> color = g_config.WeaponLightColor;

	if (lines.size())
	{
		for (int i = 0; i < 28; i++)
		{
			const std::string key = std::string("WeaponLightColor") + std::to_string(280 + i);
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

	unsigned int GetImpactColor(int modelIndex, int weaponModelIndex)
	{
		if (weaponModelIndex < 280 || weaponModelIndex >= 280 + 28)
		{
			return 0xFFFFFFFF;
		}

		auto it = this->_weaponImpactColor.find(modelIndex);

		if (it != this->_weaponImpactColor.end())
		{
			return it->second[weaponModelIndex - 280];
		}
		else
		{
			auto value = GetWeaponImpactColor(modelIndex);
			this->_weaponImpactColor.insert(std::make_pair(modelIndex, value));
			return value[weaponModelIndex - 280];
		}
	}

	WeaponEnergyBarColor GetEnergyBarColor(int modelIndex)
	{
		auto it = this->_weaponEnergyBarColor.find(modelIndex);

		if (it != this->_weaponEnergyBarColor.end())
		{
			return it->second;
		}
		else
		{
			auto value = GetWeaponEnergyBarColor(modelIndex);
			this->_weaponEnergyBarColor.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

	unsigned int GetLightColor(int modelIndex, int weaponModelIndex)
	{
		if (weaponModelIndex < 280 || weaponModelIndex >= 280 + 28)
		{
			return 0;
		}

		auto it = this->_weaponLightColor.find(modelIndex);

		if (it != this->_weaponLightColor.end())
		{
			return it->second[weaponModelIndex - 280];
		}
		else
		{
			auto value = GetWeaponLightColor(modelIndex);
			this->_weaponLightColor.insert(std::make_pair(modelIndex, value));
			return value[weaponModelIndex - 280];
		}
	}

private:
	std::map<int, std::array<int, 28>> _weaponColor;
	std::map<int, std::array<unsigned int, 28>> _weaponImpactColor;
	std::map<int, WeaponEnergyBarColor> _weaponEnergyBarColor;
	std::map<int, std::array<unsigned int, 28>> _weaponLightColor;
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

int WeaponImpactColorHook(int* params)
{
	S0x0761E70* esi = (S0x0761E70*)params[0];
	unsigned short resdataModelIndex = (unsigned short)params[1];

	const auto L004E77C0 = (XwaKnockoutData* (*)(int, int))0x004E77C0;
	const auto L004E7DF0 = (XwaKnockoutData* (*)(int, int))0x004E7DF0;

	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;

	unsigned short weaponModelIndex;
	unsigned short modelIndex;

	if (resdataModelIndex == 0x228 || resdataModelIndex == 0x117)
	{
		int weaponObjectIndex = esi->S0x0761E70_m08;
		weaponModelIndex = XwaObjects[weaponObjectIndex].ModelIndex;
		modelIndex = XwaObjects[weaponObjectIndex].pMobileObject->ModelIndex;

		esi->S0x0761E70_m08 = weaponModelIndex;
		XwaObjects[weaponObjectIndex].ModelIndex = 0;
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
		knockout->Color = g_modelIndexWeapon.GetImpactColor(modelIndex, weaponModelIndex);
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
		XwaObjects[weaponObjectIndex].ModelIndex = A8;
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

	WeaponEnergyBarColor color = g_modelIndexWeapon.GetEnergyBarColor(playerModelIndex);

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

	WeaponEnergyBarColor color = g_modelIndexWeapon.GetEnergyBarColor(playerModelIndex);

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

	unsigned int color = 0;

	if (weaponModelIndex >= 280 && weaponModelIndex < 280 + 28)
	{
		const unsigned short modelIndex = A4->pMobileObject->ModelIndex;

		color = g_modelIndexWeapon.GetLightColor(modelIndex, weaponModelIndex);
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
