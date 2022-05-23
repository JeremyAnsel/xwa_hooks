#include "targetver.h"
#include "tie.h"
#include "config.h"
#include <fstream>
#include <map>

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

class MissionConfig
{
public:
	int GetMissionId()
	{
		int missionDirectoryId = *(int*)0x00AE2A8A;
		int missionDescriptionId = ((int*)0x00AE2A8E)[missionDirectoryId];
		return missionDescriptionId;
	}

	bool IsRedAlertEnabled()
	{
		this->UpdateIfChanged();
		return this->_isRedAlertEnabled;
	}

	bool SkipHyperspacedMessages()
	{
		this->UpdateIfChanged();
		return this->_skipHyperspacedMessages;
	}

	bool ForcePlayerInTurret()
	{
		this->UpdateIfChanged();
		return this->_forcePlayerInTurret;
	}

private:
	void UpdateIfChanged()
	{
		static std::string _mission;

		const char* xwaMissionFileName = (const char*)0x06002E8;

		if (_mission != xwaMissionFileName)
		{
			_mission = xwaMissionFileName;

			const std::string path = GetStringWithoutExtension(_mission);

			auto lines = GetFileLines(path + ".txt");

			if (!lines.size())
			{
				lines = GetFileLines(path + ".ini", "mission_tie");
			}

			this->_isRedAlertEnabled = GetFileKeyValueInt(lines, "IsRedAlertEnabled", this->GetDefaultReadAlertEnabled()) != 0;
			this->_skipHyperspacedMessages = GetFileKeyValueInt(lines, "SkipHyperspacedMessages", this->GetDefaultSkipHyperspacedMessages()) != 0;
			this->_forcePlayerInTurret = GetFileKeyValueInt(lines, "ForcePlayerInTurret", this->GetDefaultForcePlayerInTurret()) != 0;
		}
	}

	int GetDefaultReadAlertEnabled()
	{
		int id = this->GetMissionId();

		if (id == 0x14)
		{
			return 1;
		}

		return 0;
	}

	int GetDefaultSkipHyperspacedMessages()
	{
		int id = this->GetMissionId();

		if (id == 0x31)
		{
			return 1;
		}

		return 0;
	}

	int GetDefaultForcePlayerInTurret()
	{
		int id = this->GetMissionId();

		if (id == 0x01)
		{
			return 1;
		}

		return 0;
	}

	bool _isRedAlertEnabled;
	bool _skipHyperspacedMessages;
	bool _forcePlayerInTurret;
};

MissionConfig g_missionConfig;

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

struct ExeCraftEntry
{
	char unk000[12];
	bool HasHyperdrive;
	char unk00D[1];
	bool HasShieldGenerator;
	unsigned int ShieldStrength;
	char unk013[2];
	unsigned int HullStrength;
	unsigned int ExplosionStrength;
	unsigned short SystemStrength;
	char unk01F[1];
	short Speed;
	short Acceleration;
	short Deceleration;
	short Yaw;
	char unk028[2];
	short Roll;
	short Pitch;
	char unk02E[941];
};

static_assert(sizeof(ExeCraftEntry) == 987, "size of ExeCraftEntry must be 987");

struct XwaCraft
{
	char Unk0000[4];
	unsigned short CraftIndex;
	char Unk0006[190];
	short Speed;
	char Unk00C6[4];
	short Pitch;
	char Unk00CC[6];
	short Roll;
	char Unk00D4[5];
	short Yaw;
	char Unk00DB[33];
	unsigned int ExplosionStrength;
	unsigned int HullStrength;
	unsigned short SystemStrength;
	char Unk0106[121];
	unsigned short m17F; // flags
	unsigned short m181; // flags
	unsigned short m183; // flags
	unsigned short m185; // flags
	char Unk0187[27];
	unsigned int ShieldStrength[2];
	char Unk01AA[293];
	unsigned char EngineThrottles[16];
	char Unk02DF[282];
};

static_assert(sizeof(XwaCraft) == 1017, "size of XwaCraft must be 1017");

struct XwaMobileObject
{
	char Unk0000[133];
	short Speed;
	char Unk0087[14];
	unsigned short ModelIndex;
	char Unk0097[2];
	unsigned char Markings;
	char Unk009A[67];
	XwaCraft* pCraft;
	char Unk00E1[4];
};

static_assert(sizeof(XwaMobileObject) == 229, "size of XwaMobileObject must be 229");

struct XwaObject
{
	short m00;
	unsigned short ModelIndex;
	unsigned char ShipCategory;
	unsigned char TieFlightGroupIndex;
	unsigned char Region;
	int PositionX;
	int PositionY;
	int PositionZ;
	short m13;
	short m15;
	short m17;
	char unk19[6];
	int PlayerIndex;
	XwaMobileObject* pMobileObject;
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

struct TieFlightGroupEx
{
	char unk000[112];
	unsigned char Iff;
	char unk071[2];
	char Markings;
	char unk074[3465];
	char PilotVoice[20];
	char unkE11[49];
};

static_assert(sizeof(TieFlightGroupEx) == 3650, "size of TieFlightGroupEx must be 3650");

#pragma pack(pop)

struct CraftStats
{
	int Speed;
	int Acceleration;
	int SpeedIncrement;
	int Deceleration;
	int SpeedDecrement;
	int Pitch;
	int Roll;
	int Yaw;
	int ExplosionStrength;
	int HullStrength;
	int SystemStrength;
	int ShieldStrength;
	int HasShieldGenerator;
	int HasHyperdrive;
};

struct CraftStatsPercent
{
	int SpeedPercent;
	int AccelerationPercent;
	int DecelerationPercent;
	int PitchPercent;
	int RollPercent;
	int YawPercent;
	int ExplosionStrengthPercent;
	int HullStrengthPercent;
	int SystemStrengthPercent;
	int ShieldStrengthPercent;
};

std::string GetFileNameWithoutExtension(const std::string& str)
{
	auto a = str.find_last_of('\\');

	return a == -1 ? str : str.substr(a + 1, -1);
}

std::vector<std::string> GetCustomFileLines(const std::string& name)
{
	static std::vector<std::string> _lines;
	static std::string _name;
	static std::string _mission;

	const char* xwaMissionFileName = (const char*)0x06002E8;

	if (_name != name || _mission != xwaMissionFileName)
	{
		_name = name;
		_mission = xwaMissionFileName;

		const std::string mission = GetStringWithoutExtension(xwaMissionFileName);
		_lines = GetFileLines(mission + "_" + name + ".txt");

		if (!_lines.size())
		{
			_lines = GetFileLines(mission + ".ini", name);
		}

		const std::string path = "FlightModels\\";

		if (!_lines.size())
		{
			_lines = GetFileLines(path + name + ".txt");
		}

		if (!_lines.size())
		{
			_lines = GetFileLines(path + "default.ini", name);
		}
	}

	return _lines;
}

CraftStats GetModelObjectProfileStats(const XwaObject* currentObject)
{
	int playerIndex = currentObject->PlayerIndex;
	unsigned short modelIndex = currentObject->ModelIndex;
	unsigned char colorIndex = currentObject->pMobileObject->Markings;

	const ExeEnableEntry* ExeEnableTable = (ExeEnableEntry*)0x005FB240;
	ShipCategoryEnum shipCategory = ExeEnableTable[modelIndex].ShipCategory;

	std::string shipPath = g_flightModelsList.GetLstLine(modelIndex);

	const auto objectLines = GetCustomFileLines("Objects");
	const std::string objectValue = GetFileKeyValue(objectLines, shipPath + ".opt");

	if (!objectValue.empty() && std::ifstream(objectValue))
	{
		shipPath = GetStringWithoutExtension(objectValue);
	}

	const auto statsLines = GetCustomFileLines("StatsProfiles");

	std::string profile;

	if (playerIndex != -1)
	{
		profile = GetFileKeyValue(statsLines, GetFileNameWithoutExtension(shipPath) + "_fg_player");
	}

	if (profile.empty())
	{
		profile = GetFileKeyValue(statsLines, GetFileNameWithoutExtension(shipPath) + "_fgc_" + std::to_string(colorIndex));
	}

	if (profile.empty())
	{
		profile = "Default";
	}

	std::string section = std::string("StatsProfile_") + profile;

	auto lines = GetFileLines(shipPath + section + ".txt");

	if (!lines.size())
	{
		lines = GetFileLines(shipPath + ".ini", section);
	}

	CraftStats stats{};

	stats.Speed = GetFileKeyValueInt(lines, "Speed", -1);
	stats.Acceleration = GetFileKeyValueInt(lines, "Acceleration", -1);
	stats.SpeedIncrement = GetFileKeyValueInt(lines, "SpeedIncrement", -1);
	stats.Deceleration = GetFileKeyValueInt(lines, "Deceleration", -1);
	stats.SpeedDecrement = GetFileKeyValueInt(lines, "SpeedDecrement", -1);
	stats.Pitch = GetFileKeyValueInt(lines, "Pitch", -1);
	stats.Roll = GetFileKeyValueInt(lines, "Roll", -1);
	stats.Yaw = GetFileKeyValueInt(lines, "Yaw", -1);
	stats.ExplosionStrength = GetFileKeyValueInt(lines, "ExplosionStrength", -1);
	stats.HullStrength = GetFileKeyValueInt(lines, "HullStrength", -1);
	stats.SystemStrength = GetFileKeyValueInt(lines, "SystemStrength", -1);
	stats.ShieldStrength = GetFileKeyValueInt(lines, "ShieldStrength", -1);
	stats.HasShieldGenerator = GetFileKeyValueInt(lines, "HasShieldGenerator", -1);
	stats.HasHyperdrive = GetFileKeyValueInt(lines, "HasHyperdrive", -1);

	if (stats.Speed != -1)
	{
		stats.Speed = (int)(stats.Speed * 2.25f + 0.5f);
	}

	if (stats.Acceleration != -1)
	{
		stats.Acceleration = (int)(stats.Acceleration * 2.25f + 0.5f);
	}

	if (stats.Deceleration != -1)
	{
		stats.Deceleration = (int)(stats.Deceleration * 2.25f + 0.5f);
	}

	if (stats.Pitch != -1)
	{
		stats.Pitch = stats.Pitch * 256;
	}

	if (stats.Roll != -1)
	{
		stats.Roll = stats.Roll * 256;
	}

	if (stats.Yaw != -1)
	{
		stats.Yaw = stats.Yaw * 256;
	}

	if (stats.ExplosionStrength != -1)
	{
		stats.ExplosionStrength = stats.ExplosionStrength * 105;
	}

	if (stats.HullStrength != -1)
	{
		stats.HullStrength = stats.HullStrength * 105;
	}

	if (stats.ShieldStrength != -1)
	{
		stats.ShieldStrength = stats.ShieldStrength * 50;
	}

	if (shipCategory == ShipCategory_Starship || shipCategory == ShipCategory_Platform)
	{
		if (stats.ExplosionStrength != -1)
		{
			stats.ExplosionStrength = stats.ExplosionStrength / 16;
		}

		if (stats.HullStrength != -1)
		{
			stats.HullStrength = stats.HullStrength / 16;
		}

		if (stats.ShieldStrength != -1)
		{
			stats.ShieldStrength = stats.ShieldStrength / 16;
		}
	}
	else if (shipCategory == ShipCategory_Freighter || shipCategory == ShipCategory_Container)
	{
		if (stats.ExplosionStrength != -1)
		{
			stats.ExplosionStrength = stats.ExplosionStrength / 4;
		}

		if (stats.HullStrength != -1)
		{
			stats.HullStrength = stats.HullStrength / 4;
		}

		if (stats.ShieldStrength != -1)
		{
			stats.ShieldStrength = stats.ShieldStrength / 4;
		}
	}

	return stats;
}

CraftStatsPercent GetObjectPlayerStatsPercent()
{
	const auto statsLines = GetCustomFileLines("StatsProfiles");

	CraftStatsPercent stats{};

	stats.SpeedPercent = GetFileKeyValueInt(statsLines, "PlayerSpeedPercent", -1);
	stats.AccelerationPercent = GetFileKeyValueInt(statsLines, "PlayerAccelerationPercent", -1);
	stats.DecelerationPercent = GetFileKeyValueInt(statsLines, "PlayerDecelerationPercent", -1);
	stats.PitchPercent = GetFileKeyValueInt(statsLines, "PlayerPitchPercent", -1);
	stats.RollPercent = GetFileKeyValueInt(statsLines, "PlayerRollPercent", -1);
	stats.YawPercent = GetFileKeyValueInt(statsLines, "PlayerYawPercent", -1);
	stats.ExplosionStrengthPercent = GetFileKeyValueInt(statsLines, "PlayerExplosionStrengthPercent", -1);
	stats.HullStrengthPercent = GetFileKeyValueInt(statsLines, "PlayerHullStrengthPercent", -1);
	stats.SystemStrengthPercent = GetFileKeyValueInt(statsLines, "PlayerSystemStrengthPercent", -1);
	stats.ShieldStrengthPercent = GetFileKeyValueInt(statsLines, "PlayerShieldStrengthPercent", -1);

	return stats;
}

CraftStatsPercent GetObjectStatsPercent()
{
	const auto statsLines = GetCustomFileLines("StatsProfiles");

	CraftStatsPercent stats{};

	stats.SpeedPercent = GetFileKeyValueInt(statsLines, "SpeedPercent", -1);
	stats.AccelerationPercent = GetFileKeyValueInt(statsLines, "AccelerationPercent", -1);
	stats.DecelerationPercent = GetFileKeyValueInt(statsLines, "DecelerationPercent", -1);
	stats.PitchPercent = GetFileKeyValueInt(statsLines, "PitchPercent", -1);
	stats.RollPercent = GetFileKeyValueInt(statsLines, "RollPercent", -1);
	stats.YawPercent = GetFileKeyValueInt(statsLines, "YawPercent", -1);
	stats.ExplosionStrengthPercent = GetFileKeyValueInt(statsLines, "ExplosionStrengthPercent", -1);
	stats.HullStrengthPercent = GetFileKeyValueInt(statsLines, "HullStrengthPercent", -1);
	stats.SystemStrengthPercent = GetFileKeyValueInt(statsLines, "SystemStrengthPercent", -1);
	stats.ShieldStrengthPercent = GetFileKeyValueInt(statsLines, "ShieldStrengthPercent", -1);

	return stats;
}

class ModelIndexProfiles
{
public:
	const CraftStats& GetProfileStats(const XwaObject* currentObject)
	{
		this->UpdateIfChanged();

		int playerIndex = currentObject->PlayerIndex;
		unsigned short modelIndex = currentObject->ModelIndex;
		int colorIndex = currentObject->pMobileObject->Markings;

		if (playerIndex != -1)
		{
			colorIndex = -1 - playerIndex;
		}

		auto it = this->_profiles.find(std::make_pair(modelIndex, colorIndex));

		if (it != this->_profiles.end())
		{
			return it->second;
		}
		else
		{
			auto value = GetModelObjectProfileStats(currentObject);
			this->_profiles.insert(std::make_pair(std::make_pair(modelIndex, colorIndex), value));
			//return value;
			return this->_profiles.find(std::make_pair(modelIndex, colorIndex))->second;
		}
	}

	const CraftStatsPercent& GetPlayerStatsPercent()
	{
		this->UpdateIfChanged();

		return this->_playerStatsPercent;
	}

	const CraftStatsPercent& GetStatsPercent()
	{
		this->UpdateIfChanged();

		return this->_statsPercent;
	}

private:
	void UpdateIfChanged()
	{
		static std::string _mission;

		const char* xwaMissionFileName = (const char*)0x06002E8;

		if (_mission != xwaMissionFileName)
		{
			_mission = xwaMissionFileName;

			this->_profiles.clear();
			this->_playerStatsPercent = GetObjectPlayerStatsPercent();
			this->_statsPercent = GetObjectStatsPercent();
		}
	}

	std::map<std::pair<int, int>, CraftStats> _profiles;
	CraftStatsPercent _playerStatsPercent{};
	CraftStatsPercent _statsPercent{};
};

ModelIndexProfiles g_modelIndexProfiles;

void ApplyStatsProfile(XwaObject* currentObject, XwaCraft* currentCraft)
{
	const ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;
	unsigned short craftIndex = currentCraft->CraftIndex;

	const CraftStats& values = g_modelIndexProfiles.GetProfileStats(currentObject);
	const CraftStatsPercent& valuesPercent = g_modelIndexProfiles.GetPlayerStatsPercent();
	const CraftStatsPercent& statsPercent = g_modelIndexProfiles.GetStatsPercent();

	if (values.Speed != -1)
	{
		currentCraft->Speed = values.Speed;
	}

	if (currentObject->PlayerIndex != -1 && valuesPercent.SpeedPercent != -1)
	{
		currentCraft->Speed = currentCraft->Speed * valuesPercent.SpeedPercent / 100;
	}
	else if (statsPercent.SpeedPercent != -1)
	{
		currentCraft->Speed = currentCraft->Speed * statsPercent.SpeedPercent / 100;
	}

	if (values.Pitch != -1)
	{
		currentCraft->Pitch = values.Pitch;
	}

	if (currentObject->PlayerIndex != -1 && valuesPercent.PitchPercent != -1)
	{
		currentCraft->Pitch = currentCraft->Pitch * valuesPercent.PitchPercent / 100;
	}
	else if (statsPercent.PitchPercent != -1)
	{
		currentCraft->Pitch = currentCraft->Pitch * statsPercent.PitchPercent / 100;
	}

	if (values.Roll != -1)
	{
		currentCraft->Roll = values.Roll;
	}

	if (currentObject->PlayerIndex != -1 && valuesPercent.RollPercent != -1)
	{
		currentCraft->Roll = currentCraft->Roll * valuesPercent.RollPercent / 100;
	}
	else if (statsPercent.RollPercent != -1)
	{
		currentCraft->Roll = currentCraft->Roll * statsPercent.RollPercent / 100;
	}

	if (values.Yaw != -1)
	{
		currentCraft->Yaw = values.Yaw;
	}

	if (currentObject->PlayerIndex != -1 && valuesPercent.YawPercent != -1)
	{
		currentCraft->Yaw = currentCraft->Yaw * valuesPercent.YawPercent / 100;
	}
	else if (statsPercent.YawPercent != -1)
	{
		currentCraft->Yaw = currentCraft->Yaw * statsPercent.YawPercent / 100;
	}

	if (values.ExplosionStrength != -1)
	{
		currentCraft->ExplosionStrength = values.ExplosionStrength;
	}

	if (currentObject->PlayerIndex != -1 && valuesPercent.ExplosionStrengthPercent != -1)
	{
		currentCraft->ExplosionStrength = currentCraft->ExplosionStrength * valuesPercent.ExplosionStrengthPercent / 100;
	}
	else if (statsPercent.ExplosionStrengthPercent != -1)
	{
		currentCraft->ExplosionStrength = currentCraft->ExplosionStrength * statsPercent.ExplosionStrengthPercent / 100;
	}

	if (values.HullStrength != -1)
	{
		currentCraft->HullStrength = values.HullStrength;
	}

	if (currentObject->PlayerIndex != -1 && valuesPercent.HullStrengthPercent != -1)
	{
		currentCraft->HullStrength = currentCraft->HullStrength * valuesPercent.HullStrengthPercent / 100;
	}
	else if (statsPercent.HullStrengthPercent != -1)
	{
		currentCraft->HullStrength = currentCraft->HullStrength * statsPercent.HullStrengthPercent / 100;
	}

	if (values.ShieldStrength != -1)
	{
		unsigned int exeShieldStrength = ExeCraftTable[craftIndex].ShieldStrength;
		currentCraft->ShieldStrength[0] = currentCraft->ShieldStrength[0] * values.ShieldStrength / exeShieldStrength;
		currentCraft->ShieldStrength[1] = currentCraft->ShieldStrength[1] * values.ShieldStrength / exeShieldStrength;
	}

	if (currentObject->PlayerIndex != -1 && valuesPercent.ShieldStrengthPercent != -1)
	{
		currentCraft->ShieldStrength[0] = currentCraft->ShieldStrength[0] * valuesPercent.ShieldStrengthPercent / 100;
		currentCraft->ShieldStrength[1] = currentCraft->ShieldStrength[1] * valuesPercent.ShieldStrengthPercent / 100;
	}
	else if (statsPercent.ShieldStrengthPercent != -1)
	{
		currentCraft->ShieldStrength[0] = currentCraft->ShieldStrength[0] * statsPercent.ShieldStrengthPercent / 100;
		currentCraft->ShieldStrength[1] = currentCraft->ShieldStrength[1] * statsPercent.ShieldStrengthPercent / 100;
	}

	bool hasShieldGenerator;
	if (values.HasShieldGenerator != -1)
	{
		hasShieldGenerator = values.HasShieldGenerator != 0;
	}
	else
	{
		hasShieldGenerator = ExeCraftTable[craftIndex].HasShieldGenerator;
	}

	if (!hasShieldGenerator)
	{
		const unsigned short Craft183_HasShieldSystem = 1;
		const unsigned short Craft17F_HasShieldGenerator = 2048;
		const unsigned char FlightGroupStatus_ShieldsAdded = 8;
		const unsigned char FlightGroupStatus_ShieldsAndHyperdriveAdded = 16;
		unsigned char s_V0x09E96E1 = *(unsigned char*)0x09E96E1;
		unsigned char s_V0x09E96F5 = *(unsigned char*)0x09E96F5;

		if (s_V0x09E96E1 != FlightGroupStatus_ShieldsAdded && s_V0x09E96F5 != FlightGroupStatus_ShieldsAdded
			&& s_V0x09E96E1 != FlightGroupStatus_ShieldsAndHyperdriveAdded && s_V0x09E96F5 != FlightGroupStatus_ShieldsAndHyperdriveAdded)
		{
			currentCraft->m183 ^= Craft183_HasShieldSystem;
			currentCraft->ShieldStrength[0] = 0;
			currentCraft->ShieldStrength[1] = 0;
			currentCraft->m17F ^= Craft17F_HasShieldGenerator;
		}
	}

	bool hasHyperdrive;
	if (values.HasHyperdrive != -1)
	{
		hasHyperdrive = values.HasHyperdrive != 0;
	}
	else
	{
		hasHyperdrive = ExeCraftTable[craftIndex].HasHyperdrive;
	}

	if (!hasHyperdrive)
	{
		const unsigned short Craft183_HasHyperdrive = 128;
		const unsigned char FlightGroupStatus_HyperdriveAdded = 9;
		const unsigned char FlightGroupStatus_ShieldsAndHyperdriveAdded = 16;
		unsigned char s_V0x09E96E1 = *(unsigned char*)0x09E96E1;
		unsigned char s_V0x09E96F5 = *(unsigned char*)0x09E96F5;

		if (s_V0x09E96E1 != FlightGroupStatus_HyperdriveAdded && s_V0x09E96F5 != FlightGroupStatus_HyperdriveAdded
			&& s_V0x09E96E1 != FlightGroupStatus_ShieldsAndHyperdriveAdded && s_V0x09E96F5 != FlightGroupStatus_ShieldsAndHyperdriveAdded)
		{
			currentCraft->m183 ^= Craft183_HasHyperdrive;
		}
	}

	currentCraft->m185 = currentCraft->m183;
	currentCraft->m181 = currentCraft->m17F;

	short oldSpeed = currentObject->pMobileObject->Speed;
	short exeSpeed = ExeCraftTable[craftIndex].Speed;
	int speedPercent = exeSpeed == 0 ? 0 : oldSpeed * 0x10000 / exeSpeed;
	short newSpeed = (currentCraft->Speed * speedPercent + 0x8000) / 0x10000;
	currentObject->pMobileObject->Speed = newSpeed;
}

TieFlightGroupEx* s_XwaTieFlightGroups = (TieFlightGroupEx*)0x80DC80;

std::vector<std::vector<std::string>> g_tieLines;

int TieHook(int* params)
{
	// init music state
	// fix a bug where a mission starts with wait state instead of intro state
	*(unsigned char*)0x0069406C = 0;

	g_tieLines.clear();

	const auto LoadMission = (int(*)(const char*))0x420300;
	const char* fileName = (const char*)params[0];

	if (LoadMission(fileName) == 0)
	{
		return 0;
	}

	const std::string path = GetStringWithoutExtension(fileName);
	auto file = GetFileLines(path + ".txt");

	if (!file.size())
	{
		file = GetFileLines(path + ".ini", "mission_tie");
	}

	if (!file.size())
	{
		return 1;
	}

	const auto lines = GetFileListValues(file);

	if (!lines.size())
	{
		return 1;
	}

	g_tieLines = lines;

	for (const auto& line : lines)
	{
		const auto& group = line[0];

		if (_stricmp(group.c_str(), "fg") == 0)
		{
			if (line.size() < 4)
			{
				continue;
			}

			int fg = std::stoi(line[1]);

			if (fg < 0 || fg > 192)
			{
				continue;
			}

			const auto& element = line[2];

			if (_stricmp(element.c_str(), "markings") == 0)
			{
				int value = std::stoi(line[3]);
				s_XwaTieFlightGroups[fg].Markings = value;
			}
			else if (_stricmp(element.c_str(), "iff") == 0)
			{
				int value = std::stoi(line[3]);
				s_XwaTieFlightGroups[fg].Iff = value;
			}
			else if (_stricmp(element.c_str(), "pilotvoice") == 0)
			{
				std::string value = line[3];
				size_t len = value.copy(s_XwaTieFlightGroups[fg].PilotVoice, sizeof(s_XwaTieFlightGroups[fg].PilotVoice) - 1);
				s_XwaTieFlightGroups[fg].PilotVoice[len] = 0;
			}
		}
	}

	return 1;
}

int CreateObjectHook(int* params)
{
	const short objectIndex = (short)params[10];
	const short A8 = (short)params[23];

	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	XwaObject* currentObject = &xwaObjects[objectIndex];
	XwaCraft* currentCraft = *(XwaCraft**)0x00910DFC;
	short tieFlightgroupIndex = *(short*)0x09E9708;
	short wingmanIndex = *(short*)0x09E96EC;

	if (A8 == -1)
	{
		for (const auto& line : g_tieLines)
		{
			const auto& group = line[0];

			if (_stricmp(group.c_str(), "fg") == 0)
			{
				if (line.size() < 6)
				{
					continue;
				}

				int fg = std::stoi(line[1]);

				if (fg < 0 || fg > 192)
				{
					continue;
				}

				if (fg != tieFlightgroupIndex)
				{
					continue;
				}

				const auto& element = line[2];

				if (_stricmp(element.c_str(), "index") == 0)
				{
					int wingman = std::stoi(line[3]);

					if (wingman == wingmanIndex)
					{
						if (_stricmp(line[4].c_str(), "markings") == 0)
						{
							int value = std::stoi(line[5]);
							xwaObjects[objectIndex].pMobileObject->Markings = value;
						}
					}
				}
			}
		}

		ApplyStatsProfile(currentObject, currentCraft);
	}

	return objectIndex;
}

int StatsProfiles_L0041FA00_Speed_Hook(int* params)
{
	const int objectIndex = params[0];

	const auto L004A2480 = (void(*)(int))0x004A2480;

	const ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	XwaObject* currentObject = &xwaObjects[objectIndex];
	XwaCraft* currentCraft = currentObject->pMobileObject->pCraft;
	unsigned short craftIndex = currentCraft->CraftIndex;

	short oldSpeed = currentObject->pMobileObject->Speed;
	short exeSpeed = ExeCraftTable[craftIndex].Speed;
	int speedPercent = exeSpeed == 0 ? 0 : oldSpeed * 0x10000 / exeSpeed;
	short newSpeed = (currentCraft->Speed * speedPercent + 0x8000) / 0x10000;
	currentObject->pMobileObject->Speed = newSpeed;

	L004A2480(objectIndex);

	return 0;
}

int StatsProfiles_L0042CF90_Acceleration_Hook(int* params)
{
	const int objectIndex = params[6];

	const ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	XwaObject* currentObject = &xwaObjects[objectIndex];
	XwaCraft* currentCraft = currentObject->pMobileObject->pCraft;
	unsigned short craftIndex = currentCraft->CraftIndex;

	const CraftStats& values = g_modelIndexProfiles.GetProfileStats(currentObject);
	const CraftStatsPercent& valuesPercent = g_modelIndexProfiles.GetPlayerStatsPercent();
	const CraftStatsPercent& statsPercent = g_modelIndexProfiles.GetStatsPercent();
	int acceleration;

	if (values.Acceleration != -1)
	{
		acceleration = values.Acceleration;
	}
	else
	{
		acceleration = ExeCraftTable[craftIndex].Acceleration;
	}

	if (currentObject->PlayerIndex != -1 && valuesPercent.AccelerationPercent != -1)
	{
		acceleration = acceleration * valuesPercent.AccelerationPercent / 100;
	}
	else if (statsPercent.AccelerationPercent != -1)
	{
		acceleration = acceleration * statsPercent.AccelerationPercent / 100;
	}

	return acceleration;
}

int StatsProfiles_L0042CF90_Deceleration_Hook(int* params)
{
	const int objectIndex = params[6];

	const ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	XwaObject* currentObject = &xwaObjects[objectIndex];
	XwaCraft* currentCraft = currentObject->pMobileObject->pCraft;
	unsigned short craftIndex = currentCraft->CraftIndex;

	const CraftStats& values = g_modelIndexProfiles.GetProfileStats(currentObject);
	const CraftStatsPercent& valuesPercent = g_modelIndexProfiles.GetPlayerStatsPercent();
	const CraftStatsPercent& statsPercent = g_modelIndexProfiles.GetStatsPercent();
	int deceleration;

	if (values.Deceleration != -1)
	{
		deceleration = values.Deceleration;
	}
	else
	{
		deceleration = ExeCraftTable[craftIndex].Deceleration;
	}

	if (currentObject->PlayerIndex != -1 && valuesPercent.DecelerationPercent != -1)
	{
		deceleration = deceleration * valuesPercent.DecelerationPercent / 100;
	}
	else if (statsPercent.DecelerationPercent != -1)
	{
		deceleration = deceleration * statsPercent.DecelerationPercent / 100;
	}

	return deceleration;
}

int StatsProfiles_L0042CF90_SpeedIncrement_Hook(int* params)
{
	const int objectIndex = params[5];

	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	XwaObject* currentObject = &xwaObjects[objectIndex];

	const CraftStats& values = g_modelIndexProfiles.GetProfileStats(currentObject);
	int speedIncrement;

	if (values.SpeedIncrement != -1)
	{
		speedIncrement = values.SpeedIncrement;
	}
	else
	{
		speedIncrement = 1;
	}

	currentObject->pMobileObject->Speed += speedIncrement;

	return 0;
}

int StatsProfiles_L0042CF90_SpeedDecrement_Hook(int* params)
{
	const int objectIndex = params[5];

	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	XwaObject* currentObject = &xwaObjects[objectIndex];

	const CraftStats& values = g_modelIndexProfiles.GetProfileStats(currentObject);
	int speedDecrement;

	if (values.SpeedDecrement != -1)
	{
		speedDecrement = values.SpeedDecrement;
	}
	else
	{
		speedDecrement = 1;
	}

	currentObject->pMobileObject->Speed -= speedDecrement;

	return 0;
}

int StatsProfiles_L0040B4F0_HullStrength_Hook(int* params)
{
	const int objectIndex = params[19];

	const ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	XwaObject* currentObject = &xwaObjects[objectIndex];
	XwaCraft* currentCraft = currentObject->pMobileObject->pCraft;
	unsigned short craftIndex = currentCraft->CraftIndex;

	const CraftStats& values = g_modelIndexProfiles.GetProfileStats(currentObject);
	const CraftStatsPercent& valuesPercent = g_modelIndexProfiles.GetPlayerStatsPercent();
	const CraftStatsPercent& statsPercent = g_modelIndexProfiles.GetStatsPercent();
	int hullStrength;

	if (values.HullStrength != -1)
	{
		hullStrength = values.HullStrength;
	}
	else
	{
		hullStrength = ExeCraftTable[craftIndex].HullStrength;
	}

	if (currentObject->PlayerIndex != -1 && valuesPercent.HullStrengthPercent != -1)
	{
		hullStrength = hullStrength * valuesPercent.HullStrengthPercent / 100;
	}
	else if (statsPercent.HullStrengthPercent != -1)
	{
		hullStrength = hullStrength * statsPercent.HullStrengthPercent / 100;
	}

	return hullStrength;
}

int StatsProfiles_SystemStrength_Hook(int* params)
{
	const short objectIndex = (short)params[0];

	const ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	XwaObject* currentObject = &xwaObjects[objectIndex];
	XwaCraft* currentCraft = currentObject->pMobileObject->pCraft;
	unsigned short craftIndex = currentCraft->CraftIndex;

	const CraftStats& values = g_modelIndexProfiles.GetProfileStats(currentObject);
	const CraftStatsPercent& valuesPercent = g_modelIndexProfiles.GetPlayerStatsPercent();
	const CraftStatsPercent& statsPercent = g_modelIndexProfiles.GetStatsPercent();
	int systemStrength;

	if (values.SystemStrength != -1)
	{
		systemStrength = values.SystemStrength;
	}
	else
	{
		systemStrength = ExeCraftTable[craftIndex].SystemStrength;
	}

	if (currentObject->PlayerIndex != -1 && valuesPercent.SystemStrengthPercent != -1)
	{
		systemStrength = systemStrength * valuesPercent.SystemStrengthPercent / 100;
	}
	else if (statsPercent.SystemStrengthPercent != -1)
	{
		systemStrength = systemStrength * statsPercent.SystemStrengthPercent / 100;
	}

	return systemStrength;
}

int StatsProfiles_ShieldStrength_Hook1(int* params)
{
	const short objectIndex = (short)params[0];

	const ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	XwaObject* currentObject = &xwaObjects[objectIndex];
	XwaCraft* currentCraft = currentObject->pMobileObject->pCraft;
	unsigned short craftIndex = currentCraft->CraftIndex;

	const CraftStats& values = g_modelIndexProfiles.GetProfileStats(currentObject);
	const CraftStatsPercent& valuesPercent = g_modelIndexProfiles.GetPlayerStatsPercent();
	const CraftStatsPercent& statsPercent = g_modelIndexProfiles.GetStatsPercent();
	int shieldStrength;

	if (values.ShieldStrength != -1)
	{
		shieldStrength = values.ShieldStrength;
	}
	else
	{
		shieldStrength = ExeCraftTable[craftIndex].ShieldStrength;
	}

	if (currentObject->PlayerIndex != -1 && valuesPercent.ShieldStrengthPercent != -1)
	{
		shieldStrength = shieldStrength * valuesPercent.ShieldStrengthPercent / 100;
	}
	else if (statsPercent.ShieldStrengthPercent != -1)
	{
		shieldStrength = shieldStrength * statsPercent.ShieldStrengthPercent / 100;
	}

	params[0] = shieldStrength;
	return 0;
}

int StatsProfiles_ShieldStrength_Hook2(int* params)
{
	const short objectIndex = params[0] / 0x27;

	const ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	XwaObject* currentObject = &xwaObjects[objectIndex];
	XwaCraft* currentCraft = currentObject->pMobileObject->pCraft;
	unsigned short craftIndex = currentCraft->CraftIndex;

	const CraftStats& values = g_modelIndexProfiles.GetProfileStats(currentObject);
	const CraftStatsPercent& valuesPercent = g_modelIndexProfiles.GetPlayerStatsPercent();
	const CraftStatsPercent& statsPercent = g_modelIndexProfiles.GetStatsPercent();
	int shieldStrength;

	if (values.ShieldStrength != -1)
	{
		shieldStrength = values.ShieldStrength;
	}
	else
	{
		shieldStrength = ExeCraftTable[craftIndex].ShieldStrength;
	}

	if (currentObject->PlayerIndex != -1 && valuesPercent.ShieldStrengthPercent != -1)
	{
		shieldStrength = shieldStrength * valuesPercent.ShieldStrengthPercent / 100;
	}
	else if (statsPercent.ShieldStrengthPercent != -1)
	{
		shieldStrength = shieldStrength * statsPercent.ShieldStrengthPercent / 100;
	}

	params[0] = shieldStrength;
	return 0;
}

int StatsProfiles_L004D5AE0_ShieldStrength_Hook(int* params)
{
	const short objectIndex = (params[19] - *(int*)0x07B33C4) / 0x27;

	const ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	XwaObject* currentObject = &xwaObjects[objectIndex];
	XwaCraft* currentCraft = currentObject->pMobileObject->pCraft;
	unsigned short craftIndex = currentCraft->CraftIndex;

	const CraftStats& values = g_modelIndexProfiles.GetProfileStats(currentObject);
	const CraftStatsPercent& valuesPercent = g_modelIndexProfiles.GetPlayerStatsPercent();
	const CraftStatsPercent& statsPercent = g_modelIndexProfiles.GetStatsPercent();
	int shieldStrength;

	if (values.ShieldStrength != -1)
	{
		shieldStrength = values.ShieldStrength;
	}
	else
	{
		shieldStrength = ExeCraftTable[craftIndex].ShieldStrength;
	}

	if (currentObject->PlayerIndex != -1 && valuesPercent.ShieldStrengthPercent != -1)
	{
		shieldStrength = shieldStrength * valuesPercent.ShieldStrengthPercent / 100;
	}
	else if (statsPercent.ShieldStrengthPercent != -1)
	{
		shieldStrength = shieldStrength * statsPercent.ShieldStrengthPercent / 100;
	}

	return shieldStrength;
}

int StatsProfiles_HasHyperdrive_Hook(int* params)
{
	const int objectIndex = *(int*)0x007CA1A0;

	const ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	XwaObject* currentObject = &xwaObjects[objectIndex];
	XwaCraft* currentCraft = currentObject->pMobileObject->pCraft;
	unsigned short craftIndex = currentCraft->CraftIndex;

	const CraftStats& values = g_modelIndexProfiles.GetProfileStats(currentObject);
	int hasHyperdrive;

	if (values.HasHyperdrive != -1)
	{
		hasHyperdrive = values.HasHyperdrive;
	}
	else
	{
		hasHyperdrive = ExeCraftTable[craftIndex].HasHyperdrive ? 1 : 0;
	}

	return hasHyperdrive;
}

int MissionIdRedAlertHook(int* params)
{
	bool isRedAlertEnabled = g_missionConfig.IsRedAlertEnabled();

	if (!isRedAlertEnabled)
	{
		params[Params_ReturnAddress] = 0x0045C381;
	}

	return 0;
}

int MissionIdSkipHyperspacedMessagesHook(int* params)
{
	bool skipHyperspacedMessages = g_missionConfig.SkipHyperspacedMessages();

	if (!skipHyperspacedMessages)
	{
		params[Params_ReturnAddress] = 0x0045C381;
	}

	return 0;
}

int MissionIdForcePlayerInTurret1Hook(int* params)
{
	bool forcePlayerInTurret = g_missionConfig.ForcePlayerInTurret();

	if (forcePlayerInTurret)
	{
		params[Params_ReturnAddress] = 0x00501884;
	}

	return 0;
}

int MissionIdForcePlayerInTurret2Hook(int* params)
{
	bool forcePlayerInTurret = g_missionConfig.ForcePlayerInTurret();

	if (!forcePlayerInTurret)
	{
		params[Params_ReturnAddress] = 0x004F9517;
	}

	return 0;
}
