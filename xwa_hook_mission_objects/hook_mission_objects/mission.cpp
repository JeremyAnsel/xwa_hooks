#include "targetver.h"
#include "mission.h"
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

class Config
{
public:
	Config()
	{
		auto lines = GetFileLines("hook_mission_objects.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_mission_objects");
		}

		this->UnlimitedTurretLaser = GetFileKeyValueInt(lines, "UnlimitedTurretLaser", 1) != 0;
	}

	bool UnlimitedTurretLaser;
};

Config g_config;

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

enum HardpointTypeEnum : unsigned int
{
	HardpointType_None = 0,
	HardpointType_RebelLaser = 1,
	HardpointType_TurboRebelLaser = 2,
	HardpointType_EmpireLaser = 3,
	HardpointType_TurboEmpireLaser = 4,
	HardpointType_IonCannon = 5,
	HardpointType_TurboIonCannon = 6,
	HardpointType_Torpedo = 7,
	HardpointType_Missile = 8,
	HardpointType_SuperRebelLaser = 9,
	HardpointType_SuperEmpireLaser = 10,
	HardpointType_SuperIonCannon = 11,
	HardpointType_SuperTorpedo = 12,
	HardpointType_SuperMissile = 13,
	HardpointType_DumbBomb = 14,
	HardpointType_FiredBomb = 15,
	HardpointType_MagPulse = 16,
	HardpointType_TurboMagPulse = 17,
	HardpointType_SuperMagPulse = 18,
	HardpointType_Gunner = 19,
	HardpointType_CockpitSparks = 20,
	HardpointType_DockingPoint = 21,
	HardpointType_Towing = 22,
	HardpointType_AccStart = 23,
	HardpointType_AccEnd = 24,
	HardpointType_InsideHangar = 25,
	HardpointType_OutsideHangar = 26,
	HardpointType_DockFromBig = 27,
	HardpointType_DockFromSmall = 28,
	HardpointType_DockToBig = 29,
	HardpointType_DockToSmall = 30,
	HardpointType_Cockpit = 31,
	HardpointType_EngineGlow = 32,
	HardpointType_Unknown1 = 33,
	HardpointType_Unknown2 = 34,
	HardpointType_Unknown3 = 35,
	HardpointType_Unknown4 = 36,
	HardpointType_Unknown5 = 37,
	HardpointType_Unknown6 = 38,
	HardpointType_JammingPoint = 39,
};

#pragma pack(push, 1)

struct ExeEnableEntry
{
	char unk00[8];
	void* pData1;
	void* pData2;
	char unk10[2];
	short CraftIndex;
	char unk14[4];
};

static_assert(sizeof(ExeEnableEntry) == 24, "size of ExeEnableEntry must be 24");

struct ExeCraftEntry
{
	char Unk0000[306];
	unsigned short LaserTypeId[3];
	char Unk0138[9];
	unsigned char LaserSequence[3];
	char Unk0144[244];
	short CockpitPositionY;
	short CockpitPositionZ;
	short CockpitPositionX;
	short TurretPositionX[2];
	short TurretPositionZ[2];
	short TurretPositionY[2];
	unsigned short TurretOrientationX[2];
	unsigned short TurretOrientationY[2];
	short TurretOptModelId[2];
	unsigned short TurretArcX[2];
	unsigned short TurretArcY[2];
	char Unk025E[381];
};

static_assert(sizeof(ExeCraftEntry) == 987, "size of ExeCraftEntry must be 987");

struct OptModelMeshesInfo
{
	int NumOfMeshes;
	int MeshesType[50];
	void* MeshesDescriptor[50];
};
static_assert(sizeof(OptModelMeshesInfo) == 404, "size of S0x08D9760 must be 404");

struct XwaCraftWeaponRack
{
	char Unk0000[2];
	char Sequence;
	char Unk0003[7];
	short ObjectIndex;
	char Unk000C[2];
};

static_assert(sizeof(XwaCraftWeaponRack) == 14, "size of XwaCraftWeaponRack must be 14");

struct XwaCraft
{
	char Unk0000[4];
	short CraftIndex;
	char Unk0006[136];
	short PickedUpObjectIndex;
	char Unk0090[286];
	char WeaponRacksCount;
	char Unk01AF[7];
	char XwaCraft_m1B6[3];
	char Unk01B9[117];
	char XwaCraft_m22E[50];
	char XwaCraft_m260[50];
	char XwaCraft_m292[50];
	char Unk02C4[27];
	XwaCraftWeaponRack WeaponRacks[16];
	char Unk03BF[2];
	short m03C1[2];
	short m03C5[2];
	float m03C9[2];
	float m03D1[2];
	char Unk03D9[32];
};

static_assert(sizeof(XwaCraft) == 1017, "size of XwaCraft must be 1017");

struct XwaMobileObject
{
	char Unk0000[147];
	short ObjectIndex;
	unsigned short ModelIndex;
	unsigned char Iff;
	char Unk0098[1];
	unsigned char Markings;
	char Unk009A[67];
	XwaCraft* pCraft;
	char Unk00E1[4];
};

static_assert(sizeof(XwaMobileObject) == 229, "size of XwaMobileObject must be 229");

struct XwaObject
{
	char Unk0000[2];
	unsigned short ModelIndex;
	char Unk0004[1];
	unsigned char TieFlightGroupIndex;
	char Unk0006[25];
	int PlayerIndex;
	XwaMobileObject* pMobileObject;
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

struct XwaPlayer
{
	int ObjectIndex;
	char Unk0004[10];
	short XwaPlayer_m00E;
	char Unk0010[1];
	char XwaPlayer_m011;
	char Unk0012[1];
	char XwaPlayer_m013;
	char Unk0014[1];
	char XwaPlayer_m015;
	char Unk0016[29];
	char XwaPlayer_m033;
	char XwaPlayer_m034;
	char Unk0035[36];
	short XwaPlayer_m059;
	short XwaPlayer_m05B;
	short XwaPlayer_m05D;
	char Unk005F[397];
	int XwaPlayer_m1EC;
	char Unk01F0[12];
	char XwaPlayer_m1FC;
	short MousePositionX;
	short MousePositionY;
	char Unk0201[24];
	short TurretIndex;
	short XwaPlayer_m21B;
	char Unk021D[2481];
	unsigned char InTraining;
};

static_assert(sizeof(XwaPlayer) == 3023, "size of XwaPlayer must be 3023");

#pragma pack(pop)

struct TurretData
{
	short PositionX;
	short PositionY;
	short PositionZ;
	unsigned short OrientationX;
	unsigned short OrientationY;
	unsigned short ArcX;
	unsigned short ArcY;
	std::string OptModel;
	bool IsInverted;
};

struct CraftData
{
	short m3C1;
	short m3C5;
	float m3C9;
	float m3D1;
};

std::string GetPathFileName(const std::string& str)
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

		if (!_lines.size())
		{
			const std::string path = "FlightModels\\";
			_lines = GetFileLines(path + name + ".txt");
		}

		if (!_lines.size())
		{
			_lines = GetFileLines("FlightModels\\default.ini", name);
		}
	}

	return _lines;
}

std::vector<TurretData> GetTurretDataList(int modelIndex)
{
	std::string shipPath = g_flightModelsList.GetLstLine(modelIndex);

	const auto objectLines = GetCustomFileLines("Objects");
	const std::string objectValue = GetFileKeyValue(objectLines, shipPath + ".opt");

	if (!objectValue.empty() && std::ifstream(objectValue))
	{
		shipPath = GetStringWithoutExtension(objectValue);
	}

	auto lines = GetFileLines(shipPath + "Turrets.txt");

	if (!lines.size())
	{
		lines = GetFileLines(shipPath + ".ini", "Turrets");
	}

	std::vector<TurretData> turrets;

	if (lines.size())
	{
		const auto values = GetFileListValues(lines);

		for (unsigned int i = 0; i < values.size(); i++)
		{
			const auto& value = values[i];

			if (value.size() < 9)
			{
				continue;
			}

			TurretData turret{};
			turret.PositionX = (short)std::stoi(value[0], 0, 0);
			turret.PositionY = (short)std::stoi(value[1], 0, 0);
			turret.PositionZ = (short)std::stoi(value[2], 0, 0);
			turret.OrientationX = (unsigned short)std::stoi(value[3], 0, 0);
			turret.OrientationY = (unsigned short)std::stoi(value[4], 0, 0);
			turret.ArcX = (unsigned short)std::stoi(value[5], 0, 0);
			turret.ArcY = (unsigned short)std::stoi(value[6], 0, 0);
			turret.OptModel = value[7];
			turret.IsInverted = std::stoi(value[8], 0, 0) != 0;

			turrets.push_back(turret);
		}
	}
	else
	{
		const ExeEnableEntry* ExeEnableTable = (ExeEnableEntry*)0x005FB240;
		const ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;

		const short craftIndex = ExeEnableTable[modelIndex].CraftIndex;
		const ExeCraftEntry& craftEntry = ExeCraftTable[craftIndex];

		const std::string shipName = GetPathFileName(shipPath);

		for (unsigned int i = 0; i < 2; i++)
		{
			if (craftEntry.TurretOptModelId[i] == 0)
			{
				continue;
			}

			TurretData turret{};
			turret.PositionX = craftEntry.TurretPositionX[i];
			turret.PositionY = craftEntry.TurretPositionY[i];
			turret.PositionZ = craftEntry.TurretPositionZ[i];
			turret.OrientationX = craftEntry.TurretOrientationX[i];
			turret.OrientationY = craftEntry.TurretOrientationY[i];
			turret.ArcX = craftEntry.TurretArcX[i];
			turret.ArcY = craftEntry.TurretArcY[i];

			switch (i)
			{
			case 0:
			{
				if (std::ifstream(shipPath + "Gunner.opt"))
				{
					turret.OptModel = shipName + "Gunner";
				}
				else
				{
					turret.OptModel = "CorellianTransportGunner";
				}

				break;
			}

			case 1:
			{
				if (std::ifstream(shipPath + "Gunner2.opt"))
				{
					turret.OptModel = shipName + "Gunner2";
				}
				else if (std::ifstream(shipPath + "Gunner.opt"))
				{
					turret.OptModel = shipName + "Gunner";
				}
				else
				{
					turret.OptModel = "CorellianTransportGunner";
				}

				break;
			}
			}

			switch (i)
			{
			case 0:
				turret.IsInverted = false;
				break;

			case 1:
				turret.IsInverted = true;
				break;
			}

			turrets.push_back(turret);
		}
	}

	return turrets;
}

bool GetTurretForwardFire(int modelIndex)
{
	std::string shipPath = g_flightModelsList.GetLstLine(modelIndex);

	const auto objectLines = GetCustomFileLines("Objects");
	const std::string objectValue = GetFileKeyValue(objectLines, shipPath + ".opt");

	if (!objectValue.empty() && std::ifstream(objectValue))
	{
		shipPath = GetStringWithoutExtension(objectValue);
	}

	auto lines = GetFileLines(shipPath + "Turrets.txt");

	if (!lines.size())
	{
		lines = GetFileLines(shipPath + ".ini", "Turrets");
	}

	return GetFileKeyValueInt(lines, "TurretForwardFire", 1) != 0;
}

class ModelIndexTurrets
{
public:
	std::vector<TurretData>& GetTurrets(int modelIndex)
	{
		this->UpdateIfChanged();

		auto it = this->_turrets.find(modelIndex);

		if (it != this->_turrets.end())
		{
			return it->second;
		}
		else
		{
			auto value = GetTurretDataList(modelIndex);
			this->_turrets.insert(std::make_pair(modelIndex, value));
			//return value;
			return this->_turrets.find(modelIndex)->second;
		}
	}

	int GetTurretsCount(int modelIndex)
	{
		return this->GetTurrets(modelIndex).size();
	}

	bool GetForwardFire(int modelIndex)
	{
		this->UpdateIfChanged();

		auto it = this->_turretsForwardFire.find(modelIndex);

		if (it != this->_turretsForwardFire.end())
		{
			return it->second;
		}
		else
		{
			auto value = GetTurretForwardFire(modelIndex);
			this->_turretsForwardFire.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

private:
	void UpdateIfChanged()
	{
		static std::string _mission;

		const char* xwaMissionFileName = (const char*)0x06002E8;

		if (_mission != xwaMissionFileName)
		{
			_mission = xwaMissionFileName;

			this->_turrets.clear();
			this->_turretsForwardFire.clear();
		}
	}

	std::map<int, std::vector<TurretData>> _turrets;
	std::map<int, bool> _turretsForwardFire;
};

ModelIndexTurrets g_modelIndexTurrets;

std::vector<int> GetModelObjectProfileIndices(int modelIndex, const std::string& profile)
{
	std::string shipPath = g_flightModelsList.GetLstLine(modelIndex);

	const auto objectLines = GetCustomFileLines("Objects");
	const std::string objectValue = GetFileKeyValue(objectLines, shipPath + ".opt");

	if (!objectValue.empty() && std::ifstream(objectValue))
	{
		shipPath = GetStringWithoutExtension(objectValue);
	}

	auto lines = GetFileLines(shipPath + "ObjectProfiles.txt");

	if (!lines.size())
	{
		lines = GetFileLines(shipPath + ".ini", "ObjectProfiles");
	}

	const auto values = Tokennize(GetFileKeyValue(lines, profile));
	std::vector<int> indices;

	for (const std::string& value : values)
	{
		int index = std::stoi(value);
		indices.push_back(index);
	}

	return indices;
}

class ModelIndexProfiles
{
public:
	std::vector<int>& GetProfileIndices(int modelIndex, const std::string& profile)
	{
		this->UpdateIfChanged();

		auto it = this->_profiles.find(std::make_pair(modelIndex, profile));

		if (it != this->_profiles.end())
		{
			return it->second;
		}
		else
		{
			auto value = GetModelObjectProfileIndices(modelIndex, profile);
			this->_profiles.insert(std::make_pair(std::make_pair(modelIndex, profile), value));
			//return value;
			return this->_profiles.find(std::make_pair(modelIndex, profile))->second;
		}
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
		}
	}

	std::map<std::pair<int, std::string>, std::vector<int>> _profiles;
};

ModelIndexProfiles g_modelIndexProfiles;

std::vector<std::vector<CraftData>> g_craftsData;

int MissionObjectsHook(int* params)
{
	auto& s_XwaTempString = *(char(*)[256])0x00ABD680;
	s_XwaTempString[0] = 0;
	s_XwaTempString[254] = 0;
	s_XwaTempString[255] = 0;

	const char* argOpt = (char*)params[0];

	const auto OptLoad = (int(*)(const char*))0x004CC940;

	const auto lines = GetCustomFileLines("Objects");
	const std::string value = GetFileKeyValue(lines, argOpt);

	if (!value.empty() && std::ifstream(value))
	{
		return OptLoad(value.c_str());
	}

	return OptLoad(argOpt);
}

int MissionObjectsStatsHook(int* params)
{
	auto& s_XwaTempString = *(char(*)[256])0x00ABD680;
	s_XwaTempString[0] = 0;
	s_XwaTempString[254] = 0;
	s_XwaTempString[255] = 0;

	static bool s_init = false;
	static short s_CockpitPositionX[265]{};
	static short s_CockpitPositionY[265]{};
	static short s_CockpitPositionZ[265]{};

	if (!s_init)
	{
		s_init = true;

		ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;

		for (int i = 0; i < 265; i++)
		{
			s_CockpitPositionX[i] = ExeCraftTable[i].CockpitPositionX;
			s_CockpitPositionY[i] = ExeCraftTable[i].CockpitPositionY;
			s_CockpitPositionZ[i] = ExeCraftTable[i].CockpitPositionZ;
		}
	}

	std::string argOpt = (char*)params[0];
	const int modelIndex = *(int*)0x07FFD6C;

	const auto OptLoad = (int(*)(const char*))0x004CC940;

	ExeEnableEntry* ExeEnableTable = (ExeEnableEntry*)0x005FB240;
	ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;

	const auto objectLines = GetCustomFileLines("Objects");
	const std::string argObjectValue = GetFileKeyValue(objectLines, argOpt);

	if (!argObjectValue.empty() && std::ifstream(argObjectValue))
	{
		argOpt = argObjectValue;
	}

	std::string objectPath = g_flightModelsList.GetLstLine(modelIndex) + ".opt";
	std::string objectValue = GetFileKeyValue(objectLines, objectPath);

	if (objectValue.empty() || !std::ifstream(objectValue))
	{
		objectValue = objectPath;
	}

	const short craftIndex = ExeEnableTable[modelIndex].CraftIndex;

	if (craftIndex == -1)
	{
		return 0;
	}

	ExeCraftEntry& craftEntry = ExeCraftTable[craftIndex];

	const std::string shipPath = GetStringWithoutExtension(objectValue);

	auto lines = GetFileLines(shipPath + "CockpitPov.txt");

	if (!lines.size())
	{
		lines = GetFileLines(shipPath + ".ini", "CockpitPov");
	}

	std::string povProfile = GetFileKeyValue(objectLines, shipPath + "_CockpitPovProfile");

	if (povProfile.empty())
	{
		povProfile = GetFileKeyValue(objectLines, "CockpitPovProfile");

		if (povProfile.empty())
		{
			povProfile = "Default";
		}
	}

	std::string povProfileSection = std::string("CockpitPov_") + povProfile;
	auto povProfileLines = GetFileLines(shipPath + povProfileSection + ".txt");

	if (!povProfileLines.size())
	{
		povProfileLines = GetFileLines(shipPath + ".ini", povProfileSection);
	}

	if (!GetFileKeyValue(povProfileLines, "CockpitPovX").empty() && !GetFileKeyValue(povProfileLines, "CockpitPovY").empty() && !GetFileKeyValue(povProfileLines, "CockpitPovZ").empty())
	{
		craftEntry.CockpitPositionX = (short)GetFileKeyValueInt(povProfileLines, "CockpitPovX", 0);
		craftEntry.CockpitPositionY = (short)GetFileKeyValueInt(povProfileLines, "CockpitPovY", 0);
		craftEntry.CockpitPositionZ = (short)GetFileKeyValueInt(povProfileLines, "CockpitPovZ", 0);
	}
	else if (!GetFileKeyValue(lines, "CockpitPovX").empty() && !GetFileKeyValue(lines, "CockpitPovY").empty() && !GetFileKeyValue(lines, "CockpitPovZ").empty())
	{
		craftEntry.CockpitPositionX = (short)GetFileKeyValueInt(lines, "CockpitPovX", 0);
		craftEntry.CockpitPositionY = (short)GetFileKeyValueInt(lines, "CockpitPovY", 0);
		craftEntry.CockpitPositionZ = (short)GetFileKeyValueInt(lines, "CockpitPovZ", 0);
	}
	else
	{
		craftEntry.CockpitPositionX = s_CockpitPositionX[craftIndex];
		craftEntry.CockpitPositionY = s_CockpitPositionY[craftIndex];
		craftEntry.CockpitPositionZ = s_CockpitPositionZ[craftIndex];
	}

	return OptLoad(argOpt.c_str());
}

void TurretOptReload(int gunnerModelIndex, int playerModelIndex, int turretIndex)
{
	auto& s_XwaTempString = *(char(*)[256])0x00ABD680;
	s_XwaTempString[0] = 0;
	s_XwaTempString[254] = 0;
	s_XwaTempString[255] = 0;

	const auto OptUnload = (void(*)(unsigned short))0x004CCA60;
	const auto OptLoad = (short(*)(const char*))0x004CC940;
	const auto Lock_Handle = (void* (*)(short))0x0050E2F0;
	const auto L004328B0 = (void(*)(int, int))0x004328B0;
	const auto OptGetNumOfMeshes = (int(*)(int))0x00485190;
	const auto OptGetMeshType = (int(*)(int, int))0x00485A70;
	const auto OptGetMeshDescriptor = (void* (*)(int, int))0x004859B0;

	int& s_V0x077333C = *(int*)0x0077333C;
	unsigned short* OptModelFileMemHandles = (unsigned short*)0x007CA6E0;
	ExeEnableEntry* ExeEnableTable = (ExeEnableEntry*)0x005FB240;

	if (OptModelFileMemHandles[gunnerModelIndex] != 0)
	{
		OptUnload(OptModelFileMemHandles[gunnerModelIndex]);

		OptModelFileMemHandles[gunnerModelIndex] = 0;
		ExeEnableTable[gunnerModelIndex].pData1 = nullptr;
		ExeEnableTable[gunnerModelIndex].pData2 = nullptr;
	}

	std::string ship;

	if (turretIndex == 0)
	{
		ship = "FlightModels\\CorellianTransportGunner.opt";
	}
	else
	{
		const TurretData& turret = g_modelIndexTurrets.GetTurrets(playerModelIndex)[turretIndex - 1];

		ship = "FlightModels\\" + turret.OptModel + ".opt";

		if (!std::ifstream(ship))
		{
			ship = "FlightModels\\CorellianTransportGunner.opt";
		}
	}

	s_V0x077333C = 1;

	short handle = OptLoad(ship.c_str());
	ExeEnableTable[gunnerModelIndex].pData1 = Lock_Handle(handle);
	OptModelFileMemHandles[gunnerModelIndex] = handle;

	L004328B0(ExeEnableTable[gunnerModelIndex].CraftIndex, gunnerModelIndex);

	if (g_craftConfig.MeshesCount == 0)
	{
		OptModelMeshesInfo* OptModelMeshesInfos = (OptModelMeshesInfo*)0x008D9760;

		int numMeshes = OptGetNumOfMeshes(gunnerModelIndex);
		OptModelMeshesInfos[gunnerModelIndex].NumOfMeshes = numMeshes;

		for (int i = 0; i < numMeshes; i++)
		{
			OptModelMeshesInfos[gunnerModelIndex].MeshesType[i] = OptGetMeshType(gunnerModelIndex, i);
			OptModelMeshesInfos[gunnerModelIndex].MeshesDescriptor[i] = OptGetMeshDescriptor(gunnerModelIndex, i);
		}
	}
	else
	{
		int OptModelMeshesInfosPtr = *(int*)0x008D9760;
		OptModelMeshesInfosPtr += gunnerModelIndex * (4 + g_craftConfig.MeshesCount * 4 * 2);
		int* NumOfMeshesPtr = (int*)(OptModelMeshesInfosPtr + 0);
		int* MeshesTypePtr = (int*)(OptModelMeshesInfosPtr + 4);
		void** MeshesDescriptorPtr = (void**)(OptModelMeshesInfosPtr + 4 + g_craftConfig.MeshesCount * 4);

		int numMeshes = OptGetNumOfMeshes(gunnerModelIndex);
		*NumOfMeshesPtr = numMeshes;

		for (int i = 0; i < numMeshes; i++)
		{
			MeshesTypePtr[i] = OptGetMeshType(gunnerModelIndex, i);
			MeshesDescriptorPtr[i] = OptGetMeshDescriptor(gunnerModelIndex, i);
		}
	}

	s_V0x077333C = 0;
}

int CraftTurretHook(int* params)
{
	const int arg4 = params[1];
	const int arg8 = params[2];

	const auto L004201E0 = (int(*)(int, int, int))0x004201E0;
	const auto L00497D40 = (void(*)(int, int))0x00497D40;

	const short CorellianTransportGunnerModelIndex = 317;
	XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	const int XwaCurrentPlayerId = *(int*)0x008C1CC8;
	char& V0x077129C = *(char*)0x0077129C;
	const int V0x07827E4 = *(int*)0x007827E4;

	int playerObjectIndex = XwaPlayers[arg4].ObjectIndex;

	XwaCraft* playerCraft = nullptr;
	short playerCraftIndex = 0;
	int playerModelIndex = 0;

	if (playerObjectIndex != 0xFFFF)
	{
		playerCraft = XwaObjects[playerObjectIndex].pMobileObject->pCraft;
		playerCraftIndex = playerCraft->CraftIndex;
		playerModelIndex = XwaObjects[playerObjectIndex].ModelIndex;
	}

	if (V0x07827E4 == 0 && playerObjectIndex != 0xFFFF)
	{
		if (XwaPlayers[arg4].XwaPlayer_m1FC != 0)
		{
			XwaPlayers[arg4].XwaPlayer_m1FC = 0;
		}

		int turretsCount = g_modelIndexTurrets.GetTurretsCount(playerModelIndex);

		if (turretsCount != 0)
		{
			bool call_L004201E0 = false;

			if (XwaPlayers[arg4].InTraining != 0)
			{
				XwaPlayers[arg4].TurretIndex++;

				if (XwaPlayers[arg4].TurretIndex > turretsCount)
				{
					XwaPlayers[arg4].TurretIndex = 1;
				}

				call_L004201E0 = true;
			}
			else
			{
				int ecx = -1;

				for (int index = 0; index < 8; index++)
				{
					if (index == arg4)
						continue;

					if (XwaPlayers[index].XwaPlayer_m011 == 0)
						continue;

					if (XwaPlayers[index].InTraining == 0)
						continue;

					if (XwaPlayers[index].XwaPlayer_m00E == XwaPlayers[arg4].XwaPlayer_m00E)
					{
						ecx = index;
						break;
					}
				}

				if (ecx == -1)
				{
					XwaPlayers[arg4].TurretIndex++;

					if (arg8 != 0 || XwaPlayers[arg4].TurretIndex > turretsCount)
					{
						XwaPlayers[arg4].TurretIndex = 0;
					}

					call_L004201E0 = true;
				}
			}

			TurretOptReload(CorellianTransportGunnerModelIndex, playerModelIndex, XwaPlayers[arg4].TurretIndex);

			if (call_L004201E0)
			{
				L004201E0(arg4, playerObjectIndex, 0);
			}
		}

		if (turretsCount != 0)
		{
			if (XwaPlayers[arg4].TurretIndex != 0)
			{
				XwaPlayers[arg4].XwaPlayer_m21B = 1;

				// MSG_GUNNER
				L00497D40(519, arg4);
			}
			else
			{
				XwaPlayers[arg4].XwaPlayer_m21B = 0;

				// MSG_PILOTING
				L00497D40(520, arg4);
			}

			for (int index = 0; index < playerCraft->WeaponRacksCount; index++)
			{
				if (playerCraft->WeaponRacks[index].Sequence >= 4)
				{
					playerCraft->WeaponRacks[index].ObjectIndex = -1;
				}
			}
		}
	}

	/*if (XwaPlayers[arg4].TurretIndex > 0)
	{
		XwaPlayers[arg4].XwaPlayer_m033 = 0;
		XwaPlayers[arg4].XwaPlayer_m034 = 0;
		XwaPlayers[arg4].MousePositionX = 0;
		XwaPlayers[arg4].MousePositionY = 0;

		if (arg4 == XwaCurrentPlayerId)
		{
			V0x077129C = 0;
		}
	}*/

	return 0;
}

bool CheckOptHardpoint(short modelIndex, int weaponType, int weaponIndex)
{
	const auto XwaOptGetMeshesCount = (int(*)(short))0x00488960;
	const auto XwaOptGetMeshHardpointsCount = (int(*)(short, int))0x00486DC0;
	const auto XwaOptRetrieveMeshHardpoint = (void(*)(short, int, int, HardpointTypeEnum*, int*, int*, int*))0x00487180;

	int meshesCount = XwaOptGetMeshesCount(modelIndex);
	for (int mesh = 0; mesh < meshesCount; mesh++)
	{
		int hardpointsCount = XwaOptGetMeshHardpointsCount(modelIndex, mesh);
		for (int hardpoint = 0; hardpoint < hardpointsCount; hardpoint++)
		{
			HardpointTypeEnum type;
			int positionX;
			int positionY;
			int positionZ;
			XwaOptRetrieveMeshHardpoint(modelIndex, mesh, hardpoint, &type, &positionX, &positionY, &positionZ);

			switch (type)
			{
			case HardpointType_RebelLaser:
			case HardpointType_TurboRebelLaser:
			case HardpointType_EmpireLaser:
			case HardpointType_TurboEmpireLaser:
			case HardpointType_SuperRebelLaser:
			case HardpointType_SuperEmpireLaser:
			{
				if (weaponType == 0 && weaponIndex == 0)
				{
					return true;
				}

				break;
			}

			case HardpointType_IonCannon:
			case HardpointType_TurboIonCannon:
			case HardpointType_SuperIonCannon:
			{
				if (weaponType == 0 && weaponIndex == 1)
				{
					return true;
				}

				break;
			}

			case HardpointType_Torpedo:
			case HardpointType_Missile:
			case HardpointType_SuperTorpedo:
			case HardpointType_SuperMissile:
			case HardpointType_DumbBomb:
			case HardpointType_FiredBomb:
			case HardpointType_MagPulse:
			case HardpointType_TurboMagPulse:
			case HardpointType_SuperMagPulse:
			{
				if (weaponType == 1)
				{
					return true;
				}

				break;
			}
			}
		}
	}

	return false;
}

int LaserShootHook(int* params)
{
	const int objectIndex = params[0];
	const int laserIndex = params[1];
	const int playerIndex = params[2];
	const int arg10 = params[3];

	const auto L004912C0 = (void(*)(int, int, int, int))0x004912C0;

	const short CorellianTransportGunnerModelIndex = 317;
	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	const ExeEnableEntry* ExeEnableTable = (ExeEnableEntry*)0x005FB240;
	const ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;

	short modelIndex = XwaObjects[objectIndex].ModelIndex;
	int turretIndex = XwaPlayers[XwaObjects[objectIndex].PlayerIndex].TurretIndex;

	if (turretIndex == 0)
	{
		L004912C0(objectIndex, laserIndex, playerIndex, arg10);
	}
	else
	{
		if (!g_config.UnlimitedTurretLaser)
		{
			if (CheckOptHardpoint(CorellianTransportGunnerModelIndex, 0, laserIndex))
			{
				L004912C0(objectIndex, laserIndex, playerIndex, arg10);
			}
		}
		else
		{
			int player_m033 = XwaPlayers[XwaObjects[objectIndex].PlayerIndex].XwaPlayer_m033;
			short craftIndex = ExeEnableTable[modelIndex].CraftIndex;

			int laserType = -1;
			int laserSlot;

			if (player_m033 == 0)
			{
				for (laserSlot = 0; laserSlot < 3; laserSlot++)
				{
					if (ExeCraftTable[craftIndex].LaserSequence[laserSlot] == 4)
					{
						laserType = 0;
						break;
					}
				}
			}
			else if (player_m033 == 1)
			{
				for (laserSlot = 0; laserSlot < 3; laserSlot++)
				{
					if (ExeCraftTable[craftIndex].LaserSequence[laserSlot] == 2)
					{
						laserType = 1;
						break;
					}
				}
			}

			if (laserType != -1)
			{
				if (CheckOptHardpoint(CorellianTransportGunnerModelIndex, 0, laserType))
				{
					L004912C0(objectIndex, laserSlot, playerIndex, arg10);
				}
			}
		}
	}

	return 0;
}

int LaserShoot2Hook(int* params)
{
	const int objectIndex = params[0];
	const int laserIndex = params[1];
	const int playerIndex = params[2];
	const int arg10 = params[3];

	const auto L004912C0 = (void(*)(int, int, int, int))0x004912C0;

	const short CorellianTransportGunnerModelIndex = 317;
	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	const ExeEnableEntry* ExeEnableTable = (ExeEnableEntry*)0x005FB240;
	const ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;

	short modelIndex = XwaObjects[objectIndex].ModelIndex;
	int turretIndex = XwaPlayers[XwaObjects[objectIndex].PlayerIndex].TurretIndex;

	if (!g_modelIndexTurrets.GetForwardFire(modelIndex))
	{
		return 0;
	}

	L004912C0(objectIndex, laserIndex, playerIndex, arg10);
	return 0;
}

int WarheadShootHook(int* params)
{
	const int objectIndex = params[0];
	const int warheadIndex = params[1];

	const auto L004918F0 = (void(*)(int, int))0x004918F0;

	const short CorellianTransportGunnerModelIndex = 317;
	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;

	short modelIndex = XwaObjects[objectIndex].ModelIndex;
	int turretIndex = XwaPlayers[XwaObjects[objectIndex].PlayerIndex].TurretIndex;

	if (turretIndex == 0)
	{
		L004918F0(objectIndex, warheadIndex);
	}
	else
	{
		if (CheckOptHardpoint(CorellianTransportGunnerModelIndex, 1, warheadIndex))
		{
			L004918F0(objectIndex, warheadIndex);
		}
	}

	return 0;
}

int SetXwaCraftsHook(int* params)
{
	const int handle = params[0];

	const auto Lock_Handle = (int(*)(short))0x0050E2F0;

	const int craftsCount = *(int*)0x008C1CE4;

	g_craftsData.clear();
	g_craftsData.resize(craftsCount);

	return Lock_Handle(handle);
}

int ClearCraftDataHook(int* params)
{
	const int s_XwaCrafts = *(int*)0x009106A0;
	XwaCraft* s_pXwaCurrentCraft = *(XwaCraft**)0x00910DFC;
	const short modelIndex = *(short*)0x009E96F2;

	for (int i = 0; i < 2; i++)
	{
		s_pXwaCurrentCraft->m03C1[i] = 0;
		s_pXwaCurrentCraft->m03C5[i] = 0;
		s_pXwaCurrentCraft->m03C9[i] = 0;
		s_pXwaCurrentCraft->m03D1[i] = 0;
	}

	int index = ((int)s_pXwaCurrentCraft - s_XwaCrafts) / g_craftConfig.Craft_Size;

	if ((unsigned int)index < g_craftsData.size())
	{
		int count = g_modelIndexTurrets.GetTurretsCount(modelIndex);

		g_craftsData[index].clear();
		g_craftsData[index].resize(count, {});
	}

	return 0;
}

//int CloneObjectHook(int* params)
//{
//	int edi = params[0];
//	int esi = params[1];
//
//	const int s_XwaCrafts = *(int*)0x009106A0;
//
//	int ediIndex = (edi - s_XwaCrafts) / g_craftConfig.Craft_Size;
//	int esiIndex = (esi - s_XwaCrafts) / g_craftConfig.Craft_Size;
//
//	memcpy((void*)edi, (void*)esi, g_craftConfig.Craft_Size);
//
//	if ((unsigned int)ediIndex < g_craftsData.size() && (unsigned int)esiIndex < g_craftsData.size())
//	{
//		g_craftsData[ediIndex] = g_craftsData[esiIndex];
//	}
//
//	return 0;
//}

int CloneObjectHook(int* params)
{
	int A4 = params[5];
	int A8 = params[6];

	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	const int s_XwaCrafts = *(int*)0x009106A0;

	int edi = (int)XwaObjects[A4].pMobileObject->pCraft;
	int esi = (int)XwaObjects[A8].pMobileObject->pCraft;

	if (edi != 0 && esi != 0)
	{
		int ediIndex = (edi - s_XwaCrafts) / g_craftConfig.Craft_Size;
		int esiIndex = (esi - s_XwaCrafts) / g_craftConfig.Craft_Size;

		if ((unsigned int)ediIndex < g_craftsData.size() && (unsigned int)esiIndex < g_craftsData.size())
		{
			g_craftsData[ediIndex] = g_craftsData[esiIndex];
		}
	}

	return A4 * 0x27;
}

int ComputeCraftAnglesHook(int* params)
{
	const int* ebp = (int*)params[0];
	const int ebx = params[1];

	XwaPlayer* s_XwaPlayers = (XwaPlayer*)0x008B94E0;
	const XwaObject* s_XwaObjects = *(XwaObject**)0x007B33C4;
	const int s_XwaCrafts = *(int*)0x009106A0;

	XwaCraft* craft = (XwaCraft*)ebp[-5];
	int craftIndex = ((int)craft - s_XwaCrafts) / g_craftConfig.Craft_Size;
	int turretIndex = ebp[-3];
	int playerIndex = ebx / sizeof(XwaPlayer);
	short modelIndex = s_XwaObjects[s_XwaPlayers[playerIndex].ObjectIndex].ModelIndex;

	CraftData& craftData = g_craftsData[craftIndex][turretIndex];
	TurretData turretData = g_modelIndexTurrets.GetTurrets(modelIndex)[turretIndex];

	craftData.m3C9 += s_XwaPlayers[playerIndex].XwaPlayer_m05B * 0.1f - craftData.m3C9 * 0.25f;
	craftData.m3C1 += (short)craftData.m3C9;

	craftData.m3D1 += s_XwaPlayers[playerIndex].XwaPlayer_m059 * 0.1f - craftData.m3D1 * 0.25f;
	craftData.m3C5 += (short)craftData.m3D1;

	if (craftData.m3C1 > turretData.ArcX)
	{
		craftData.m3C1 = turretData.ArcX;
	}
	else if (craftData.m3C1 < -turretData.ArcX)
	{
		craftData.m3C1 = -turretData.ArcX;
	}

	if (craftData.m3C5 > turretData.ArcY)
	{
		craftData.m3C5 = turretData.ArcY;
	}
	else if (craftData.m3C5 < -turretData.ArcY)
	{
		craftData.m3C5 = -turretData.ArcY;
	}

	s_XwaPlayers[playerIndex].XwaPlayer_m059 = 0;
	s_XwaPlayers[playerIndex].XwaPlayer_m05B = 0;
	s_XwaPlayers[playerIndex].XwaPlayer_m05D = 0;

	return 0;
}

int SetRotationAngle1Hook(int* params)
{
	const XwaCraft* craft = (XwaCraft*)params[0];
	int& angle = params[0];

	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const int XwaCrafts = *(int*)0x009106A0;
	const int XwaCurrentPlayerId = *(int*)0x008C1CC8;

	int craftIndex = ((int)craft - XwaCrafts) / g_craftConfig.Craft_Size;
	int turretIndex = XwaPlayers[XwaCurrentPlayerId].TurretIndex - 1;

	angle = g_craftsData[craftIndex][turretIndex].m3C1;

	return 0;
}

int SetRotationAngle2Hook(int* params)
{
	const XwaCraft* craft = (XwaCraft*)params[0];
	int& angle = params[0];

	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const int XwaCrafts = *(int*)0x009106A0;
	const int XwaCurrentPlayerId = *(int*)0x008C1CC8;

	int craftIndex = ((int)craft - XwaCrafts) / g_craftConfig.Craft_Size;
	int turretIndex = XwaPlayers[XwaCurrentPlayerId].TurretIndex - 1;

	angle = g_craftsData[craftIndex][turretIndex].m3C5;

	return 0;
}

int SetL00491EB0Angle1Hook(int* params)
{
	const int* ebp = (int*)params[0];
	int& angle = params[0];

	int playerIndex = ebp[5];

	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const int XwaCrafts = *(int*)0x009106A0;
	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;

	XwaCraft* craft = XwaObjects[XwaPlayers[playerIndex].ObjectIndex].pMobileObject->pCraft;
	int craftIndex = ((int)craft - XwaCrafts) / g_craftConfig.Craft_Size;
	int turretIndex = XwaPlayers[playerIndex].TurretIndex - 1;

	angle = g_craftsData[craftIndex][turretIndex].m3C1;

	return 0;
}

int SetL00491EB0Angle2Hook(int* params)
{
	const int* ebp = (int*)params[0];
	int& angle = params[0];

	int playerIndex = ebp[5];

	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const int XwaCrafts = *(int*)0x009106A0;
	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;

	XwaCraft* craft = XwaObjects[XwaPlayers[playerIndex].ObjectIndex].pMobileObject->pCraft;
	int craftIndex = ((int)craft - XwaCrafts) / g_craftConfig.Craft_Size;
	int turretIndex = XwaPlayers[playerIndex].TurretIndex - 1;

	angle = g_craftsData[craftIndex][turretIndex].m3C5;

	return 0;
}

int SetL00497610Angle1Hook(int* params)
{
	const int playerIndex = params[34] / sizeof(XwaPlayer);
	const XwaCraft* craft = (XwaCraft*)params[0];
	int& angle = params[0];

	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const int XwaCrafts = *(int*)0x009106A0;

	int craftIndex = ((int)craft - XwaCrafts) / g_craftConfig.Craft_Size;
	int turretIndex = XwaPlayers[playerIndex].TurretIndex - 1;

	angle = g_craftsData[craftIndex][turretIndex].m3C1;

	return 0;
}

int SetL00497610Angle2Hook(int* params)
{
	const int playerIndex = params[33] / sizeof(XwaPlayer);
	const XwaCraft* craft = (XwaCraft*)params[0];
	int& angle = params[0];

	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const int XwaCrafts = *(int*)0x009106A0;

	int craftIndex = ((int)craft - XwaCrafts) / g_craftConfig.Craft_Size;
	int turretIndex = XwaPlayers[playerIndex].TurretIndex - 1;

	angle = g_craftsData[craftIndex][turretIndex].m3C5;

	return 0;
}

int SetL0040D410Angle1Hook(int* params)
{
	const XwaCraft* craft = (XwaCraft*)params[0];
	int& angle = params[0];

	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const int XwaCrafts = *(int*)0x009106A0;
	const int XwaCurrentPlayerId = *(int*)0x008C1CC8;

	int craftIndex = ((int)craft - XwaCrafts) / g_craftConfig.Craft_Size;
	int turretIndex = XwaPlayers[XwaCurrentPlayerId].TurretIndex - 1;

	angle = g_craftsData[craftIndex][turretIndex].m3C1;

	return 0;
}

int SetL0040D410Angle2Hook(int* params)
{
	const XwaCraft* craft = (XwaCraft*)params[0];
	int& angle = params[0];

	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const int XwaCrafts = *(int*)0x009106A0;
	const int XwaCurrentPlayerId = *(int*)0x008C1CC8;

	int craftIndex = ((int)craft - XwaCrafts) / g_craftConfig.Craft_Size;
	int turretIndex = XwaPlayers[XwaCurrentPlayerId].TurretIndex - 1;

	angle = g_craftsData[craftIndex][turretIndex].m3C5;

	return 0;
}

int SetL0043F8E0Angle1Hook(int* params)
{
	const int playerIndex = params[0] / sizeof(XwaPlayer);
	int& angle = params[0];

	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const int XwaCrafts = *(int*)0x009106A0;
	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;

	int turretIndex = XwaPlayers[playerIndex].TurretIndex - 1;
	XwaCraft* craft = XwaObjects[XwaPlayers[playerIndex].ObjectIndex].pMobileObject->pCraft;
	int craftIndex = ((int)craft - XwaCrafts) / g_craftConfig.Craft_Size;

	angle = g_craftsData[craftIndex][turretIndex].m3C1;

	return 0;
}

int SetL0043F8E0Angle2Hook(int* params)
{
	const int playerIndex = params[0] / sizeof(XwaPlayer);
	int& angle = params[0];

	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const int XwaCrafts = *(int*)0x009106A0;
	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;

	int turretIndex = XwaPlayers[playerIndex].TurretIndex - 1;
	XwaCraft* craft = XwaObjects[XwaPlayers[playerIndex].ObjectIndex].pMobileObject->pCraft;
	int craftIndex = ((int)craft - XwaCrafts) / g_craftConfig.Craft_Size;

	angle = g_craftsData[craftIndex][turretIndex].m3C5;

	return 0;
}

int ReadTurretPositionYHook(int* params)
{
	const int playerIndex = params[32] / sizeof(XwaPlayer);
	const int craftIndex = params[0] / sizeof(ExeCraftEntry);
	int& position = params[0];

	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;

	int turretIndex = XwaPlayers[playerIndex].TurretIndex - 1;
	int modelIndex = XwaObjects[XwaPlayers[playerIndex].ObjectIndex].ModelIndex;

	position = g_modelIndexTurrets.GetTurrets(modelIndex)[turretIndex].PositionY;

	return 0;
}

int ReadTurretPositionXHook(int* params)
{
	const int playerIndex = params[32] / sizeof(XwaPlayer);
	const int craftIndex = params[0] / sizeof(ExeCraftEntry);
	int& position = params[0];

	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;

	int turretIndex = XwaPlayers[playerIndex].TurretIndex - 1;
	int modelIndex = XwaObjects[XwaPlayers[playerIndex].ObjectIndex].ModelIndex;

	position = g_modelIndexTurrets.GetTurrets(modelIndex)[turretIndex].PositionX;

	return 0;
}

int ReadTurretPositionZHook(int* params)
{
	const int playerIndex = params[32] / sizeof(XwaPlayer);
	const int craftIndex = params[0] / sizeof(ExeCraftEntry);
	int& position = params[0];

	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;

	int turretIndex = XwaPlayers[playerIndex].TurretIndex - 1;
	int modelIndex = XwaObjects[XwaPlayers[playerIndex].ObjectIndex].ModelIndex;

	position = g_modelIndexTurrets.GetTurrets(modelIndex)[turretIndex].PositionZ;

	return 0;
}

int ReadTurretOrientationHook(int* params)
{
	const int playerIndex = params[0] / sizeof(XwaPlayer);
	int& orientationX = params[0];
	int& orientationY = params[1];

	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;

	int turretIndex = XwaPlayers[playerIndex].TurretIndex - 1;
	int modelIndex = XwaObjects[XwaPlayers[playerIndex].ObjectIndex].ModelIndex;

	const TurretData& turret = g_modelIndexTurrets.GetTurrets(modelIndex)[turretIndex];

	orientationX = turret.OrientationX;
	orientationY = turret.OrientationY;

	return 0;
}

int ReadL00507B20HasTurretHook(int* params)
{
	const int modelIndex = (unsigned short)params[0];

	int count = g_modelIndexTurrets.GetTurretsCount(modelIndex);

	return count == 0 ? 0 : 1;
}

int ReadL004C3EF0HasTurretHook(int* params)
{
	const int playerIndex = (short)params[4];

	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;

	int objectIndex = XwaPlayers[playerIndex].XwaPlayer_m015 != 0 ? XwaPlayers[playerIndex].XwaPlayer_m1EC : XwaPlayers[playerIndex].ObjectIndex;
	int modelIndex = XwaObjects[objectIndex].ModelIndex;

	int count = g_modelIndexTurrets.GetTurretsCount(modelIndex);

	return count == 0 ? 0 : 1;
}

int TurretIndex2TransformHook(int* params)
{
	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	const int XwaCurrentPlayerId = *(int*)0x008C1CC8;

	int turretIndex = XwaPlayers[XwaCurrentPlayerId].TurretIndex - 1;
	int modelIndex = XwaObjects[XwaPlayers[XwaCurrentPlayerId].ObjectIndex].ModelIndex;

	if (turretIndex == -1)
	{
		return 1;
	}

	const TurretData& turret = g_modelIndexTurrets.GetTurrets(modelIndex)[turretIndex];

	return turret.IsInverted ? 0 : 1;
}

int TurretIndex2BlockedHook(int* params)
{
	const int playerIndex = params[0];

	const auto XwaShowMessageLine = (void(*)(int, int))0x00497D40;

	const int StringsMessageLine_MSG_GUNNER_BLOCKED = 530;
	const int StringsMessageLine_MSG_GUNNER_DISABLED = 531;

	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;

	const XwaPlayer* player = &XwaPlayers[playerIndex];
	int objectIndex = player->ObjectIndex;
	params[0] = objectIndex;

	if (objectIndex == 0xFFFF)
	{
		return 0;
	}

	const XwaCraft* craft = XwaObjects[objectIndex].pMobileObject->pCraft;
	int turretIndex = player->TurretIndex - 1;
	int modelIndex = XwaObjects[objectIndex].ModelIndex;

	if (turretIndex != -1)
	{
		const TurretData& turret = g_modelIndexTurrets.GetTurrets(modelIndex)[turretIndex];
		unsigned short orientation = turret.OrientationY;

		if (orientation > 0xA000 && orientation < 0xE000)
		{
			if (craft->PickedUpObjectIndex != -1)
			{
				XwaShowMessageLine(StringsMessageLine_MSG_GUNNER_BLOCKED, playerIndex);
				return 0;
			}

			if (player->XwaPlayer_m013 != 0 && player->XwaPlayer_m013 != 5)
			{
				XwaShowMessageLine(StringsMessageLine_MSG_GUNNER_DISABLED, playerIndex);
				return 0;
			}
		}
	}

	return 1;
}

void ApplyProfile(short objectIndex, unsigned short modelIndex, unsigned short flightgroupIndex, XwaCraft* s_pXwaCurrentCraft)
{
	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;

	unsigned char markings = 0;

	if (objectIndex != -1 && XwaObjects[objectIndex].pMobileObject != nullptr)
	{
		markings = XwaObjects[objectIndex].pMobileObject->Markings;
	}

	const auto objectLines = GetCustomFileLines("Objects");

	std::string profile = GetFileKeyValue(objectLines, "ObjectProfile_fg_" + std::to_string(flightgroupIndex));

	if (profile.empty())
	{
		profile = "Default_" + std::to_string(markings);
	}

	char* m292;
	char* m22E;

	if (g_craftConfig.MeshesCount == 0)
	{
		m292 = s_pXwaCurrentCraft->XwaCraft_m292;
		m22E = s_pXwaCurrentCraft->XwaCraft_m22E;
	}
	else
	{
		m292 = (char*)((int)s_pXwaCurrentCraft + g_craftConfig.Craft_Offset_292);
		m22E = (char*)((int)s_pXwaCurrentCraft + g_craftConfig.Craft_Offset_22E);
	}

	auto& indices = g_modelIndexProfiles.GetProfileIndices(modelIndex, profile);

	if (indices.empty())
	{
		indices = g_modelIndexProfiles.GetProfileIndices(modelIndex, "Default");
	}

	for (const int index : indices)
	{
		if (index == -1)
		{
			continue;
		}

		m292[index] = 0;
		m22E[index] = 4;
	}
}

int ObjectProfileHook(int* params)
{
	const short objectIndex = (short)params[10];

	const unsigned short modelIndex = *(unsigned short*)0x09E96F2;
	const unsigned short flightgroupIndex = *(unsigned short*)0x09E9708;
	XwaCraft* s_pXwaCurrentCraft = *(XwaCraft**)0x00910DFC;

	ApplyProfile(objectIndex, modelIndex, flightgroupIndex, s_pXwaCurrentCraft);

	return 0;
}

int AddObjectProfileHook(int* params)
{
	const short objectIndex = (short)params[0];

	const auto L004016B0 = (void(*)(int))0x004016B0;

	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	XwaCraft* s_pXwaCurrentCraft = *(XwaCraft**)0x00910DFC;
	unsigned short modelIndex = XwaObjects[objectIndex].ModelIndex;
	unsigned short flightgroupIndex = XwaObjects[objectIndex].TieFlightGroupIndex;

	L004016B0(objectIndex);

	if (s_pXwaCurrentCraft == nullptr)
	{
		return 0;
	}

	ApplyProfile(objectIndex, modelIndex, flightgroupIndex, s_pXwaCurrentCraft);

	return 0;
}

int BriefingObjectProfileHook(int* params)
{
	*(short*)0x0078260F = 0;

	bool s_XwaBriefingIsPlaying = *(short*)0x009EB826 != 0;
	unsigned short modelIndex = *(unsigned short*)0x007827A8;
	unsigned short flightgroupIndex = *(unsigned short*)0x009F4A04;
	XwaCraft* craft = *(XwaCraft**)0x00782705;

	if (!s_XwaBriefingIsPlaying)
	{
		flightgroupIndex = 0xffff;
	}

	ApplyProfile(-1, modelIndex, flightgroupIndex, craft);

	return 0;
}

int RenderOptObjectProfileHook(int* params)
{
	const XwaObject* currentObject = (XwaObject*)params[Params_EBP];
	const XwaCraft* currentCraft = currentObject->pMobileObject->pCraft;

	if (currentCraft == nullptr)
	{
		unsigned short modelIndex = currentObject->ModelIndex;
		unsigned short originModelIndex = currentObject->pMobileObject->ModelIndex;
		int meshIndex = params[Params_EDI] - 1;

		const auto objectLines = GetCustomFileLines("Objects");
		const std::string shipName = GetFileNameWithoutExtension(g_flightModelsList.GetLstLine(originModelIndex));

		std::string profileKey = "ObjectProfile_" + shipName + "_" + std::to_string(modelIndex);
		std::string profile = GetFileKeyValue(objectLines, profileKey);

		if (profile.empty())
		{
			profile = "Default";
		}

		const auto& indices = g_modelIndexProfiles.GetProfileIndices(modelIndex, profile);

		for (const int index : indices)
		{
			if (index == -1)
			{
				continue;
			}

			if (index == meshIndex)
			{
				params[Params_ReturnAddress] = 0x00481609;
				break;
			}
		}
	}

	params[Params_EAX] = (int)currentCraft;
	return 0;
}
