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
		}
	}

	std::map<int, WarheadLockSettings> _warheadLockSettings;
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
