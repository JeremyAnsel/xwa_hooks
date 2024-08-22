#include "targetver.h"
#include "shield.h"
#include "config.h"
#include <fstream>
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
		this->CraftUpdateTime = GetFileKeyValueInt(lines, "CraftUpdateTime", 123);
		this->RechargeRatePercent = GetFileKeyValueInt(lines, "RechargeRatePercent", 100);
	}

	bool IsShieldRechargeForStarshipsEnabled;
	int CraftUpdateTime;
	int RechargeRatePercent;
};

Config g_config;

#pragma pack(push, 1)

enum ShipCategoryEnum : unsigned char
{
	ShipCategory_Starfighter = 0,
	ShipCategory_Transport = 1,
	ShipCategory_UtilityVehicle = 2,
	ShipCategory_Freighter = 3,
	ShipCategory_Starship = 4,
	ShipCategory_Platform = 5,
	ShipCategory_PlayerProjectile = 6,
	ShipCategory_OtherProjectile = 7,
	ShipCategory_Mine = 8,
	ShipCategory_Satellite = 9,
	ShipCategory_NormalDebris = 10,
	ShipCategory_SmallDebris = 11,
	ShipCategory_Backdrop = 12,
	ShipCategory_Explosion = 13,
	ShipCategory_Obstacle = 14,
	ShipCategory_DeathStarII = 15,
	ShipCategory_People = 16,
	ShipCategory_Container = 17,
	ShipCategory_Droid = 18,
	ShipCategory_Armament = 19,
	ShipCategory_LargeDebris = 20,
	ShipCategory_SalvageYard = 21,
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
	char Unk0000[6];
	int LeaderObjectIndex;
	char Unk000A[377];
	unsigned short m183; // flags
	unsigned short m185; // flags
	char Unk0187[27];
	unsigned int ShieldStrength[2];
	unsigned char PresetShield;
	unsigned char ShieldSetting;
	char Unk01AC[230];
	unsigned char XwaCraft_m292[50];
	char Unk02C4[309];
};

static_assert(sizeof(XwaCraft) == 1017, "size of XwaCraft must be 1017");

struct XwaObject
{
	char Unk0000[2];
	unsigned short ModelIndex;
	ShipCategoryEnum ShipCategory;
	unsigned char TieFlightGroupIndex;
	char Unk0006[25];
	int PlayerIndex;
	char Unk0023[4];
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

struct XwaPlayer
{
	int ObjectIndex;
	char Unk0004[17];
	unsigned char MapState;
	char Unk0016[15];
	short CurrentTargetIndex;
	char Unk0027[453];
	int AiObjectIndex;
	char Unk01F0[2527];
};

static_assert(sizeof(XwaPlayer) == 3023, "size of XwaPlayer must be 3023");

#pragma pack(pop)

class ObjectIndexTime
{
public:
	int Get(int objectIndex)
	{
		auto it = this->_time.find(objectIndex);

		if (it != this->_time.end())
		{
			return it->second;
		}
		else
		{
			int value = 0;
			this->_time.insert(std::make_pair(objectIndex, value));
			return value;
		}
	}

	void Set(int objectIndex, int value)
	{
		auto it = this->_time.find(objectIndex);

		if (it != this->_time.end())
		{
			it->second = value;
		}
		else
		{
			this->_time.insert(std::make_pair(objectIndex, value));
		}
	}

	int RetrieveTimeSpeed(int timeFrame, int objectIndex, int elapsedTime)
	{
		int time = this->Get(objectIndex);
		time += elapsedTime;

		int timeSpeed = 0;
		if (time >= timeFrame)
		{
			timeSpeed = 1;
			time = time % timeFrame;
		}

		this->Set(objectIndex, time);

		return timeSpeed;
	}

private:
	std::map<int, int> _time;
};

int GetDefaultShieldGeneratorCount(int modelIndex)
{
	const auto XwaOptGetMeshesCount = (int(*)(int))0x00488960;
	const auto XwaModelGetMeshType = (int(*)(int, int))0x00488A00;

	int count = 0;

	int meshesCount = XwaOptGetMeshesCount(modelIndex);

	for (int meshIndex = 0; meshIndex < meshesCount; meshIndex++)
	{
		int meshType = XwaModelGetMeshType(modelIndex, meshIndex);

		if (meshType == 0x08) // MeshType_ShieldGen
		{
			count++;
		}
	}

	return count;
}

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

int GetShieldRechargeRateDelay(int modelIndex)
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

	int delay = GetFileKeyValueInt(lines, "RechargeRateDelay");

	return delay;
}

class ModelIndexShield
{
public:
	void UpdatePerMissionRechargeRate()
	{
		const char* xwaMissionFileName = (const char*)0x06002E8;
		const int missionFileNameIndex = *(int*)0x06002E4;

		if (missionFileNameIndex == 0 ? (this->_currentMissionFileName == xwaMissionFileName) : (this->_currentMissionFileNameIndex == missionFileNameIndex))
		{
			return;
		}

		this->_currentMissionFileName = xwaMissionFileName;
		this->_currentMissionFileNameIndex = missionFileNameIndex;

		std::vector<std::string> lines;
		const std::string mission = GetStringWithoutExtension(xwaMissionFileName);
		lines = GetFileLines(mission + "_Shield.txt");

		if (!lines.size())
		{
			lines = GetFileLines(mission + ".ini", "Shield");
		}

		this->_shieldRechargeRate.clear();
		this->_perMissionShieldRechargeRate.clear();
		memset(this->_shieldRechargeDelay, 0xff, sizeof(this->_shieldRechargeDelay));
		memset(this->_shieldRechargeDelayTime, 0xff, sizeof(this->_shieldRechargeDelayTime));

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

	int GetRechargeRateDelay(int objectIndex)
	{
		XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
		unsigned char fg = XwaObjects[objectIndex].TieFlightGroupIndex;
		int modelIndex = XwaObjects[objectIndex].ModelIndex;

		int value = this->_shieldRechargeDelay[fg];

		if (value == -1)
		{
			value = GetShieldRechargeRateDelay(modelIndex);
			this->_shieldRechargeDelay[fg] = value;
		}

		return value;
	}

	int GetRechargeRateDelayTime(int objectIndex)
	{
		XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
		unsigned char fg = XwaObjects[objectIndex].TieFlightGroupIndex;

		int value = this->_shieldRechargeDelayTime[fg];

		if (value == -1)
		{
			value = 0;
			this->_shieldRechargeDelayTime[fg] = value;
		}

		return value;
	}

	void SetRechargeRateDelayTime(int objectIndex, int time)
	{
		XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
		int fg = XwaObjects[objectIndex].TieFlightGroupIndex;
		this->_shieldRechargeDelayTime[fg] = time;
	}

private:
	std::map<int, ModelShieldRate> _shieldRechargeRate;
	std::map<int, ModelShieldRate> _perMissionShieldRechargeRate;
	int _shieldRechargeDelay[256];
	int _shieldRechargeDelayTime[256];
	std::string _currentMissionFileName;
	int _currentMissionFileNameIndex = 0;
};

ModelIndexShield g_modelIndexShield;

int GetMissionTime()
{
	const auto XwaGetTotalSecondsFromSecondsMinutesHours = (int(*)(unsigned char, unsigned char, unsigned char))0x004DA310;

	unsigned char MissionTimeHours = *(unsigned char*)(0x008053E0 + 0x0017);
	unsigned char MissionTimeMinutes = *(unsigned char*)(0x008053E0 + 0x0018);
	unsigned char MissionTimeSeconds = *(unsigned char*)(0x008053E0 + 0x0019);
	int missionTime = XwaGetTotalSecondsFromSecondsMinutesHours(MissionTimeHours, MissionTimeMinutes, MissionTimeSeconds);
	return missionTime;
}

int ShieldRechargeHook(int* params)
{
	const bool isPart1 = params[Params_ReturnAddress] == 0x49032F;
	const bool isPart2 = params[Params_ReturnAddress] == 0x49066F;

	auto& rechargeRate = params[6];
	int objectIndex = (unsigned short)params[7];

	XwaCraft* XwaCurrentCraft = *(XwaCraft**)0x00910DFC;
	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	int XwaCurrentPlayerId = *(int*)0x008C1CC8;
	unsigned char mapState = XwaPlayers[XwaCurrentPlayerId].MapState;
	unsigned char difficulty = *(unsigned char*)0x0080540A;
	unsigned short modelIndex = XwaObjects[objectIndex].ModelIndex;
	ShipCategoryEnum shipCategory = XwaObjects[objectIndex].ShipCategory;

	int missionTime = GetMissionTime();
	int rechargeDelay = g_modelIndexShield.GetRechargeRateDelay(objectIndex);
	int rechargeDelayTime = g_modelIndexShield.GetRechargeRateDelayTime(objectIndex);
	int shieldStrength = XwaCurrentCraft->ShieldStrength[0] + XwaCurrentCraft->ShieldStrength[1];

	bool isPlayer = (mapState == 0 ? XwaPlayers[XwaCurrentPlayerId].ObjectIndex : XwaPlayers[XwaCurrentPlayerId].AiObjectIndex) == objectIndex;

	if (rechargeDelayTime > missionTime)
	{
		rechargeRate = 0;
		return 0;
	}
	else
	{
		if (shieldStrength != 0)
		{
			g_modelIndexShield.SetRechargeRateDelayTime(objectIndex, 0);
		}

		if (shieldStrength == 0 && rechargeDelayTime == 0)
		{
			g_modelIndexShield.SetRechargeRateDelayTime(objectIndex, missionTime + rechargeDelay);
		}
	}

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

			if (isPart2 && !isPlayer)
			{
				// Craft183_HasShieldSystem
				if ((XwaCurrentCraft->m183 & 0x01) != 0)
				{
					if (XwaCurrentCraft->PresetShield == 0x02)
					{
						XwaCurrentCraft->PresetShield = difficulty >= 0x01 ? 0x04 : 0x03;
					}
				}
			}
		}
		else if (shipCategory == ShipCategory_Starship)
		{
			if (g_config.IsShieldRechargeForStarshipsEnabled)
			{
				if (difficulty >= 0x01)
				{
					rechargeRate = g_modelIndexShield.GetTotalRechargeRate(modelIndex);

					if (isPart2 && !isPlayer)
					{
						XwaCurrentCraft->PresetShield = 0x03;
					}
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

			if (isPart2 && !isPlayer && difficulty >= 0x01)
			{
				XwaCurrentCraft->PresetShield = 0x03;
			}
		}
	}

	rechargeRate = rechargeRate * g_config.RechargeRatePercent / 100;

	return 0;
}

int CraftUpdateHook(int* params)
{
	params[Params_ECX] = *(int*)0x008BF378;

	unsigned char esp13 = *(unsigned char*)((int)params + 0x13);

	int currentRegion = *(int*)0x008C1CD8;
	int regionCount = *(int*)0x00807E84;

	if (currentRegion == regionCount - 1)
	{
		short& timeDelta = *(short*)0x008C1634;

		if (timeDelta == 236)
		{
			timeDelta = 8;
		}
	}

	return 0;
}

ObjectIndexTime g_craftUpdateObjectIndexTime;

int CraftUpdateMulShieldHook(int* params)
{
	int esp18 = params[6];

	int objectIndex = params[Params_EBP] / 0x27;
	short elapsedTime = *(short*)0x008C1640;
	int timeSpeed = g_craftUpdateObjectIndexTime.RetrieveTimeSpeed(236, objectIndex, elapsedTime);

	if (timeSpeed == 1)
	{
		params[Params_EDX] = params[Params_EDX] * esp18;
	}
	else
	{
		params[Params_EDX] = 0;
	}

	return 0;
}

ObjectIndexTime g_craftUpdateTimeObjectIndexTime;

int CraftUpdateTimeHook(int* params)
{
	params[Params_EAX] = *(int*)0x007B33C4;

	int objectIndex = params[7];
	short elapsedTime = *(short*)0x008C1640;
	int timeSpeed = g_craftUpdateTimeObjectIndexTime.RetrieveTimeSpeed(g_config.CraftUpdateTime, objectIndex, elapsedTime);

	if (timeSpeed == 0)
	{
		params[Params_ReturnAddress] = 0x00490A99;
	}

	return 0;
}

ObjectIndexTime g_lasersEnergyToShieldsObjectIndexTime;

int LasersEnergyToShieldsHook(int* params)
{
	const short esp18 = (short)params[6];
	const short bp = (short)params[Params_EBP];

	int objectIndex = params[7];
	short elapsedTime = *(short*)0x008C1640;
	int timeSpeed = g_lasersEnergyToShieldsObjectIndexTime.RetrieveTimeSpeed(236, objectIndex, elapsedTime);

	if (timeSpeed == 0 || esp18 >= bp)
	{
		params[Params_ReturnAddress] = 0x0049060E;
	}

	return 0;
}
