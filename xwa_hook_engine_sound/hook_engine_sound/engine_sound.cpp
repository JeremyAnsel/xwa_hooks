#include "targetver.h"
#include "engine_sound.h"
#include "config.h"
#include "zip_file.hpp"
#include <map>
#include <fstream>

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

class Config
{
public:
	Config()
	{
		auto lines = GetFileLines("hook_engine_sound.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_engine_sound");
		}

		this->AreStarshipAmbientEnabled = GetFileKeyValueInt(lines, "AreStarshipAmbientEnabled", 1) != 0;
	}

	bool AreStarshipAmbientEnabled;
};

Config g_config;

class SoundsConfig
{
public:
	SoundsConfig()
	{
		this->SoundsCountHookExists = std::ifstream("Hook_Sounds_Count.dll") ? true : false;
		this->SoundEffectIds = this->SoundsCountHookExists ? *(int**)0x00917E80 : (int*)0x00917E80;

		std::vector<std::string> lines;

		if (this->SoundsCountHookExists)
		{
			lines = GetFileLines("Hook_Sounds_Count.txt");
		}

		this->SfxInteriorIndex = GetFileKeyValueInt(lines, "sfx_interior_index");
		this->SfxInteriorCount = GetFileKeyValueInt(lines, "sfx_interior_count");
		this->SfxFlyByIndex = GetFileKeyValueInt(lines, "sfx_flyby_index");
		this->SfxFlyByCount = GetFileKeyValueInt(lines, "sfx_flyby_count");
		this->SfxEngineWashIndex = GetFileKeyValueInt(lines, "sfx_enginewash_index");
		this->SfxEngineWashCount = GetFileKeyValueInt(lines, "sfx_enginewash_count");
		this->SfxTakeOffIndex = GetFileKeyValueInt(lines, "sfx_takeoff_index");
		this->SfxTakeOffCount = GetFileKeyValueInt(lines, "sfx_takeoff_count");
		this->SfxEngineSlowingIndex = GetFileKeyValueInt(lines, "sfx_engineslowing_index");
		this->SfxEngineSlowingCount = GetFileKeyValueInt(lines, "sfx_engineslowing_count");
		this->SfxCanopyOpeningIndex = GetFileKeyValueInt(lines, "sfx_canopyopening_index");
		this->SfxCanopyOpeningCount = GetFileKeyValueInt(lines, "sfx_canopyopening_count");
		this->SfxShuttleTakeOffIndex = GetFileKeyValueInt(lines, "sfx_shuttletakeoff_index");
		this->SfxShuttleTakeOffCount = GetFileKeyValueInt(lines, "sfx_shuttletakeoff_count");
		this->SfxShuttleBlastOffIndex = GetFileKeyValueInt(lines, "sfx_shuttleblastoff_index");
		this->SfxShuttleBlastOffCount = GetFileKeyValueInt(lines, "sfx_shuttleblastoff_count");
		this->SfxShuttleShutDownIndex = GetFileKeyValueInt(lines, "sfx_shuttleshutdown_index");
		this->SfxShuttleShutDownCount = GetFileKeyValueInt(lines, "sfx_shuttleshutdown_count");
		this->SfxWeaponIndex = GetFileKeyValueInt(lines, "sfx_weapon_index");
		this->SfxWeaponCount = GetFileKeyValueInt(lines, "sfx_weapon_count");
		this->SfxWeaponRangeIndex = GetFileKeyValueInt(lines, "sfx_weaponrange_index");
		this->SfxWeaponRangeCount = GetFileKeyValueInt(lines, "sfx_weaponrange_count");
		this->SfxWeaponExplosionIndex = GetFileKeyValueInt(lines, "sfx_weaponexplosion_index");
		this->SfxWeaponExplosionCount = GetFileKeyValueInt(lines, "sfx_weaponexplosion_count");
		this->SfxHyperStartIndex = GetFileKeyValueInt(lines, "sfx_hyperstart_index");
		this->SfxHyperStartCount = GetFileKeyValueInt(lines, "sfx_hyperstart_count");
		this->SfxHyperZoomIndex = GetFileKeyValueInt(lines, "sfx_hyperzoom_index");
		this->SfxHyperZoomCount = GetFileKeyValueInt(lines, "sfx_hyperzoom_count");
		this->SfxHyperEndIndex = GetFileKeyValueInt(lines, "sfx_hyperend_index");
		this->SfxHyperEndCount = GetFileKeyValueInt(lines, "sfx_hyperend_count");
		this->SfxHyperAbortIndex = GetFileKeyValueInt(lines, "sfx_hyperabort_index");
		this->SfxHyperAbortCount = GetFileKeyValueInt(lines, "sfx_hyperabort_count");
		this->SfxStarshipAmbientIndex = GetFileKeyValueInt(lines, "sfx_starshipambient_index");
		this->SfxStarshipAmbientCount = GetFileKeyValueInt(lines, "sfx_starshipambient_count");
		this->SfxSoundsIndex = GetFileKeyValueInt(lines, "sfx_sounds_index");
		this->SfxSoundsCount = GetFileKeyValueInt(lines, "sfx_sounds_count");
	}

	bool SoundsCountHookExists;
	int* SoundEffectIds;
	int SfxInteriorIndex;
	int SfxInteriorCount;
	int SfxFlyByIndex;
	int SfxFlyByCount;
	int SfxEngineWashIndex;
	int SfxEngineWashCount;
	int SfxTakeOffIndex;
	int SfxTakeOffCount;
	int SfxEngineSlowingIndex;
	int SfxEngineSlowingCount;
	int SfxCanopyOpeningIndex;
	int SfxCanopyOpeningCount;
	int SfxShuttleTakeOffIndex;
	int SfxShuttleTakeOffCount;
	int SfxShuttleBlastOffIndex;
	int SfxShuttleBlastOffCount;
	int SfxShuttleShutDownIndex;
	int SfxShuttleShutDownCount;
	int SfxWeaponIndex;
	int SfxWeaponCount;
	int SfxWeaponRangeIndex;
	int SfxWeaponRangeCount;
	int SfxWeaponExplosionIndex;
	int SfxWeaponExplosionCount;
	int SfxHyperStartIndex;
	int SfxHyperStartCount;
	int SfxHyperZoomIndex;
	int SfxHyperZoomCount;
	int SfxHyperEndIndex;
	int SfxHyperEndCount;
	int SfxHyperAbortIndex;
	int SfxHyperAbortCount;
	int SfxStarshipAmbientIndex;
	int SfxStarshipAmbientCount;
	int SfxSoundsIndex;
	int SfxSoundsCount;
};

SoundsConfig& GetSoundsConfig()
{
	static SoundsConfig g_soundsConfig;
	return g_soundsConfig;
}

enum SfxSounds
{
	SfxSounds_PlayerCraftTargetedSound,
	SfxSounds_MapEnterSound,
	SfxSounds_MapExitSound,
	SfxSounds_TurretSwitchSound,
	SfxSounds_TargetSelectSound,
};

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
	unsigned char EnableOptions; // flags
	unsigned char RessourceOptions; // flags
	unsigned char ObjectCategory;
	unsigned char ShipCategory;
	unsigned int ObjectSize;
	void* pData1;
	void* pData2;
	unsigned short GameOptions; // flags
	unsigned short CraftIndex;
	short DataIndex1;
	short DataIndex2;
};

static_assert(sizeof(ExeEnableEntry) == 24, "size of ExeEnableEntry must be 24");

struct XwaMobileObject
{
	char Unk0000[147];
	short ObjectIndex;
	unsigned short ModelIndex;
	char Unk0097[40];
	bool RecalculateForwardVector;
	short ForwardX;
	short ForwardY;
	short ForwardZ;
	bool RecalculateTransformMatrix;
	short TransformMatrixFrontX;
	short TransformMatrixFrontY;
	short TransformMatrixFrontZ;
	short TransformMatrixRightX;
	short TransformMatrixRightY;
	short TransformMatrixRightZ;
	short TransformMatrixUpX;
	short TransformMatrixUpY;
	short TransformMatrixUpZ;
	int pWarhead;
	int pCraft;
	int pChar;
};

static_assert(sizeof(XwaMobileObject) == 229, "size of XwaMobileObject must be 229");

struct XwaObject
{
	char Unk0000[2];
	unsigned short ModelIndex;
	unsigned char ShipCategory;
	unsigned char TieFlightGroupIndex;
	unsigned char Region;
	int PositionX;
	int PositionY;
	int PositionZ;
	short HeadingXY;
	short HeadingZ;
	short HeadingRoll;
	char Unk0019[6];
	int PlayerIndex;
	XwaMobileObject* pMobileObject;
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

struct XwaPlayer
{
	int ObjectIndex;
	char Unk0004[6];
	short Iff;
	char Unk000C[4];
	unsigned char Region;
	char Unk0011[4];
	unsigned char MapState;
	char Unk0016[61];
	short m053;
	char Unk0055[407];
	int AiObjectIndex;
	char Unk01F0[2527];
};

static_assert(sizeof(XwaPlayer) == 3023, "size of XwaPlayer must be 3023");

#pragma pack(pop)

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

int GetEngineSoundTypeInterior(int modelIndex)
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

	std::string path = g_flightModelsList.GetLstLine(modelIndex);

	const std::string objectValue = GetFileKeyValue(lines, path + ".opt");

	if (!objectValue.empty() && std::ifstream(objectValue))
	{
		path = GetStringWithoutExtension(objectValue);
	}

	lines = GetFileLines(path + "Sound.txt");

	if (!lines.size())
	{
		lines = GetFileLines(path + ".ini", "Sound");
	}

	int type = 0;

	if (lines.size())
	{
		type = GetFileKeyValueInt(lines, "EngineSoundInterior");
	}
	else
	{
		const auto& soundConfig = GetSoundsConfig();

		if (soundConfig.SoundsCountHookExists && soundConfig.SfxInteriorCount)
		{
			return 0;
		}

		switch (modelIndex)
		{
		case 1: // ModelIndex_001_0_0_Xwing
		case 4: // ModelIndex_004_0_3_Bwing
		case 14: // ModelIndex_014_0_13_Z_95
		case 15: // ModelIndex_015_0_14_R41
		case 31: // ModelIndex_031_0_30_SlaveOne
			type = 1;
			break;

		case 2: // ModelIndex_002_0_1_Ywing
		case 11: // ModelIndex_011_0_10_ToscanFighter
		case 23: // ModelIndex_023_0_22_CloakshapeFighter
			type = 2;
			break;

		case 3: // ModelIndex_003_0_2_Awing
		case 10: // ModelIndex_010_0_9_IrdFighter
		case 13: // ModelIndex_013_0_12_Twing
		case 27: // ModelIndex_027_0_26_Piggyback
			type = 3;
			break;

		case 5: // ModelIndex_005_0_4_TieFighter
		case 6: // ModelIndex_006_0_5_TieInterceptor
		case 7: // ModelIndex_007_0_6_TieBomber
		case 8: // ModelIndex_008_0_7_TieAdvanced
		case 9: // ModelIndex_009_0_8_TieDefender
			type = 4;
			break;

		case 12: // ModelIndex_012_0_11_MissileBoat
		case 16: // ModelIndex_016_0_15_AssaultGunboat
		case 24: // ModelIndex_024_0_23_RazorFighter
		case 25: // ModelIndex_025_0_24_PlanetaryFighter
		case 29: // ModelIndex_029_0_28_PreybirdFighter
			type = 5;
			break;

		case 17: // ModelIndex_017_0_16_SkiprayBlastBoat
		case 58: // ModelIndex_058_0_45_CorellianTransport2
		case 65: // ModelIndex_065_0_52_FamilyTransport
			type = 6;
			break;

		case 26: // ModelIndex_026_0_25_SupaFighter
		case 32: // ModelIndex_032_0_31_SlaveTwo
		case 59: // ModelIndex_059_0_46_MilleniumFalcon2
			type = 7;
			break;
		}
	}

	return type;
}

int GetEngineSoundTypeFlyBy(int modelIndex)
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

	std::string path = g_flightModelsList.GetLstLine(modelIndex);

	const std::string objectValue = GetFileKeyValue(lines, path + ".opt");

	if (!objectValue.empty() && std::ifstream(objectValue))
	{
		path = GetStringWithoutExtension(objectValue);
	}

	lines = GetFileLines(path + "Sound.txt");

	if (!lines.size())
	{
		lines = GetFileLines(path + ".ini", "Sound");
	}

	int type = 0;

	if (lines.size())
	{
		type = GetFileKeyValueInt(lines, "EngineSoundFlyBy");
	}
	else
	{
		const auto& soundConfig = GetSoundsConfig();

		if (soundConfig.SoundsCountHookExists && soundConfig.SfxFlyByCount)
		{
			return 0;
		}

		switch (modelIndex)
		{
		case 1: // ModelIndex_001_0_0_Xwing
		case 4: // ModelIndex_004_0_3_Bwing
		case 14: // ModelIndex_014_0_13_Z_95
		case 15: // ModelIndex_015_0_14_R41
		case 31: // ModelIndex_031_0_30_SlaveOne
			type = 1;
			break;

		case 2: // ModelIndex_002_0_1_Ywing
		case 11: // ModelIndex_011_0_10_ToscanFighter
		case 23: // ModelIndex_023_0_22_CloakshapeFighter
			type = 2;
			break;

		case 3: // ModelIndex_003_0_2_Awing
		case 10: // ModelIndex_010_0_9_IrdFighter
		case 13: // ModelIndex_013_0_12_Twing
		case 27: // ModelIndex_027_0_26_Piggyback
			type = 3;
			break;

		case 5: // ModelIndex_005_0_4_TieFighter
		case 6: // ModelIndex_006_0_5_TieInterceptor
		case 7: // ModelIndex_007_0_6_TieBomber
		case 8: // ModelIndex_008_0_7_TieAdvanced
		case 9: // ModelIndex_009_0_8_TieDefender
			type = 4;
			break;

		case 12: // ModelIndex_012_0_11_MissileBoat
		case 16: // ModelIndex_016_0_15_AssaultGunboat
		case 24: // ModelIndex_024_0_23_RazorFighter
		case 25: // ModelIndex_025_0_24_PlanetaryFighter
		case 29: // ModelIndex_029_0_28_PreybirdFighter
		case 43: // ModelIndex_043_0_32_Tug
		case 44: // ModelIndex_044_0_33_CombatUtilityVehicle
		case 45: // ModelIndex_045_0_34_HeavyLifter
		case 50: // ModelIndex_050_0_37_Shuttle
		case 51: // ModelIndex_051_0_38_EscortShuttle
		case 52: // ModelIndex_052_0_39_StormtrooperTransport
		case 53: // ModelIndex_053_0_40_AssaultTransport
		case 54: // ModelIndex_054_0_41_EscortTransport
		case 55: // ModelIndex_055_0_42_SystemPatrolCraft
		case 56: // ModelIndex_056_0_43_ScoutCraft
			type = 5;
			break;

		case 17: // ModelIndex_017_0_16_SkiprayBlastBoat
		case 58: // ModelIndex_058_0_45_CorellianTransport2
		case 65: // ModelIndex_065_0_52_FamilyTransport
			type = 6;
			break;

		case 26: // ModelIndex_026_0_25_SupaFighter
		case 32: // ModelIndex_032_0_31_SlaveTwo
		case 59: // ModelIndex_059_0_46_MilleniumFalcon2
			// type 6
			type = 7;
			break;
		}
	}

	return type;
}

int GetEngineSoundTypeWash(int modelIndex)
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

	std::string path = g_flightModelsList.GetLstLine(modelIndex);

	const std::string objectValue = GetFileKeyValue(lines, path + ".opt");

	if (!objectValue.empty() && std::ifstream(objectValue))
	{
		path = GetStringWithoutExtension(objectValue);
	}

	lines = GetFileLines(path + "Sound.txt");

	if (!lines.size())
	{
		lines = GetFileLines(path + ".ini", "Sound");
	}

	int type = 0;

	if (lines.size())
	{
		type = GetFileKeyValueInt(lines, "EngineSoundWash");
	}
	else
	{
		const auto& soundConfig = GetSoundsConfig();

		if (soundConfig.SoundsCountHookExists && soundConfig.SfxEngineWashCount)
		{
			return 0;
		}

		switch (modelIndex)
		{
		case 137: // ModelIndex_137_0_91_Interdictor2
		case 138: // ModelIndex_138_0_92_VictoryStarDestroyer2
		case 139: // ModelIndex_139_0_93_ImperialStarDestroyer2
		case 140: // ModelIndex_140_0_94_SuperStarDestroyer
			type = 2;
			break;

		default:
			type = 1;
		}
	}

	return type;
}

int GetEngineSoundTypeTakeOff(int modelIndex)
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

	std::string path = g_flightModelsList.GetLstLine(modelIndex);

	const std::string objectValue = GetFileKeyValue(lines, path + ".opt");

	if (!objectValue.empty() && std::ifstream(objectValue))
	{
		path = GetStringWithoutExtension(objectValue);
	}

	lines = GetFileLines(path + "Sound.txt");

	if (!lines.size())
	{
		lines = GetFileLines(path + ".ini", "Sound");
	}

	int type = 0;

	if (lines.size())
	{
		type = GetFileKeyValueInt(lines, "EngineSoundTakeOff");
	}
	else
	{
		const auto& soundConfig = GetSoundsConfig();

		if (soundConfig.SoundsCountHookExists && soundConfig.SfxTakeOffCount)
		{
			return 0;
		}

		switch (modelIndex)
		{
		case 58: // ModelIndex_058_0_45_CorellianTransport2
		case 59: // ModelIndex_059_0_46_MilleniumFalcon2
		case 65: // ModelIndex_065_0_52_FamilyTransport
			type = 2;
			break;

		default:
			type = 1;
		}
	}

	return type;
}

std::string GetWeaponSoundBehavior(int modelIndex)
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

	std::string path = g_flightModelsList.GetLstLine(modelIndex);

	const std::string objectValue = GetFileKeyValue(lines, path + ".opt");

	if (!objectValue.empty() && std::ifstream(objectValue))
	{
		path = GetStringWithoutExtension(objectValue);
	}

	lines = GetFileLines(path + "Sound.txt");

	if (!lines.size())
	{
		lines = GetFileLines(path + ".ini", "Sound");
	}

	std::string behavior;

	if (lines.size())
	{
		behavior = GetFileKeyValue(lines, "WeaponSoundBehavior");
	}
	else
	{
		const auto& soundConfig = GetSoundsConfig();

		if (soundConfig.SoundsCountHookExists && soundConfig.SfxWeaponCount)
		{
			return std::string();
		}

		switch (modelIndex)
		{
		case 58: // ModelIndex_058_0_45_CorellianTransport2
		case 59: // ModelIndex_059_0_46_MilleniumFalcon2
		case 65: // ModelIndex_065_0_52_FamilyTransport
			behavior = "CorellianTransport";
			break;

		case 5: // ModelIndex_005_0_4_TieFighter
		case 6: // ModelIndex_006_0_5_TieInterceptor
		case 7: // ModelIndex_007_0_6_TieBomber
		case 8: // ModelIndex_008_0_7_TieAdvanced
		case 9: // ModelIndex_009_0_8_TieDefender
		case 18: // ModelIndex_018_0_17_TieBizarro
		case 19: // ModelIndex_019_0_18_TieBigGun
		case 20: // ModelIndex_020_0_19_TieWarheads
		case 21: // ModelIndex_021_0_20_TieBomb
		case 22: // ModelIndex_022_0_21_TieBooster
			behavior = "TieFighter";
			break;

		default:
			behavior = std::string();
		}
	}

	return behavior;
}

std::tuple<int, int> GetWeaponSoundRange(int modelIndex, int weaponIndex)
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

	std::string path = g_flightModelsList.GetLstLine(modelIndex);

	const std::string objectValue = GetFileKeyValue(lines, path + ".opt");

	if (!objectValue.empty() && std::ifstream(objectValue))
	{
		path = GetStringWithoutExtension(objectValue);
	}

	lines = GetFileLines(path + "Sound.txt");

	if (!lines.size())
	{
		lines = GetFileLines(path + ".ini", "Sound");
	}

	int rangeStart = GetFileKeyValueInt(lines, "WeaponSoundRangeStart_" + std::to_string(weaponIndex), -1);
	int rangeCount = GetFileKeyValueInt(lines, "WeaponSoundRangeCount_" + std::to_string(weaponIndex), -1);

	if (rangeStart >= 0 && rangeCount >= 0)
	{
		const auto& soundConfig = GetSoundsConfig();

		if (soundConfig.SoundsCountHookExists && soundConfig.SfxWeaponRangeCount)
		{
			if (rangeStart + rangeCount > soundConfig.SfxWeaponRangeCount)
			{
				rangeStart = -1;
				rangeCount = -1;
			}
		}
	}

	return std::make_tuple(rangeStart, rangeCount);
}

int GetHyperSoundBehavior(int modelIndex)
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

	std::string path = g_flightModelsList.GetLstLine(modelIndex);

	const std::string objectValue = GetFileKeyValue(lines, path + ".opt");

	if (!objectValue.empty() && std::ifstream(objectValue))
	{
		path = GetStringWithoutExtension(objectValue);
	}

	lines = GetFileLines(path + "Sound.txt");

	if (!lines.size())
	{
		lines = GetFileLines(path + ".ini", "Sound");
	}

	int behavior;

	if (lines.size())
	{
		behavior = GetFileKeyValueInt(lines, "HyperSoundBehavior", -1);
	}
	else
	{
		behavior = -1;
	}

	return behavior;
}

struct AmbientSoundSettings
{
	int OffsetX;
	int OffsetY;
	int OffsetZ;
	int Distance;
	int DistanceX;
	int DistanceY;
	int DistanceZ;
};

AmbientSoundSettings GetAmbientSoundSettings(int modelIndex)
{
	const float ScaleFactor = 0.0244140625f;
	const ExeEnableEntry* ExeEnableTable = (ExeEnableEntry*)0x005FB240;

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

	std::string path = g_flightModelsList.GetLstLine(modelIndex);

	const std::string objectValue = GetFileKeyValue(lines, path + ".opt");

	if (!objectValue.empty() && std::ifstream(objectValue))
	{
		path = GetStringWithoutExtension(objectValue);
	}

	lines = GetFileLines(path + "Sound.txt");

	if (!lines.size())
	{
		lines = GetFileLines(path + ".ini", "Sound");
	}

	std::vector<std::string> defaultLines = GetFileLines("FlightModels\\Sound.txt");

	if (!lines.size())
	{
		defaultLines = GetFileLines("FlightModels\\default.ini", "Sound");
	}

	AmbientSoundSettings settings;

	int modelSize = (int)(ExeEnableTable[modelIndex].ObjectSize * ScaleFactor);
	int defaultDistance = GetFileKeyValueInt(defaultLines, "AmbientSoundDistance", modelSize);

	settings.OffsetX = GetFileKeyValueInt(lines, "AmbientSoundOffsetX", 0);
	settings.OffsetY = GetFileKeyValueInt(lines, "AmbientSoundOffsetY", 0);
	settings.OffsetZ = GetFileKeyValueInt(lines, "AmbientSoundOffsetZ", 0);
	settings.Distance = GetFileKeyValueInt(lines, "AmbientSoundDistance", defaultDistance);
	settings.DistanceX = GetFileKeyValueInt(lines, "AmbientSoundDistanceX", settings.Distance);
	settings.DistanceY = GetFileKeyValueInt(lines, "AmbientSoundDistanceY", settings.Distance);
	settings.DistanceZ = GetFileKeyValueInt(lines, "AmbientSoundDistanceZ", settings.Distance);

	if (settings.Distance == 0)
	{
		settings.Distance = 1;
	}

	if (settings.DistanceX == 0)
	{
		settings.DistanceX = settings.Distance;
	}

	if (settings.DistanceY == 0)
	{
		settings.DistanceY = settings.Distance;
	}

	if (settings.DistanceZ == 0)
	{
		settings.DistanceZ = settings.Distance;
	}

	return settings;
}

class ModelIndexSound
{
public:
	int GetEngineTypeInterior(int modelIndex)
	{
		this->Update();

		auto it = this->_typeInterior.find(modelIndex);

		if (it != this->_typeInterior.end())
		{
			return it->second;
		}
		else
		{
			int value = GetEngineSoundTypeInterior(modelIndex);
			this->_typeInterior.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

	int GetEngineTypeFlyBy(int modelIndex)
	{
		this->Update();

		auto it = this->_typeFlyBy.find(modelIndex);

		if (it != this->_typeFlyBy.end())
		{
			return it->second;
		}
		else
		{
			int value = GetEngineSoundTypeFlyBy(modelIndex);
			this->_typeFlyBy.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

	int GetEngineTypeWash(int modelIndex)
	{
		this->Update();

		auto it = this->_typeWash.find(modelIndex);

		if (it != this->_typeWash.end())
		{
			return it->second;
		}
		else
		{
			int value = GetEngineSoundTypeWash(modelIndex);
			this->_typeWash.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

	int GetEngineTypeTakeOff(int modelIndex)
	{
		this->Update();

		auto it = this->_typeTakeOff.find(modelIndex);

		if (it != this->_typeTakeOff.end())
		{
			return it->second;
		}
		else
		{
			int value = GetEngineSoundTypeTakeOff(modelIndex);
			this->_typeTakeOff.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

	std::string GetWeaponBehavior(int modelIndex)
	{
		this->Update();

		auto it = this->_weaponBehavior.find(modelIndex);

		if (it != this->_weaponBehavior.end())
		{
			return it->second;
		}
		else
		{
			std::string value = GetWeaponSoundBehavior(modelIndex);
			this->_weaponBehavior.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

	std::map<int, std::tuple<int, int>>& GetWeaponRange(int modelIndex)
	{
		this->Update();

		auto it = this->_weaponRange.find(modelIndex);

		if (it != this->_weaponRange.end())
		{
			return it->second;
		}
		else
		{
			auto value = std::map<int, std::tuple<int, int>>();
			this->_weaponRange.insert(std::make_pair(modelIndex, value));
			return this->_weaponRange.find(modelIndex)->second;
		}
	}

	std::tuple<int, int> GetWeaponRange(int modelIndex, int weaponIndex)
	{
		this->Update();

		auto& weaponRange = GetWeaponRange(modelIndex);

		auto it = weaponRange.find(weaponIndex);

		if (it != weaponRange.end())
		{
			return it->second;
		}
		else
		{
			auto value = GetWeaponSoundRange(modelIndex, weaponIndex);
			weaponRange.insert(std::make_pair(weaponIndex, value));
			return value;
		}
	}

	int GetHyperBehavior(int modelIndex)
	{
		this->Update();

		auto it = this->_hyperBehavior.find(modelIndex);

		if (it != this->_hyperBehavior.end())
		{
			return it->second;
		}
		else
		{
			int value = GetHyperSoundBehavior(modelIndex);
			this->_hyperBehavior.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

	AmbientSoundSettings GetAmbientSound(int modelIndex)
	{
		this->Update();

		auto it = this->_ambientSounds.find(modelIndex);

		if (it != this->_ambientSounds.end())
		{
			return it->second;
		}
		else
		{
			AmbientSoundSettings value = GetAmbientSoundSettings(modelIndex);
			this->_ambientSounds.insert(std::make_pair(modelIndex, value));
			return value;
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

			this->_typeInterior.clear();
			this->_typeFlyBy.clear();
			this->_typeWash.clear();
			this->_typeTakeOff.clear();
			this->_weaponBehavior.clear();
			this->_weaponRange.clear();
			this->_hyperBehavior.clear();
			this->_ambientSounds.clear();
		}
	}

	std::map<int, int> _typeInterior;
	std::map<int, int> _typeFlyBy;
	std::map<int, int> _typeWash;
	std::map<int, int> _typeTakeOff;
	std::map<int, std::string> _weaponBehavior;
	std::map<int, std::map<int, std::tuple<int, int>>> _weaponRange;
	std::map<int, int> _hyperBehavior;
	std::map<int, AmbientSoundSettings> _ambientSounds;
};

ModelIndexSound g_modelIndexSound;

int ReplaceMissionSoundsHook(int* params)
{
	static std::vector<std::string> _lines;
	static std::string _mission;
	static int _missionIndex = 0;

	const char* A4 = (const char*)params[1];
	const char* A8 = (const char*)params[2];
	int AC = params[3];

	const auto LoadSfx2 = (int(*)(const char*, const char*, int, int))0x004DAD40;
	const char* xwaMissionFileName = (const char*)0x06002E8;
	const int missionFileNameIndex = *(int*)0x06002E4;

	if (missionFileNameIndex == 0 ? (_mission != xwaMissionFileName) : (_missionIndex != missionFileNameIndex))
	{
		_mission = xwaMissionFileName;
		_missionIndex = missionFileNameIndex;

		const std::string mission = GetStringWithoutExtension(xwaMissionFileName);
		_lines = GetFileLines(mission + "_Sounds.txt");

		if (!_lines.size())
		{
			_lines = GetFileLines(mission + ".ini", "Sounds");
		}
	}

	if (_lines.size())
	{
		const std::string value = GetFileKeyValue(_lines, A4);

		if (!value.empty() && std::ifstream(value))
		{
			return LoadSfx2(value.c_str(), A8, 0, AC);
		}
	}

	return LoadSfx2(A4, A8, 0, AC);
}

int InteriorSoundHook(int* params)
{
	const int modelIndex = params[0];
	int& esi = params[1];
	int& ebp = params[2];
	int& ebx = params[3];

	const char sfx_quality = *(char*)0xB0C890;

	const int type = g_modelIndexSound.GetEngineTypeInterior(modelIndex);

	if (type == 0)
	{
		const auto& soundConfig = GetSoundsConfig();

		if (soundConfig.SoundsCountHookExists && soundConfig.SfxInteriorCount)
		{
			if (modelIndex < soundConfig.SfxInteriorCount)
			{
				esi = soundConfig.SfxInteriorIndex + modelIndex;
				ebp = 0x2AF8;

				if (sfx_quality != 0)
				{
					ebx = 0x01;
				}
			}

			return 0;
		}
	}

	switch (type)
	{
	case 1:
		esi = 0x5E; // EngineIntXW
		ebp = 0x2AF8;
		break;

	case 2:
		esi = 0x5F; // EngineIntYW
		ebp = 0x2AF8;
		break;

	case 3:
		esi = 0x60; // EngineIntAW
		ebp = 0x157C;
		break;

	case 4:
		esi = 0x61; // EngineIntTI
		ebp = 0x2AF8;
		break;

	case 5:
		esi = 0x62; // EngineIntAG
		ebp = 0x2AF8;
		break;

	case 6:
		esi = 0x63; // EngineIntCort
		ebp = 0x2AF8;
		break;

	case 7:
		esi = 0x64; // EngineIntFalc
		ebp = 0x2AF8;
		break;
	}

	if (ebp == 0x2AF8)
	{
		if (sfx_quality != 0)
		{
			ebx = 0x01;
		}
	}

	return 0;
}

int StopInteriorSoundHook(int* params)
{
	const int modelIndex = params[0];
	int& esi = params[1];

	const int type = g_modelIndexSound.GetEngineTypeInterior(modelIndex);

	if (type == 0)
	{
		const auto& soundConfig = GetSoundsConfig();

		if (soundConfig.SoundsCountHookExists && soundConfig.SfxInteriorCount)
		{
			if (modelIndex < soundConfig.SfxInteriorCount)
			{
				esi = soundConfig.SfxInteriorIndex + modelIndex;
			}

			return 0;
		}
	}

	switch (type)
	{
	case 1:
		esi = 0x5E; // EngineIntXW
		break;

	case 2:
		esi = 0x5F; // EngineIntYW
		break;

	case 3:
		esi = 0x60; // EngineIntAW
		break;

	case 4:
		esi = 0x61; // EngineIntTI
		break;

	case 5:
		esi = 0x62; // EngineIntAG
		break;

	case 6:
		esi = 0x63; // EngineIntCort
		break;

	case 7:
		esi = 0x64; // EngineIntFalc
		break;
	}

	return 0;
}

void SetFlyBySound(int& slot, int modelIndex)
{
	const char xwaSound3dEnabled = *(char*)0x5BA990;
	const int* xwaSoundEffectsBufferId = GetSoundsConfig().SoundEffectIds;

	const int type = g_modelIndexSound.GetEngineTypeFlyBy(modelIndex);

	if (type == 0)
	{
		const auto& soundConfig = GetSoundsConfig();

		if (soundConfig.SoundsCountHookExists && soundConfig.SfxFlyByCount)
		{
			if (modelIndex < soundConfig.SfxFlyByCount)
			{
				slot = soundConfig.SfxFlyByIndex + modelIndex;
			}

			return;
		}
	}

	switch (type)
	{
	case 1:
		if (xwaSound3dEnabled == 0 || xwaSoundEffectsBufferId[0x6A] == -1)
		{
			slot = 0x69; // FlyByXW
		}
		else
		{
			slot = 0x6A; // FlyByXW3D
		}

		break;

	case 2:
		if (xwaSound3dEnabled == 0 || xwaSoundEffectsBufferId[0x6C] == -1)
		{
			slot = 0x6B; // FlyByYW
		}
		else
		{
			slot = 0x6C; // FlyByYW3D
		}

		break;

	case 3:
		if (xwaSound3dEnabled == 0 || xwaSoundEffectsBufferId[0x6E] == -1)
		{
			slot = 0x6D; // FlyByAW
		}
		else
		{
			slot = 0x6E; // FlyByAW3D
		}

		break;

	case 4:
		if (xwaSound3dEnabled == 0 || xwaSoundEffectsBufferId[0x66] == -1)
		{
			slot = 0x65; // FlyByTI
		}
		else
		{
			slot = 0x66; // FlyByTI3D
		}

		break;

	case 5:
		if (xwaSound3dEnabled == 0 || xwaSoundEffectsBufferId[0x68] == -1)
		{
			slot = 0x67; // FlyByAG
		}
		else
		{
			slot = 0x68; // FlyByAG3D
		}

		break;

	case 6:
	case 7:
		if (xwaSound3dEnabled == 0 || xwaSoundEffectsBufferId[0x70] == -1)
		{
			slot = 0x6F; // FlyByCort
		}
		else
		{
			slot = 0x70; // FlyByCort3D
		}

		break;

	case 8:
		if (xwaSound3dEnabled == 0 || xwaSoundEffectsBufferId[0x82] == -1)
		{
			slot = 0x81; // unused
		}
		else
		{
			slot = 0x82; // unused3D
		}

		break;

	case 11:
		slot = 0x69; // FlyByXW
		break;

	case 12:
		slot = 0x6B; // FlyByYW
		break;

	case 13:
		slot = 0x6D; // FlyByAW
		break;

	case 14:
		slot = 0x65; // FlyByTI
		break;

	case 15:
		slot = 0x67; // FlyByAG
		break;

	case 16:
	case 17:
		slot = 0x6F; // FlyByCort
		break;

	case 18:
		slot = 0x81; // unused
		break;

	case 21:
		slot = 0x6A; // FlyByXW3D
		break;

	case 22:
		slot = 0x6C; // FlyByYW3D
		break;

	case 23:
		slot = 0x6E; // FlyByAW3D
		break;

	case 24:
		slot = 0x66; // FlyByTI3D
		break;

	case 25:
		slot = 0x68; // FlyByAG3D
		break;

	case 26:
	case 27:
		slot = 0x70; // FlyByCort3D
		break;

	case 28:
		slot = 0x82; // unused3D
		break;
	}
}

int FlyBySoundHook(int* params)
{
	const int modelIndex = params[0];
	int& esi = params[1];

	SetFlyBySound(esi, modelIndex);

	return 0;
}

int LaunchSoundHook(int* params)
{
	const int modelIndex = params[0];
	int& eax = params[1];

	SetFlyBySound(eax, modelIndex);

	return 0;
}

int WashSoundHook(int* params)
{
	const int modelIndex = params[0];

	const int type = g_modelIndexSound.GetEngineTypeWash(modelIndex);

	if (type == 0)
	{
		const auto& soundConfig = GetSoundsConfig();

		if (soundConfig.SoundsCountHookExists && soundConfig.SfxEngineWashCount)
		{
			if (modelIndex < soundConfig.SfxEngineWashCount)
			{
				return soundConfig.SfxEngineWashIndex + modelIndex;
			}
		}
	}

	int soundIndex;

	switch (type)
	{
	case 1:
		soundIndex = 0x72; // EngineWash
		break;

	case 2:
		soundIndex = 0x71; // EngineWashSD
		break;

	default:
		soundIndex = 0x72; // EngineWash
		break;
	}

	return soundIndex;
}

int WashSoundStopHook(int* params)
{
	const auto XwaGetPlayingSoundsCount = (int(*)(int))0x004DC850;
	const auto XwaStopSound = (char(*)(int))0x004DC400;

	const auto& soundConfig = GetSoundsConfig();
	const int* xwaSoundEffectsBufferId = GetSoundsConfig().SoundEffectIds;

	if (XwaGetPlayingSoundsCount(xwaSoundEffectsBufferId[0x71]) != 0)
	{
		XwaStopSound(xwaSoundEffectsBufferId[0x71]);
	}

	if (XwaGetPlayingSoundsCount(xwaSoundEffectsBufferId[0x72]) != 0)
	{
		XwaStopSound(xwaSoundEffectsBufferId[0x72]);
	}

	if (soundConfig.SoundsCountHookExists && soundConfig.SfxEngineWashCount)
	{
		int start = soundConfig.SfxEngineWashIndex;
		int end = soundConfig.SfxEngineWashIndex + soundConfig.SfxEngineWashCount;

		for (int id = start; id < end; id++)
		{
			if (XwaGetPlayingSoundsCount(xwaSoundEffectsBufferId[id]) != 0)
			{
				XwaStopSound(xwaSoundEffectsBufferId[id]);
			}
		}
	}

	return 0;
}

int TakeOffSoundHook(int* params)
{
	const int modelIndex = (short)params[Params_ECX];

	params[Params_ECX] = *(int*)0x007B33C4;

	const auto playSound = (int(*)(int, int, int))0x0043BF90;

	int s_XwaHangarPlayerObjectIndex = *(int*)0x0068BC08;
	int s_XwaCurrentPlayerId = *(int*)0x008C1CC8;

	const int type = g_modelIndexSound.GetEngineTypeTakeOff(modelIndex);

	if (type == 0)
	{
		const auto& soundConfig = GetSoundsConfig();

		if (soundConfig.SoundsCountHookExists && soundConfig.SfxTakeOffCount)
		{
			if (modelIndex < soundConfig.SfxTakeOffCount)
			{
				playSound(soundConfig.SfxTakeOffIndex + modelIndex, s_XwaHangarPlayerObjectIndex, s_XwaCurrentPlayerId);
				return 0;
			}
		}
	}

	if (type == 2)
	{
		// MfTakeOff.wav
		playSound(0x8B, s_XwaHangarPlayerObjectIndex, s_XwaCurrentPlayerId);
	}
	else
	{
		// LandingGearUp.wav
		playSound(0x84, s_XwaHangarPlayerObjectIndex, s_XwaCurrentPlayerId);
	}

	return 0;
}

int EngineSlowingSoundHook(int* params)
{
	int soundIndex = params[0];
	int xwaHangarPlayerObjectIndex = params[1];
	int xwaCurrentPlayerId = params[2];

	const auto playSound = (int(*)(int, int, int))0x0043BF90;

	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	const int modelIndex = XwaObjects[xwaHangarPlayerObjectIndex].ModelIndex;

	const auto& soundConfig = GetSoundsConfig();

	if (soundConfig.SoundsCountHookExists && soundConfig.SfxEngineSlowingCount)
	{
		if (modelIndex < soundConfig.SfxEngineSlowingCount)
		{
			soundIndex = soundConfig.SfxEngineSlowingIndex + modelIndex;
		}
	}

	playSound(soundIndex, xwaHangarPlayerObjectIndex, xwaCurrentPlayerId);

	return 0;
}

int CanopyOpeningSoundHook(int* params)
{
	int soundIndex = params[0];
	int xwaHangarPlayerObjectIndex = params[1];
	int xwaCurrentPlayerId = params[2];

	const auto playSound = (int(*)(int, int, int))0x0043BF90;

	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	const int modelIndex = XwaObjects[xwaHangarPlayerObjectIndex].ModelIndex;

	const auto& soundConfig = GetSoundsConfig();

	if (soundConfig.SoundsCountHookExists && soundConfig.SfxCanopyOpeningCount)
	{
		if (modelIndex < soundConfig.SfxCanopyOpeningCount)
		{
			soundIndex = soundConfig.SfxCanopyOpeningIndex + modelIndex;
		}
	}

	playSound(soundIndex, xwaHangarPlayerObjectIndex, xwaCurrentPlayerId);

	return 0;
}

int HangarShuttleSoundsHook(int* params)
{
	int soundIndex = params[0];
	int xwaObjectIndex = params[1];
	int xwaCurrentPlayerId = params[2];

	const auto playSound = (int(*)(int, int, int))0x0043BF90;

	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	const int modelIndex = xwaObjectIndex == 0xffff ? -1 : XwaObjects[xwaObjectIndex].ModelIndex;

	const auto& soundConfig = GetSoundsConfig();

	switch (soundIndex)
	{
	case 0x84:
		// LandingGearUp
		if (soundConfig.SoundsCountHookExists && soundConfig.SfxTakeOffCount)
		{
			if (modelIndex < soundConfig.SfxTakeOffCount)
			{
				soundIndex = soundConfig.SfxTakeOffIndex + modelIndex;
			}
		}

		break;

	case 0x95:
		// ImperialShuttleTakeOff
		if (soundConfig.SoundsCountHookExists && soundConfig.SfxShuttleTakeOffCount)
		{
			if (modelIndex < soundConfig.SfxShuttleTakeOffCount)
			{
				soundIndex = soundConfig.SfxShuttleTakeOffIndex + modelIndex;
			}
		}

		break;

	case 0x92:
		// ImperialShuttleBlastOff
		if (soundConfig.SoundsCountHookExists && soundConfig.SfxShuttleBlastOffCount)
		{
			if (modelIndex < soundConfig.SfxShuttleBlastOffCount)
			{
				soundIndex = soundConfig.SfxShuttleBlastOffIndex + modelIndex;
			}
		}

		break;

	case 0x94:
		// ImperialShuttleShutDown
		if (soundConfig.SoundsCountHookExists && soundConfig.SfxShuttleShutDownCount)
		{
			if (modelIndex < soundConfig.SfxShuttleShutDownCount)
			{
				soundIndex = soundConfig.SfxShuttleShutDownIndex + modelIndex;
			}
		}

		break;

	case 0x83:
		// LandingGearDown
		if (soundConfig.SoundsCountHookExists && soundConfig.SfxEngineSlowingCount)
		{
			if (modelIndex < soundConfig.SfxEngineSlowingCount)
			{
				soundIndex = soundConfig.SfxEngineSlowingIndex + modelIndex;
			}
		}

		break;
	}

	playSound(soundIndex, xwaObjectIndex, xwaCurrentPlayerId);

	return 0;
}

int WeaponModelIndexToSoundIndex(int weaponIndex)
{
	switch (weaponIndex)
	{
	case 280: // ModelIndex_280_1_17_LaserRebel
	case 281: // ModelIndex_281_1_18_LaserRebelTurbo
	case 282: // ModelIndex_282_1_19_LaserImp
	case 283: // ModelIndex_283_1_20_LaserImpTurbo
	case 284: // ModelIndex_284_1_22_LaserIon
	case 285: // ModelIndex_285_1_23_LaserIonTurbo
		return weaponIndex - 280;

	case 288: // ModelIndex_288_1_18_LaserRebelTurbo
	case 297: // ModelIndex_297_1_18_LaserRebelTurbo
	case 301: // ModelIndex_301_1_18_LaserRebelTurbo
	case 302: // ModelIndex_302_1_18_LaserRebelTurbo
		return 6; // RebelLaserStarship

	case 289: // ModelIndex_289_1_20_LaserImpTurbo
	case 303: // ModelIndex_303_1_20_LaserImpTurbo
	case 304: // ModelIndex_304_1_20_LaserImpTurbo
	case 305: // ModelIndex_305_1_20_LaserImpTurbo
		return 7; // EmpireLaserStarship

	case 290: // ModelIndex_290_1_23_LaserIonTurbo
		return 8; // IonCannonStarship

	case 307: // ModelIndex_307__1_0, open weapon
		return 9;
	}

	return -1;
}

int WeaponSoundHook(int* params)
{
	const int A4 = params[0];
	const int A8 = params[1];
	int modelIndex = params[2];
	const int weaponIndex = params[3];

	const auto playSound = (int(*)(int, int, int))0x0043BF90;
	const auto XwaRandom = (unsigned short(*)())0x00494E10;

	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;

	if (XwaObjects[A4].pMobileObject != nullptr && XwaObjects[A4].pMobileObject->ModelIndex == 0)
	{
		short objectIndex = XwaObjects[A4].pMobileObject->ObjectIndex;

		if (objectIndex != 0)
		{
			modelIndex = XwaObjects[objectIndex].ModelIndex;
		}
	}

	if (g_modelIndexSound.GetWeaponBehavior(modelIndex).empty())
	{
		std::tuple<int, int> weaponSoundRange = g_modelIndexSound.GetWeaponRange(modelIndex, weaponIndex);
		int weaponSoundRangeStart = std::get<0>(weaponSoundRange);
		int weaponSoundRangeCount = std::get<1>(weaponSoundRange);
		const auto& soundConfig = GetSoundsConfig();

		if (soundConfig.SoundsCountHookExists)
		{
			if (weaponSoundRangeStart >= 0 && weaponSoundRangeCount >= 0)
			{
				int soundIndex = soundConfig.SfxWeaponRangeIndex + weaponSoundRangeStart + (XwaRandom() % weaponSoundRangeCount);
				playSound(soundIndex, A4, A8);

				return 0;
			}
			else
			{
				int weaponSoundIndex = WeaponModelIndexToSoundIndex(weaponIndex);

				if (weaponSoundIndex != -1 && soundConfig.SoundsCountHookExists && soundConfig.SfxWeaponCount)
				{
					if (modelIndex < soundConfig.SfxWeaponCount / 10)
					{
						int soundIndex = soundConfig.SfxWeaponIndex + modelIndex * 10 + weaponSoundIndex;
						playSound(soundIndex, A4, A8);
					}

					return 0;
				}
			}
		}
	}

	switch (weaponIndex)
	{
	case 280: // ModelIndex_280_1_17_LaserRebel
	case 281: // ModelIndex_281_1_18_LaserRebelTurbo
	case 282: // ModelIndex_282_1_19_LaserImp
	case 283: // ModelIndex_283_1_20_LaserImpTurbo
	case 284: // ModelIndex_284_1_22_LaserIon
	case 285: // ModelIndex_285_1_23_LaserIonTurbo
	{
		std::string behavior = g_modelIndexSound.GetWeaponBehavior(modelIndex);

		if (!behavior.empty())
		{
			if (behavior == "CorellianTransport")
			{
				switch (weaponIndex)
				{
				case 280: // ModelIndex_280_1_17_LaserRebel
					return playSound(15, A4, A8); // FalconLaser

				case 281: // ModelIndex_281_1_18_LaserRebelTurbo
					return playSound(16, A4, A8); // FalconLaserTurbo
				}
			}
			else if (behavior == "TieFighter")
			{
				return playSound(20, A4, A8); // EmpireLaserChChChhh
			}
			else if (behavior == "TieFighterWithIon")
			{
				switch (weaponIndex)
				{
				case 280: // ModelIndex_280_1_17_LaserRebel
				case 281: // ModelIndex_281_1_18_LaserRebelTurbo
				case 282: // ModelIndex_282_1_19_LaserImp
				case 283: // ModelIndex_283_1_20_LaserImpTurbo
					return playSound(20, A4, A8); // EmpireLaserChChChhh
				}
			}
			else if (behavior == "Rebel")
			{
				switch (weaponIndex)
				{
				case 280: // ModelIndex_280_1_17_LaserRebel
				case 281: // ModelIndex_281_1_18_LaserRebelTurbo
					return playSound(4, A4, A8); // RebelLaser
				}
			}
			else if (behavior == "RebelTurbo")
			{
				switch (weaponIndex)
				{
				case 280: // ModelIndex_280_1_17_LaserRebel
				case 281: // ModelIndex_281_1_18_LaserRebelTurbo
					return playSound(5, A4, A8); // RebelLaserTurbo
				}
			}
			else if (behavior == "Empire")
			{
				switch (weaponIndex)
				{
				case 282: // ModelIndex_282_1_19_LaserImp
				case 283: // ModelIndex_283_1_20_LaserImpTurbo
					return playSound(6, A4, A8); // EmpireLaser
				}
			}
			else if (behavior == "EmpireTurbo")
			{
				switch (weaponIndex)
				{
				case 282: // ModelIndex_282_1_19_LaserImp
				case 283: // ModelIndex_283_1_20_LaserImpTurbo
					return playSound(7, A4, A8); // EmpireLaserTurbo
				}
			}
			else if (behavior == "Falcon")
			{
				switch (weaponIndex)
				{
				case 280: // ModelIndex_280_1_17_LaserRebel
				case 281: // ModelIndex_281_1_18_LaserRebelTurbo
				case 282: // ModelIndex_282_1_19_LaserImp
				case 283: // ModelIndex_283_1_20_LaserImpTurbo
					return playSound(15, A4, A8); // FalconLaser
				}
			}
			else if (behavior == "FalconTurbo")
			{
				switch (weaponIndex)
				{
				case 280: // ModelIndex_280_1_17_LaserRebel
				case 281: // ModelIndex_281_1_18_LaserRebelTurbo
				case 282: // ModelIndex_282_1_19_LaserImp
				case 283: // ModelIndex_283_1_20_LaserImpTurbo
					return playSound(16, A4, A8); // FalconLaserTurbo
				}
			}
			else if (behavior == "Ion")
			{
				switch (weaponIndex)
				{
				case 284: // ModelIndex_284_1_22_LaserIon
				case 285: // ModelIndex_285_1_23_LaserIonTurbo
					return playSound(8, A4, A8); // IonCannon
				}
			}
			else if (behavior == "IonTurbo")
			{
				switch (weaponIndex)
				{
				case 284: // ModelIndex_284_1_22_LaserIon
				case 285: // ModelIndex_285_1_23_LaserIonTurbo
					return playSound(9, A4, A8); // IonCannonTurbo
				}
			}
		}

		return playSound(4 + weaponIndex - 280, A4, A8);
	}

	case 286: // ModelIndex_286_1_24_ProtonTorpedo
	case 291: // ModelIndex_291_1_24_ProtonTorpedo
		return playSound(10, A4, A8); // TorpedoFire

	case 287: // ModelIndex_287_1_25_ConcussionMissile
	case 292: // ModelIndex_292_1_25_ConcussionMissile
		return playSound(11, A4, A8); // MissileFire

	case 288: // ModelIndex_288_1_18_LaserRebelTurbo
	case 297: // ModelIndex_297_1_18_LaserRebelTurbo
	case 301: //ModelIndex_301_1_18_LaserRebelTurbo
	case 302: // ModelIndex_302_1_18_LaserRebelTurbo
		return playSound(12, A4, A8); // RebelLaserStarship

	case 289: // ModelIndex_289_1_20_LaserImpTurbo
	case 303: // ModelIndex_303_1_20_LaserImpTurbo
	case 304: // ModelIndex_304_1_20_LaserImpTurbo
	case 305: // ModelIndex_305_1_20_LaserImpTurbo
		return playSound(13, A4, A8); // EmpireLaserStarship

	case 290: // ModelIndex_290_1_23_LaserIonTurbo
		return playSound(14, A4, A8); // IonCannonStarship

	case 293: // ModelIndex_293_1_26_SpaceBomb
		return playSound(17, A4, A8); // BombFire

	case 294: // ModelIndex_294_1_27_HeavyRocket
		return playSound(18, A4, A8); // RocketFire

	case 295: // ModelIndex_295_1_28_MagPulse
	case 296: // ModelIndex_296_1_28_MagPulse
	case 298: // ModelIndex_298_1_29_Flare
		return playSound(19, A4, A8); // MagPulseFire

	case 307: // ModelIndex_307__1_0, open weapon
	{
		std::string behavior = g_modelIndexSound.GetWeaponBehavior(modelIndex);

		if (!behavior.empty())
		{
			if (behavior == "Rebel")
			{
				return playSound(4, A4, A8); // RebelLaser
			}
			else if (behavior == "RebelTurbo")
			{
				return playSound(5, A4, A8); // RebelLaserTurbo
			}
			else if (behavior == "Empire")
			{
				return playSound(6, A4, A8); // EmpireLaser
			}
			else if (behavior == "EmpireTurbo")
			{
				return playSound(7, A4, A8); // EmpireLaserTurbo
			}
			else if (behavior == "Falcon")
			{
				return playSound(15, A4, A8); // FalconLaser
			}
			else if (behavior == "FalconTurbo")
			{
				return playSound(16, A4, A8); // FalconLaserTurbo
			}
			else if (behavior == "Ion")
			{
				return playSound(8, A4, A8); // IonCannon
			}
			else if (behavior == "IonTurbo")
			{
				return playSound(9, A4, A8); // IonCannonTurbo
			}
			else if (behavior == "TieFighter" || behavior == "TieFighterWithIon")
			{
				return playSound(20, A4, A8); // EmpireLaserChChChhh
			}
		}

		break;
	}
	}

	return 0;
}

int WeaponExplosionsSoundHook(int* params)
{
	params[Params_EAX] = *(int*)0x0063D0D4;
	const int objectIndex = params[1];

	const auto XwaRand = (unsigned short(*)())0x00494E40;

	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;

	if (objectIndex == 0xFFFF)
	{
		return 0;
	}

	const XwaObject* object = &XwaObjects[objectIndex];
	unsigned short modelIndex = 0;

	if (object->pMobileObject)
	{
		modelIndex = object->pMobileObject->ModelIndex;
	}

	if (modelIndex == 0)
	{
		return 0;
	}

	const auto& soundConfig = GetSoundsConfig();
	int soundIndex = -1;

	if (soundConfig.SoundsCountHookExists && soundConfig.SfxWeaponExplosionCount)
	{
		if (modelIndex < soundConfig.SfxWeaponExplosionCount / 8)
		{
			soundIndex = soundConfig.SfxWeaponExplosionIndex + modelIndex * 8;
		}
	}

	if (soundIndex == -1)
	{
		return 0;
	}

	static int soundsCounter[256]{};
	static unsigned char soundsUsed[256 * 8]{};

	if (soundsCounter[modelIndex] <= 0)
	{
		soundsCounter[modelIndex] = 4;
		memset(soundsUsed + modelIndex * 8, 0, 8);
	}

	short randomSoundIndex = 0;
	do
	{
		randomSoundIndex = XwaRand() & 0x07;
	} while (soundsUsed[modelIndex * 8 + randomSoundIndex] != 0);

	soundsUsed[modelIndex * 8 + randomSoundIndex] = 1;
	soundsCounter[modelIndex]--;
	soundIndex += randomSoundIndex;

	params[Params_EAX] = soundIndex;
	params[Params_ReturnAddress] = 0x0043BD13;
	return 0;
}

int HyperStartSoundHook(int* params)
{
	const int A4 = params[0];
	const int A8 = params[1];
	const int playerIndex = params[2];

	const auto playSound = (int(*)(int, int, int))0x0043BF90;

	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	const short iff = XwaPlayers[playerIndex].Iff;
	const int modelIndex = XwaObjects[XwaPlayers[playerIndex].ObjectIndex].ModelIndex;

	int behavior = g_modelIndexSound.GetHyperBehavior(modelIndex);

	if (behavior == -1)
	{
		const auto& soundConfig = GetSoundsConfig();

		if (soundConfig.SoundsCountHookExists && soundConfig.SfxHyperStartCount)
		{
			if (modelIndex < soundConfig.SfxHyperStartCount)
			{
				int sound = soundConfig.SfxHyperStartIndex + modelIndex;
				playSound(sound, 0xFFFF, playerIndex);
			}
		}
		else
		{
			playSound(iff == 1 ? 0x73 : 0x76, 0xFFFF, playerIndex);
		}
	}
	else
	{
		switch (behavior)
		{
		case 0:
			// reb start
			playSound(0x76, 0xFFFF, playerIndex);
			break;

		case 1:
			// imp start
			playSound(0x73, 0xFFFF, playerIndex);
			break;

		case 2:
			// imp start + zoom
			playSound(0x73, 0xFFFF, playerIndex);
			break;
		}
	}

	return 0;
}

int HyperZoomSoundHook(int* params)
{
	const int A4 = params[0];
	const int A8 = params[1];
	const int playerIndex = params[2];

	const auto playSound = (int(*)(int, int, int))0x0043BF90;

	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	const short iff = XwaPlayers[playerIndex].Iff;
	const int modelIndex = XwaObjects[XwaPlayers[playerIndex].ObjectIndex].ModelIndex;

	int behavior = g_modelIndexSound.GetHyperBehavior(modelIndex);

	if (behavior == -1)
	{
		const auto& soundConfig = GetSoundsConfig();

		if (soundConfig.SoundsCountHookExists && soundConfig.SfxHyperZoomCount)
		{
			if (modelIndex < soundConfig.SfxHyperZoomCount)
			{
				int sound = soundConfig.SfxHyperZoomIndex + modelIndex;
				playSound(sound, 0xFFFF, playerIndex);
			}
		}
		else
		{
			if (iff == 1)
			{
				playSound(0x74, 0xFFFF, playerIndex);
			}
		}
	}
	else
	{
		switch (behavior)
		{
		case 0:
			// reb start
			break;

		case 1:
			// imp start
			break;

		case 2:
			// imp start + zoom
			playSound(0x74, 0xFFFF, playerIndex);
			break;
		}
	}

	return 0;
}

bool HasRegionInterdictor(int region)
{
	std::vector<std::string> lines = GetCustomFileLines("Interdiction");

	const auto values = Tokennize(GetFileKeyValue(lines, "Region"));

	for (const std::string& value : values)
	{
		int index = std::stoi(value);

		if (index == region)
		{
			return true;
		}
	}

	return false;
}

int HyperEndSoundHook(int* params)
{
	const int A4 = params[0];
	const int A8 = params[1];
	const int playerIndex = params[2];
	const auto playSound = (int(*)(int, int, int))0x0043BF90;

	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	const int modelIndex = XwaObjects[XwaPlayers[playerIndex].ObjectIndex].ModelIndex;
	const int region = XwaPlayers[playerIndex].m053;

	if (HasRegionInterdictor(region))
	{
		const auto& soundConfig = GetSoundsConfig();

		if (soundConfig.SoundsCountHookExists && soundConfig.SfxHyperAbortCount)
		{
			if (modelIndex < soundConfig.SfxHyperAbortCount)
			{
				int sound = soundConfig.SfxHyperAbortIndex + modelIndex;
				playSound(sound, 0xFFFF, playerIndex);
			}
		}
		else
		{
			// HyperAbort.wav
			playSound(0x77, 0xFFFF, playerIndex);
		}
	}
	else
	{
		const auto& soundConfig = GetSoundsConfig();

		if (soundConfig.SoundsCountHookExists && soundConfig.SfxHyperEndCount)
		{
			if (modelIndex < soundConfig.SfxHyperEndCount)
			{
				int sound = soundConfig.SfxHyperEndIndex + modelIndex;
				playSound(sound, 0xFFFF, playerIndex);
			}
		}
		else
		{
			// HyperEnd.wav
			playSound(0x75, 0xFFFF, playerIndex);
		}
	}

	return 0;
}

void MobileObjectTransform(const XwaMobileObject* mobileObject, int x, int y, int z, int* newX, int* newY, int* newZ)
{
	int tempX =
		(int)((long long)mobileObject->TransformMatrixRightX * x / 32767)
		+ (int)((long long)mobileObject->TransformMatrixRightY * y / 32767)
		+ (int)((long long)mobileObject->TransformMatrixRightZ * z / 32767);

	int tempY =
		(int)((long long)mobileObject->TransformMatrixFrontX * x / 32767)
		+ (int)((long long)mobileObject->TransformMatrixFrontY * y / 32767)
		+ (int)((long long)mobileObject->TransformMatrixFrontZ * z / 32767);

	int tempZ =
		(int)((long long)mobileObject->TransformMatrixUpX * x / 32767)
		+ (int)((long long)mobileObject->TransformMatrixUpY * y / 32767)
		+ (int)((long long)mobileObject->TransformMatrixUpZ * z / 32767);

	*newX = tempX;
	*newY = -tempY;
	*newZ = tempZ;
}

void MobileObjectTransformBack(const XwaMobileObject* mobileObject, int x, int y, int z, int* newX, int* newY, int* newZ)
{
	y = -y;

	int tempX =
		(int)((long long)mobileObject->TransformMatrixRightX * x / 32767)
		+ (int)((long long)mobileObject->TransformMatrixFrontX * y / 32767)
		+ (int)((long long)mobileObject->TransformMatrixUpX * z / 32767);

	int tempY =
		(int)((long long)mobileObject->TransformMatrixRightY * x / 32767)
		+ (int)((long long)mobileObject->TransformMatrixFrontY * y / 32767)
		+ (int)((long long)mobileObject->TransformMatrixUpY * z / 32767);

	int tempZ =
		(int)((long long)mobileObject->TransformMatrixRightZ * x / 32767)
		+ (int)((long long)mobileObject->TransformMatrixFrontZ * y / 32767)
		+ (int)((long long)mobileObject->TransformMatrixUpZ * z / 32767);

	*newX = tempX;
	*newY = tempY;
	*newZ = tempZ;
}

float GetRelativeDistance(const XwaObject* playerObject, const XwaObject* targetObject)
{
	const float ScaleFactor = 0.0244140625f;

	AmbientSoundSettings ambientSound = g_modelIndexSound.GetAmbientSound(targetObject->ModelIndex);

	int targetX = targetObject->PositionX
		+ targetObject->pMobileObject->TransformMatrixFrontX * (int)(ambientSound.OffsetZ / ScaleFactor) / 32767
		+ targetObject->pMobileObject->TransformMatrixRightX * (int)(ambientSound.OffsetX / ScaleFactor) / 32767
		+ targetObject->pMobileObject->TransformMatrixUpX * (int)(ambientSound.OffsetY / ScaleFactor) / 32767;

	int targetY = targetObject->PositionY
		+ targetObject->pMobileObject->TransformMatrixFrontY * (int)(ambientSound.OffsetZ / ScaleFactor) / 32767
		+ targetObject->pMobileObject->TransformMatrixRightY * (int)(ambientSound.OffsetX / ScaleFactor) / 32767
		+ targetObject->pMobileObject->TransformMatrixUpY * (int)(ambientSound.OffsetY / ScaleFactor) / 32767;

	int targetZ = targetObject->PositionZ
		+ targetObject->pMobileObject->TransformMatrixFrontZ * (int)(ambientSound.OffsetZ / ScaleFactor) / 32767
		+ targetObject->pMobileObject->TransformMatrixRightZ * (int)(ambientSound.OffsetX / ScaleFactor) / 32767
		+ targetObject->pMobileObject->TransformMatrixUpZ * (int)(ambientSound.OffsetY / ScaleFactor) / 32767;

	int offsetX = playerObject->PositionX - targetX;
	int offsetY = playerObject->PositionY - targetY;
	int offsetZ = playerObject->PositionZ - targetZ;

	int distX, distY, distZ;
	MobileObjectTransformBack(targetObject->pMobileObject, offsetX, offsetY, offsetZ, &distX, &distY, &distZ);

	int sizeX = (int)(ambientSound.DistanceX / ScaleFactor);
	int sizeY = (int)(ambientSound.DistanceY / ScaleFactor);
	int sizeZ = (int)(ambientSound.DistanceZ / ScaleFactor);

	float dX = ((float)distX / (float)sizeX);
	float dY = ((float)distY / (float)sizeY);
	float dZ = ((float)distZ / (float)sizeZ);
	float d = dX * dX + dY * dY + dZ * dZ;

	return d;
}

std::vector<int> GetCustomSoundIndices()
{
	const auto& soundConfig = GetSoundsConfig();
	const int* xwaSoundEffectsBufferId = GetSoundsConfig().SoundEffectIds;

	const int modelIndexDistanceCount = 223;

	std::vector<int> sounds;

	for (int modelIndex = 0; modelIndex < modelIndexDistanceCount; modelIndex++)
	{
		int soundSlot = -1;

		if (soundConfig.SoundsCountHookExists && soundConfig.SfxStarshipAmbientCount)
		{
			if (modelIndex < soundConfig.SfxStarshipAmbientCount)
			{
				soundSlot = soundConfig.SfxStarshipAmbientIndex + modelIndex;
			}
		}

		if (soundSlot == -1 || xwaSoundEffectsBufferId[soundSlot] == -1)
		{
			sounds.push_back(-1);
		}
		else
		{
			sounds.push_back(xwaSoundEffectsBufferId[soundSlot]);
		}
	}

	return sounds;
}

int CustomSoundsHook(int* params)
{
	const float ScaleFactor = 0.0244140625f;

	((void(*)())0x00402D20)();

	if (!g_config.AreStarshipAmbientEnabled)
	{
		return 0;
	}

	const auto XwaStopSound = (char(*)(int))0x004DC400;
	const auto XwaGetPlayingSoundsCount = (int(*)(int))0x004DC850;
	const auto XwaQueueSound = (int(*)(int, int, int, int, int, int, int, int))0x004DB1A0;
	const auto XwaSetSoundVolume = (char(*)(int, int))0x004DC5D0;

	const auto& soundConfig = GetSoundsConfig();
	const int* xwaSoundEffectsBufferId = GetSoundsConfig().SoundEffectIds;

	const int modelIndexDistanceCount = 223;
	static std::vector<int> modelIndexToBufferId = GetCustomSoundIndices();

	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	const int currentPlayerId = *(int*)0x008C1CC8;
	const int currentPlayerObjectIndex = XwaPlayers[currentPlayerId].MapState != 0 ? XwaPlayers[currentPlayerId].AiObjectIndex : XwaPlayers[currentPlayerId].ObjectIndex;
	const XwaObject* currentPlayerObject = &XwaObjects[currentPlayerObjectIndex];
	unsigned char currentPlayerRegion = XwaPlayers[currentPlayerId].Region;

	bool isPlayerInHangar = *(int*)0x09C6E40 != 0;
	const int currentGameState = *(int*)(0x009F60E0 + 0x25FA9);
	const int updateCallback = *(int*)(0x009F60E0 + 0x25FB1 + 0x850 * currentGameState + 0x0844);
	const bool isConfigMenuGameStateUpdate = updateCallback == 0x0051D100;

	((void(*)(int))0x004154A0)(currentPlayerRegion);

	if (currentPlayerObjectIndex == 0xffff || isPlayerInHangar || isConfigMenuGameStateUpdate)
	{
		for (const int& bufferId : modelIndexToBufferId)
		{
			if (bufferId == -1)
			{
				continue;
			}

			int soundsCount = XwaGetPlayingSoundsCount(bufferId);

			if (soundsCount != 0)
			{
				XwaStopSound(bufferId);
			}
		}

		return 0;
	}

	std::map<int, float> minDistances;

	for (int objectIndex = *(int*)0x08BF378; objectIndex < *(int*)0x08D9628; objectIndex++)
	{
		const XwaObject* pObject = &XwaObjects[objectIndex];
		const unsigned short modelIndex = pObject->ModelIndex;

		if (modelIndex == 0 || modelIndex >= modelIndexDistanceCount || pObject->Region != currentPlayerRegion || pObject->PlayerIndex == currentPlayerId)
		{
			continue;
		}

		unsigned char category = XwaObjects[objectIndex].ShipCategory;

		//if (category != ShipCategory_Starship && category != ShipCategory_Freighter && category != ShipCategory_Platform)
		//{
		//	continue;
		//}

		float distance = GetRelativeDistance(currentPlayerObject, pObject);

		int bufferId = modelIndexToBufferId[modelIndex];

		if (bufferId == -1)
		{
			continue;
		}

		float minDistance = FLT_MAX;

		auto it = minDistances.find(bufferId);

		if (it != minDistances.end())
		{
			minDistance = it->second;
		}

		if (distance < minDistance)
		{
			minDistances[bufferId] = distance;
		}
	}

	for (const auto& buffer : minDistances)
	{
		int bufferId = buffer.first;
		float minDistance = buffer.second;

		int soundsCount = XwaGetPlayingSoundsCount(bufferId);

		if (minDistance == FLT_MAX)
		{
			if (soundsCount != 0)
			{
				XwaStopSound(bufferId);
			}

			continue;
		}

		if (minDistance < 1.0f)
		{
			int volume = (int)(0x7E * (1.0f - minDistance)) + 0x01;

			if (soundsCount == 0)
			{
				XwaQueueSound(bufferId, 0x01, 0x01, 0x7F, 0x41, 0x40, -1, 0xFFFF);
				XwaSetSoundVolume(bufferId, volume);
			}
			else
			{
				XwaSetSoundVolume(bufferId, volume);
			}
		}
		else
		{
			if (soundsCount != 0)
			{
				XwaStopSound(bufferId);
			}
		}
	}

	for (const int& bufferId : modelIndexToBufferId)
	{
		if (bufferId == -1)
		{
			continue;
		}

		if (minDistances.count(bufferId) != 0)
		{
			continue;
		}

		int soundsCount = XwaGetPlayingSoundsCount(bufferId);

		if (soundsCount != 0)
		{
			XwaStopSound(bufferId);
		}
	}

	return 0;
}

int PlayerCraftTargetedSoundHook(int* params)
{
	const auto L0043F4C0 = (int(*)(int, int, int, int, int, int))0x0043F4C0;

	const auto& soundConfig = GetSoundsConfig();

	// AlarmDanger.wav
	int sound = 0x3D;

	if (soundConfig.SoundsCountHookExists && soundConfig.SfxSoundsCount)
	{
		if (SfxSounds_PlayerCraftTargetedSound < soundConfig.SfxSoundsCount)
		{
			sound = soundConfig.SfxSoundsIndex + SfxSounds_PlayerCraftTargetedSound;
		}
	}

	L0043F4C0(sound, 0, 0, 0, 0xFFFF, 0xFFFF);

	return 0;
}

int MapEnterSoundHook(int* params)
{
	const int returnAddress = params[7];
	const bool isMapEnter = returnAddress == 0x00501603;

	const int A4 = 0x4B;
	const int A8 = 0xFFFF;
	const int AC = params[Params_EAX];

	const auto XwaPlaySoundByIndex = (int(*)(int, int, int))0x0043BF90;

	const auto& soundConfig = GetSoundsConfig();

	if (isMapEnter)
	{
		// MapEnter.wav
		int sound = -1;

		if (soundConfig.SoundsCountHookExists && soundConfig.SfxSoundsCount)
		{
			if (SfxSounds_MapEnterSound < soundConfig.SfxSoundsCount)
			{
				sound = soundConfig.SfxSoundsIndex + SfxSounds_MapEnterSound;
			}
		}

		if (sound != -1)
		{
			XwaPlaySoundByIndex(sound, A8, AC);
		}
	}

	unsigned short objectIndex = (unsigned short)params[Params_EBX];
	unsigned short playerCurrentTargetIndex = *(unsigned short*)(0x8B9505 + params[Params_EDI]);

	if (objectIndex == playerCurrentTargetIndex)
	{
		/*
		if( A4 == s_XwaPlayers[A8].CurrentTargetIndex )
			return;
		*/

		params[Params_ReturnAddress] = 0x005041B7;
		return 0;
	}

	if (!isMapEnter)
	{
		// TargetSelect.wav
		int sound = 0x4B;

		if (soundConfig.SoundsCountHookExists && soundConfig.SfxSoundsCount)
		{
			if (SfxSounds_TargetSelectSound < soundConfig.SfxSoundsCount)
			{
				sound = soundConfig.SfxSoundsIndex + SfxSounds_TargetSelectSound;
			}
		}

		XwaPlaySoundByIndex(sound, A8, AC);
	}

	return 0;
}

int MapExitSoundHook(int* params)
{
	const int A4 = params[0];
	const int A8 = params[1];
	const int AC = params[2];

	const auto XwaPlaySoundByIndex = (int(*)(int, int, int))0x0043BF90;

	const auto& soundConfig = GetSoundsConfig();

	// BombFire.wav
	int sound = 0x11;

	if (soundConfig.SoundsCountHookExists && soundConfig.SfxSoundsCount)
	{
		if (SfxSounds_MapExitSound < soundConfig.SfxSoundsCount)
		{
			sound = soundConfig.SfxSoundsIndex + SfxSounds_MapExitSound;
		}
	}

	XwaPlaySoundByIndex(sound, A8, AC);

	return 0;
}

int TurretSwitchSoundHook(int* params)
{
	const int A4 = params[0];
	const int A8 = params[1];
	const int AC = params[2];

	const auto XwaPlaySoundByIndex = (int(*)(int, int, int))0x0043BF90;

	const auto& soundConfig = GetSoundsConfig();

	// BombFire.wav
	int sound = 0x11;

	if (soundConfig.SoundsCountHookExists && soundConfig.SfxSoundsCount)
	{
		if (SfxSounds_TurretSwitchSound < soundConfig.SfxSoundsCount)
		{
			sound = soundConfig.SfxSoundsIndex + SfxSounds_TurretSwitchSound;
		}
	}

	XwaPlaySoundByIndex(sound, A8, AC);

	return 0;
}

std::string GetFilePathParent(const std::string& path)
{
	return path.substr(0, path.find_last_of("/\\"));
}

int LoadSoundsHook(int* params)
{
	const int A4 = params[1];
	const char* A8 = (const char*)params[2];
	void* AC = (void*)params[3];
	int* A10 = (int*)params[4];
	int* A14 = (int*)params[5];

	const auto L00539320 = (int(*)(const void*, void*, int*, int*))0x00539320;

	std::ifstream file(A8, std::ios::binary);

	if (file)
	{
		static std::vector<char> _buffer;

		file.seekg(0, std::ios::end);
		size_t size = (size_t)file.tellg();
		file.seekg(0, std::ios::beg);

		if (_buffer.capacity() < size)
		{
			_buffer.reserve(size * 2);
		}

		file.read(_buffer.data(), size);

		if (L00539320(_buffer.data(), AC, A10, A14) == 0)
		{
			return 0;
		}

		return 1;
	}

	static std::string _zipFilename;
	static bool _zipFileOpened = false;
	static std::vector<char> _zipBuffer;
	static std::map<std::string, std::vector<char>> _zipFiles;

	std::string directoryPath = GetFilePathParent(A8);
	std::string zipPath = directoryPath + ".zip";

	if (zipPath != _zipFilename)
	{
		_zipFilename = zipPath;
		_zipFileOpened = false;
		_zipFiles.clear();

		std::ifstream fileArchiveStream(_zipFilename, std::ios::binary);

		if (fileArchiveStream)
		{
			fileArchiveStream.seekg(0, std::ios::end);
			size_t fileArchiveStreamSize = (size_t)fileArchiveStream.tellg();
			fileArchiveStream.seekg(0, std::ios::beg);

			if (_zipBuffer.capacity() < fileArchiveStreamSize)
			{
				_zipBuffer.reserve(fileArchiveStreamSize * 2);
			}

			fileArchiveStream.read(_zipBuffer.data(), fileArchiveStreamSize);

			mz_zip_archive zip{};
			mz_zip_reader_init_mem(&zip, _zipBuffer.data(), fileArchiveStreamSize, 0);

			for (unsigned int fileIndex = 0; fileIndex < zip.m_total_files; fileIndex++)
			{
				mz_zip_archive_file_stat stat{};
				mz_zip_reader_file_stat(&zip, fileIndex, &stat);

				std::string name(stat.m_filename);

				if (name.end()[-1] == '/')
				{
					continue;
				}

				name = directoryPath + "/" + name;
				std::replace(name.begin(), name.end(), '\\', '/');
				std::transform(name.begin(), name.end(), name.begin(), _toupper);

				std::vector<char> extracted;
				extracted.resize((size_t)stat.m_uncomp_size);
				mz_zip_reader_extract_to_mem(&zip, fileIndex, extracted.data(), extracted.size(), 0);

				_zipFiles.emplace(name, extracted);
			}

			mz_zip_reader_end(&zip);

			_zipFileOpened = true;
		}
	}

	if (_zipFileOpened)
	{
		std::string soundFilename = std::string(A8);
		std::replace(soundFilename.begin(), soundFilename.end(), '\\', '/');
		std::transform(soundFilename.begin(), soundFilename.end(), soundFilename.begin(), _toupper);

		auto it = _zipFiles.find(soundFilename);

		if (it != _zipFiles.end())
		{
			const char* data = it->second.data();

			if (L00539320(data, AC, A10, A14) == 0)
			{
				return 0;
			}

			return 1;
		}
	}

	return 0;
}
