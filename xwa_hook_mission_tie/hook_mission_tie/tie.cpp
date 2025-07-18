#include "targetver.h"
#include "tie.h"
#include "config.h"
#include <fstream>
#include <map>
#include <set>

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

float GetFileKeyValueFloat(const std::vector<std::string>& lines, const std::string& key, float defaultValue)
{
	std::string value = GetFileKeyValue(lines, key);

	if (value.empty())
	{
		return defaultValue;
	}

	return std::stof(value);
}

std::vector<int> GetFileKeyListInt(const std::vector<std::string>& lines, const std::string& key)
{
	const std::string value = GetFileKeyValue(lines, key);
	std::vector<std::string> values = Tokennize(value);
	return GetFileListIntValues(values);
}

std::string GetPathFileName(const std::string& str)
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
		auto lines = GetFileLines("hook_mission_tie.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_mission_tie");
		}

		this->IsWarheadCollisionDamagesEnabled = GetFileKeyValueInt(lines, "IsWarheadCollisionDamagesEnabled", 1) != 0;
		this->IsMissionRanksModifierEnabled = GetFileKeyValueInt(lines, "IsMissionRanksModifierEnabled", 1) != 0;
		this->TargetCraftKeyMethod = GetFileKeyValueInt(lines, "TargetCraftKeyMethod", 0);
		this->TargetCraftKeySelectOnlyNotInspected = GetFileKeyValueInt(lines, "TargetCraftKeySelectOnlyNotInspected", 0) != 0;
	}

	bool IsWarheadCollisionDamagesEnabled;
	bool IsMissionRanksModifierEnabled;
	int TargetCraftKeyMethod;
	bool TargetCraftKeySelectOnlyNotInspected;
};

Config g_config;

std::string GetMissionFileName();

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

	int SkipObjectsMessagesIff()
	{
		this->UpdateIfChanged();
		return this->_skipObjectsMessagesIff;
	}

	bool ForcePlayerInTurret()
	{
		this->UpdateIfChanged();
		return this->_forcePlayerInTurret;
	}

	int ForcePlayerInTurretHours()
	{
		this->UpdateIfChanged();
		return this->_forcePlayerInTurretHours;
	}

	int ForcePlayerInTurretMinutes()
	{
		this->UpdateIfChanged();
		return this->_forcePlayerInTurretMinutes;
	}

	int ForcePlayerInTurretSeconds()
	{
		this->UpdateIfChanged();
		return this->_forcePlayerInTurretSeconds;
	}

	bool DisablePlayerLaserShoot()
	{
		this->UpdateIfChanged();
		return this->_disablePlayerLaserShoot;
	}

	bool DisablePlayerWarheadShoot()
	{
		this->UpdateIfChanged();
		return this->_disablePlayerWarheadShoot;
	}

	bool IsWarheadCollisionDamagesEnabled()
	{
		this->UpdateIfChanged();
		return this->_isWarheadCollisionDamagesEnabled;
	}

	bool CanShootThroughtShieldOnHardDifficulty()
	{
		this->UpdateIfChanged();
		return this->_canShootThroughtShieldOnHardDifficulty;
	}

	const std::set<int>& GetTargetCraftKeyFgIndices()
	{
		this->UpdateIfChanged();
		return this->_targetCraftKeyFgIndices;
	}

	int TargetCraftKeyMethod()
	{
		this->UpdateIfChanged();
		return this->_targetCraftKeyMethod;
	}

	bool TargetCraftKeySelectOnlyNotInspected()
	{
		this->UpdateIfChanged();
		return this->_targetCraftKeySelectOnlyNotInspected;
	}

	bool IsSkipProjectilesProximityCheckEnabled()
	{
		this->UpdateIfChanged();
		return this->_isSkipProjectilesProximityCheckEnabled;
	}

	const std::vector<int>& CampaignCraftsList()
	{
		this->UpdateIfChanged();
		return this->_campaignCraftsList;
	}

private:
	void UpdateIfChanged()
	{
		static std::string _mission;
		static int _missionIndex = 0;

		std::string xwaMissionFileName = (const char*)0x06002E8;
		const int missionFileNameIndex = *(int*)0x06002E4;

		const bool s_XwaIsInConcourse = *(int*)0x005FFD9C != 0;

		if (s_XwaIsInConcourse)
		{
			xwaMissionFileName = GetMissionFileName();
		}

		if (missionFileNameIndex == 0 ? (_mission != xwaMissionFileName) : (_missionIndex != missionFileNameIndex))
		{
			_mission = xwaMissionFileName;
			_missionIndex = missionFileNameIndex;

			const std::string path = GetStringWithoutExtension(_mission);

			auto lines = GetFileLines(path + ".txt");

			if (!lines.size())
			{
				lines = GetFileLines(path + ".ini", "mission_tie");
			}

			this->_isRedAlertEnabled = GetFileKeyValueInt(lines, "IsRedAlertEnabled", this->GetDefaultReadAlertEnabled()) != 0;
			this->_skipHyperspacedMessages = GetFileKeyValueInt(lines, "SkipHyperspacedMessages", this->GetDefaultSkipHyperspacedMessages()) != 0;
			this->_skipObjectsMessagesIff = GetFileKeyValueInt(lines, "SkipObjectsMessagesIff", this->GetDefaultSkipObjectsMessagesIff());
			this->_forcePlayerInTurret = GetFileKeyValueInt(lines, "ForcePlayerInTurret", this->GetDefaultForcePlayerInTurret()) != 0;
			this->_forcePlayerInTurretHours = GetFileKeyValueInt(lines, "ForcePlayerInTurretHours", 0);
			this->_forcePlayerInTurretMinutes = GetFileKeyValueInt(lines, "ForcePlayerInTurretMinutes", 0);
			this->_forcePlayerInTurretSeconds = GetFileKeyValueInt(lines, "ForcePlayerInTurretSeconds", 8);
			this->_disablePlayerLaserShoot = GetFileKeyValueInt(lines, "DisablePlayerLaserShoot", 0) != 0;
			this->_disablePlayerWarheadShoot = GetFileKeyValueInt(lines, "DisablePlayerWarheadShoot", 0) != 0;
			this->_isWarheadCollisionDamagesEnabled = GetFileKeyValueInt(lines, "IsWarheadCollisionDamagesEnabled", 1) != 0;
			this->_canShootThroughtShieldOnHardDifficulty = GetFileKeyValueInt(lines, "CanShootThroughtShieldOnHardDifficulty", this->GetDefaultCanShootThroughtShieldOnHardDifficulty()) != 0;

			this->_targetCraftKeyFgIndices.clear();
			const auto targetCraftKeyFgIndicesValues = Tokennize(GetFileKeyValue(lines, "KEY_O_TargetCraftFGs"));

			for (const std::string& value : targetCraftKeyFgIndicesValues)
			{
				int index = std::stoi(value, 0, 0);
				this->_targetCraftKeyFgIndices.insert(index);
			}

			int targetCraftKeyMethod = GetFileKeyValueInt(lines, "TargetCraftKeyMethod", -1);
			this->_targetCraftKeyMethod = targetCraftKeyMethod == -1 ? g_config.TargetCraftKeyMethod : targetCraftKeyMethod;

			int targetCraftKeySelectOnlyNotInspected = GetFileKeyValueInt(lines, "TargetCraftKeySelectOnlyNotInspected", -1);
			this->_targetCraftKeySelectOnlyNotInspected = targetCraftKeySelectOnlyNotInspected == -1 ? g_config.TargetCraftKeySelectOnlyNotInspected : targetCraftKeySelectOnlyNotInspected != 0;

			this->_isSkipProjectilesProximityCheckEnabled = GetFileKeyValueInt(lines, "SkipProjectilesProximityCheck", 0) != 0;

			this->_campaignCraftsList = GetFileKeyListInt(lines, "CampaignCraftsList");
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

	int GetDefaultSkipObjectsMessagesIff()
	{
		int id = this->GetMissionId();

		switch (id)
		{
		case 0x31:
		case 0x32:
		case 0x33:
		case 0x34:
			return 1;
		}

		return -1;
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

	int GetDefaultCanShootThroughtShieldOnHardDifficulty()
	{
		int id = this->GetMissionId();

		// Death Star phase 2
		if (id == 0x32)
		{
			return 1;
		}

		return 0;
	}

	bool _isRedAlertEnabled;
	bool _skipHyperspacedMessages;
	int _skipObjectsMessagesIff;
	bool _forcePlayerInTurret;
	int _forcePlayerInTurretHours;
	int _forcePlayerInTurretMinutes;
	int _forcePlayerInTurretSeconds;
	bool _disablePlayerLaserShoot;
	bool _disablePlayerWarheadShoot;
	bool _isWarheadCollisionDamagesEnabled;
	bool _canShootThroughtShieldOnHardDifficulty;
	std::set<int> _targetCraftKeyFgIndices;
	int _targetCraftKeyMethod;
	bool _targetCraftKeySelectOnlyNotInspected;
	bool _isSkipProjectilesProximityCheckEnabled;
	std::vector<int> _campaignCraftsList;
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

enum ExeEnable10Enum : unsigned short
{
	ExeEnable10_None = 0,
	ExeEnable10_Targetable = 1,
	ExeEnable10_AIControlled = 2,
	ExeEnable10_IsBuoy = 4,
	ExeEnable10_In3dModeOnly = 8,
	ExeEnable10_U5 = 16,
	ExeEnable10_IsBackdrop = 32,
	ExeEnable10_U7 = 64,
	ExeEnable10_InfiniteWaves = 128,
	ExeEnable10_IsAnimation = 256,
	ExeEnable10_AnimationLoop = 512,
	ExeEnable10_HasDatImageBorder = 1024,
	ExeEnable10_HardpointsMirroring = 2048,
	ExeEnable10_AxisAligned = 4096,
	ExeEnable10_U14 = 8192,
	ExeEnable10_UseImageColorKey = 16384,
	ExeEnable10_UseImageAlpha = 32768,
};

struct ExeEnableEntry
{
	char unk00[3];
	ShipCategoryEnum ShipCategory;
	char unk04[12];
	ExeEnable10Enum GameOptions;
	unsigned short CraftIndex;
	char unk14[4];
};

static_assert(sizeof(ExeEnableEntry) == 24, "size of ExeEnableEntry must be 24");

struct ExeCraftEntry
{
	const char* SpecShortName;
	const char* SpecName;
	char unk008[4];
	bool HasHyperdrive;
	char unk00D[1];
	bool HasShieldGenerator;
	unsigned int ShieldStrength;
	unsigned char AIHitsTakenToEvade;
	char unk014[1];
	unsigned int HullStrength;
	unsigned int CriticalDamageThreshold;
	unsigned short SystemStrength;
	char unk01F[1];
	short Speed;
	short Acceleration;
	short Deceleration;
	short Yaw;
	char unk028[2];
	short Roll;
	short Pitch;
	char unk02E[260];
	short LaserTypeId[3];
	unsigned char LaserStartRack[3];
	unsigned char LaserEndRack[3];
	unsigned char LaserLinkCode[3];
	unsigned char LaserSequence[3];
	int LaserRange[3];
	short LaserFireRatio[3];
	short WarheadTypeId[2];
	unsigned char WarheadStartRack[2];
	unsigned char WarheadEndRack[2];
	unsigned char WarheadLinkCode[2];
	unsigned char WarheadCapacity[2];
	char unk162[209];
	unsigned char CounterMeasuresCount;
	char unk234[423];
};

static_assert(sizeof(ExeCraftEntry) == 987, "size of ExeCraftEntry must be 987");

struct XwaCraft
{
	char Unk0000[4];
	unsigned short CraftIndex;
	char Unk0006[5];
	unsigned char m00B;
	char Unk000C[184];
	short Speed;
	char Unk00C6[4];
	short Pitch;
	char Unk00CC[6];
	short Roll;
	char Unk00D4[5];
	short Yaw;
	char Unk00DB[33];
	unsigned int CriticalDamageThreshold;
	unsigned int HullStrength;
	unsigned short SystemStrength;
	char Unk0106[121];
	unsigned short m17F; // flags
	unsigned short m181; // flags
	unsigned short m183; // flags
	unsigned short m185; // flags
	char Unk0187[15];
	char m196[10];
	char Unk01A0[2];
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
	char unk071[1];
	unsigned char Rank;
	char Markings;
	char unk074[3465];
	char PilotVoice[20];
	char unkE11[49];
};

static_assert(sizeof(TieFlightGroupEx) == 3650, "size of TieFlightGroupEx must be 3650");

struct XwaPlayerCamera
{
	int PositionX;
	int PositionY;
	int PositionZ;
	char unk000C[65];
};

static_assert(sizeof(XwaPlayerCamera) == 77, "size of XwaPlayerCamera must be 77");

struct XwaPlayer
{
	int ObjectIndex;
	char unk0004[8];
	short Team;
	short FGIndex;
	unsigned char Region;
	char unk0011[2871];
	XwaPlayerCamera Camera;
	char unk0B95[58];
};

static_assert(sizeof(XwaPlayer) == 3023, "size of XwaPlayer must be 3023");

struct ShiplistEntry
{
	char Name[256];
	char unk100[24];
	int MapIconRect_left;
	int MapIconRect_top;
	int MapIconRect_right;
	int MapIconRect_bottom;
};

static_assert(sizeof(ShiplistEntry) == 296, "size of ShiplistEntry must be 296");

struct XwaMissionDescription
{
	char MissionFileName[64];
	char MissionDesciption[128];
	char BattleDescription[128];
	int Id;
	int m144;
};

static_assert(sizeof(XwaMissionDescription) == 328, "size of XwaMissionDescription must be 328");

struct SpecRciEntry
{
	unsigned char CraftId;
	char m01[3];
	ShipCategoryEnum ShipCategory;
	char m05[3];
	int Speed;
	int Acceleration;
	int Maneuverability;
	int Laser;
	int Ion;
	int Missile;
	int Shield;
	int Hull;
	int Size;
	int Score;
};

static_assert(sizeof(SpecRciEntry) == 48, "size of SpecRciEntry must be 48");

#pragma pack(pop)

TieFlightGroupEx* s_XwaTieFlightGroups = (TieFlightGroupEx*)0x80DC80;

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
	int CriticalDamageThreshold;
	int HullStrength;
	int SystemStrength;
	int ShieldStrength;
	int HasShieldGenerator;
	int HasHyperdrive;
	bool IsWarheadCollisionDamagesEnabled;
};

struct CraftStatsModifiers
{
	float Speed;
	float Acceleration;
	float SpeedIncrement;
	float Deceleration;
	float SpeedDecrement;
	float Pitch;
	float Roll;
	float Yaw;
	float CriticalDamageThreshold;
	float HullStrength;
	float SystemStrength;
	float ShieldStrength;
};

struct CraftStatsPercent
{
	int SpeedPercent;
	int AccelerationPercent;
	int DecelerationPercent;
	int PitchPercent;
	int RollPercent;
	int YawPercent;
	int CriticalDamageThresholdPercent;
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
	static int _missionIndex = 0;

	const char* xwaMissionFileName = (const char*)0x06002E8;
	const int missionFileNameIndex = *(int*)0x06002E4;

	if ((_name != name) || (missionFileNameIndex == 0 ? (_mission != xwaMissionFileName) : (_missionIndex != missionFileNameIndex)))
	{
		_name = name;
		_mission = xwaMissionFileName;
		_missionIndex = missionFileNameIndex;

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

std::string GetRankKey(unsigned char rank)
{
	switch (rank)
	{
	case 0:
		return "_Novice";
	case 1:
		return "_Officer";
	case 2:
		return "_Veteran";
	case 3:
		return "_Ace";
	case 4:
		return "_TopAce";
	case 5:
		return "_SuperAce";
	}

	return std::string();
}

CraftStatsModifiers GetModelObjectProfileStatsModifiers(const XwaObject* currentObject)
{
	int playerIndex = currentObject->PlayerIndex;
	unsigned short modelIndex = currentObject->ModelIndex;
	unsigned char rank = currentObject->TieFlightGroupIndex == 0xff ? 0xff : s_XwaTieFlightGroups[currentObject->TieFlightGroupIndex].Rank;

	std::string shipPath = g_flightModelsList.GetLstLine(modelIndex);

	const auto objectLines = GetCustomFileLines("Objects");
	const std::string objectValue = GetFileKeyValue(objectLines, shipPath + ".opt");

	if (!objectValue.empty() && std::ifstream(objectValue))
	{
		shipPath = GetStringWithoutExtension(objectValue);
	}

	std::string playerKey = playerIndex == -1 ? "" : "_Player";
	std::string rankKey = GetRankKey(rank);
	std::string section = "StatsModifiers";

	std::vector<std::string> lines;

	if (!playerKey.empty() && !rankKey.empty())
	{
		if (!lines.size())
		{
			lines = GetFileLines(shipPath + section + playerKey + rankKey + ".txt");
		}

		if (!lines.size())
		{
			lines = GetFileLines(shipPath + ".ini", section + playerKey + rankKey);
		}
	}

	if (!playerKey.empty())
	{
		if (!lines.size())
		{
			lines = GetFileLines(shipPath + section + playerKey + ".txt");
		}

		if (!lines.size())
		{
			lines = GetFileLines(shipPath + ".ini", section + playerKey);
		}
	}

	if (!rankKey.empty())
	{
		if (!lines.size())
		{
			lines = GetFileLines(shipPath + section + rankKey + ".txt");
		}

		if (!lines.size())
		{
			lines = GetFileLines(shipPath + ".ini", section + rankKey);
		}
	}

	if (!lines.size())
	{
		lines = GetFileLines(shipPath + section + ".txt");
	}

	if (!lines.size())
	{
		lines = GetFileLines(shipPath + ".ini", section);
	}

	CraftStatsModifiers modifiers{};

	modifiers.Speed = GetFileKeyValueFloat(lines, "Speed", 1.0f);
	modifiers.Acceleration = GetFileKeyValueFloat(lines, "Acceleration", 1.0f);
	modifiers.SpeedIncrement = GetFileKeyValueFloat(lines, "SpeedIncrement", 1.0f);
	modifiers.Deceleration = GetFileKeyValueFloat(lines, "Deceleration", 1.0f);
	modifiers.SpeedDecrement = GetFileKeyValueFloat(lines, "SpeedDecrement", 1.0f);
	modifiers.Pitch = GetFileKeyValueFloat(lines, "Pitch", 1.0f);
	modifiers.Roll = GetFileKeyValueFloat(lines, "Roll", 1.0f);
	modifiers.Yaw = GetFileKeyValueFloat(lines, "Yaw", 1.0f);
	modifiers.CriticalDamageThreshold = GetFileKeyValueFloat(lines, "CriticalDamageThreshold", GetFileKeyValueFloat(lines, "ExplosionStrength", 1.0f));
	modifiers.HullStrength = GetFileKeyValueFloat(lines, "HullStrength", 1.0f);
	modifiers.SystemStrength = GetFileKeyValueFloat(lines, "SystemStrength", 1.0f);
	modifiers.ShieldStrength = GetFileKeyValueFloat(lines, "ShieldStrength", 1.0f);

	return modifiers;
}

CraftStats GetModelObjectProfileStats(const XwaObject* currentObject)
{
	const unsigned char difficulty = *(unsigned char*)(0x08053E0 + 0x002A);

	int playerIndex = currentObject->PlayerIndex;
	unsigned short modelIndex = currentObject->ModelIndex;
	unsigned char colorIndex = currentObject->pMobileObject->Markings;
	unsigned char rank = currentObject->TieFlightGroupIndex == 0xff ? 0xff : s_XwaTieFlightGroups[currentObject->TieFlightGroupIndex].Rank;

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
		profile = GetFileKeyValue(statsLines, GetFileNameWithoutExtension(shipPath));
	}

	if (profile.empty())
	{
		profile = "Default";
	}

	std::string playerKey;
	if (playerIndex != -1)
	{
		playerKey = "_Player";
	}

	std::string difficultyKey;
	switch (difficulty)
	{
	case 0:
		difficultyKey = "_Easy";
		break;

	case 1:
		difficultyKey = "_Medium";
		break;

	case 2:
		difficultyKey = "_Hard";
		break;
	}

	std::string section = std::string("StatsProfile_") + profile;

	std::vector<std::string> lines;

	if (!playerKey.empty())
	{
		if (!lines.size())
		{
			lines = GetFileLines(shipPath + section + playerKey + difficultyKey + ".txt");
		}

		if (!lines.size())
		{
			lines = GetFileLines(shipPath + ".ini", section + playerKey + difficultyKey);
		}

		if (!lines.size())
		{
			lines = GetFileLines(shipPath + section + playerKey + ".txt");
		}

		if (!lines.size())
		{
			lines = GetFileLines(shipPath + ".ini", section + playerKey);
		}
	}

	if (!lines.size())
	{
		lines = GetFileLines(shipPath + section + difficultyKey + ".txt");
	}

	if (!lines.size())
	{
		lines = GetFileLines(shipPath + ".ini", section + difficultyKey);
	}

	if (!lines.size())
	{
		lines = GetFileLines(shipPath + section + ".txt");
	}

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
	stats.CriticalDamageThreshold = GetFileKeyValueInt(lines, "CriticalDamageThreshold", GetFileKeyValueInt(lines, "ExplosionStrength", -1));
	stats.HullStrength = GetFileKeyValueInt(lines, "HullStrength", -1);
	stats.SystemStrength = GetFileKeyValueInt(lines, "SystemStrength", -1);
	stats.ShieldStrength = GetFileKeyValueInt(lines, "ShieldStrength", -1);
	stats.HasShieldGenerator = GetFileKeyValueInt(lines, "HasShieldGenerator", -1);
	stats.HasHyperdrive = GetFileKeyValueInt(lines, "HasHyperdrive", -1);
	stats.IsWarheadCollisionDamagesEnabled = GetFileKeyValueInt(lines, "IsWarheadCollisionDamagesEnabled", 1) != 0;

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

	if (stats.CriticalDamageThreshold != -1)
	{
		stats.CriticalDamageThreshold = stats.CriticalDamageThreshold * 105;
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
		if (stats.CriticalDamageThreshold != -1)
		{
			stats.CriticalDamageThreshold = stats.CriticalDamageThreshold / 16;
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
		if (stats.CriticalDamageThreshold != -1)
		{
			stats.CriticalDamageThreshold = stats.CriticalDamageThreshold / 4;
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

	CraftStatsModifiers modifiers = GetModelObjectProfileStatsModifiers(currentObject);

	if (stats.Speed != -1)
	{
		stats.Speed = (int)(stats.Speed * modifiers.Speed);
	}

	if (stats.Acceleration != -1)
	{
		stats.Acceleration = (int)(stats.Acceleration * modifiers.Acceleration);
	}

	if (stats.SpeedIncrement != -1)
	{
		stats.SpeedIncrement = (int)(stats.SpeedIncrement * modifiers.SpeedIncrement);
	}

	if (stats.Deceleration != -1)
	{
		stats.Deceleration = (int)(stats.Deceleration * modifiers.Deceleration);
	}

	if (stats.SpeedDecrement != -1)
	{
		stats.SpeedDecrement = (int)(stats.SpeedDecrement * modifiers.SpeedDecrement);
	}

	if (stats.Pitch != -1)
	{
		stats.Pitch = (int)(stats.Pitch * modifiers.Pitch);
	}

	if (stats.Roll != -1)
	{
		stats.Roll = (int)(stats.Roll * modifiers.Roll);
	}

	if (stats.Yaw != -1)
	{
		stats.Yaw = (int)(stats.Yaw * modifiers.Yaw);
	}

	if (stats.CriticalDamageThreshold != -1)
	{
		stats.CriticalDamageThreshold = (int)(stats.CriticalDamageThreshold * modifiers.CriticalDamageThreshold);
	}

	if (stats.HullStrength != -1)
	{
		stats.HullStrength = (int)(stats.HullStrength * modifiers.HullStrength);
	}

	if (stats.SystemStrength != -1)
	{
		stats.SystemStrength = (int)(stats.SystemStrength * modifiers.SystemStrength);
	}

	if (stats.ShieldStrength != -1)
	{
		stats.ShieldStrength = (int)(stats.ShieldStrength * modifiers.ShieldStrength);
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
	stats.CriticalDamageThresholdPercent = GetFileKeyValueInt(statsLines, "PlayerCriticalDamageThresholdPercent", GetFileKeyValueInt(statsLines, "PlayerExplosionStrengthPercent", -1));
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
	stats.CriticalDamageThresholdPercent = GetFileKeyValueInt(statsLines, "CriticalDamageThresholdPercent", GetFileKeyValueInt(statsLines, "ExplosionStrengthPercent", -1));
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
		unsigned char rank = currentObject->TieFlightGroupIndex == 0xff ? 0xff : s_XwaTieFlightGroups[currentObject->TieFlightGroupIndex].Rank;

		if (playerIndex != -1)
		{
			colorIndex = -1 - playerIndex;
		}

		colorIndex *= 16;

		if (rank != 0xff)
		{
			colorIndex += rank;
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
		static int _missionIndex = 0;

		const char* xwaMissionFileName = (const char*)0x06002E8;
		const int missionFileNameIndex = *(int*)0x06002E4;

		if (missionFileNameIndex == 0 ? (_mission != xwaMissionFileName) : (_missionIndex != missionFileNameIndex))
		{
			_mission = xwaMissionFileName;
			_missionIndex = missionFileNameIndex;

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

	if (values.CriticalDamageThreshold != -1)
	{
		currentCraft->CriticalDamageThreshold = values.CriticalDamageThreshold;
	}

	if (currentObject->PlayerIndex != -1 && valuesPercent.CriticalDamageThresholdPercent != -1)
	{
		currentCraft->CriticalDamageThreshold = currentCraft->CriticalDamageThreshold * valuesPercent.CriticalDamageThresholdPercent / 100;
	}
	else if (statsPercent.CriticalDamageThresholdPercent != -1)
	{
		currentCraft->CriticalDamageThreshold = currentCraft->CriticalDamageThreshold * statsPercent.CriticalDamageThresholdPercent / 100;
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
		currentCraft->ShieldStrength[0] = exeShieldStrength == 0 ? 0 : (currentCraft->ShieldStrength[0] * (unsigned long long)values.ShieldStrength / exeShieldStrength);
		currentCraft->ShieldStrength[1] = exeShieldStrength == 0 ? 0 : (currentCraft->ShieldStrength[1] * (unsigned long long)values.ShieldStrength / exeShieldStrength);
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
	int speedPercent = exeSpeed == 0 ? 0 : (oldSpeed * 0x10000 / exeSpeed);
	short newSpeed = (currentCraft->Speed * speedPercent + 0x8000) / 0x10000;
	currentObject->pMobileObject->Speed = newSpeed;
}

std::vector<std::string> GetCraftExeCraftValues(int modelIndex)
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

	std::string shipPath = g_flightModelsList.GetLstLine(modelIndex);

	const std::string objectValue = GetFileKeyValue(lines, shipPath + ".opt");

	if (!objectValue.empty() && std::ifstream(objectValue))
	{
		shipPath = GetStringWithoutExtension(objectValue);
	}

	lines = GetFileLines(shipPath + "ExeCraftValues.txt");

	if (!lines.size())
	{
		lines = GetFileLines(shipPath + ".ini", "ExeCraftValues");
	}

	return lines;
}

struct SpecEntry
{
	char Name[256];
	int MapIconRect_left;
	int MapIconRect_top;
	int MapIconRect_right;
	int MapIconRect_bottom;
	char SpecName[256];
	unsigned char Gender;
	char PluralName[256];
	char ShortName[256];
};

int GetCraftIndex(int craftId)
{
	const auto XwaGetCraftIndex = (int(*)(int))0x004DCE30;
	const unsigned short* ExeSpecies = (const unsigned short*)0x005B0F70;

	int modelIndex = ExeSpecies[craftId];
	int craftIndex = XwaGetCraftIndex(modelIndex);
	return craftIndex;
}

int GetCraftId(int craftIndex)
{
	const auto XwaGetModelCraftId = (int(*)(int))0x00422B50;
	const ExeEnableEntry* ExeEnableTable = (ExeEnableEntry*)0x005FB240;

	int modelIndex = -1;

	for (int i = 0; i < 557; i++)
	{
		if (ExeEnableTable[i].CraftIndex == craftIndex)
		{
			modelIndex = i;
			break;
		}
	}

	if (modelIndex == -1)
	{
		return -1;
	}

	int craftId = XwaGetModelCraftId(modelIndex);
	return craftId;
}

int GetCraftModelIndex(int craftIndex)
{
	const auto XwaGetModelCraftId = (int(*)(int))0x00422B50;
	const ExeEnableEntry* ExeEnableTable = (ExeEnableEntry*)0x005FB240;

	int modelIndex = -1;

	for (int i = 0; i < 557; i++)
	{
		if (ExeEnableTable[i].CraftIndex == craftIndex)
		{
			modelIndex = i;
			break;
		}
	}

	return modelIndex;
}

class SpecTable
{
public:
	SpecTable()
	{
		this->SetDefaultValues();
	}

	void SetDefaultValues()
	{
		const ShiplistEntry* ShiplistEntries = *(ShiplistEntry**)0x00ABD22C;
		const int* ShiplistEntriesIndexFromId = (int*)0x00ABD280;
		const ExeEnableEntry* ExeEnableTable = (ExeEnableEntry*)0x005FB240;
		const ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;
		const unsigned char* SpecGender = (unsigned char*)0x009CF640;
		const char** SpecPluralName = (const char**)0x0091B4C0;

		this->_defaultExeEnableTable.reserve(557);
		memcpy(this->_defaultExeEnableTable.data(), ExeEnableTable, sizeof(ExeEnableEntry) * this->_defaultExeEnableTable.capacity());

		this->_defaultExeCraftTable.reserve(265);
		memcpy(this->_defaultExeCraftTable.data(), ExeCraftTable, sizeof(ExeCraftEntry) * this->_defaultExeCraftTable.capacity());

		this->_defaultEntries.reserve(218);
		this->_currentEntries.reserve(218);

		for (int i = 0; i < 218; i++)
		{
			SpecEntry* entry = this->_defaultEntries.data() + i;

			//int modelIndex = GetCraftModelIndex(i);
			int craftId = GetCraftId(i);

			if (craftId == -1)
			{
				*entry = {};
				continue;
			}

			strcpy_s(entry->Name, ShiplistEntries[ShiplistEntriesIndexFromId[craftId]].Name);
			entry->MapIconRect_left = ShiplistEntries[ShiplistEntriesIndexFromId[craftId]].MapIconRect_left;
			entry->MapIconRect_top = ShiplistEntries[ShiplistEntriesIndexFromId[craftId]].MapIconRect_top;
			entry->MapIconRect_right = ShiplistEntries[ShiplistEntriesIndexFromId[craftId]].MapIconRect_right;
			entry->MapIconRect_bottom = ShiplistEntries[ShiplistEntriesIndexFromId[craftId]].MapIconRect_bottom;

			strcpy_s(entry->SpecName, ExeCraftTable[i].SpecName);
			entry->Gender = SpecGender[i];
			strcpy_s(entry->PluralName, SpecPluralName[i]);
			strcpy_s(entry->ShortName, ExeCraftTable[i].SpecShortName);
		}
	}

	void RestoreDefaultValues()
	{
		ShiplistEntry* ShiplistEntries = *(ShiplistEntry**)0x00ABD22C;
		const int* ShiplistEntriesIndexFromId = (int*)0x00ABD280;
		ExeEnableEntry* ExeEnableTable = (ExeEnableEntry*)0x005FB240;
		ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;
		unsigned char* SpecGender = (unsigned char*)0x009CF640;
		const char** SpecPluralName = (const char**)0x0091B4C0;

		for (size_t i = 0; i < this->_defaultExeEnableTable.capacity(); i++)
		{
			ExeEnableEntry* defaultEntry = this->_defaultExeEnableTable.data() + i;
			ExeEnableEntry* exeEntry = ExeEnableTable + i;

			exeEntry->GameOptions = defaultEntry->GameOptions;
		}

		for (size_t i = 0; i < this->_defaultExeCraftTable.capacity(); i++)
		{
			ExeCraftEntry* defaultEntry = this->_defaultExeCraftTable.data() + i;
			ExeCraftEntry* exeEntry = ExeCraftTable + i;

			for (int index = 0; index < 3; index++)
			{
				exeEntry->LaserTypeId[index] = defaultEntry->LaserTypeId[index];
				exeEntry->LaserSequence[index] = defaultEntry->LaserSequence[index];
				exeEntry->LaserRange[index] = defaultEntry->LaserRange[index];
				exeEntry->LaserFireRatio[index] = defaultEntry->LaserFireRatio[index];
			}

			for (int index = 0; index < 2; index++)
			{
				exeEntry->WarheadTypeId[index] = defaultEntry->WarheadTypeId[index];
				exeEntry->WarheadCapacity[index] = defaultEntry->WarheadCapacity[index];
			}

			exeEntry->CounterMeasuresCount = defaultEntry->CounterMeasuresCount;
			exeEntry->AIHitsTakenToEvade = defaultEntry->AIHitsTakenToEvade;
		}

		for (int i = 0; i < 218; i++)
		{
			SpecEntry* entry = this->_defaultEntries.data() + i;

			//int modelIndex = GetCraftModelIndex(i);
			int craftId = GetCraftId(i);

			if (craftId == -1)
			{
				continue;
			}

			strcpy_s(ShiplistEntries[ShiplistEntriesIndexFromId[craftId]].Name, entry->Name);
			ShiplistEntries[ShiplistEntriesIndexFromId[craftId]].MapIconRect_left = entry->MapIconRect_left;
			ShiplistEntries[ShiplistEntriesIndexFromId[craftId]].MapIconRect_top = entry->MapIconRect_top;
			ShiplistEntries[ShiplistEntriesIndexFromId[craftId]].MapIconRect_right = entry->MapIconRect_right;
			ShiplistEntries[ShiplistEntriesIndexFromId[craftId]].MapIconRect_bottom = entry->MapIconRect_bottom;

			ExeCraftTable[i].SpecName = entry->SpecName;
			SpecGender[i] = entry->Gender;
			SpecPluralName[i] = entry->PluralName;
			ExeCraftTable[i].SpecShortName = entry->ShortName;
		}
	}

	void SetName(int craftIndex, const char* name)
	{
		if (craftIndex < 0 || craftIndex >= 218)
		{
			return;
		}

		ShiplistEntry* ShiplistEntries = *(ShiplistEntry**)0x00ABD22C;
		const int* ShiplistEntriesIndexFromId = (int*)0x00ABD280;
		SpecEntry* entry = this->_currentEntries.data() + craftIndex;

		int craftId = GetCraftId(craftIndex);

		if (craftId == -1)
		{
			return;
		}

		strcpy_s(entry->Name, name);
		strcpy_s(ShiplistEntries[ShiplistEntriesIndexFromId[craftId]].Name, entry->Name);
	}

	void SetMapIconRect(int craftIndex, int icon_left, int icon_top, int icon_right, int icon_bottom)
	{
		if (craftIndex < 0 || craftIndex >= 218)
		{
			return;
		}

		ShiplistEntry* ShiplistEntries = *(ShiplistEntry**)0x00ABD22C;
		const int* ShiplistEntriesIndexFromId = (int*)0x00ABD280;
		SpecEntry* entry = this->_currentEntries.data() + craftIndex;

		int craftId = GetCraftId(craftIndex);

		if (craftId == -1)
		{
			return;
		}

		entry->MapIconRect_left = icon_left;
		entry->MapIconRect_top = icon_top;
		entry->MapIconRect_right = icon_right;
		entry->MapIconRect_bottom = icon_bottom;
		ShiplistEntries[ShiplistEntriesIndexFromId[craftId]].MapIconRect_left = entry->MapIconRect_left;
		ShiplistEntries[ShiplistEntriesIndexFromId[craftId]].MapIconRect_top = entry->MapIconRect_top;
		ShiplistEntries[ShiplistEntriesIndexFromId[craftId]].MapIconRect_right = entry->MapIconRect_right;
		ShiplistEntries[ShiplistEntriesIndexFromId[craftId]].MapIconRect_bottom = entry->MapIconRect_bottom;
	}

	void SetSpecName(int craftIndex, const char* specName)
	{
		if (craftIndex < 0 || craftIndex >= 218)
		{
			return;
		}

		ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;
		SpecEntry* entry = this->_currentEntries.data() + craftIndex;

		strcpy_s(entry->SpecName, specName);
		ExeCraftTable[craftIndex].SpecName = entry->SpecName;
	}

	void SetGender(int craftIndex, unsigned char gender)
	{
		if (craftIndex < 0 || craftIndex >= 218)
		{
			return;
		}

		unsigned char* SpecGender = (unsigned char*)0x009CF640;
		SpecEntry* entry = this->_currentEntries.data() + craftIndex;

		entry->Gender = gender;
		SpecGender[craftIndex] = entry->Gender;
	}

	void SetPluralName(int craftIndex, const char* pluralName)
	{
		if (craftIndex < 0 || craftIndex >= 218)
		{
			return;
		}

		const char** SpecPluralName = (const char**)0x0091B4C0;
		SpecEntry* entry = this->_currentEntries.data() + craftIndex;

		strcpy_s(entry->PluralName, pluralName);
		SpecPluralName[craftIndex] = entry->PluralName;
	}

	void SetShortName(int craftIndex, const char* shortName)
	{
		if (craftIndex < 0 || craftIndex >= 218)
		{
			return;
		}

		ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;
		SpecEntry* entry = this->_currentEntries.data() + craftIndex;

		strcpy_s(entry->ShortName, shortName);
		ExeCraftTable[craftIndex].SpecShortName = entry->ShortName;
	}

	void SetCraftsValues()
	{
		ExeEnableEntry* ExeEnableTable = (ExeEnableEntry*)0x005FB240;
		ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;

		for (int modelIndex = 0; modelIndex < 219; modelIndex++)
		{
			unsigned short craftIndex = ExeEnableTable[modelIndex].CraftIndex;

			if (craftIndex >= 218)
			{
				continue;
			}

			std::vector<std::string> lines = GetCraftExeCraftValues(modelIndex);

			if (lines.empty())
			{
				continue;
			}

			SpecEntry* entry = this->_currentEntries.data() + craftIndex;

			ExeEnable10Enum gameOptions = ExeEnableTable[modelIndex].GameOptions;

			int IsHardpointsMirroring = GetFileKeyValueInt(lines, "GameOptions_IsHardpointsMirroring", (gameOptions & ExeEnable10_HardpointsMirroring) ? 1 : 0) != 0;
			int IsAxisAligned = GetFileKeyValueInt(lines, "GameOptions_IsAxisAligned", (gameOptions & ExeEnable10_AxisAligned) ? 1 : 0) != 0;

			if (IsHardpointsMirroring)
			{
				gameOptions = (ExeEnable10Enum)(gameOptions | ExeEnable10_HardpointsMirroring);
			}
			else
			{
				gameOptions = (ExeEnable10Enum)(gameOptions & ~ExeEnable10_HardpointsMirroring);
			}

			if (IsAxisAligned)
			{
				gameOptions = (ExeEnable10Enum)(gameOptions | ExeEnable10_AxisAligned);
			}
			else
			{
				gameOptions = (ExeEnable10Enum)(gameOptions & ~ExeEnable10_AxisAligned);
			}

			ExeEnableTable[modelIndex].GameOptions = gameOptions;

			for (int index = 0; index < 3; index++)
			{
				ExeCraftTable[craftIndex].LaserTypeId[index] = (short)GetFileKeyValueInt(lines, "LaserTypeId" + std::to_string(index), this->_defaultExeCraftTable[craftIndex].LaserTypeId[index]);
				ExeCraftTable[craftIndex].LaserSequence[index] = (unsigned char)GetFileKeyValueInt(lines, "LaserSequence" + std::to_string(index), this->_defaultExeCraftTable[craftIndex].LaserSequence[index]);
				ExeCraftTable[craftIndex].LaserRange[index] = (int)GetFileKeyValueInt(lines, "LaserRange" + std::to_string(index), this->_defaultExeCraftTable[craftIndex].LaserRange[index]);
				ExeCraftTable[craftIndex].LaserFireRatio[index] = (short)GetFileKeyValueInt(lines, "LaserFireRatio" + std::to_string(index), this->_defaultExeCraftTable[craftIndex].LaserFireRatio[index]);
			}

			for (int index = 0; index < 2; index++)
			{
				ExeCraftTable[craftIndex].WarheadTypeId[index] = (short)GetFileKeyValueInt(lines, "WarheadTypeId" + std::to_string(index), this->_defaultExeCraftTable[craftIndex].WarheadTypeId[index]);
				ExeCraftTable[craftIndex].WarheadCapacity[index] = (unsigned char)GetFileKeyValueInt(lines, "WarheadCapacity" + std::to_string(index), this->_defaultExeCraftTable[craftIndex].WarheadCapacity[index]);
			}

			ExeCraftTable[craftIndex].CounterMeasuresCount = (unsigned char)GetFileKeyValueInt(lines, "CounterMeasuresCount", ExeCraftTable[craftIndex].CounterMeasuresCount);
			ExeCraftTable[craftIndex].AIHitsTakenToEvade = (unsigned char)GetFileKeyValueInt(lines, "AIHitsTakenToEvade", ExeCraftTable[craftIndex].AIHitsTakenToEvade);
		}
	}

private:
	std::vector<ExeEnableEntry> _defaultExeEnableTable;
	std::vector<ExeCraftEntry> _defaultExeCraftTable;
	std::vector<SpecEntry> _defaultEntries;
	std::vector<SpecEntry> _currentEntries;
};

SpecTable& GetSpecTable()
{
	static SpecTable _specTable;

	return _specTable;
}

std::vector<std::vector<std::string>> g_tieLines;

std::string GetMissionFileName()
{
	const int missionDirectoryId = *(int*)(0x00AE2A60 + 0x002A);
	const char* missionDirectory = ((const char**)0x00603168)[missionDirectoryId];
	const int s_V0x09F5E74 = *(int*)0x009F5E74;
	const XwaMissionDescription* s_XwaMissionDescriptions = *(XwaMissionDescription**)0x009F4B98;

	if (s_XwaMissionDescriptions == nullptr)
	{
		return "default.tie";
	}

	std::string missionFilename = std::string(missionDirectory) + std::string("\\") + std::string(s_XwaMissionDescriptions[s_V0x09F5E74].MissionFileName);
	return missionFilename;
}

std::string GetCraftName(int craftIndex, const std::string& mission)
{
	static std::string _mission;
	static std::vector<std::vector<std::string>> _lines;

	if (mission != _mission)
	{
		_mission = mission;

		const std::string path = GetStringWithoutExtension(mission);
		auto file = GetFileLines(path + ".txt");

		if (!file.size())
		{
			file = GetFileLines(path + ".ini", "mission_tie");
		}

		_lines = GetFileListValues(file);
	}

	if (!_lines.size())
	{
		return std::string();
	}

	for (const auto& line : _lines)
	{
		const auto& group = line[0];

		if (_stricmp(group.c_str(), "craft") == 0)
		{
			if (line.size() < 4)
			{
				continue;
			}

			int craft = std::stoi(line[1]);

			if (craft < 0 || craft >= 218)
			{
				continue;
			}

			if (craft != craftIndex)
			{
				continue;
			}

			const auto& element = line[2];

			if (_stricmp(element.c_str(), "name") == 0)
			{
				std::string value = line[3];
				return value;
			}
		}
	}

	return std::string();
}

int TieHook(int* params)
{
	// init music state
	// fix a bug where a mission starts with wait state instead of intro state
	*(unsigned char*)0x0069406C = 0;

	g_tieLines.clear();

	const auto LoadMission = (int(*)(const char*))0x420300;
	const char* fileName = (const char*)params[0];

	int& missionFileNameIndex = *(int*)0x06002E4;
	missionFileNameIndex++;

	if (LoadMission(fileName) == 0)
	{
		return 0;
	}

	GetSpecTable().SetCraftsValues();

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

			if (fg < 0 || fg >= 192)
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
		else if (_stricmp(group.c_str(), "craft") == 0)
		{
			if (line.size() < 4)
			{
				continue;
			}

			int craft = std::stoi(line[1]);

			if (craft < 0 || craft >= 218)
			{
				continue;
			}

			const auto& element = line[2];

			if (_stricmp(element.c_str(), "name") == 0)
			{
				std::string value = line[3];
				GetSpecTable().SetName(craft, value.c_str());
			}
			else if (_stricmp(element.c_str(), "mapicon") == 0)
			{
				if (line.size() < 7)
				{
					continue;
				}

				int left = std::stoi(line[3]);
				int top = std::stoi(line[4]);
				int right = std::stoi(line[5]);
				int bottom = std::stoi(line[6]);
				GetSpecTable().SetMapIconRect(craft, left, top, right, bottom);
			}
			else if (_stricmp(element.c_str(), "specname") == 0)
			{
				std::string value = line[3];
				GetSpecTable().SetSpecName(craft, value.c_str());
			}
			else if (_stricmp(element.c_str(), "pluralname") == 0)
			{
				std::string value = line[3];
				GetSpecTable().SetPluralName(craft, value.c_str());
			}
			else if (_stricmp(element.c_str(), "shortname") == 0)
			{
				std::string value = line[3];
				GetSpecTable().SetShortName(craft, value.c_str());
			}
		}
	}

	return 1;
}

int MissionFreeHook(int* params)
{
	const auto L00558180 = (int(*)())0x00558180;

	GetSpecTable().RestoreDefaultValues();

	return L00558180();
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
	int speedPercent = exeSpeed == 0 ? 0 : (oldSpeed * 0x10000 / exeSpeed);
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

int MissionIdRedAlertFilterHook(int* params)
{
	const XwaObject* currentObject = (XwaObject*)(params[Params_ECX] - 0x02);
	ShipCategoryEnum shipCategory = (ShipCategoryEnum)currentObject->ShipCategory;

	bool isHostCraft;

	if (currentObject->ModelIndex == 0 || currentObject->ModelIndex == 0xFFFF)
	{
		isHostCraft = false;
	}
	else
	{
		//isHostCraft = shipCategory == ShipCategory_Starship || shipCategory == ShipCategory_Platform;
		isHostCraft = true;
	}

	params[Params_ReturnAddress] = isHostCraft ? 0x0045C303 : 0x0045C2F9;

	return 0;
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
		params[Params_ReturnAddress] = 0x00498D55;
	}

	return 0;
}

int MissionIdSkipEnteringAreaMessagesHook(int* params)
{
	const auto ShowMessage = (void(*)(int, int))0x00497D40;

	const int A4 = params[0];
	const int A8 = params[1];

	bool skipHyperspacedMessages = g_missionConfig.SkipHyperspacedMessages();

	if (!skipHyperspacedMessages)
	{
		ShowMessage(A4, A8);
	}

	return 0;
}

int MissionIdSkipObjectsMessagesIffHook(int* params)
{
	int skipObjectsMessagesIff = g_missionConfig.SkipObjectsMessagesIff();
	int tieFlightGroup = (short)params[Params_EDI];
	unsigned char iff = s_XwaTieFlightGroups[tieFlightGroup].Iff;

	if (skipObjectsMessagesIff == 255 || skipObjectsMessagesIff == iff)
	{
		params[Params_ReturnAddress] = 0x00498CD3;
		return 0;
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
	int forcePlayerInTurretHours = g_missionConfig.ForcePlayerInTurretHours();
	int forcePlayerInTurretMinutes = g_missionConfig.ForcePlayerInTurretMinutes();
	int forcePlayerInTurretSeconds = g_missionConfig.ForcePlayerInTurretSeconds();

	int hours = *(unsigned char*)0x008053F7;
	int minutes = *(unsigned char*)0x008053F8;
	int seconds = *(unsigned char*)0x008053F9;

	if (!forcePlayerInTurret
		|| hours != forcePlayerInTurretHours
		|| minutes != forcePlayerInTurretMinutes
		|| seconds != forcePlayerInTurretSeconds)
	{
		params[Params_ReturnAddress] = 0x004F9517;
	}

	return 0;
}

int MissionPlayerShootHook(int* params)
{
	const int objectIndex = params[Params_EBP];

	bool disableLaser = g_missionConfig.DisablePlayerLaserShoot();
	bool disableWarhead = g_missionConfig.DisablePlayerWarheadShoot();

	if (objectIndex == 0xFFFF || (disableLaser && disableWarhead))
	{
		params[Params_ReturnAddress] = 0x004912A1;
	}

	return 0;
}

int MissionPlayerTypeShootHook(int* params)
{
	unsigned char player_m034 = (unsigned char)params[Params_EAX];

	bool disableLaser = g_missionConfig.DisablePlayerLaserShoot();
	bool disableWarhead = g_missionConfig.DisablePlayerWarheadShoot();

	if (player_m034 == 0)
	{
		if (disableLaser)
		{
			params[Params_ReturnAddress] = 0x004912A1;
		}
		else
		{
			params[Params_ReturnAddress] = 0x00490F5C;
		}
	}
	else
	{
		if (disableWarhead)
		{
			params[Params_ReturnAddress] = 0x004912A1;
		}
		else
		{
			params[Params_ReturnAddress] = 0x004911B4;
		}
	}

	return 0;
}

int MissionDrawLaserCharge3DHook(int* params)
{
	if (g_missionConfig.DisablePlayerLaserShoot())
	{
		return 0;
	}

	const auto DrawLaserCharge3D = (void(*)())0x0046A7F0;

	DrawLaserCharge3D();

	return 0;
}

int WarheadCollisionDamagesHook(int* params)
{
	const int tieVersion = params[Params_EAX];
	const int objectIndex = params[Params_EDI];

	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;

	bool enabled = g_config.IsWarheadCollisionDamagesEnabled && g_missionConfig.IsWarheadCollisionDamagesEnabled();
	bool objectEnabled = g_modelIndexProfiles.GetProfileStats(&xwaObjects[objectIndex]).IsWarheadCollisionDamagesEnabled;

	if (!enabled || !objectEnabled || tieVersion < 0x0E)
	{
		params[Params_ReturnAddress] = 0x0040F539;
	}

	return 0;
}

int ShootThroughtShieldOnHardDifficultyHook(int* params)
{
	params[Params_EAX] = *(unsigned char*)0x0080540A;

	int shield = params[Params_EDX];
	bool skip = g_missionConfig.CanShootThroughtShieldOnHardDifficulty();

	if (skip)
	{
		params[Params_ReturnAddress] = 0x0040FBDF;
	}
	else
	{
		params[Params_ReturnAddress] = shield == 0 ? 0x0040FBDF : 0x0040FC73;
	}

	return 0;
}

int TriggerProximityCheck1Hook(int* params)
{
	XwaObject* object = (XwaObject*)(params[Params_EAX] + params[Params_EDI]);
	bool ret = false;

	if (object->ModelIndex == 0)
	{
		ret = true;
	}

	if (!ret && g_missionConfig.IsSkipProjectilesProximityCheckEnabled())
	{
		if (object->pMobileObject == nullptr || object->pMobileObject->pCraft == nullptr)
		{
			ret = true;
		}
	}

	if (ret)
	{
		params[Params_ReturnAddress] = 0x004D706B;
	}

	return 0;
}

int TriggerProximityCheck2Hook(int* params)
{
	XwaObject* object = (XwaObject*)(params[Params_ESI] + params[Params_EDX]);
	bool ret = false;

	if (object->ModelIndex == 0)
	{
		ret = true;
	}

	if (!ret && g_missionConfig.IsSkipProjectilesProximityCheckEnabled())
	{
		if (object->pMobileObject == nullptr || object->pMobileObject->pCraft == nullptr)
		{
			ret = true;
		}
	}

	if (ret)
	{
		params[Params_ReturnAddress] = 0x004D7214;
	}

	return 0;
}

int OrderOfBattleHook(int* params)
{
	const int tieFlightGroupPtr = params[Params_EAX];
	const unsigned char craftId = *(unsigned char*)(tieFlightGroupPtr + 0x006B);
	char* buffer = (char*)params[0];
	const char* format = (const char*)params[1];
	const char* craftTypeName = (const char*)params[2];
	const char* fgName = (const char*)params[3];
	const int fgIndex = params[11];

	const auto L0059A680 = (int(*)(char*, const char*, ...))0x0059A680;

	int craftIndex = GetCraftIndex(craftId);
	std::string missionFileName = GetMissionFileName();
	std::string craftName = GetCraftName(craftIndex, missionFileName);

	if (craftName.empty())
	{
		L0059A680(buffer, format, craftTypeName, fgName);
	}
	else
	{
		L0059A680(buffer, format, craftName.c_str(), fgName);
	}

	return 0;
}

int LoadMissionRanksModifierHook(int* params)
{
	params[Params_EBX] = *(char*)0x0080540A;

	if (!g_config.IsMissionRanksModifierEnabled)
	{
		params[Params_ReturnAddress] = 0x0041669F;
	}

	return 0;
}

SpecRciEntry GetSpecRciMissionEntry(int modelIndex, const std::string& mission)
{
	static std::string _mission;
	static std::vector<std::string> _lines;

	if (mission != _mission)
	{
		_mission = mission;

		const std::string path = GetStringWithoutExtension(mission);
		_lines = GetFileLines(path + "_SpecRci.txt");

		if (!_lines.size())
		{
			_lines = GetFileLines(path + ".ini", "SpecRci");
		}
	}

	std::string shipPath = g_flightModelsList.GetLstLine(modelIndex);
	std::string shipName = GetPathFileName(shipPath);

	SpecRciEntry entry{};
	entry.Speed = GetFileKeyValueInt(_lines, shipName + "_Speed", -1);
	entry.Acceleration = GetFileKeyValueInt(_lines, shipName + "_Acceleration", -1);
	entry.Maneuverability = GetFileKeyValueInt(_lines, shipName + "_Maneuverability", -1);
	entry.Laser = GetFileKeyValueInt(_lines, shipName + "_Laser", -1);
	entry.Ion = GetFileKeyValueInt(_lines, shipName + "_Ion", -1);
	entry.Missile = GetFileKeyValueInt(_lines, shipName + "_Missile", -1);
	entry.Shield = GetFileKeyValueInt(_lines, shipName + "_Shield", -1);
	entry.Hull = GetFileKeyValueInt(_lines, shipName + "_Hull", -1);
	entry.Size = GetFileKeyValueInt(_lines, shipName + "_Size", -1);
	entry.Score = GetFileKeyValueInt(_lines, shipName + "_Score", -1);

	return entry;
}

SpecRciEntry GetSpecRciModelEntry(int modelIndex)
{
	std::string shipPath = g_flightModelsList.GetLstLine(modelIndex);
	std::vector<std::string> lines;

	if (!lines.size())
	{
		lines = GetFileLines(shipPath + "SpecRci.txt");
	}

	if (!lines.size())
	{
		lines = GetFileLines(shipPath + ".ini", "SpecRci");
	}

	SpecRciEntry entry{};
	entry.Speed = GetFileKeyValueInt(lines, "Speed", -1);
	entry.Acceleration = GetFileKeyValueInt(lines, "Acceleration", -1);
	entry.Maneuverability = GetFileKeyValueInt(lines, "Maneuverability", -1);
	entry.Laser = GetFileKeyValueInt(lines, "Laser", -1);
	entry.Ion = GetFileKeyValueInt(lines, "Ion", -1);
	entry.Missile = GetFileKeyValueInt(lines, "Missile", -1);
	entry.Shield = GetFileKeyValueInt(lines, "Shield", -1);
	entry.Hull = GetFileKeyValueInt(lines, "Hull", -1);
	entry.Size = GetFileKeyValueInt(lines, "Size", -1);
	entry.Score = GetFileKeyValueInt(lines, "Score", -1);

	return entry;
}

int FillSpecRciEntryHook(int* params)
{
	unsigned short modelIndex = (unsigned short)params[4];
	SpecRciEntry* modelSpecRciEntry = (SpecRciEntry*)params[Params_ESI];

	std::string missionFileName = GetMissionFileName();
	SpecRciEntry specRciMissionEntry = GetSpecRciMissionEntry(modelIndex, missionFileName);
	SpecRciEntry specRciModelEntry = GetSpecRciModelEntry(modelIndex);

	if (specRciMissionEntry.Speed != -1)
	{
		modelSpecRciEntry->Speed = specRciMissionEntry.Speed;
	}
	else if (specRciModelEntry.Speed != -1)
	{
		modelSpecRciEntry->Speed = specRciModelEntry.Speed;
	}

	if (specRciMissionEntry.Acceleration != -1)
	{
		modelSpecRciEntry->Acceleration = specRciMissionEntry.Acceleration;
	}
	else if (specRciModelEntry.Acceleration != -1)
	{
		modelSpecRciEntry->Acceleration = specRciModelEntry.Acceleration;
	}

	if (specRciMissionEntry.Maneuverability != -1)
	{
		modelSpecRciEntry->Maneuverability = specRciMissionEntry.Maneuverability;
	}
	else if (specRciModelEntry.Maneuverability != -1)
	{
		modelSpecRciEntry->Maneuverability = specRciModelEntry.Maneuverability;
	}

	if (specRciMissionEntry.Laser != -1)
	{
		modelSpecRciEntry->Laser = specRciMissionEntry.Laser;
	}
	else if (specRciModelEntry.Laser != -1)
	{
		modelSpecRciEntry->Laser = specRciModelEntry.Laser;
	}

	if (specRciMissionEntry.Ion != -1)
	{
		modelSpecRciEntry->Ion = specRciMissionEntry.Ion;
	}
	else if (specRciModelEntry.Ion != -1)
	{
		modelSpecRciEntry->Ion = specRciModelEntry.Ion;
	}

	if (specRciMissionEntry.Missile != -1)
	{
		modelSpecRciEntry->Missile = specRciMissionEntry.Missile;
	}
	else if (specRciModelEntry.Missile != -1)
	{
		modelSpecRciEntry->Missile = specRciModelEntry.Missile;
	}

	if (specRciMissionEntry.Shield != -1)
	{
		modelSpecRciEntry->Shield = specRciMissionEntry.Shield;
	}
	else if (specRciModelEntry.Shield != -1)
	{
		modelSpecRciEntry->Shield = specRciModelEntry.Shield;
	}

	if (specRciMissionEntry.Hull != -1)
	{
		modelSpecRciEntry->Hull = specRciMissionEntry.Hull;
	}
	else if (specRciModelEntry.Hull != -1)
	{
		modelSpecRciEntry->Hull = specRciModelEntry.Hull;
	}

	if (specRciMissionEntry.Size != -1)
	{
		modelSpecRciEntry->Size = specRciMissionEntry.Size;
	}
	else if (specRciModelEntry.Size != -1)
	{
		modelSpecRciEntry->Size = specRciModelEntry.Size;
	}

	if (specRciMissionEntry.Score != -1)
	{
		modelSpecRciEntry->Score = specRciMissionEntry.Score;
	}
	else if (specRciModelEntry.Score != -1)
	{
		modelSpecRciEntry->Score = specRciModelEntry.Score;
	}

	return 1;
}

bool FilterTargetCraft(int objectIndex, const XwaObject* object, int team, bool selectOnlyNotInspected)
{
	if (object->pMobileObject == nullptr || object->pMobileObject->pCraft == nullptr)
	{
		return false;
	}

	XwaCraft* craft = object->pMobileObject->pCraft;

	if (selectOnlyNotInspected)
	{
		// inspected
		if (craft->m196[team] >= 1)
		{
			return false;
		}
	}

	return true;
}

short GetNearestTargetCraft(const std::set<int>& targetFGs, int playerIndex)
{
	const auto L0041DA50 = (void(*)(int, int, int, int))0x0041DA50;
	const auto L004EAC30 = (void(*)(int, int, int))0x004EAC30;
	const auto L004A2CC0 = (void(*)(int, int))0x004A2CC0;

	short targetObjectIndex = -1;
	unsigned int targetObjectDistance = 0xffffffff;
	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const XwaPlayer* xwaPlayers = (XwaPlayer*)0x008B94E0;

	const XwaPlayer* player = &xwaPlayers[playerIndex];

	bool selectOnlyNotInspected = g_missionConfig.TargetCraftKeySelectOnlyNotInspected();

	for (int objectIndex = *(int*)0x08BF378; objectIndex < *(int*)0x07CA3B8; objectIndex++)
	{
		const XwaObject* object = &xwaObjects[objectIndex];

		if (object->ModelIndex == 0)
		{
			continue;
		}

		if (object->Region != player->Region)
		{
			continue;
		}

		if (!FilterTargetCraft(objectIndex, object, player->Team, selectOnlyNotInspected))
		{
			continue;
		}

		int fg = object->TieFlightGroupIndex;

		if (!targetFGs.contains(fg))
		{
			continue;
		}

		if (player->ObjectIndex == 0xFFFF)
		{
			L0041DA50(objectIndex, 0, 0, 0);

			L004EAC30(
				*(int*)0x09109BC - player->Camera.PositionX,
				*(int*)0x09109B8 - player->Camera.PositionY,
				*(int*)0x09109B4 - player->Camera.PositionZ);
		}
		else
		{
			L004A2CC0(player->ObjectIndex, objectIndex);
		}

		if (*(unsigned int*)0x07D4B60 < targetObjectDistance)
		{
			targetObjectIndex = objectIndex;
			targetObjectDistance = *(unsigned int*)0x07D4B60;
		}
	}

	return targetObjectIndex;
}

short GetCycleTargetCraft(const std::set<int>& targetFGs, int playerIndex)
{
	static short _lastTargetObjectIndex[10];
	static bool _init = true;

	if (_init)
	{
		_init = false;

		for (int i = 0; i < 10; i++)
		{
			_lastTargetObjectIndex[i] = -1;
		}
	}

	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const XwaPlayer* xwaPlayers = (XwaPlayer*)0x008B94E0;
	int firstObject = *(int*)0x08BF378;
	int lastObject = *(int*)0x07CA3B8;
	int objectsCount = lastObject - firstObject;

	const XwaPlayer* player = &xwaPlayers[playerIndex];
	short lastTargetObjectIndex = _lastTargetObjectIndex[playerIndex];

	bool selectOnlyNotInspected = g_missionConfig.TargetCraftKeySelectOnlyNotInspected();

	if (lastTargetObjectIndex < firstObject || lastTargetObjectIndex >= lastObject)
	{
		lastTargetObjectIndex = firstObject - 1;

		_lastTargetObjectIndex[playerIndex] = lastTargetObjectIndex;
	}

	for (int currentIndex = 0; currentIndex < objectsCount; currentIndex++)
	{
		lastTargetObjectIndex++;

		if (lastTargetObjectIndex >= lastObject)
		{
			lastTargetObjectIndex = firstObject;
		}

		_lastTargetObjectIndex[playerIndex] = lastTargetObjectIndex;

		const XwaObject* object = &xwaObjects[lastTargetObjectIndex];

		if (object->ModelIndex == 0)
		{
			continue;
		}

		if (object->Region != player->Region)
		{
			continue;
		}

		if (!FilterTargetCraft(lastTargetObjectIndex, object, player->Team, selectOnlyNotInspected))
		{
			continue;
		}

		int fg = object->TieFlightGroupIndex;

		if (!targetFGs.contains(fg))
		{
			continue;
		}

		return lastTargetObjectIndex;
	}

	return -1;
}

int TargetNextCraftHook(int* params)
{
	const auto XwaTargetObject = (void(*)(short, int))0x00503E10;
	const auto L0041DA50 = (void(*)(int, int, int, int))0x0041DA50;
	const auto L004EAC30 = (void(*)(int, int, int))0x004EAC30;
	const auto L004A2CC0 = (void(*)(int, int))0x004A2CC0;

	unsigned char isMelee = *(unsigned char*)0x008053E5;
	params[Params_EAX] = isMelee;

	if (isMelee != 0)
	{
		return 0;
	}

	const std::set<int>& targetFGs = g_missionConfig.GetTargetCraftKeyFgIndices();

	if (targetFGs.empty())
	{
		return 0;
	}

	params[Params_ReturnAddress] = 0x004FFC05;

	int playerIndex = params[Params_EDI];
	short targetObjectIndex = -1;

	int targetCraftKeyMethod = g_missionConfig.TargetCraftKeyMethod();

	switch (targetCraftKeyMethod)
	{
	case 0:
	default:
		targetObjectIndex = GetNearestTargetCraft(targetFGs, playerIndex);
		break;

	case 1:
		targetObjectIndex = GetCycleTargetCraft(targetFGs, playerIndex);
		break;
	}

	if (targetObjectIndex == -1)
	{
		return 0;
	}

	XwaTargetObject(targetObjectIndex, playerIndex);

	return 0;
}

int CampaignCraftsListHook(int* params)
{
	int fgIndex = params[7];

	int teamFg = params[Params_ECX];
	int teamPilot = params[Params_EDX];

	const std::vector<int>& camapignCraftsList = g_missionConfig.CampaignCraftsList();

	if (camapignCraftsList.empty())
	{
		if (teamFg != teamPilot)
		{
			params[Params_ReturnAddress] = 0x00578DFC;
			return 0;
		}

		return 0;
	}

	if (std::find(camapignCraftsList.begin(), camapignCraftsList.end(), fgIndex) == camapignCraftsList.end())
	{
		params[Params_ReturnAddress] = 0x00578DFC;
	}
	else
	{
		int edx0 = *(unsigned char*)(params[Params_EAX] + 0x6B);
		int ecx0 = ((int*)0x00ABD280)[edx0];
		int edx = *(int*)0x00ABD22C + ecx0 * 0x128;
		int shiplistType = *(int*)(edx + 0x104);

		const int ShiplistType_LightTransport = 2;
		const int ShiplistType_UtilityCraft = 3;
		const int ShiplistType_Container = 4;
		const int ShiplistType_Freighter = 5;
		const int ShiplistType_Starship = 6;
		const int ShiplistType_Station = 7;

		if (shiplistType == ShiplistType_LightTransport
			|| shiplistType == ShiplistType_UtilityCraft
			|| shiplistType == ShiplistType_Container
			|| shiplistType == ShiplistType_Freighter
			|| shiplistType == ShiplistType_Starship
			|| shiplistType == ShiplistType_Station)
		{
			params[Params_EDX] = edx;
			params[Params_ReturnAddress] = 0x00578C3B;
		}
		else
		{
			params[Params_ReturnAddress] = 0x00578DFC;
		}
	}

	return 0;
}

int CampaignCraftsFillListHook(int* params)
{
	int fgIndex = params[Params_EBP];

	int teamFg = *(unsigned char*)(params[Params_ECX] - 0x17);
	int teamPilot = params[Params_EDX];

	const std::vector<int>& camapignCraftsList = g_missionConfig.CampaignCraftsList();

	if (camapignCraftsList.empty())
	{
		if (teamFg != teamPilot)
		{
			params[Params_ReturnAddress] = 0x0057901D;
			return 0;
		}

		return 0;
	}

	if (std::find(camapignCraftsList.begin(), camapignCraftsList.end(), fgIndex) == camapignCraftsList.end())
	{
		params[Params_ReturnAddress] = 0x0057901D;
	}
	else
	{
		int eax0 = (int)*(unsigned char*)(params[Params_ECX] - 0x1D) * 0x04;
		int edx0 = *(int*)(0x00ABD280 + eax0);
		int edx = params[Params_EDI] + edx0 * 0x128;
		int shiplistType = *(int*)(edx + 0x104);

		const int ShiplistType_Fighter = 1;

		if (shiplistType == ShiplistType_Fighter)
		{
			params[Params_EAX] = eax0;
			params[Params_ReturnAddress] = 0x00579007;
		}
		else
		{
			params[Params_ReturnAddress] = 0x0057901D;
		}
	}

	return 0;
}
