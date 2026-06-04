#include "targetver.h"
#include "weapon.h"
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

std::string GetFileNameWithoutExtension(const std::string& str)
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

#pragma pack(push, 1)

enum AIRankEnum : unsigned char
{
	AIRank_Novice = 0,
	AIRank_Officer = 1,
	AIRank_Veteran = 2,
	AIRank_Ace = 3,
	AIRank_TopAce = 4,
	AIRank_SuperAce = 5,
};

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

struct XwaObject
{
	char Unk0000[2];
	unsigned short ModelIndex;
	ShipCategoryEnum ShipCategory;
	unsigned char TieFlightGroupIndex;
	char Unk0006[33];
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

#pragma pack(pop)

std::string GetShipPath(const std::vector<std::string>& linesObjects, unsigned short modelIndex)
{
	std::string shipPath = g_flightModelsList.GetLstLine(modelIndex);
	const std::string objectValue = GetFileKeyValue(linesObjects, shipPath + ".opt");

	if (!objectValue.empty() && std::ifstream(objectValue))
	{
		shipPath = GetStringWithoutExtension(objectValue);
	}

	return shipPath;
}

struct WarheadLockSettings
{
	int LockDistance0;
	int LockDistance1;
	int LockDistance2;
	int LockTime;
	int LockStrengthDivisor;
	int AILockDistance0;
	int AILockDistance1;
	int AILockDistance2;
	int AILockTimeFactor;
};

WarheadLockSettings GetWarheadLockSettings(int objectIndex)
{
	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	XwaObject* object = &XwaObjects[objectIndex];

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

	std::string profile = GetFileKeyValue(lines, "ObjectProfile_fg_" + std::to_string(object->TieFlightGroupIndex));

	if (profile.empty())
	{
		const std::string shipName = GetFileNameWithoutExtension(g_flightModelsList.GetLstLine(object->ModelIndex));
		profile = GetFileKeyValue(lines, "ObjectProfile_" + shipName);
	}

	std::string shipPath = g_flightModelsList.GetLstLine(object->ModelIndex);

	const std::string objectValue = GetFileKeyValue(lines, shipPath + ".opt");

	if (!objectValue.empty() && std::ifstream(objectValue))
	{
		shipPath = GetStringWithoutExtension(objectValue);
	}

	lines = GetFileLines(shipPath + "WarheadLock.txt");

	if (!lines.size())
	{
		lines = GetFileLines(shipPath + ".ini", "WarheadLock");
	}

	WarheadLockSettings settings{};
	settings.LockDistance0 = 0x018DAD;
	settings.LockDistance1 = 0x03BA6C;
	settings.LockDistance2 = 0x027C48;
	settings.LockTime = 0x2C4;
	settings.LockStrengthDivisor = 0xEC;
	settings.AILockDistance0 = 0x18DAD;
	settings.AILockDistance1 = 0x31B5A;
	settings.AILockDistance2 = 0x3BA6C;
	settings.AILockTimeFactor = 0x1D8;

	std::vector<std::string> defaultLines = GetFileLines("FlightModels\\WarheadLock.txt");

	if (!defaultLines.size())
	{
		defaultLines = GetFileLines("FlightModels\\default.ini", "WarheadLock");
	}

	if (defaultLines.size())
	{
		switch (object->ShipCategory)
		{
		case ShipCategory_Starship:
			settings.AILockDistance1 = GetFileKeyValueInt(defaultLines, "AILockDistance1_Starship", settings.AILockDistance1);
			settings.AILockDistance2 = GetFileKeyValueInt(defaultLines, "AILockDistance2_Starship", settings.AILockDistance2);
			break;

		case ShipCategory_Platform:
			settings.AILockDistance1 = GetFileKeyValueInt(defaultLines, "AILockDistance1_Platform", settings.AILockDistance1);
			settings.AILockDistance2 = GetFileKeyValueInt(defaultLines, "AILockDistance2_Platform", settings.AILockDistance2);
			break;

		case ShipCategory_Freighter:
			settings.AILockDistance1 = GetFileKeyValueInt(defaultLines, "AILockDistance1_Freighter", settings.AILockDistance1);
			settings.AILockDistance2 = GetFileKeyValueInt(defaultLines, "AILockDistance2_Freighter", settings.AILockDistance2);
			break;

		case ShipCategory_Transport:
			settings.AILockDistance1 = GetFileKeyValueInt(defaultLines, "AILockDistance1_Transport", settings.AILockDistance1);
			settings.AILockDistance2 = GetFileKeyValueInt(defaultLines, "AILockDistance2_Transport", settings.AILockDistance2);
			break;

		case ShipCategory_Container:
			settings.AILockDistance1 = GetFileKeyValueInt(defaultLines, "AILockDistance1_Container", settings.AILockDistance1);
			settings.AILockDistance2 = GetFileKeyValueInt(defaultLines, "AILockDistance2_Container", settings.AILockDistance2);
			break;

		case ShipCategory_Armament:
			settings.AILockDistance1 = GetFileKeyValueInt(defaultLines, "AILockDistance1_Armament", settings.AILockDistance1);
			settings.AILockDistance2 = GetFileKeyValueInt(defaultLines, "AILockDistance2_Armament", settings.AILockDistance2);
			break;
		}
	}

	if (lines.size())
	{
		settings.LockDistance0 = GetFileKeyValueInt(lines, "LockDistance0", settings.LockDistance0);
		settings.LockDistance0 = GetFileKeyValueInt(lines, "LockDistance0_" + profile, settings.LockDistance0);
		settings.LockDistance1 = GetFileKeyValueInt(lines, "LockDistance1", settings.LockDistance1);
		settings.LockDistance1 = GetFileKeyValueInt(lines, "LockDistance1_" + profile, settings.LockDistance1);
		settings.LockDistance2 = GetFileKeyValueInt(lines, "LockDistance2", settings.LockDistance2);
		settings.LockDistance2 = GetFileKeyValueInt(lines, "LockDistance2_" + profile, settings.LockDistance2);
		settings.LockTime = GetFileKeyValueInt(lines, "LockTime", settings.LockTime);
		settings.LockTime = GetFileKeyValueInt(lines, "LockTime_" + profile, settings.LockTime);
		settings.LockStrengthDivisor = GetFileKeyValueInt(lines, "LockStrengthDivisor", settings.LockStrengthDivisor);
		settings.LockStrengthDivisor = GetFileKeyValueInt(lines, "LockStrengthDivisor_" + profile, settings.LockStrengthDivisor);
		settings.AILockDistance0 = GetFileKeyValueInt(lines, "AILockDistance0", settings.AILockDistance0);
		settings.AILockDistance0 = GetFileKeyValueInt(lines, "AILockDistance0_" + profile, settings.AILockDistance0);
		settings.AILockDistance1 = GetFileKeyValueInt(lines, "AILockDistance1", settings.AILockDistance1);
		settings.AILockDistance1 = GetFileKeyValueInt(lines, "AILockDistance1_" + profile, settings.AILockDistance1);
		settings.AILockDistance2 = GetFileKeyValueInt(lines, "AILockDistance2", settings.AILockDistance2);
		settings.AILockDistance2 = GetFileKeyValueInt(lines, "AILockDistance2_" + profile, settings.AILockDistance2);
		settings.AILockTimeFactor = GetFileKeyValueInt(lines, "AILockTimeFactor", settings.AILockTimeFactor);
		settings.AILockTimeFactor = GetFileKeyValueInt(lines, "AILockTimeFactor_" + profile, settings.AILockTimeFactor);
	}
	else
	{
		// ModelIndex_012_0_11_MissileBoat
		if (object->ModelIndex == 0x0C)
		{
			settings.LockTime = 0x162;
		}
	}

	return settings;
}

struct ModelAttackBreakSettings
{
	int Warhead;
	int SmallObject_NoviceRank;
	int SmallObject_OfficerRank;
	int SmallObject_VeteranRank;
	int SmallObject_AceRank;
	int SmallObject_TopAceRank;
	int SmallObject_SuperAceRank;
	int LargeObject_MaxDistance;
	int Model_Offsets[557];
	int MaxRange_Novice;
	int MaxRange_Officer;
	int MaxRange_Veteran;
	int MaxRange_Ace;
	int MaxRange_TopAce;
	int MaxRange_SuperAce;
};

ModelAttackBreakSettings GetModelAttackBreakSettings(unsigned short objectModelIndex)
{
	const char* xwaMissionFileName = (const char*)0x06002E8;

	const std::string mission = GetStringWithoutExtension(xwaMissionFileName);
	std::vector<std::string> linesObjects = GetFileLines(mission + "_Objects.txt");

	if (!linesObjects.size())
	{
		linesObjects = GetFileLines(mission + ".ini", "Objects");
	}

	if (!linesObjects.size())
	{
		linesObjects = GetFileLines("FlightModels\\Objects.txt");
	}

	if (!linesObjects.size())
	{
		linesObjects = GetFileLines("FlightModels\\default.ini", "Objects");
	}

	std::string shipPath = GetShipPath(linesObjects, objectModelIndex);

	std::vector<std::string> lines = GetFileLines(shipPath + "AttackBreakDistance.txt");

	if (!lines.size())
	{
		lines = GetFileLines(shipPath + ".ini", "AttackBreakDistance");
	}

	ModelAttackBreakSettings settings{};
	settings.Warhead = 5120;
	settings.SmallObject_NoviceRank = 5120;
	settings.SmallObject_OfficerRank = 5120;
	settings.SmallObject_VeteranRank = 4096;
	settings.SmallObject_AceRank = 3072;
	settings.SmallObject_TopAceRank = 2304;
	settings.SmallObject_SuperAceRank = 1536;
	settings.LargeObject_MaxDistance = 8192;
	memset(settings.Model_Offsets, 0, sizeof(settings.Model_Offsets));
	settings.Model_Offsets[140] = 32768; // ModelIndex_140_0_94_SuperStarDestroyer
	settings.Model_Offsets[162] = 32768; // ModelIndex_162_0_111_RepairYard
	settings.Model_Offsets[140] = 32768; // ModelIndex_161_0_110_ShipYard
	settings.MaxRange_Novice = 24576;
	settings.MaxRange_Officer = 32768;
	settings.MaxRange_Veteran = 40960;
	settings.MaxRange_Ace = 40960;
	settings.MaxRange_TopAce = 40960;
	settings.MaxRange_SuperAce = 40960;

	std::vector<std::string> defaultLines = GetFileLines("FlightModels\\AttackBreakDistance.txt");

	if (!defaultLines.size())
	{
		defaultLines = GetFileLines("FlightModels\\default.ini", "AttackBreakDistance");
	}

	if (defaultLines.size())
	{
		settings.Warhead = GetFileKeyValueInt(defaultLines, "Warhead", settings.Warhead);
		settings.SmallObject_NoviceRank = GetFileKeyValueInt(defaultLines, "SmallObject_NoviceRank", settings.SmallObject_NoviceRank);
		settings.SmallObject_OfficerRank = GetFileKeyValueInt(defaultLines, "SmallObject_OfficerRank", settings.SmallObject_OfficerRank);
		settings.SmallObject_VeteranRank = GetFileKeyValueInt(defaultLines, "SmallObject_VeteranRank", settings.SmallObject_VeteranRank);
		settings.SmallObject_AceRank = GetFileKeyValueInt(defaultLines, "SmallObject_AceRank", settings.SmallObject_AceRank);
		settings.SmallObject_TopAceRank = GetFileKeyValueInt(defaultLines, "SmallObject_TopAceRank", settings.SmallObject_TopAceRank);
		settings.SmallObject_SuperAceRank = GetFileKeyValueInt(defaultLines, "SmallObject_SuperAceRank", settings.SmallObject_SuperAceRank);
		settings.LargeObject_MaxDistance = GetFileKeyValueInt(defaultLines, "LargeObject_MaxDistance", settings.LargeObject_MaxDistance);

		for (int modelIndex = 0; modelIndex < 557; modelIndex++)
		{
			std::string name = GetFileNameWithoutExtension(GetShipPath(linesObjects, modelIndex));

			if (name.empty())
			{
				continue;
			}

			settings.Model_Offsets[modelIndex] = GetFileKeyValueInt(defaultLines, "Model_" + name, settings.Model_Offsets[modelIndex]);
		}

		settings.MaxRange_Novice = GetFileKeyValueInt(defaultLines, "MaxRange_Novice", settings.MaxRange_Novice);
		settings.MaxRange_Officer = GetFileKeyValueInt(defaultLines, "MaxRange_Officer", settings.MaxRange_Officer);
		settings.MaxRange_Veteran = GetFileKeyValueInt(defaultLines, "MaxRange_Veteran", settings.MaxRange_Veteran);
		settings.MaxRange_Ace = GetFileKeyValueInt(defaultLines, "MaxRange_Ace", settings.MaxRange_Ace);
		settings.MaxRange_TopAce = GetFileKeyValueInt(defaultLines, "MaxRange_TopAce", settings.MaxRange_TopAce);
		settings.MaxRange_SuperAce = GetFileKeyValueInt(defaultLines, "MaxRange_SuperAce", settings.MaxRange_SuperAce);
	}

	if (lines.size())
	{
		settings.Warhead = GetFileKeyValueInt(lines, "Warhead", settings.Warhead);
		settings.SmallObject_NoviceRank = GetFileKeyValueInt(lines, "SmallObject_NoviceRank", settings.SmallObject_NoviceRank);
		settings.SmallObject_OfficerRank = GetFileKeyValueInt(lines, "SmallObject_OfficerRank", settings.SmallObject_OfficerRank);
		settings.SmallObject_VeteranRank = GetFileKeyValueInt(lines, "SmallObject_VeteranRank", settings.SmallObject_VeteranRank);
		settings.SmallObject_AceRank = GetFileKeyValueInt(lines, "SmallObject_AceRank", settings.SmallObject_AceRank);
		settings.SmallObject_TopAceRank = GetFileKeyValueInt(lines, "SmallObject_TopAceRank", settings.SmallObject_TopAceRank);
		settings.SmallObject_SuperAceRank = GetFileKeyValueInt(lines, "SmallObject_SuperAceRank", settings.SmallObject_SuperAceRank);
		settings.LargeObject_MaxDistance = GetFileKeyValueInt(lines, "LargeObject_MaxDistance", settings.LargeObject_MaxDistance);

		for (int modelIndex = 0; modelIndex < 557; modelIndex++)
		{
			std::string name = GetFileNameWithoutExtension(GetShipPath(linesObjects, modelIndex));

			if (name.empty())
			{
				continue;
			}

			settings.Model_Offsets[modelIndex] = GetFileKeyValueInt(lines, "Model_" + name, settings.Model_Offsets[modelIndex]);
		}

		settings.MaxRange_Novice = GetFileKeyValueInt(lines, "MaxRange_Novice", settings.MaxRange_Novice);
		settings.MaxRange_Officer = GetFileKeyValueInt(lines, "MaxRange_Officer", settings.MaxRange_Officer);
		settings.MaxRange_Veteran = GetFileKeyValueInt(lines, "MaxRange_Veteran", settings.MaxRange_Veteran);
		settings.MaxRange_Ace = GetFileKeyValueInt(lines, "MaxRange_Ace", settings.MaxRange_Ace);
		settings.MaxRange_TopAce = GetFileKeyValueInt(lines, "MaxRange_TopAce", settings.MaxRange_TopAce);
		settings.MaxRange_SuperAce = GetFileKeyValueInt(lines, "MaxRange_SuperAce", settings.MaxRange_SuperAce);
	}

	return settings;
}

class ModelIndexConfig
{
public:
	const WarheadLockSettings& GetWarheadLock(int objectIndex)
	{
		this->Update();

		XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
		XwaObject* object = &XwaObjects[objectIndex];

		auto it = this->_warheadLockSettings.find(object->TieFlightGroupIndex);

		if (it != this->_warheadLockSettings.end())
		{
			return it->second;
		}
		else
		{
			WarheadLockSettings value = GetWarheadLockSettings(objectIndex);
			this->_warheadLockSettings.insert(std::make_pair(object->TieFlightGroupIndex, value));

			it = this->_warheadLockSettings.find(object->TieFlightGroupIndex);
			return it->second;
		}
	}

	const ModelAttackBreakSettings& GetModelAttackBreak(unsigned short objectModelIndex)
	{
		this->Update();

		auto it = this->_modelAttackBreakSettings.find(objectModelIndex);

		if (it != this->_modelAttackBreakSettings.end())
		{
			return it->second;
		}
		else
		{
			ModelAttackBreakSettings value = GetModelAttackBreakSettings(objectModelIndex);
			this->_modelAttackBreakSettings.insert(std::make_pair(objectModelIndex, value));

			it = this->_modelAttackBreakSettings.find(objectModelIndex);
			return it->second;
		}
	}


private:
	void Update()
	{
		static std::string _mission;
		static int _missionIndex = 0;

		const char* xwaMissionFileName = (const char*)0x06002E8;
		const int missionFileNameIndex = *(int*)0x06002E4;

		if (missionFileNameIndex == 0 ? (_mission != xwaMissionFileName) : (_missionIndex != missionFileNameIndex))
		{
			_mission = xwaMissionFileName;
			_missionIndex = missionFileNameIndex;

			this->_warheadLockSettings.clear();
			this->_modelAttackBreakSettings.clear();
		}
	}

	std::map<int, WarheadLockSettings> _warheadLockSettings;
	std::map<unsigned short, ModelAttackBreakSettings> _modelAttackBreakSettings;
};

ModelIndexConfig g_modelIndexConfig;

int WarheadLockDistance0Hook(int* params)
{
	int objectIndex = params[8] / 0x27;
	params[Params_EBX] = g_modelIndexConfig.GetWarheadLock(objectIndex).LockDistance0;

	params[Params_ECX] = *(int*)0x007B33C4;

	// XWA Warhead Lock Distance hook
	if (*(unsigned int*)0x0048FE13 != 0x018DADBB)
	{
		params[Params_ReturnAddress] = 0x0048FE13;
	}
	else
	{
		int esi = params[Params_ESI];

		if (esi < *(int*)0x008BF378 || esi >= *(int*)0x007CA3B8)
		{
			params[Params_ReturnAddress] = 0x0048FE46;
		}
		else
		{
			params[Params_ReturnAddress] = 0x0048FE22;
		}
	}

	return 0;
}

int WarheadLockDistance1Hook(int* params)
{
	int objectIndex = params[8] / 0x27;
	params[Params_EBX] = g_modelIndexConfig.GetWarheadLock(objectIndex).LockDistance1;
	return 0;
}

int WarheadLockDistance2Hook(int* params)
{
	int objectIndex = params[8] / 0x27;
	params[Params_EBX] = g_modelIndexConfig.GetWarheadLock(objectIndex).LockDistance2;
	return 0;
}

int WarheadLockTime0Hook(int* params)
{
	int objectIndex = params[8] / 0x27;
	params[Params_EAX] = g_modelIndexConfig.GetWarheadLock(objectIndex).LockTime;
	return 0;
}

int WarheadLockTime1Hook(int* params)
{
	int objectIndex = *(int*)(0x008B94E0 + *(int*)0x008C1CC8 * 0xBCF);
	params[Params_ESI] = g_modelIndexConfig.GetWarheadLock(objectIndex).LockTime;
	return 0;
}

int WarheadLockTime2Hook(int* params)
{
	int objectIndex = *(int*)(0x008B94E0 + *(int*)0x008C1CC8 * 0xBCF);
	params[Params_ECX] = g_modelIndexConfig.GetWarheadLock(objectIndex).LockTime;
	return 0;
}

int WarheadLockStrengthHook(int* params)
{
	params[Params_ReturnAddress] = 0x00491CC7;

	int objectIndex = params[Params_EDI] / 0x27;
	int time = params[Params_EDX];

	int divisor = g_modelIndexConfig.GetWarheadLock(objectIndex).LockStrengthDivisor;
	int strength = time / divisor;

	params[Params_EDX] = strength;

	return 0;
}

int WarheadAILockDistance0Hook(int* params)
{
	//int objectIndex = params[Params_ESI];
	int objectIndex = *(int*)(0x07CA1A0 + 0x00);
	int aiLockDistance0 = g_modelIndexConfig.GetWarheadLock(objectIndex).AILockDistance0;

	params[10] = aiLockDistance0;
	return 0;
}

int WarheadAILockDistance1Hook(int* params)
{
	//int objectIndex = params[Params_ESI];
	int objectIndex = *(int*)(0x07CA1A0 + 0x00);
	int aiLockDistance1 = g_modelIndexConfig.GetWarheadLock(objectIndex).AILockDistance1;
	int aiLockDistance2 = g_modelIndexConfig.GetWarheadLock(objectIndex).AILockDistance2;
	int rank = *(short*)0x007CA1CC;
	int distance = rank == 2 ? aiLockDistance2 : aiLockDistance1;

	params[Params_EAX] = distance;
	return 0;
}

int WarheadAILockTimeFactorHook(int* params)
{
	//int objectIndex = params[17] / 0x27;
	int objectIndex = *(int*)(0x07CA1A0 + 0x00);
	int aiLockTimeFactor = g_modelIndexConfig.GetWarheadLock(objectIndex).AILockTimeFactor;
	int rank = *(short*)0x007CA1CC;
	int time = (rank + 1) * aiLockTimeFactor;

	params[Params_ESI] = time;
	return 0;
}

int ModelAttackBreakWarheadHook(int* params)
{
	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	int objectIndex = *(int*)0x007CA1A0;
	unsigned short modelIndex = XwaObjects[objectIndex].ModelIndex;
	const ModelAttackBreakSettings& settings = g_modelIndexConfig.GetModelAttackBreak(modelIndex);
	params[Params_EDI] = settings.Warhead;
	return 0;
}

int ModelAttackBreakSmallObjectRankHook(int* params)
{
	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	int objectIndex = *(int*)0x007CA1A0;
	unsigned short modelIndex = XwaObjects[objectIndex].ModelIndex;
	AIRankEnum rank = (AIRankEnum)params[Params_EAX];
	const ModelAttackBreakSettings& settings = g_modelIndexConfig.GetModelAttackBreak(modelIndex);

	int edi = settings.SmallObject_NoviceRank;
	switch (rank)
	{
	case AIRank_Novice:
		edi = settings.SmallObject_NoviceRank;
		break;
	case AIRank_Officer:
		edi = settings.SmallObject_OfficerRank;
		break;
	case AIRank_Veteran:
		edi = settings.SmallObject_VeteranRank;
		break;
	case AIRank_Ace:
		edi = settings.SmallObject_AceRank;
		break;
	case AIRank_TopAce:
		edi = settings.SmallObject_TopAceRank;
		break;
	case AIRank_SuperAce:
		edi = settings.SmallObject_SuperAceRank;
		break;
	}

	params[Params_EDI] = edi;
	params[Params_ReturnAddress] = 0x004AD3CF;
	return 0;
}

int ModelAttackBreakLargeObjectMaxDistanceHook(int* params)
{
	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	int objectIndex = *(int*)0x007CA1A0;
	unsigned short modelIndex = XwaObjects[objectIndex].ModelIndex;
	const ModelAttackBreakSettings& settings = g_modelIndexConfig.GetModelAttackBreak(modelIndex);
	int maxDistance = settings.LargeObject_MaxDistance;

	if (params[Params_EDI] > maxDistance)
	{
		params[Params_EDI] = maxDistance;
	}

	params[Params_ReturnAddress] = 0x004AD45C;
	return 0;
}

int ModelAttackLargeObjectOffsetHook(int* params)
{
	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	int objectIndex = *(int*)0x007CA1A0;
	unsigned short modelIndex = XwaObjects[objectIndex].ModelIndex;
	const ModelAttackBreakSettings& settings = g_modelIndexConfig.GetModelAttackBreak(modelIndex);
	unsigned short targetModelIndex = (unsigned short)params[Params_ESI];

	if (targetModelIndex >= 0 && targetModelIndex < 557)
	{
		params[Params_EDI] += settings.Model_Offsets[targetModelIndex];
	}

	params[Params_ReturnAddress] = 0x004AD48E;
	return 0;
}

int ModelAttackMaxRangeHook(int* params)
{
	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	int objectIndex = *(int*)0x007CA1A0;
	unsigned short modelIndex = XwaObjects[objectIndex].ModelIndex;
	const ModelAttackBreakSettings& settings = g_modelIndexConfig.GetModelAttackBreak(modelIndex);

	int fg = *(short*)0x007CA1BC;
	AIRankEnum rank = (AIRankEnum)(*(unsigned char*)(0x0080DCF2 + fg * 0xE42));

	int range = settings.MaxRange_Novice;
	switch (rank)
	{
	case AIRank_Novice:
		range = settings.MaxRange_Novice;
		break;
	case AIRank_Officer:
		range = settings.MaxRange_Officer;
		break;
	case AIRank_Veteran:
		range = settings.MaxRange_Veteran;
		break;
	case AIRank_Ace:
		range = settings.MaxRange_Ace;
		break;
	case AIRank_TopAce:
		range = settings.MaxRange_TopAce;
		break;
	case AIRank_SuperAce:
		range = settings.MaxRange_SuperAce;
		break;
	}

	params[Params_EBP] = range;
	return 0;
}
