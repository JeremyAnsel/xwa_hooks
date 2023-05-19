#include "targetver.h"
#include "shield.h"
#include "config.h"
#include <fstream>
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

class CraftConfig
{
public:
	CraftConfig()
	{
		auto lines = GetFileLines("hook_opt_limit.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_opt_limit");
		}

		this->MeshesCount = GetFileKeyValueInt(lines, "MeshesCount", 0);
		this->Craft_Size = 0x3F9 + GetFileKeyValueInt(lines, "Craft_ExtraSize", 0);
		this->Craft_Offset_22E = 0x3F9 + GetFileKeyValueInt(lines, "Craft_Offset_22E", 0);
		this->Craft_Offset_260 = 0x3F9 + GetFileKeyValueInt(lines, "Craft_Offset_260", 0);
		this->Craft_Offset_292 = 0x3F9 + GetFileKeyValueInt(lines, "Craft_Offset_292", 0);
	}

	int MeshesCount;
	int Craft_Size;
	int Craft_Offset_22E;
	int Craft_Offset_260;
	int Craft_Offset_292;
};

CraftConfig g_craftConfig;

class Config
{
public:
	Config()
	{
		auto lines = GetFileLines("hook_shield.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_shield");
		}

		this->IsShieldRechargeForStarshipsEnabled = GetFileKeyValueInt(lines, "IsShieldRechargeForStarshipsEnabled", 1) != 0;
	}

	bool IsShieldRechargeForStarshipsEnabled;
};

Config g_config;

#pragma pack(push, 1)

enum ShipCategoryEnum : unsigned char
{
	ShipCategory_Starfighter = 0,
	ShipCategory_Starship = 4,
};

struct ExeEnableEntry
{
	char unk00[3];
	ShipCategoryEnum ShipCategory;
	char unk04[20];
};

static_assert(sizeof(ExeEnableEntry) == 24, "size of ExeEnableEntry must be 24");

struct XwaCraft
{
	char Unk0000[658];
	unsigned char XwaCraft_m292[50];
	char Unk02C4[309];
};

static_assert(sizeof(XwaCraft) == 1017, "size of XwaCraft must be 1017");

struct XwaObject
{
	char Unk0000[2];
	unsigned short ModelIndex;
	ShipCategoryEnum ShipCategory;
	char Unk0005[26];
	int PlayerIndex;
	char Unk0023[4];
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

#pragma pack(pop)

int GetShieldGeneratorCount(int modelIndex)
{
	const auto XwaOptGetMeshesCount = (int(*)(int))0x00488960;
	const auto XwaModelGetMeshType = (int(*)(int, int))0x00488A00;

	XwaCraft* XwaCurrentCraft = *(XwaCraft**)0x00910DFC;

	unsigned char* craft_m292 = g_craftConfig.MeshesCount == 0 ? XwaCurrentCraft->XwaCraft_m292 : (unsigned char*)((int)XwaCurrentCraft + g_craftConfig.Craft_Offset_292);

	int count = 0;

	int meshesCount = XwaOptGetMeshesCount(modelIndex);

	for (int meshIndex = 0; meshIndex < meshesCount; meshIndex++)
	{
		int meshType = XwaModelGetMeshType(modelIndex, meshIndex);

		if (meshType == 0x08) // MeshType_ShieldGen
		{
			if (craft_m292[meshIndex] != 0)
			{
				count++;
			}
		}
	}

	return count;
}

struct ModelShieldRate
{
	bool useGenerator;
	int rechargeRate;
};

ModelShieldRate GetShieldRechargeRate(int modelIndex)
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

	std::string ship = g_flightModelsList.GetLstLine(modelIndex);

	const std::string objectValue = GetFileKeyValue(lines, ship + ".opt");

	if (!objectValue.empty() && std::ifstream(objectValue))
	{
		ship = GetStringWithoutExtension(objectValue);
	}

	lines = GetFileLines(ship + "Shield.txt");

	if (!lines.size())
	{
		lines = GetFileLines(ship + ".ini", "Shield");
	}

	ModelShieldRate rate{};

	if (lines.size())
	{
		if (GetFileKeyValueInt(lines, "UseGenerator") == 1)
		{
			rate.useGenerator = true;
			rate.rechargeRate = GetFileKeyValueInt(lines, "PerGeneratorRechargeRate");
		}
		else
		{
			rate.useGenerator = false;
			rate.rechargeRate = GetFileKeyValueInt(lines, "RechargeRate");
		}
	}
	else
	{
		ExeEnableEntry* ExeEnableTable = (ExeEnableEntry*)0x005FB240;
		ShipCategoryEnum shipCategory = ExeEnableTable[modelIndex].ShipCategory;

		if (shipCategory == ShipCategory_Starfighter)
		{
			rate.useGenerator = false;
			rate.rechargeRate = 20;
		}
		else if (shipCategory == ShipCategory_Starship)
		{
			switch (modelIndex)
			{
			case 137: // ModelIndex_137_0_91_Interdictor2
			case 138: // ModelIndex_138_0_92_VictoryStarDestroyer2
			case 149: // ModelIndex_149_0_92_VictoryStarDestroyer2
			case 139: // ModelIndex_139_0_93_ImperialStarDestroyer2
			case 150: // ModelIndex_150_0_141_ImperialStarDestroyer2
			case 140: // ModelIndex_140_0_94_SuperStarDestroyer
				rate.useGenerator = true;
				rate.rechargeRate = 5;
				break;

			default:
				rate.useGenerator = false;
				rate.rechargeRate = 5;
				break;
			}
		}
		else
		{
			rate.useGenerator = false;
			rate.rechargeRate = 0;
		}
	}

	return rate;
}

class ModelIndexShield
{
public:
	void UpdatePerMissionRechargeRate()
	{
		const char* xwaMissionFileName = (const char*)0x06002E8;

		if (this->_currentMissionFileName == xwaMissionFileName)
		{
			return;
		}

		this->_currentMissionFileName = xwaMissionFileName;

		std::vector<std::string> lines;
		const std::string mission = GetStringWithoutExtension(xwaMissionFileName);
		lines = GetFileLines(mission + "_Shield.txt");

		if (!lines.size())
		{
			lines = GetFileLines(mission + ".ini", "Shield");
		}

		this->_shieldRechargeRate.clear();
		this->_perMissionShieldRechargeRate.clear();

		for (const auto& line : lines)
		{
			const auto values = Tokennize(line);

			if (values.size() < 4)
			{
				continue;
			}

			unsigned short modelIndex = static_cast<unsigned short>(std::stoi(values[0], 0, 0));
			int useGenerator = std::stoi(values[1], 0, 0);
			int perGeneratorRechargeRate = std::stoi(values[2], 0, 0);
			int rechargeRate = std::stoi(values[3], 0, 0);

			ModelShieldRate rate{};

			if (useGenerator == 1)
			{
				rate.useGenerator = true;
				rate.rechargeRate = perGeneratorRechargeRate;
			}
			else
			{
				rate.useGenerator = false;
				rate.rechargeRate = rechargeRate;
			}

			this->_perMissionShieldRechargeRate.insert(std::make_pair(modelIndex, rate));
		}
	}

	ModelShieldRate GetRechargeRate(int modelIndex)
	{
		this->UpdatePerMissionRechargeRate();

		auto it = this->_perMissionShieldRechargeRate.find(modelIndex);

		if (it != this->_perMissionShieldRechargeRate.end())
		{
			return it->second;
		}

		it = this->_shieldRechargeRate.find(modelIndex);

		if (it != this->_shieldRechargeRate.end())
		{
			return it->second;
		}
		else
		{
			ModelShieldRate value = GetShieldRechargeRate(modelIndex);
			this->_shieldRechargeRate.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

	int GetTotalRechargeRate(int modelIndex)
	{
		ModelShieldRate rate = this->GetRechargeRate(modelIndex);
		int total;

		if (rate.useGenerator)
		{
			int generatorCount = GetShieldGeneratorCount(modelIndex);
			total = generatorCount * rate.rechargeRate;
		}
		else
		{
			total = rate.rechargeRate;
		}

		return total;
	}

private:
	std::map<int, ModelShieldRate> _shieldRechargeRate;
	std::map<int, ModelShieldRate> _perMissionShieldRechargeRate;
	std::string _currentMissionFileName;
};

ModelIndexShield g_modelIndexShield;

int ShieldRechargeHook(int* params)
{
	auto& rechargeRate = params[6];
	int objectIndex = (unsigned short)params[7];

	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	unsigned char difficulty = *(unsigned char*)0x0080540A;
	unsigned short modelIndex = XwaObjects[objectIndex].ModelIndex;
	ShipCategoryEnum shipCategory = XwaObjects[objectIndex].ShipCategory;

	if (XwaObjects[objectIndex].PlayerIndex != -1)
	{
		rechargeRate = g_modelIndexShield.GetTotalRechargeRate(modelIndex);

		if (rechargeRate == 0)
		{
			rechargeRate = 20;
		}
	}
	else
	{
		if (shipCategory == ShipCategory_Starfighter)
		{
			rechargeRate = g_modelIndexShield.GetTotalRechargeRate(modelIndex);
		}
		else if (shipCategory == ShipCategory_Starship)
		{
			if (g_config.IsShieldRechargeForStarshipsEnabled)
			{
				if (difficulty >= 0x01)
				{
					rechargeRate = g_modelIndexShield.GetTotalRechargeRate(modelIndex);
				}
				else
				{
					rechargeRate = 0;
				}
			}
			else
			{
				rechargeRate = 0;
			}
		}
		else
		{
			rechargeRate = g_modelIndexShield.GetTotalRechargeRate(modelIndex);
		}
	}

	return 0;
}
