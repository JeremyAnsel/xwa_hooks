#include "targetver.h"
#include "weapon.h"
#include "config.h"
#include <fstream>
#include <map>
#include <utility>
#include <Windows.h>

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
		this->Craft_Offset_2CF = 0x3F9 + GetFileKeyValueInt(lines, "Craft_Offset_2CF", 0);
		this->Craft_Offset_2DF = 0x3F9 + GetFileKeyValueInt(lines, "Craft_Offset_2DF", 0x2DF - 0x3F9);
	}

	int MeshesCount;
	int Craft_Size;
	int Craft_Offset_22E;
	int Craft_Offset_260;
	int Craft_Offset_292;
	int Craft_Offset_2CF;
	int Craft_Offset_2DF;
};

CraftConfig g_craftConfig;

class Config
{
public:
	Config()
	{
		auto lines = GetFileLines("hook_weapon_rate.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_weapon_rate");
		}

		this->EnableFireMeshFilter = GetFileKeyValueInt(lines, "EnableFireMeshFilter", 1) != 0;
		this->EnableWeaponStatsLog = GetFileKeyValueInt(lines, "EnableWeaponStatsLog", 0) != 0;
		this->EnableWeaponStatsProfiles = GetFileKeyValueInt(lines, "EnableWeaponStatsProfiles", 1) != 0;
		this->PreventAILasersIonWhenDisabling = GetFileKeyValueInt(lines, "PreventAILasersIonWhenDisabling", 0) != 0;
		this->PreventAIIonWhenNotDisabling = GetFileKeyValueInt(lines, "PreventAIIonWhenNotDisabling", 0) != 0;
		this->PreventPlayerLinkedLasersIon = GetFileKeyValueInt(lines, "PreventPlayerLinkedLasersIon", 0) != 0;
		this->LimitLinkedWarheadFireRate = GetFileKeyValueInt(lines, "LimitLinkedWarheadFireRate", 0) != 0;
		this->MaxTorpedoCountPerPass = GetFileKeyValueInt(lines, "MaxTorpedoCountPerPass", 6);
		this->MaxTorpedoCountPerTarget = GetFileKeyValueInt(lines, "MaxTorpedoCountPerTarget", 16);
		this->WeaponsCount = GetFileKeyValueInt(lines, "WeaponsCount", 12);
		this->DechargeRatePercent = GetFileKeyValueInt(lines, "DechargeRatePercent", 100);
		this->RechargeRatePercent = GetFileKeyValueInt(lines, "RechargeRatePercent", 100);
	}

	bool EnableFireMeshFilter;
	bool EnableWeaponStatsLog;
	bool EnableWeaponStatsProfiles;
	bool PreventAILasersIonWhenDisabling;
	bool PreventAIIonWhenNotDisabling;
	bool PreventPlayerLinkedLasersIon;
	bool LimitLinkedWarheadFireRate;
	int MaxTorpedoCountPerPass;
	int MaxTorpedoCountPerTarget;
	int WeaponsCount;
	int DechargeRatePercent;
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

enum MeshTypeEnum : unsigned int
{
	MeshType_Default = 0,
	MeshType_MainHull = 1,
	MeshType_Wing = 2,
	MeshType_Fuselage = 3,
	MeshType_GunTurret = 4,
	MeshType_SmallGun = 5,
	MeshType_Engine = 6,
	MeshType_Bridge = 7,
	MeshType_ShieldGen = 8,
	MeshType_EnergyGen = 9,
	MeshType_Launcher = 10,
	MeshType_CommSys = 11,
	MeshType_BeamSys = 12,
	MeshType_CommandSys = 13,
	MeshType_DockingPlat = 14,
	MeshType_LandingPlat = 15,
	MeshType_Hangar = 16,
	MeshType_CargoPod = 17,
	MeshType_MiscHull = 18,
	MeshType_Antenna = 19,
	MeshType_RotaryWing = 20,
	MeshType_RotaryGunTurret = 21,
	MeshType_RotaryLauncher = 22,
	MeshType_RotaryCommSys = 23,
	MeshType_RotaryBeamSys = 24,
	MeshType_RotaryCommandSys = 25,
	MeshType_Hatch = 26,
	MeshType_Custom = 27,
	MeshType_WeaponSys1 = 28,
	MeshType_WeaponSys2 = 29,
	MeshType_PowerReg = 30,
	MeshType_Reactor = 31,
};

struct OptRotationScaleNode_17_Data
{
	float PivotX;
	float PivotY;
	float PivotZ;
	float LookX;
	float LookY;
	float LookZ;
	float UpX;
	float UpY;
	float UpZ;
	float RightX;
	float RightY;
	float RightZ;
};

static_assert(sizeof(OptRotationScaleNode_17_Data) == 48, "size of OptRotationScaleNode_17_Data must be 48");

struct OptMeshDescriptorNode_19_Data
{
	MeshTypeEnum MeshType;
	unsigned int ExplosionType; // flags
	float SpanX;
	float SpanY;
	float SpanZ;
	float CenterX;
	float CenterY;
	float CenterZ;
	float MinX;
	float MinY;
	float MinZ;
	float MaxX;
	float MaxY;
	float MaxZ;
	int TargetId;
	float TargetX;
	float TargetY;
	float TargetZ;
};

static_assert(sizeof(OptMeshDescriptorNode_19_Data) == 72, "size of OptMeshDescriptorNode_19_Data must be 72");

struct ExeCraftWeaponSlot
{
	short PositionX;
	short PositionZ;
	short PositionY;
	unsigned char HardpointId;
	unsigned char MeshId;
};

static_assert(sizeof(ExeCraftWeaponSlot) == 8, "size of ExeCraftWeaponSlot must be 8");

struct ExeCraftEntry
{
	char Unk0000[354];
	ExeCraftWeaponSlot WeaponSlots[16];
	char Unk01E2[310];
	unsigned char NumOfFloatHardPts;
	char Unk0319[194];
};

static_assert(sizeof(ExeCraftEntry) == 987, "size of ExeCraftEntry must be 987");

struct XwaFloatHardPt
{
	int Position[3];
	unsigned char MeshId;
};

static_assert(sizeof(XwaFloatHardPt) == 13, "size of XwaFloatHardPt must be 13");

struct XwaCraftWeaponRack
{
	unsigned short ModelIndex;
	char Sequence;
	char Unk0003[1];
	char Charge;
	char Unk0005[3];
	unsigned char MeshId;
	unsigned char HardpointId;
	char Unk000A[4];
};

static_assert(sizeof(XwaCraftWeaponRack) == 14, "size of XwaCraftWeaponRack must be 14");

struct XwaCraft
{
	char Unk0000[4];
	short CraftIndex;
	char Unk0006[424];
	char WeaponRacksCount;
	char Unk01AF[16];
	short m1BF[3];
	int m1C5[3];
	char Unk01D1[1];
	unsigned short WarheadsModelIndex[2];
	unsigned char m1D6[2];
	short m1D8[2];
	char Unk01DC[82];
	char XwaCraft_m22E[50];
	char XwaCraft_m260[50];
	char XwaCraft_m292[50];
	char Unk02C4[27];
	XwaCraftWeaponRack WeaponRacks[16];
	char Unk03BF[58];
};

static_assert(sizeof(XwaCraft) == 1017, "size of XwaCraft must be 1017");

struct XwaMobileObject
{
	char Unk0000[141];
	int Duration;
	char Unk0091[2];
	short ObjectIndex;
	unsigned short ModelIndex;
	unsigned char Iff;
	char Unk0098[39];
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
	char Unk00D9[4];
	XwaCraft* pCraft;
	char Unk00E1[4];
};

static_assert(sizeof(XwaMobileObject) == 229, "size of XwaMobileObject must be 229");

struct XwaObject
{
	char Unk0000[2];
	short ModelIndex;
	ShipCategoryEnum ShipCategory;
	unsigned char TieFlightGroupIndex;
	unsigned char Region;
	int PositionX;
	int PositionY;
	int PositionZ;
	short HeadingXY;
	short HeadingZ;
	short HeadingRoll;
	short XwaObject_m19;
	char Unk001B[4];
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
	char unk074[3534];
};

static_assert(sizeof(TieFlightGroupEx) == 3650, "size of TieFlightGroupEx must be 3650");

struct XwaPlayer
{
	int ObjectIndex;
	char Unk0004[3019];
};

static_assert(sizeof(XwaPlayer) == 3023, "size of XwaPlayer must be 3023");

#pragma pack(pop)

TieFlightGroupEx* s_XwaTieFlightGroups = (TieFlightGroupEx*)0x80DC80;

int GetHangarWarheadMaxCount(int fgIndex, int warheadType, int capacity);

std::vector<std::string> GetShipLines(int modelIndex)
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

	lines = GetFileLines("FlightModels\\WeaponRate.txt");

	if (!lines.size())
	{
		lines = GetFileLines("FlightModels\\default.ini", "WeaponRate");
	}

	if (!lines.size())
	{
		lines = GetFileLines(ship + "WeaponRate.txt");
	}

	if (!lines.size())
	{
		lines = GetFileLines(ship + ".ini", "WeaponRate");
	}

	return lines;
}

std::vector<std::string> GetModelLines(int modelIndex, const std::string& name)
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

	lines = GetFileLines(ship + name + ".txt");

	if (!lines.size())
	{
		lines = GetFileLines(ship + ".ini", name);
	}

	if (!lines.size())
	{
		lines = GetFileLines("FlightModels\\" + name + ".txt");
	}

	if (!lines.size())
	{
		lines = GetFileLines("FlightModels\\default.ini", name);
	}

	return lines;
}

std::vector<std::string> GetMissionLines(const std::string& name)
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

int GetMissionWeaponRate(int fgIndex, const std::string& name)
{
	auto lines = GetMissionLines("WeaponRates");
	int rate = GetFileKeyValueInt(lines, name + "_fg_" + std::to_string(fgIndex), -1);
	return rate;
}

int GetWeaponDechargeRate(unsigned short modelIndex, int fgIndex)
{
	auto lines = GetShipLines(modelIndex);

	int rate = GetMissionWeaponRate(fgIndex, "DechargeRate");

	if (rate == -1)
	{
		rate = GetFileKeyValueInt(lines, "DechargeRate", -1);
	}

	if (rate == -1)
	{
		const int xwaObjectStats = 0x05FB240;
		const char category = *(char*)(xwaObjectStats + modelIndex * 0x18 + 0x03);

		// ShipCategory_Starfighter
		if (category == 0)
		{
			switch (modelIndex)
			{
			case 5: // ModelIndex_005_0_4_TieFighter
			case 7: // ModelIndex_007_0_6_TieBomber
				rate = 3;
				break;

			default:
				rate = 4;
				break;
			}
		}
		else
		{
			rate = 3;
		}
	}

	return rate;
}

int GetWeaponRechargeRate(unsigned short modelIndex, int fgIndex)
{
	auto lines = GetShipLines(modelIndex);

	int rate = GetMissionWeaponRate(fgIndex, "RechargeRate");

	if (rate == -1)
	{
		rate = GetFileKeyValueInt(lines, "RechargeRate", -1);
	}

	if (rate == -1)
	{
		switch (modelIndex)
		{
		case 5: // ModelIndex_005_0_4_TieFighter
		case 7: // ModelIndex_007_0_6_TieBomber
			rate = 3;
			break;

		default:
			rate = 2;
			break;
		}
	}

	return rate;
}

int GetWeaponCooldownTime(int modelIndex)
{
	auto lines = GetShipLines(modelIndex);

	int rate = GetFileKeyValueInt(lines, "CooldownTimeFactor", 0x2F);

	return rate;
}

int GetModelEnergyTransferRate(int objectIndex)
{
	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	unsigned short modelIndex = XwaObjects[objectIndex].ModelIndex;
	int fgIndex = XwaObjects[objectIndex].TieFlightGroupIndex;

	auto lines = GetShipLines(modelIndex);

	int rate = GetMissionWeaponRate(fgIndex, "EnergyTransferRate");

	if (rate == -1)
	{
		rate = GetFileKeyValueInt(lines, "EnergyTransferRate", -1);
	}

	if (rate == -1)
	{
		switch (modelIndex)
		{
		case 12: // ModelIndex_012_0_11_MissileBoat
			rate = 32;
			break;

		default:
			//rate = 4;
			rate = *(int*)0x004FD317;
			break;
		}
	}

	return rate;
}

int GetModelEnergyTransferRatePenalty(int objectIndex)
{
	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	unsigned short modelIndex = XwaObjects[objectIndex].ModelIndex;
	int fgIndex = XwaObjects[objectIndex].TieFlightGroupIndex;

	auto lines = GetShipLines(modelIndex);

	int penalty = GetMissionWeaponRate(fgIndex, "EnergyTransferRatePenalty");

	if (penalty == -1)
	{
		penalty = GetFileKeyValueInt(lines, "EnergyTransferRatePenalty", -1);
	}

	if (penalty == -1)
	{
		penalty = 0;
	}

	return penalty;
}

int GetModelEnergyTransferWeaponLimit(int objectIndex)
{
	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	unsigned short modelIndex = XwaObjects[objectIndex].ModelIndex;
	int fgIndex = XwaObjects[objectIndex].TieFlightGroupIndex;

	auto lines = GetShipLines(modelIndex);

	int limit = GetMissionWeaponRate(fgIndex, "EnergyTransferWeaponLimit");

	if (limit == -1)
	{
		limit = GetFileKeyValueInt(lines, "EnergyTransferWeaponLimit", -1);
	}

	if (limit == -1)
	{
		limit = 100;
	}

	return limit;
}

int GetModelEnergyTransferShieldLimit(int objectIndex)
{
	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	unsigned short modelIndex = XwaObjects[objectIndex].ModelIndex;
	int fgIndex = XwaObjects[objectIndex].TieFlightGroupIndex;

	auto lines = GetShipLines(modelIndex);

	int limit = GetMissionWeaponRate(fgIndex, "EnergyTransferShieldLimit");

	if (limit == -1)
	{
		limit = GetFileKeyValueInt(lines, "EnergyTransferShieldLimit", -1);
	}

	if (limit == -1)
	{
		limit = 800;
	}

	return limit;
}

int GetModelAIMaxTorpedoCountPerPass(int objectIndex)
{
	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	unsigned short modelIndex = XwaObjects[objectIndex].ModelIndex;
	int fgIndex = XwaObjects[objectIndex].TieFlightGroupIndex;

	auto lines = GetShipLines(modelIndex);

	int count = GetMissionWeaponRate(fgIndex, "MaxTorpedoCountPerPass");

	if (count == -1)
	{
		count = GetFileKeyValueInt(lines, "MaxTorpedoCountPerPass", -1);
	}

	if (count == -1)
	{
		count = g_config.MaxTorpedoCountPerPass;
	}

	return count;
}

int GetModelAIMaxTorpedoCountPerTarget(int objectIndex)
{
	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	unsigned short modelIndex = XwaObjects[objectIndex].ModelIndex;
	int fgIndex = XwaObjects[objectIndex].TieFlightGroupIndex;

	auto lines = GetShipLines(modelIndex);

	int count = GetMissionWeaponRate(fgIndex, "MaxTorpedoCountPerTarget");

	if (count == -1)
	{
		count = GetFileKeyValueInt(lines, "MaxTorpedoCountPerTarget", -1);
	}

	if (count == -1)
	{
		count = g_config.MaxTorpedoCountPerTarget;
	}

	return count;
}

bool GetModelIsImpactSpinningEnabled(int objectIndex)
{
	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	unsigned short modelIndex = XwaObjects[objectIndex].ModelIndex;
	int fgIndex = XwaObjects[objectIndex].TieFlightGroupIndex;

	auto lines = GetShipLines(modelIndex);

	int rate = GetMissionWeaponRate(fgIndex, "IsImpactSpinningEnabled");

	if (rate == -1)
	{
		rate = GetFileKeyValueInt(lines, "IsImpactSpinningEnabled", -1);
	}

	if (rate == -1)
	{
		rate = *(unsigned char*)0x00805415;
	}

	return rate != 0;
}

int GetModelImpactSpinningSpeedFactorPercent(int objectIndex)
{
	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	unsigned short modelIndex = XwaObjects[objectIndex].ModelIndex;
	int fgIndex = XwaObjects[objectIndex].TieFlightGroupIndex;

	auto lines = GetShipLines(modelIndex);

	int rate = GetMissionWeaponRate(fgIndex, "ImpactSpinningSpeedFactorPercent");

	if (rate == -1)
	{
		rate = GetFileKeyValueInt(lines, "ImpactSpinningSpeedFactorPercent", -1);
	}

	if (rate == -1)
	{
		rate = 100;
	}

	return rate;
}

int GetModelImpactSpinningAngleFactorPercent(int objectIndex)
{
	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	unsigned short modelIndex = XwaObjects[objectIndex].ModelIndex;
	int fgIndex = XwaObjects[objectIndex].TieFlightGroupIndex;

	auto lines = GetShipLines(modelIndex);

	int rate = GetMissionWeaponRate(fgIndex, "ImpactSpinningAngleFactorPercent");

	if (rate == -1)
	{
		rate = GetFileKeyValueInt(lines, "ImpactSpinningAngleFactorPercent", -1);
	}

	if (rate == -1)
	{
		rate = 100;
	}

	return rate;
}

struct WeaponStats
{
	unsigned short DurationIntegerPart;
	unsigned short DurationDecimalPart;
	short Score;
	short Speed;
	int Power;
	int PowerSpeedPercent;
	int FireRate;
	int DurationOffset;
	int FireRatio;
	int Range;
	bool IsPrecise;
	unsigned int DegreeOfSpreadMask;
	int DechargeRate;
	int RechargeRate;
	int EnergyLowHighSeparation;
	unsigned char Side;
	unsigned short SideModel;
};

struct WeaponStatsModifiers
{
	float Score;
	float Speed;
	float Power;
	float PowerSpeedPercent;
	float FireRate;
	float FireRatio;
	float Range;
	float DechargeRate;
	float RechargeRate;
};

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

WeaponStatsModifiers GetWeaponStatsModifiers(const XwaObject* currentObject)
{
	int playerIndex = currentObject->PlayerIndex;
	unsigned short modelIndex = currentObject->ModelIndex;
	unsigned char rank = currentObject->TieFlightGroupIndex == 0xff ? 0xff : s_XwaTieFlightGroups[currentObject->TieFlightGroupIndex].Rank;

	std::string shipPath = g_flightModelsList.GetLstLine(modelIndex);

	const auto objectLines = GetMissionLines("Objects");
	const std::string objectValue = GetFileKeyValue(objectLines, shipPath + ".opt");

	if (!objectValue.empty() && std::ifstream(objectValue))
	{
		shipPath = GetStringWithoutExtension(objectValue);
	}

	std::string playerKey = playerIndex == -1 ? "" : "_Player";
	std::string rankKey = GetRankKey(rank);
	std::string section = "WeaponStatsModifiers";

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

	WeaponStatsModifiers modifiers{};

	modifiers.Score = GetFileKeyValueFloat(lines, "Score", 1.0f);
	modifiers.Speed = GetFileKeyValueFloat(lines, "Speed", 1.0f);
	modifiers.Power = GetFileKeyValueFloat(lines, "Power", 1.0f);
	modifiers.PowerSpeedPercent = GetFileKeyValueFloat(lines, "PowerSpeedPercent", 1.0f);
	modifiers.FireRate = GetFileKeyValueFloat(lines, "FireRate", 1.0f);
	modifiers.FireRatio = GetFileKeyValueFloat(lines, "FireRatio", 1.0f);
	modifiers.Range = GetFileKeyValueFloat(lines, "Range", 1.0f);
	modifiers.DechargeRate = GetFileKeyValueFloat(lines, "DechargeRate", 1.0f);
	modifiers.RechargeRate = GetFileKeyValueFloat(lines, "RechargeRate", 1.0f);

	return modifiers;
}

int GetWeaponKeyValue(const std::vector<std::string>& lines, const std::string& key, const std::string& weaponKey, const std::string& playerKey, const std::string& difficultyKey)
{
	int value = -1;

	if (!playerKey.empty())
	{
		if (value == -1)
		{
			value = GetFileKeyValueInt(lines, weaponKey + playerKey + difficultyKey + key, -1);
		}

		if (value == -1)
		{
			value = GetFileKeyValueInt(lines, weaponKey + playerKey + key, -1);
		}
	}

	if (value == -1)
	{
		value = GetFileKeyValueInt(lines, weaponKey + difficultyKey + key, -1);
	}

	if (value == -1)
	{
		value = GetFileKeyValueInt(lines, weaponKey + key, -1);
	}

	return value;
}

WeaponStats GetWeaponStats(const XwaObject* sourceObject, const std::vector<std::string>& lines, int playerIndex, int sourceModelIndex, int sourceFgIndex, const std::string& profileName, int weaponIndex)
{
	const unsigned short* s_ExeWeaponDurationIntegerPart = (unsigned short*)0x005B6560;
	const unsigned short* s_ExeWeaponDurationDecimalPart = (unsigned short*)0x005B6598;
	const short* s_ExeWeaponScore = (short*)0x005B6628;
	const short* s_ExeWeaponSpeed = (short*)0x005B6528;
	const int* s_ExeWeaponPower = (int*)0x005B64B8;
	const unsigned char* s_ExeWeaponBehavior = (unsigned char*)0x005B6608;
	const unsigned char* s_ExeWeaponSide = (unsigned char*)0x005B6660;
	const unsigned short* s_ExeWeaponSideModel = (unsigned short*)0x005B6680;
	const unsigned char difficulty = *(unsigned char*)(0x08053E0 + 0x002A);

	//auto lines = GetModelLines(sourceModelIndex, "WeaponStats");

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

	WeaponStats stats{};
	std::string weaponKey = "Weapon" + std::to_string(280 + weaponIndex);

	if (!profileName.empty())
	{
		weaponKey = profileName + "_" + weaponKey;
	}

	int durationIntegerPart = GetWeaponKeyValue(lines, "_DurationIntegerPart", weaponKey, playerKey, difficultyKey);

	if (durationIntegerPart == -1 || !g_config.EnableWeaponStatsProfiles)
	{
		stats.DurationIntegerPart = s_ExeWeaponDurationIntegerPart[weaponIndex];
	}
	else
	{
		stats.DurationIntegerPart = (unsigned short)durationIntegerPart;
	}

	int durationDecimalPart = GetWeaponKeyValue(lines, "_DurationDecimalPart", weaponKey, playerKey, difficultyKey);

	if (durationDecimalPart == -1 || !g_config.EnableWeaponStatsProfiles)
	{
		stats.DurationDecimalPart = s_ExeWeaponDurationDecimalPart[weaponIndex];
	}
	else
	{
		stats.DurationDecimalPart = (unsigned short)durationDecimalPart;
	}

	int score = GetWeaponKeyValue(lines, "_Score", weaponKey, playerKey, difficultyKey);

	if (score == -1 || !g_config.EnableWeaponStatsProfiles)
	{
		stats.Score = s_ExeWeaponScore[weaponIndex];
	}
	else
	{
		stats.Score = (short)score;
	}

	int speed = GetWeaponKeyValue(lines, "_Speed", weaponKey, playerKey, difficultyKey);

	if (speed == -1 || !g_config.EnableWeaponStatsProfiles)
	{
		stats.Speed = s_ExeWeaponSpeed[weaponIndex];
	}
	else
	{
		stats.Speed = (short)speed;
	}

	int power = GetWeaponKeyValue(lines, "_Power", weaponKey, playerKey, difficultyKey);

	if (power == -1 || !g_config.EnableWeaponStatsProfiles)
	{
		stats.Power = s_ExeWeaponPower[weaponIndex];
	}
	else
	{
		stats.Power = power;
	}

	int powerSpeedPercent = GetWeaponKeyValue(lines, "_PowerSpeedPercent", weaponKey, playerKey, difficultyKey);

	if (powerSpeedPercent == -1 || !g_config.EnableWeaponStatsProfiles)
	{
		stats.PowerSpeedPercent = 100;
	}
	else
	{
		if (powerSpeedPercent < 0)
		{
			powerSpeedPercent = 0;
		}

		stats.PowerSpeedPercent = powerSpeedPercent;
	}

	int fireRate = GetWeaponKeyValue(lines, "_FireRate", weaponKey, playerKey, difficultyKey);

	if (fireRate == -1 || !g_config.EnableWeaponStatsProfiles)
	{
		if (s_ExeWeaponBehavior[weaponIndex] == 0)
		{
			stats.FireRate = GetWeaponCooldownTime(sourceModelIndex);
		}
		else
		{
			// ModelIndex_020_0_19_TieWarheads
			stats.FireRate = sourceModelIndex == 0x14 ? 0x76 : 0x1D8;
		}
	}
	else
	{
		stats.FireRate = fireRate;
	}

	int durationOffset = GetWeaponKeyValue(lines, "_DurationOffset", weaponKey, playerKey, difficultyKey);

	if (durationOffset == -1 || !g_config.EnableWeaponStatsProfiles)
	{
		stats.DurationOffset = 0x3B;
	}
	else
	{
		stats.DurationOffset = durationOffset;
	}

	int fireRatio = GetWeaponKeyValue(lines, "_FireRatio", weaponKey, playerKey, difficultyKey);

	if (fireRatio == -1 || !g_config.EnableWeaponStatsProfiles)
	{
		stats.FireRatio = -1;
	}
	else
	{
		stats.FireRatio = fireRatio;
	}

	int range = GetWeaponKeyValue(lines, "_Range", weaponKey, playerKey, difficultyKey);

	if (range == -1 || !g_config.EnableWeaponStatsProfiles)
	{
		stats.Range = -1;
	}
	else
	{
		stats.Range = range;
	}

	int isPrecise = GetWeaponKeyValue(lines, "_IsPrecise", weaponKey, playerKey, difficultyKey);

	if (isPrecise == -1 || !g_config.EnableWeaponStatsProfiles)
	{
		stats.IsPrecise = true;
	}
	else
	{
		stats.IsPrecise = isPrecise != 0;
	}

	int degreeOfSpreadMask = GetWeaponKeyValue(lines, "_DegreeOfSpreadMask", weaponKey, playerKey, difficultyKey);

	if (degreeOfSpreadMask == -1 || !g_config.EnableWeaponStatsProfiles)
	{
		stats.DegreeOfSpreadMask = 0xff;
	}
	else
	{
		stats.DegreeOfSpreadMask = (unsigned int)degreeOfSpreadMask;
	}

	int dechargeRate = GetWeaponKeyValue(lines, "_DechargeRate", weaponKey, playerKey, difficultyKey);

	if (dechargeRate == -1 || !g_config.EnableWeaponStatsProfiles)
	{
		stats.DechargeRate = GetWeaponDechargeRate(sourceModelIndex, sourceFgIndex);
	}
	else
	{
		stats.DechargeRate = dechargeRate;
	}

	stats.DechargeRate = stats.DechargeRate * g_config.DechargeRatePercent / 100;

	int rechargeRate = GetWeaponKeyValue(lines, "_RechargeRate", weaponKey, playerKey, difficultyKey);

	if (rechargeRate == -1 || !g_config.EnableWeaponStatsProfiles)
	{
		stats.RechargeRate = GetWeaponRechargeRate(sourceModelIndex, sourceFgIndex);
	}
	else
	{
		stats.RechargeRate = rechargeRate;
	}

	stats.RechargeRate = stats.RechargeRate * g_config.RechargeRatePercent / 100;

	int energyLowHighSeparation = GetWeaponKeyValue(lines, "_EnergyLowHighSeparation", weaponKey, playerKey, difficultyKey);

	if (energyLowHighSeparation == -1 || !g_config.EnableWeaponStatsProfiles)
	{
		stats.EnergyLowHighSeparation = 0x40;
	}
	else
	{
		stats.EnergyLowHighSeparation = energyLowHighSeparation;
	}

	int side = GetWeaponKeyValue(lines, "_Side", weaponKey, playerKey, difficultyKey);

	if (side == -1)
	{
		stats.Side = s_ExeWeaponSide[weaponIndex];
	}
	else
	{
		stats.Side = (unsigned char)side;
	}

	int sideModel = GetWeaponKeyValue(lines, "_SideModel", weaponKey, playerKey, difficultyKey);

	if (sideModel == -1)
	{
		stats.SideModel = s_ExeWeaponSideModel[weaponIndex];
	}
	else
	{
		stats.SideModel = (unsigned short)sideModel;
	}

	if (sourceObject != nullptr)
	{
		WeaponStatsModifiers modifiers = GetWeaponStatsModifiers(sourceObject);

		stats.Score = (short)(stats.Score * modifiers.Score);
		stats.Speed = (short)(stats.Speed * modifiers.Speed);
		stats.Power = (int)(stats.Power * modifiers.Power);
		stats.PowerSpeedPercent = (int)(stats.PowerSpeedPercent * modifiers.PowerSpeedPercent);
		stats.FireRate = (int)(stats.FireRate * modifiers.FireRate);
		stats.FireRatio = (int)(stats.FireRatio * modifiers.FireRatio);
		stats.Range = (int)(stats.Range * modifiers.Range);
		stats.DechargeRate = (int)(stats.DechargeRate * modifiers.DechargeRate);
		stats.RechargeRate = (int)(stats.RechargeRate * modifiers.RechargeRate);
	}

	return stats;
}

std::string GetWeaponProfileName(int fgIndex)
{
	auto lines = GetMissionLines("WeaponProfiles");

	std::string name = GetFileKeyValue(lines, "WeaponProfile_fg_" + std::to_string(fgIndex));

	if (_stricmp(name.c_str(), "Default") == 0)
	{
		name = std::string();
	}

	return name;
}

class ModelIndexWeapon
{
public:
	int GetDechargeRate(int objectIndex)
	{
		this->Update();

		XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
		unsigned short modelIndex = XwaObjects[objectIndex].ModelIndex;
		int fgIndex = XwaObjects[objectIndex].TieFlightGroupIndex;

		auto it = this->_weaponDechargeRate.find(fgIndex);

		if (it != this->_weaponDechargeRate.end())
		{
			return it->second;
		}
		else
		{
			//bool fpsLimit = *(unsigned char*)0x008C163F != 0;
			int value = GetWeaponDechargeRate(modelIndex, fgIndex);

			//if (fpsLimit)
			//{
			//	//value *= 2;
			//}

			this->_weaponDechargeRate.insert(std::make_pair(fgIndex, value));
			return value;
		}
	}

	int GetRechargeRate(int objectIndex)
	{
		this->Update();

		XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
		unsigned short modelIndex = XwaObjects[objectIndex].ModelIndex;
		int fgIndex = XwaObjects[objectIndex].TieFlightGroupIndex;

		auto it = this->_weaponRechargeRate.find(fgIndex);

		if (it != this->_weaponRechargeRate.end())
		{
			return it->second;
		}
		else
		{
			bool fpsLimit = *(unsigned char*)0x008C163F != 0;
			int value = GetWeaponRechargeRate(modelIndex, fgIndex);

			if (fpsLimit)
			{
				//value *= 2;
			}

			this->_weaponRechargeRate.insert(std::make_pair(fgIndex, value));
			return value;
		}
	}

	int GetCooldownTime(int modelIndex)
	{
		this->Update();

		auto it = this->_weaponCooldownTime.find(modelIndex);

		if (it != this->_weaponCooldownTime.end())
		{
			return it->second;
		}
		else
		{
			int value = GetWeaponCooldownTime(modelIndex);
			this->_weaponCooldownTime.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

	int GetEnergyTransferRate(int objectIndex)
	{
		this->Update();

		XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
		int fgIndex = XwaObjects[objectIndex].TieFlightGroupIndex;

		auto it = this->_modelEnergyTransferRate.find(fgIndex);

		if (it != this->_modelEnergyTransferRate.end())
		{
			return it->second;
		}
		else
		{
			int value = GetModelEnergyTransferRate(objectIndex);

			this->_modelEnergyTransferRate.insert(std::make_pair(fgIndex, value));
			return value;
		}
	}

	int GetEnergyTransferRatePenalty(int objectIndex)
	{
		this->Update();

		XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
		int fgIndex = XwaObjects[objectIndex].TieFlightGroupIndex;

		auto it = this->_modelEnergyTransferRatePenalty.find(fgIndex);

		if (it != this->_modelEnergyTransferRatePenalty.end())
		{
			return it->second;
		}
		else
		{
			int value = GetModelEnergyTransferRatePenalty(objectIndex);

			this->_modelEnergyTransferRatePenalty.insert(std::make_pair(fgIndex, value));
			return value;
		}
	}

	int GetEnergyTransferWeaponLimit(int objectIndex)
	{
		this->Update();

		XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
		int fgIndex = XwaObjects[objectIndex].TieFlightGroupIndex;

		auto it = this->_modelEnergyTransferWeaponLimit.find(fgIndex);

		if (it != this->_modelEnergyTransferWeaponLimit.end())
		{
			return it->second;
		}
		else
		{
			int value = GetModelEnergyTransferWeaponLimit(objectIndex);

			this->_modelEnergyTransferWeaponLimit.insert(std::make_pair(fgIndex, value));
			return value;
		}
	}

	int GetEnergyTransferShieldLimit(int objectIndex)
	{
		this->Update();

		XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
		int fgIndex = XwaObjects[objectIndex].TieFlightGroupIndex;

		auto it = this->_modelEnergyTransferShieldLimit.find(fgIndex);

		if (it != this->_modelEnergyTransferShieldLimit.end())
		{
			return it->second;
		}
		else
		{
			int value = GetModelEnergyTransferShieldLimit(objectIndex);

			this->_modelEnergyTransferShieldLimit.insert(std::make_pair(fgIndex, value));
			return value;
		}
	}

	int GetAIMaxTorpedoCountPerPass(int objectIndex)
	{
		this->Update();

		XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
		int fgIndex = XwaObjects[objectIndex].TieFlightGroupIndex;

		auto it = this->_modelAIMaxTorpedoCountPerPass.find(fgIndex);

		if (it != this->_modelAIMaxTorpedoCountPerPass.end())
		{
			return it->second;
		}
		else
		{
			int value = GetModelAIMaxTorpedoCountPerPass(objectIndex);

			this->_modelAIMaxTorpedoCountPerPass.insert(std::make_pair(fgIndex, value));
			return value;
		}
	}

	int GetAIMaxTorpedoCountPerTarget(int objectIndex)
	{
		this->Update();

		XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
		int fgIndex = XwaObjects[objectIndex].TieFlightGroupIndex;

		auto it = this->_modelAIMaxTorpedoCountPerTarget.find(fgIndex);

		if (it != this->_modelAIMaxTorpedoCountPerTarget.end())
		{
			return it->second;
		}
		else
		{
			int value = GetModelAIMaxTorpedoCountPerTarget(objectIndex);

			this->_modelAIMaxTorpedoCountPerTarget.insert(std::make_pair(fgIndex, value));
			return value;
		}
	}

	bool GetIsImpactSpinningEnabled(int objectIndex)
	{
		this->Update();

		XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
		int fgIndex = XwaObjects[objectIndex].TieFlightGroupIndex;

		auto it = this->_modelIsImpactSpinningEnabled.find(fgIndex);

		if (it != this->_modelIsImpactSpinningEnabled.end())
		{
			return it->second;
		}
		else
		{
			bool value = GetModelIsImpactSpinningEnabled(objectIndex);

			this->_modelIsImpactSpinningEnabled.insert(std::make_pair(fgIndex, value));
			return value;
		}
	}

	int GetImpactSpinningSpeedFactorPercent(int objectIndex)
	{
		this->Update();

		XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
		int fgIndex = XwaObjects[objectIndex].TieFlightGroupIndex;

		auto it = this->_modelImpactSpinningSpeedFactorPercent.find(fgIndex);

		if (it != this->_modelImpactSpinningSpeedFactorPercent.end())
		{
			return it->second;
		}
		else
		{
			int value = GetModelImpactSpinningSpeedFactorPercent(objectIndex);

			this->_modelImpactSpinningSpeedFactorPercent.insert(std::make_pair(fgIndex, value));
			return value;
		}
	}

	int GetImpactSpinningAngleFactorPercent(int objectIndex)
	{
		this->Update();

		XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
		int fgIndex = XwaObjects[objectIndex].TieFlightGroupIndex;

		auto it = this->_modelImpactSpinningAngleFactorPercent.find(fgIndex);

		if (it != this->_modelImpactSpinningAngleFactorPercent.end())
		{
			return it->second;
		}
		else
		{
			int value = GetModelImpactSpinningAngleFactorPercent(objectIndex);

			this->_modelImpactSpinningSpeedFactorPercent.insert(std::make_pair(fgIndex, value));
			return value;
		}
	}

	const std::vector<std::string>& GetWeaponStatsLines(int sourceModelIndex)
	{
		auto it = this->_weaponStatsLines.find(sourceModelIndex);

		if (it == this->_weaponStatsLines.end())
		{
			auto modelLines = GetModelLines(sourceModelIndex, "WeaponStats");

			this->_weaponStatsLines.insert(std::make_pair(sourceModelIndex, modelLines));
			it = this->_weaponStatsLines.find(sourceModelIndex);
		}

		return it->second;
	}

	const WeaponStats& GetStats(int sourceObjectIndex, int weaponIndex)
	{
		this->Update();

		const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
		const XwaObject* sourceObject = &xwaObjects[sourceObjectIndex];

		unsigned short sourceModelIndex = sourceObject->ModelIndex;
		int sourceFgIndex = sourceObject->TieFlightGroupIndex;
		int playerIndex = sourceObject->PlayerIndex;

		auto it = this->_weaponStats.find(std::make_tuple(sourceFgIndex, weaponIndex));

		if (it == this->_weaponStats.end())
		{
			std::string profileName = GetWeaponProfileName(sourceFgIndex);
			auto& lines = GetWeaponStatsLines(sourceModelIndex);
			WeaponStats stats = GetWeaponStats(sourceObject, lines, playerIndex, sourceModelIndex, sourceFgIndex, profileName, weaponIndex);
			this->_weaponStats.insert(std::make_pair(std::make_tuple(sourceFgIndex, weaponIndex), stats));
			it = this->_weaponStats.find(std::make_tuple(sourceFgIndex, weaponIndex));

			if (g_config.EnableWeaponStatsLog)
			{
				OutputDebugString((
					__FUNCTION__ " fg_" + std::to_string(sourceFgIndex) + "=" + g_flightModelsList.GetLstLine(sourceModelIndex)
					+ " Weapon=" + std::to_string(280 + weaponIndex)
					+ " Profile=" + profileName
					+ " Duration=" + std::to_string(stats.DurationIntegerPart) + ";" + std::to_string(stats.DurationDecimalPart)
					+ " Score=" + std::to_string(stats.Score)
					+ " Speed=" + std::to_string(stats.Speed)
					+ " Power=" + std::to_string(stats.Power)
					+ " PowerSpeedPercent=" + std::to_string(stats.PowerSpeedPercent)
					+ " FireRate=" + std::to_string(stats.FireRate)
					+ " DurationOffset=" + std::to_string(stats.DurationOffset)
					+ " FireRatio=" + std::to_string(stats.FireRatio)
					+ " Range=" + std::to_string(stats.Range)
					+ " IsPrecise=" + std::to_string(stats.IsPrecise)
					+ " DegreeOfSpreadMask=" + std::to_string(stats.DegreeOfSpreadMask)
					+ " DechargeRate=" + std::to_string(stats.DechargeRate)
					+ " RechargeRate=" + std::to_string(stats.RechargeRate)
					+ " EnergyLowHighSeparation=" + std::to_string(stats.EnergyLowHighSeparation)
					+ " Side=" + std::to_string(stats.Side)
					+ " SideModel=" + std::to_string(stats.SideModel)
					).c_str());
			}
		}

		return it->second;
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

			this->_weaponDechargeRate.clear();
			this->_weaponRechargeRate.clear();
			this->_weaponCooldownTime.clear();
			this->_modelEnergyTransferRate.clear();
			this->_modelEnergyTransferRatePenalty.clear();
			this->_modelEnergyTransferWeaponLimit.clear();
			this->_modelEnergyTransferShieldLimit.clear();
			this->_modelAIMaxTorpedoCountPerPass.clear();
			this->_modelAIMaxTorpedoCountPerTarget.clear();
			this->_modelIsImpactSpinningEnabled.clear();
			this->_modelImpactSpinningSpeedFactorPercent.clear();
			this->_modelImpactSpinningAngleFactorPercent.clear();
			this->_weaponStats.clear();
			this->_weaponStatsLines.clear();
		}
	}

	std::map<int, int> _weaponDechargeRate;
	std::map<int, int> _weaponRechargeRate;
	std::map<int, int> _weaponCooldownTime;
	std::map<int, int> _modelEnergyTransferRate;
	std::map<int, int> _modelEnergyTransferRatePenalty;
	std::map<int, int> _modelEnergyTransferWeaponLimit;
	std::map<int, int> _modelEnergyTransferShieldLimit;
	std::map<int, int> _modelAIMaxTorpedoCountPerPass;
	std::map<int, int> _modelAIMaxTorpedoCountPerTarget;
	std::map<int, bool> _modelIsImpactSpinningEnabled;
	std::map<int, int> _modelImpactSpinningSpeedFactorPercent;
	std::map<int, int> _modelImpactSpinningAngleFactorPercent;
	std::map<std::tuple<int, int>, WeaponStats> _weaponStats;
	std::map<int, std::vector<std::string>> _weaponStatsLines;
};

ModelIndexWeapon g_modelIndexWeapon;

int WeaponDechargeHook(int* params)
{
	const int objectIndex = params[12];
	const int weaponRackIndex = (unsigned short)params[14];

	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	XwaCraft* XwaCurrentCraft = *(XwaCraft**)0x00910DFC;

	XwaCraftWeaponRack* weaponRacks = (XwaCraftWeaponRack*)((int)XwaCurrentCraft + g_craftConfig.Craft_Offset_2DF);

	if (weaponRacks[weaponRackIndex].Sequence < 4)
	{
		//unsigned short modelIndex = XwaObjects[objectIndex].ModelIndex;
		//int dechargeRate = g_modelIndexWeapon.GetDechargeRate(objectIndex);

		unsigned short weaponRackModelIndex = weaponRacks[weaponRackIndex].ModelIndex;
		int energyLowHighSeparation = g_modelIndexWeapon.GetStats(objectIndex, weaponRackModelIndex - 0x118).EnergyLowHighSeparation;

		if (weaponRacks[weaponRackIndex].Charge >= energyLowHighSeparation)
		{
			weaponRackModelIndex++;
		}

		int dechargeRate = g_modelIndexWeapon.GetStats(objectIndex, weaponRackModelIndex - 0x118).DechargeRate;

		weaponRacks[weaponRackIndex].Charge -= dechargeRate;
	}

	return 0;
}

int WeaponRechargeHook(int* params)
{
	unsigned short modelIndex = (unsigned short)params[0];
	int objectIndex = params[Params_EBP] / 0x27;
	const int weaponRackIndex = (unsigned short)params[Params_EDI];

	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	XwaCraft* XwaCurrentCraft = *(XwaCraft**)0x00910DFC;

	XwaCraftWeaponRack* weaponRacks = (XwaCraftWeaponRack*)((int)XwaCurrentCraft + g_craftConfig.Craft_Offset_2DF);

	//int rechargeRate = g_modelIndexWeapon.GetRechargeRate(objectIndex);

	unsigned short weaponRackModelIndex = weaponRacks[weaponRackIndex].ModelIndex;
	int energyLowHighSeparation = g_modelIndexWeapon.GetStats(objectIndex, weaponRackModelIndex - 0x118).EnergyLowHighSeparation;

	if (weaponRacks[weaponRackIndex].Charge >= energyLowHighSeparation)
	{
		weaponRackModelIndex++;
	}

	int rechargeRate = g_modelIndexWeapon.GetStats(objectIndex, weaponRackModelIndex - 0x118).RechargeRate;

	params[0] = rechargeRate;

	return 0;
}

int WeaponCooldownTimeHook(int* params)
{
	const int esp10 = params[4];
	const int A4 = params[12];
	const int A8 = params[13];
	const int AC = params[Params_EBX];
	const unsigned short weaponModelIndex = (unsigned short)params[Params_EBP];

	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	XwaCraft* XwaCurrentCraft = *(XwaCraft**)0x00910DFC;

	int rate;

	if (weaponModelIndex >= 280 && weaponModelIndex < 280 + 28)
	{
		rate = g_modelIndexWeapon.GetStats(A4, weaponModelIndex - 0x118).FireRate;
	}
	else
	{
		unsigned short modelIndex = XwaObjects[A4].ModelIndex;
		rate = g_modelIndexWeapon.GetCooldownTime(modelIndex);
	}

	if (AC != -1)
	{
		XwaCurrentCraft->m1BF[0] += esp10 * rate + 0x02;
		XwaCurrentCraft->m1C5[0] += esp10 * rate + 0x02;
	}
	else
	{
		XwaCurrentCraft->m1BF[A8] += esp10 * rate + 0x02;
		XwaCurrentCraft->m1C5[A8] += esp10 * rate + 0x02;
	}

	return 0;
}

int ModelEnergyTransfer1Hook(int* params)
{
	for (int i = 0; i < 5; i++)
	{
		*(unsigned char*)(0x004FBD0D + i) = 0x90;
	}

	int playerIndex = *(int*)(params[Params_EBP] + 0x08);
	XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	int objectIndex = XwaPlayers[playerIndex].ObjectIndex;

	int rate = g_modelIndexWeapon.GetEnergyTransferRate(objectIndex);

	params[Params_EAX] = rate;
	return 0;
}

int ModelEnergyTransfer2Hook(int* params)
{
	for (int i = 0; i < 5; i++)
	{
		*(unsigned char*)(0x004FCDFE + i) = 0x90;
	}

	int playerIndex = *(int*)(params[Params_EBP] + 0x08);
	XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	int objectIndex = XwaPlayers[playerIndex].ObjectIndex;

	int rate = g_modelIndexWeapon.GetEnergyTransferRate(objectIndex);

	params[Params_EAX] = rate;

	return 0;
}

int ModelEnergyTransfer3Hook(int* params)
{
	int playerIndex = *(int*)(params[Params_EBP] + 0x08);
	XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	int objectIndex = XwaPlayers[playerIndex].ObjectIndex;

	int rate = g_modelIndexWeapon.GetEnergyTransferRate(objectIndex);
	int penalty = g_modelIndexWeapon.GetEnergyTransferRatePenalty(objectIndex);

	rate -= penalty;

	params[Params_EDI] = rate;
	params[Params_EAX] = params[Params_EAX] / rate;

	params[Params_ReturnAddress] = 0x004FD320;
	return 0;
}

int ModelEnergyTransferShieldToLaser1Hook(int* params)
{
	int playerIndex = *(int*)(params[Params_EBP] + 0x08);
	XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	int objectIndex = XwaPlayers[playerIndex].ObjectIndex;

	int limit = g_modelIndexWeapon.GetEnergyTransferWeaponLimit(objectIndex);
	int energy = *(short*)(params[Params_EBP] - 0x04);

	if (limit > energy)
	{
		limit = energy;
	}

	params[Params_EAX] = limit;
	return 0;
}

int ModelEnergyTransferShieldToLaser2Hook(int* params)
{
	int playerIndex = *(int*)(params[Params_EBP] + 0x08);
	XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	int objectIndex = XwaPlayers[playerIndex].ObjectIndex;

	int limit = g_modelIndexWeapon.GetEnergyTransferWeaponLimit(objectIndex);
	int energy = *(short*)(params[Params_EBP] - 0x0C);

	if (energy >= limit)
	{
		params[Params_ReturnAddress] = 0x004FCF0D;
	}

	return 0;
}

int ModelEnergyTransferLaserToShield1Hook(int* params)
{
	int playerIndex = *(int*)(params[Params_EBP] + 0x08);
	XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	int objectIndex = XwaPlayers[playerIndex].ObjectIndex;

	int limit = g_modelIndexWeapon.GetEnergyTransferShieldLimit(objectIndex);
	int energy = *(int*)(params[Params_EBP] - 0x08);

	if (energy > limit)
	{
		energy = limit;
	}

	*(int*)(params[Params_EBP] - 0x08) = energy;

	return 0;
}

int ModelEnergyTransferLaserToShield2Hook(int* params)
{
	int playerIndex = *(int*)(params[Params_EBP] + 0x08);
	XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	int objectIndex = XwaPlayers[playerIndex].ObjectIndex;

	int limit = g_modelIndexWeapon.GetEnergyTransferWeaponLimit(objectIndex);
	int energy = *(short*)(params[Params_EBP] - 0x0C);

	if (energy >= limit)
	{
		params[Params_ReturnAddress] = 0x004FD433;
	}

	return 0;
}

int ModelAIMaxTorpedoCountHook(int* params)
{
	const int objectIndex = *(int*)(0x07CA1A0 + 0x0000);

	int perPass = g_modelIndexWeapon.GetAIMaxTorpedoCountPerPass(objectIndex);
	int perTarget = g_modelIndexWeapon.GetAIMaxTorpedoCountPerTarget(objectIndex);

	params[13] = perPass;
	params[14] = perTarget;

	return 0;
}

int ModelIsImpactSpinningEnabledHook(int* params)
{
	int objectIndex = *(int*)(params[Params_EBP] + 0x0C);
	bool value = g_modelIndexWeapon.GetIsImpactSpinningEnabled(objectIndex);

	return value ? 1 : 0;
}

int ModelImpactSpinningFactorHook(int* params)
{
	const int origin = params[14];
	const bool isImpactSpinning = origin == 0x0040E8F3;
	const int objectIndex = params[Params_ESI] / 0x27;

	short speed = (short)params[Params_EDI];
	short angle = (short)params[Params_EAX];

	if (angle < 0)
	{
		angle = -angle;
	}

	if (angle > 0x4000)
	{
		angle = (short)0x8000 - angle;
	}

	if (isImpactSpinning)
	{
		int speedFactorPercent = g_modelIndexWeapon.GetImpactSpinningSpeedFactorPercent(objectIndex);
		int angleFactorPercent = g_modelIndexWeapon.GetImpactSpinningAngleFactorPercent(objectIndex);

		speed = (short)((speed * speedFactorPercent + 50) / 100);
		angle = (short)((angle * angleFactorPercent + 50) / 100);
	}

	params[Params_EDI] = speed;
	params[Params_EAX] = angle;

	return 0;
}

int WeaponDurationOffsetHook(int* params)
{
	const int weaponIndex = params[Params_EDI];
	XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_EBX] / 0x27;
	const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	int value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).DurationOffset;

	xwaObjects[weaponObjectIndex].pMobileObject->Duration += value;

	return 0;
}

int WeaponFireRatioHook(int* params)
{
	const int weaponIndex = *(unsigned short*)(params[Params_EAX] + params[Params_ESI] + g_craftConfig.Craft_Offset_2DF) - 0x118;
	const int sourceObjectIndex = *(short*)(params[Params_EBP] + 0x08);

	int value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).FireRatio;

	if (value == -1)
	{
		value = ((short*)(0x005BB5D0 + params[Params_ECX]))[params[Params_EDX]];
	}

	params[Params_EBX] = value;
	return 0;
}

int WeaponRange_004A8D42_Hook(int* params)
{
	const int weaponIndex = *(unsigned short*)(params[Params_EBP] + params[6] + g_craftConfig.Craft_Offset_2DF) - 0x118;
	const int sourceObjectIndex = *(int*)0x07CA1A0;

	int value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Range;

	if (value == -1)
	{
		value = ((int*)(0x005BB5C4 + params[Params_ECX]))[params[Params_EDX]];
	}

	params[Params_ECX] = value;
	return 0;
}

int WeaponRange_004A8FB6_Hook(int* params)
{
	const int weaponIndex = *(unsigned short*)(params[Params_EBP] + params[6] + g_craftConfig.Craft_Offset_2DF) - 0x118;
	const int sourceObjectIndex = *(int*)0x07CA1A0;

	int value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Range;

	if (value == -1)
	{
		value = ((int*)(0x005BB5C4 + params[Params_EAX]))[params[Params_EDX]];
	}

	params[Params_ECX] = value;
	return 0;
}

int WeaponRange_004A91B2_Hook(int* params)
{
	const int weaponIndex = *(unsigned short*)(params[Params_EBP] + params[6] + g_craftConfig.Craft_Offset_2DF) - 0x118;
	const int sourceObjectIndex = *(int*)0x07CA1A0;

	int value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Range;

	if (value == -1)
	{
		value = ((int*)(0x005BB5C4 + params[Params_EAX]))[params[Params_ECX]];
	}

	params[Params_EAX] = value;
	return 0;
}

int WeaponRange_004A9B15_Hook(int* params)
{
	const int weaponIndex = *(unsigned short*)(params[Params_ECX] + params[Params_ESI] + g_craftConfig.Craft_Offset_2DF) - 0x118;
	const int sourceObjectIndex = *(int*)0x07CA1A0;

	int value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Range;

	if (value == -1)
	{
		value = ((int*)(0x005BB5C4 + params[Params_EAX]))[params[Params_EDX]];
	}

	params[Params_EAX] = value;
	return 0;
}

int WeaponRange_004E2119_Hook(int* params)
{
	const int weaponIndex = *(unsigned short*)(params[Params_EAX] + *(int*)(params[Params_EBP] - 0x30) + g_craftConfig.Craft_Offset_2DF) - 0x118;
	const int sourceObjectIndex = *(short*)(params[Params_EBP] + 0x08);

	int value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Range;

	if (value == -1)
	{
		value = ((int*)(0x005BB5C4 + params[Params_EDX]))[params[Params_ECX]];
	}

	params[Params_ESI] = value;
	return 0;
}

int WeaponSpreadHook(int* params)
{
	const int weaponIndex = (unsigned short)params[Params_ESI] - 0x118;
	const int sourceObjectIndex = *(short*)(params[Params_EBP] + 0x08);
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	int targetObjectIndex = params[Params_ECX] / 0x27;
	int targetPlayerIndex = xwaObjects[targetObjectIndex].PlayerIndex;
	int targetShipCategory = xwaObjects[targetObjectIndex].ShipCategory;

	bool value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).IsPrecise;
	unsigned int degreeOfSpreadMask = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).DegreeOfSpreadMask;
	unsigned char degreeOfSpreadMaskDiv2 = degreeOfSpreadMask >= 0xFF ? 0x7F : ((degreeOfSpreadMask + 1) / 2 - 1);

	*(unsigned char*)(0x004E243B + 0x02) = degreeOfSpreadMaskDiv2;
	*(unsigned int*)(0x004E2443 + 0x01) = degreeOfSpreadMask;
	*(unsigned int*)(0x004E2454 + 0x02) = degreeOfSpreadMask;
	*(unsigned int*)(0x004E2472 + 0x02) = degreeOfSpreadMask;

	if (targetShipCategory == ShipCategory_PlayerProjectile
		|| targetShipCategory == ShipCategory_OtherProjectile
		|| (targetPlayerIndex == -1 && value))
	{
		params[Params_ReturnAddress] = 0x004E249D;
	}

	return 0;
}

int WeaponFireRateHook(int* params)
{
	XwaCraft* XwaCurrentCraft = *(XwaCraft**)0x00910DFC;

	const int warheadIndex = params[Params_ESI];
	const unsigned short weaponModelIndex = XwaCurrentCraft->WarheadsModelIndex[warheadIndex];
	const int sourceObjectIndex = params[Params_EBP] / 0x27;
	const int linkCount = params[6];

	int rate = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponModelIndex - 0x118).FireRate;

	if (g_config.LimitLinkedWarheadFireRate)
	{
		rate *= linkCount;
	}

	XwaCurrentCraft->m1D8[warheadIndex] += rate;

	params[Params_ReturnAddress] = 0x00491ACD;
	return 0;
}

int GetWeaponDurationHook(int* params)
{
	const unsigned short A4 = (unsigned short)params[0];

	const auto GetWeaponDuration = (int(*)(unsigned short))0x00490EB0;
	const auto XwaMulWordPercent = (unsigned short(*)(unsigned short, unsigned short))0x00494C90;

	//int duration = GetWeaponDuration(A4);

	const int weaponIndex = params[Params_EDI] - 0x118;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_ESI] / 0x27;
	const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	int valueInteger = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).DurationIntegerPart;
	int valueDecimal = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).DurationDecimalPart;

	int duration = valueInteger * 0xEC + XwaMulWordPercent(valueDecimal, 0xEC);

	params[Params_EAX] = duration;
	return 0;
}

int WeaponDuration_0040D54C_Hook(int* params)
{
	EnergyLowHighSeparationFunction1(params);

	const int weaponIndex = params[Params_ECX] / 2;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const XwaObject* sourceObject = (XwaObject*)params[Params_ESI];
	const int sourceObjectIndex = sourceObject - xwaObjects;

	unsigned short value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).DurationIntegerPart;

	params[Params_EAX] = value;
	return 0;
}

int WeaponDuration_0040D570_Hook(int* params)
{
	const int weaponIndex = params[Params_ECX] / 2;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const XwaObject* sourceObject = (XwaObject*)params[Params_ESI];
	const int sourceObjectIndex = sourceObject - xwaObjects;

	unsigned short value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).DurationDecimalPart;

	params[Params_EAX] = value;
	return 0;
}

int WeaponDuration_004922DB_Hook(int* params)
{
	const int weaponIndex = params[Params_EAX] - 0x118;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_ESI] / 0x27;
	const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	unsigned short value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).DurationIntegerPart;

	params[Params_EBX] = value;
	return 0;
}

int WeaponDuration_004922E3_Hook(int* params)
{
	const int weaponIndex = params[Params_EAX] - 0x118;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_ESI] / 0x27;
	const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	unsigned short value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).DurationDecimalPart;

	params[Params_EAX] = value;
	return 0;
}

int WeaponDuration_0049312E_Hook(int* params)
{
	const int weaponIndex = params[Params_EAX] - 0x118;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_ESI] / 0x27;
	const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	unsigned short value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).DurationIntegerPart;

	params[Params_EDI] = value;
	return 0;
}

int WeaponDuration_00493136_Hook(int* params)
{
	const int weaponIndex = params[Params_EAX] - 0x118;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_ESI] / 0x27;
	const unsigned short sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	unsigned short value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).DurationDecimalPart;

	params[Params_EAX] = value;
	return 0;
}

int WeaponDuration_00493616_Hook(int* params)
{
	const int weaponIndex = params[Params_EDX] - 0x118;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_EBX] / 0x27;
	const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	unsigned short value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).DurationIntegerPart;

	params[Params_ESI] = value;
	return 0;
}

int WeaponDuration_00493628_Hook(int* params)
{
	const int weaponIndex = params[Params_EDX] - 0x118;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_EBX] / 0x27;
	const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	unsigned short value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).DurationDecimalPart;

	params[Params_EAX] = value;
	return 0;
}

int WeaponScore_00497454_Hook(int* params)
{
	const int weaponIndex = params[Params_ECX] - 0x118;
	const XwaObject* weaponObject = (XwaObject*)params[Params_ESI];
	const int sourceObjectIndex = weaponObject->pMobileObject->ObjectIndex;

	short value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Score;

	params[Params_EDX] = value;
	return 0;
}

int WeaponScore_004D9B16_Hook(int* params)
{
	const int weaponIndex = params[Params_EDI] - 0x118;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_EBP] / 0x27;
	const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	short value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Score;

	params[Params_EBX] = value;
	return 0;
}

int WeaponScore_004D9B4E_Hook(int* params)
{
	const int weaponIndex = params[Params_EDX] - 0x118;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_EBP] / 0x27;
	const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	short value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Score;

	params[Params_ECX] = value;
	return 0;
}

int WeaponSpeed_0040D553_Hook(int* params)
{
	const int weaponIndex = params[Params_ECX] / 2;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const XwaObject* sourceObject = (XwaObject*)params[Params_ESI];
	const int sourceObjectIndex = sourceObject - xwaObjects;

	short value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Speed;

	params[Params_EBX] = value;
	return 0;
}

int WeaponSpeed_0049221A_Hook(int* params)
{
	const int weaponIndex = params[Params_EAX] - 0x118;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_ESI] / 0x27;
	const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	short value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Speed;

	params[Params_EBX] = value;
	return 0;
}

int WeaponSpeed_00492235_Hook(int* params)
{
	const int weaponIndex = params[Params_EAX] - 0x118;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_ESI] / 0x27;
	const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	short value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Speed;

	params[Params_EBX] = value;
	return 0;
}

int WeaponSpeed_00492EE5_Hook(int* params)
{
	const int weaponIndex = 13;
	const XwaObject* weaponObject = (XwaObject*)(params[Params_ESI] - 0x23);
	const int sourceObjectIndex = weaponObject->pMobileObject->ObjectIndex;

	short value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Speed;

	params[Params_EDX] = value;
	return 0;
}

int WeaponSpeed_004930F3_Hook(int* params)
{
	const int weaponIndex = params[Params_EAX] - 0x118;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_ESI] / 0x27;
	const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	short value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Speed;

	params[Params_EDX] = value;
	return 0;
}

int WeaponSpeed_0049359A_Hook(int* params)
{
	const int weaponIndex = params[Params_ESI] - 0x118;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_EBX] / 0x27;
	const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	short value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Speed;

	params[Params_EDX] = value;
	return 0;
}

int WeaponSpeed_004935C3_Hook(int* params)
{
	const int weaponIndex = params[Params_ESI] - 0x118;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_EBX] / 0x27;
	const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	short value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Speed;

	params[Params_EDX] += value;
	return 0;
}

int WeaponSpeed_0049424E_Hook(int* params)
{
	const int weaponIndex = 26;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_ESI] / 0x27;
	//const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;
	const unsigned short sourceModelIndex = 487; // ModelIndex_487_6250_0_ResData_DsFire

	auto& lines = g_modelIndexWeapon.GetWeaponStatsLines(sourceModelIndex);
	WeaponStats stats = GetWeaponStats(nullptr, lines, -1, sourceModelIndex, -1, std::string(), weaponIndex);

	short value = stats.Speed;

	params[Params_EAX] = value;
	return 0;
}

int WeaponSpeed_004942CF_Hook(int* params)
{
	const int weaponIndex = 26;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_ESI] / 0x27;
	//const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;
	const unsigned short sourceModelIndex = 487; // ModelIndex_487_6250_0_ResData_DsFire

	auto& lines = g_modelIndexWeapon.GetWeaponStatsLines(sourceModelIndex);
	WeaponStats stats = GetWeaponStats(nullptr, lines, -1, sourceModelIndex, -1, std::string(), weaponIndex);

	short value = stats.Speed;

	params[Params_ECX] = value;
	return 0;
}

int WeaponSpeed_00496903_Hook(int* params)
{
	const int weaponIndex = 13;
	const XwaObject* weaponObject = (XwaObject*)params[Params_ESI];
	const int sourceObjectIndex = weaponObject->pMobileObject->ObjectIndex;

	short value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Speed;

	params[Params_ECX] = value;
	return 0;
}

int WeaponSpeed_004B5F76_Hook(int* params)
{
	const int weaponIndex = params[Params_ECX] - 0x118;
	const int sourceObjectIndex = *(int*)0x07CA1A0;

	short value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Speed;

	params[Params_ESI] = value;
	return 0;
}

int WeaponSpeed_004E22F7_Hook(int* params)
{
	const int weaponIndex = params[Params_ECX] - 0x118;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const XwaObject* sourceObject = (XwaObject*)params[Params_EDI];
	const int sourceObjectIndex = sourceObject - xwaObjects;

	short value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Speed;

	params[Params_EAX] = value;
	return 0;
}

int WeaponSpeed_004E2678_Hook(int* params)
{
	const int weaponIndex = params[Params_EDI];
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_EBX] / 0x27;
	const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	short value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Speed;

	params[Params_EAX] = value;
	return 0;
}

int WeaponSpeed_004E26A2_Hook(int* params)
{
	const int weaponIndex = params[Params_EDI];
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_EBX] / 0x27;
	const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	short value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Speed;

	params[Params_EAX] = value;
	return 0;
}

int WeaponSpeed_004E4D75_Hook(int* params)
{
	const int weaponIndex = params[Params_EDI] - 0x118;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_EBX] / 0x27;
	const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	short value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Speed;

	params[Params_EAX] = value;
	return 0;
}

int WeaponPower_0040F511_Hook(int* params)
{
	const int weaponIndex = params[Params_EAX] - 0x118;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int sourceObjectIndex = *(int*)(params[Params_EBP] - 0x24) / 0x27;

	int value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Power;

	params[Params_EAX] = value;
	return 0;
}

int WeaponPower_00492279_Hook(int* params)
{
	const int weaponIndex = params[Params_EAX] - 0x118;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_ESI] / 0x27;
	const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	int value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Power;

	params[Params_EAX] = value;
	return 0;
}

int WeaponPower_004922A2_Hook(int* params)
{
	params[Params_ECX] = *(int*)0x007B33C4;

	const int weaponIndex = params[Params_EAX] - 0x118;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_ESI] / 0x27;
	const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	int speed = params[Params_EDX];
	int speedPercent = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).PowerSpeedPercent;
	speed = speed * speedPercent / 100;
	params[Params_EDX] = speed;

	int value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Power;

	params[Params_EBX] = value;
	return 0;
}

int WeaponPower_004922BA_Hook(int* params)
{
	const int weaponIndex = params[Params_EAX] - 0x118;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_ESI] / 0x27;
	const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	int value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Power;

	params[Params_EAX] = value;
	return 0;
}

int WeaponPower_00493121_Hook(int* params)
{
	const int weaponIndex = params[Params_EAX] - 0x118;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_ESI] / 0x27;
	const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	int value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Power;

	params[Params_EDX] = value;
	return 0;
}

int WeaponPower_004935E1_Hook(int* params)
{
	const int weaponIndex = params[Params_ESI] - 0x118;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_EBX] / 0x27;
	const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	int speed = params[Params_ECX];
	int speedPercent = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).PowerSpeedPercent;
	speed = speed * speedPercent / 100;
	params[Params_ECX] = speed;

	int value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Power;

	params[Params_EAX] = value;
	return 0;
}

int WeaponPower_004935FE_Hook(int* params)
{
	const int weaponIndex = params[Params_ESI] - 0x118;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_EBX] / 0x27;
	const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	int value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Power;

	params[Params_ECX] = value;
	return 0;
}

int WeaponPower_00494265_Hook(int* params)
{
	const int weaponIndex = 26;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_ESI] / 0x27;
	//const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;
	const unsigned short sourceModelIndex = 487; // ModelIndex_487_6250_0_ResData_DsFire

	auto& lines = g_modelIndexWeapon.GetWeaponStatsLines(sourceModelIndex);
	WeaponStats stats = GetWeaponStats(nullptr, lines, -1, sourceModelIndex, -1, std::string(), weaponIndex);

	int value = stats.Power;

	params[Params_EAX] = value;
	return 0;
}

int WeaponPower_004A7983_Hook(int* params)
{
	const int weaponIndex = params[Params_EAX] - 0x118;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_EDI];
	const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	int value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Power;

	params[Params_EAX] = value;
	return 0;
}

int WeaponPower_004A7D68_Hook(int* params)
{
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponIndex = params[Params_EAX] - 0x118;
	const int sourceObjectIndex = *(int*)((int)params + 0x44) / 0x27;

	int value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Power;

	params[Params_EAX] = value;
	return 0;
}

int WeaponPower_004A7E46_Hook(int* params)
{
	const int weaponIndex = params[Params_EAX] - 0x118;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_EDI];
	const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	int value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Power;

	params[Params_EAX] = value;
	return 0;
}

int WeaponPower_004E2691_Hook(int* params)
{
	const int weaponIndex = params[Params_EDI];
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_EBX] / 0x27;
	const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	int value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Power;

	params[Params_EAX] = value;
	return 0;
}

int WeaponPower_004E4DAF_Hook(int* params)
{
	const int weaponIndex = params[Params_EDI] - 0x118;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_ESI] / 0x27;
	const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	int value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Power;

	params[Params_EDX] = value;
	return 0;
}

int WeaponPower_00519C36_Hook(int* params)
{
	const int weaponIndex = 21;
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const int weaponObjectIndex = params[Params_ESI] / 0x27;
	const int sourceObjectIndex = xwaObjects[weaponObjectIndex].pMobileObject->ObjectIndex;

	int value = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).Power;

	params[Params_EDX] = value;
	return 0;
}

int WeaponRackHook(int* params)
{
	if (!EnergyLowHighSeparationFunction2(params))
	{
		return 0xffff;
	}

	const auto L00491EB0 = (int(*)(int, int, int, int))0x00491EB0;

	const int A4 = params[0];
	const int A8 = params[1];
	const int AC = params[2];
	const int A10 = params[3];

	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	XwaCraft* XwaCurrentCraft = *(XwaCraft**)0x00910DFC;

	XwaCraftWeaponRack* weaponRacks = (XwaCraftWeaponRack*)((int)XwaCurrentCraft + g_craftConfig.Craft_Offset_2DF);

	ShipCategoryEnum category = XwaObjects[A4].ShipCategory;

	if (category == ShipCategory_Freighter
		|| category == ShipCategory_Starship
		|| category == ShipCategory_Platform
		|| category == ShipCategory_Satellite)
	{
		char* m292;

		if (g_craftConfig.MeshesCount == 0)
		{
			m292 = XwaCurrentCraft->XwaCraft_m292;
		}
		else
		{
			m292 = (char*)((int)XwaCurrentCraft + g_craftConfig.Craft_Offset_292);
		}

		if (m292[weaponRacks[A8].MeshId] == 0)
		{
			return 0xffff;
		}
	}

	int result = L00491EB0(A4, A8, AC, A10);

	return result;
}

bool IsMeshTypeGunTurret(MeshTypeEnum meshType)
{
	switch (meshType)
	{
	case MeshType_GunTurret:
	case MeshType_RotaryGunTurret:
	case MeshType_SmallGun:
	case MeshType_WeaponSys1:
	case MeshType_WeaponSys2:
		return true;
	}

	return false;
}

bool IsMeshGunTurret(int modelIndex, int meshIndex)
{
	const auto XwaOptGetMeshType = (MeshTypeEnum(*)(int, int))0x00485A70;

	MeshTypeEnum meshType = XwaOptGetMeshType(modelIndex, meshIndex);
	return IsMeshTypeGunTurret(meshType);
}

bool HasOptGunTurrets(int modelIndex)
{
	const auto XwaOptGetMeshesCount = (int(*)(int))0x00485190;

	bool hasGunTurrets = false;
	int meshesCount = XwaOptGetMeshesCount(modelIndex);

	for (int i = 0; i < meshesCount; i++)
	{
		if (IsMeshGunTurret(modelIndex, i))
		{
			hasGunTurrets = true;
			break;
		}
	}

	return hasGunTurrets;
}

bool IsHardpointInFront(int modelIndex, XwaFloatHardPt* hardPts, int hardHpIndex)
{
	const auto XwaOptGetMeshRotationScale = (OptRotationScaleNode_17_Data * (*)(int, int))0x00486C30;

	float hpPositionX = (float)hardPts[hardHpIndex].Position[0];
	float hpPositionY = (float)hardPts[hardHpIndex].Position[2];
	float hpPositionZ = (float)hardPts[hardHpIndex].Position[1];
	int hpMeshIndex = hardPts[hardHpIndex].MeshId;

	if (!IsMeshGunTurret(modelIndex, hpMeshIndex))
	{
		return true;
	}

	float targetPositionX = (float)*(int*)0x080DA48;
	float targetPositionY = (float)*(int*)0x080DA44;
	float targetPositionZ = (float)*(int*)0x080DB64;

	float offsetX = targetPositionX - hpPositionX;
	float offsetY = targetPositionY - hpPositionY;
	float offsetZ = targetPositionZ - hpPositionZ;
	float offsetLength = sqrtf(offsetX * offsetX + offsetY * offsetY + offsetZ * offsetZ);
	if (offsetLength != 0.0f)
	{
		offsetX /= offsetLength;
		offsetY /= offsetLength;
		offsetZ /= offsetLength;
	}

	OptRotationScaleNode_17_Data* rotation = XwaOptGetMeshRotationScale(modelIndex, hpMeshIndex);
	float lookX = rotation->LookX;
	float lookY = rotation->LookY;
	float lookZ = rotation->LookZ;
	float lookLength = sqrtf(lookX * lookX + lookY * lookY + lookZ * lookZ);
	if (lookLength != 0.0f)
	{
		lookX /= lookLength;
		lookY /= lookLength;
		lookZ /= lookLength;
	}

	float dot = offsetX * lookX + offsetY * lookY + offsetZ * lookZ;
	bool inFront = dot > 0.0f;

	return inFront;
}

int WeaponHardpointHook(int* params)
{
	int A4 = params[0];
	int A8 = params[1];
	int AC = params[2];
	int A10 = params[3];
	int A14 = params[4];
	int* A18 = (int*)params[5];
	XwaFloatHardPt* A1C = (XwaFloatHardPt*)params[6];
	XwaCraft* A20 = (XwaCraft*)params[7];

	//const auto L00487FD0 = (int(*)(int, int, int, int, int, int*, XwaFloatHardPt*, XwaCraft*))0x00487FD0;

	//return L00487FD0(A4, A8, AC, A10, A14, A18, A1C, A20);

	const auto XwaOptGetMeshesCount = (int(*)(int))0x00488960;

	ExeCraftEntry* s_ExeCraftTable = (ExeCraftEntry*)0x005BB480;

	char* m292;

	if (g_craftConfig.MeshesCount == 0)
	{
		m292 = A20->XwaCraft_m292;
	}
	else
	{
		m292 = (char*)((int)A20 + g_craftConfig.Craft_Offset_292);
	}

	int esi = -1;

	float esp18 = (float)A8;
	float esp1C = (float)AC;
	float esp20 = (float)A10;

	int ebp = s_ExeCraftTable[A20->CraftIndex].NumOfFloatHardPts;

	int edx = A14;
	if (edx >= ebp)
	{
		edx = ebp - 1;
	}

	unsigned char esp24[256];

	memset(esp24, 0, ebp);

	for (int ecx = 0; ecx < edx; ecx++)
	{
		esp24[A18[ecx]] = 0x01;
	}

	esi = 0;

	for (; esi < ebp; esi++)
	{
		if (m292[A1C[esi].MeshId] != 0 && esp24[esi] == 0 && IsHardpointInFront(A4, A1C, esi))
			break;
	}

	if (esi == ebp)
	{
		return -1;
	}

	float esp14 =
		(A1C[esi].Position[0] - esp18) * (A1C[esi].Position[0] - esp18)
		+ (A1C[esi].Position[2] - esp1C) * (A1C[esi].Position[2] - esp1C)
		+ (A1C[esi].Position[1] - esp20) * (A1C[esi].Position[1] - esp20);

	for (int edx = esi + 1; edx < ebp; edx++)
	{
		if (m292[A1C[edx].MeshId] != 0 && esp24[edx] == 0 && IsHardpointInFront(A4, A1C, edx))
		{
			float esp10 =
				(A1C[edx].Position[0] - esp18) * (A1C[edx].Position[0] - esp18)
				+ (A1C[edx].Position[2] - esp1C) * (A1C[edx].Position[2] - esp1C)
				+ (A1C[edx].Position[1] - esp20) * (A1C[edx].Position[1] - esp20);

			if (esp10 < esp14)
			{
				esp14 = esp10;
				esi = edx;
			}
		}
	}

	return esi;
}

int GetNearestWeaponMeshHook(int* params)
{
	const int modelIndex = params[0];
	const int hardpointPositionX = params[1];
	const int hardpointPositionY = params[2];
	const int hardpointPositionZ = params[3];

	const auto XwaOptGetNearestWeaponSysOrMainHull = (int(*)(int, int, int, int))0x00487680;

	bool hasGunTurrets = HasOptGunTurrets(modelIndex);

	if (!hasGunTurrets)
	{
		return XwaOptGetNearestWeaponSysOrMainHull(modelIndex, hardpointPositionX, hardpointPositionY, hardpointPositionZ);
	}

	const auto XwaOptGetMeshesCount = (int(*)(int))0x00485190;
	const auto XwaOptGetMeshDescriptor = (OptMeshDescriptorNode_19_Data * (*)(int, int))0x004859B0;
	const auto XwaOptGetMeshHardpointsCount = (int(*)(int, int))0x00486DC0;
	const auto XwaOptRetrieveMeshHardpoint = (void(*)(int, int, int, int*, int*, int*, int*))0x00487180;

	float minDistance = 2.14748365E+09f;
	float distanceX = (float)hardpointPositionX;
	float distanceY = (float)hardpointPositionY;
	float distanceZ = (float)hardpointPositionZ;

	int selectedMeshIndex = -1;

	int meshesCount = XwaOptGetMeshesCount(modelIndex);

	for (int meshIndex = 0; meshIndex < meshesCount; meshIndex++)
	{
		if (!IsMeshGunTurret(modelIndex, meshIndex))
		{
			continue;
		}

		OptMeshDescriptorNode_19_Data* meshDescriptor = XwaOptGetMeshDescriptor(modelIndex, meshIndex);

		if (meshDescriptor == nullptr)
		{
			continue;
		}

		int hardpointsCount = XwaOptGetMeshHardpointsCount(modelIndex, meshIndex);

		for (int hardpointIndex = 0; hardpointIndex < hardpointsCount; hardpointIndex++)
		{
			int hpType = 0, hpPositionX = 0, hpPositionY = 0, hpPositionZ = 0;
			XwaOptRetrieveMeshHardpoint(modelIndex, meshIndex, hardpointIndex, &hpType, &hpPositionX, &hpPositionY, &hpPositionZ);
			hpPositionY = -hpPositionY;

			if (hardpointPositionX == hpPositionX
				&& hardpointPositionY == hpPositionY
				&& hardpointPositionZ == hpPositionZ)
			{
				return meshIndex;
			}
		}

		float offsetX;

		if (distanceX > meshDescriptor->MaxX)
		{
			offsetX = distanceX - meshDescriptor->MaxX;
		}
		else if (distanceX < meshDescriptor->MinX)
		{
			offsetX = meshDescriptor->MinX - distanceX;
		}
		else
		{
			offsetX = 0.0f;
		}

		float offsetY;

		if (distanceY > meshDescriptor->MaxY)
		{
			offsetY = distanceY - meshDescriptor->MaxY;
		}
		else if (distanceY < meshDescriptor->MinY)
		{
			offsetY = meshDescriptor->MinY - distanceY;
		}
		else
		{
			offsetY = 0.0f;
		}

		float offsetZ;

		if (distanceZ > meshDescriptor->MaxZ)
		{
			offsetZ = distanceZ - meshDescriptor->MaxZ;
		}
		else if (distanceZ < meshDescriptor->MinZ)
		{
			offsetZ = meshDescriptor->MinZ - distanceZ;
		}
		else
		{
			offsetZ = 0.0f;
		}

		float offsetDistance = offsetX * offsetX + offsetY * offsetY + offsetZ * offsetZ;

		if (offsetDistance < minDistance)
		{
			minDistance = offsetDistance;
			selectedMeshIndex = meshIndex;

			if (minDistance == 0.0f)
			{
				break;
			}
		}
	}

	return selectedMeshIndex;
}

int MirrorHardpointHook(int* params)
{
	int edi = params[Params_EDI];
	MeshTypeEnum meshType = (MeshTypeEnum)params[18];

	if (edi >= 0x100 || IsMeshTypeGunTurret(meshType))
	{
		params[Params_ReturnAddress] = 0x00433045;
	}

	return 0;
}

struct WeaponSlot
{
	int PositionX;
	int PositionZ;
	int PositionY;
	unsigned char HardpointId;
	unsigned char MeshId;
};

WeaponSlot GetWeaponSlots(short craftIndex, int weaponSlotIndex)
{
	ExeCraftEntry* s_ExeCraftTable = (ExeCraftEntry*)0x005BB480;

	unsigned int* ptr = (unsigned int*)s_ExeCraftTable[0].WeaponSlots;

	WeaponSlot slot{};

	if (ptr[0] == 0x7FFF7FFFU && ptr[1] == 0xFFFF7FFFU)
	{
		int ptrIndex = craftIndex * 987 + weaponSlotIndex * 8;
		int offset1 = ptr[2] + ptrIndex;
		int offset2 = ptr[3] + ptrIndex;

		slot.PositionX = *(int*)(offset1 + 0);
		slot.PositionZ = *(int*)(offset1 + 4);
		slot.PositionY = *(int*)(offset2 + 0);
		slot.HardpointId = *(unsigned char*)(offset2 + 4);
		slot.MeshId = *(unsigned char*)(offset2 + 5);
	}
	else
	{
		const ExeCraftWeaponSlot& exeWeaponSlot = s_ExeCraftTable[craftIndex].WeaponSlots[weaponSlotIndex];

		slot.PositionX = exeWeaponSlot.PositionX;
		slot.PositionZ = exeWeaponSlot.PositionZ;
		slot.PositionY = exeWeaponSlot.PositionY;
		slot.HardpointId = exeWeaponSlot.HardpointId;
		slot.MeshId = exeWeaponSlot.MeshId;
	}

	return slot;
}

int PlayerFireHook(int* params)
{
	params[Params_EBX] = 0xffff;

	if (!g_config.EnableFireMeshFilter)
	{
		return 0;
	}

	const int objectIndex = params[54];
	const int weaponSlotIndex = params[55];

	ExeCraftEntry* s_ExeCraftTable = (ExeCraftEntry*)0x005BB480;
	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	XwaCraft* XwaCurrentCraft = *(XwaCraft**)0x00910DFC;

	char* m292;

	if (g_craftConfig.MeshesCount == 0)
	{
		m292 = XwaCurrentCraft->XwaCraft_m292;
	}
	else
	{
		m292 = (char*)((int)XwaCurrentCraft + g_craftConfig.Craft_Offset_292);
	}

	unsigned char meshId = GetWeaponSlots(XwaCurrentCraft->CraftIndex, weaponSlotIndex).MeshId;

	if (m292[meshId] == 0)
	{
		params[Params_ReturnAddress] = 0x00492F12;
		return 0;
	}

	return 0;
}

struct WarheadStats
{
	int CapacityPercent;
};

WarheadStats GeWarheadStats(const std::vector<std::string>& lines, int sourceModelIndex, const std::string& profileName, int warheadIndex)
{
	//auto lines = GetModelLines(sourceModelIndex, "WarheadStats");

	WarheadStats stats{};
	std::string warheadKey = "Warhead" + std::to_string(warheadIndex);

	if (!profileName.empty())
	{
		warheadKey = profileName + "_" + warheadKey;
	}

	stats.CapacityPercent = GetFileKeyValueInt(lines, warheadKey + "_CapacityPercent", -1);

	if (stats.CapacityPercent < -1)
	{
		stats.CapacityPercent = 0;
	}

	return stats;
}

std::string GetWarheadProfileName(int fgIndex)
{
	auto lines = GetMissionLines("WarheadProfiles");

	std::string name = GetFileKeyValue(lines, "WarheadProfile_fg_" + std::to_string(fgIndex));

	if (_stricmp(name.c_str(), "Default") == 0)
	{
		name = std::string();
	}

	return name;
}

class ModelIndexWarhead
{
public:
	const std::vector<std::string>& GetWarheadStatsLines(int sourceModelIndex)
	{
		auto it = this->_warheadStatsLines.find(sourceModelIndex);

		if (it == this->_warheadStatsLines.end())
		{
			auto modelLines = GetModelLines(sourceModelIndex, "WarheadStats");

			this->_warheadStatsLines.insert(std::make_pair(sourceModelIndex, modelLines));
			it = this->_warheadStatsLines.find(sourceModelIndex);
		}

		return it->second;
	}

	const WarheadStats& GetStats(int sourceObjectIndex, int warheadIndex)
	{
		this->Update();

		const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
		const XwaObject* sourceObject = &xwaObjects[sourceObjectIndex];

		unsigned short sourceModelIndex = sourceObject->ModelIndex;
		int sourceFgIndex = sourceObject->TieFlightGroupIndex;

		auto it = this->_warheadStats.find(std::make_tuple(sourceFgIndex, warheadIndex));

		if (it == this->_warheadStats.end())
		{
			std::string profileName = GetWarheadProfileName(sourceFgIndex);
			auto& lines = GetWarheadStatsLines(sourceModelIndex);
			WarheadStats stats = GeWarheadStats(lines, sourceModelIndex, profileName, warheadIndex);
			this->_warheadStats.insert(std::make_pair(std::make_tuple(sourceFgIndex, warheadIndex), stats));
			it = this->_warheadStats.find(std::make_tuple(sourceFgIndex, warheadIndex));
		}

		return it->second;
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

			this->_warheadStats.clear();
			this->_warheadStatsLines.clear();
		}
	}

	std::map<std::tuple<int, int>, WarheadStats> _warheadStats;
	std::map<int, std::vector<std::string>> _warheadStatsLines;
};

ModelIndexWarhead g_modelIndexWarhead;

int WarheadCapacity_0041BE55_Hook(int* params)
{
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const unsigned short* s_XwaTieWarheadTypeCapacityPercent = (unsigned short*)0x005B12C0;
	int capacity = (unsigned short)params[0];
	int warheadType = params[Params_EDX];
	int percent = s_XwaTieWarheadTypeCapacityPercent[warheadType];
	percent = percent == 0xffff ? 100 : ((percent * 100 + 0x8000) / 0x10000);

	int objectIndex = (unsigned short)params[12];
	int value = g_modelIndexWarhead.GetStats(objectIndex, warheadType).CapacityPercent;

	if (value != -1)
	{
		percent = value;
	}

	capacity = (capacity * percent + 50) / 100;

	int maxCapacity = GetHangarWarheadMaxCount(xwaObjects[objectIndex].TieFlightGroupIndex, warheadType, capacity);
	if (maxCapacity != -1)
	{
		capacity = min(capacity, maxCapacity);
	}

	return capacity;
}

int WarheadCapacity_0045CBFD_Hook(int* params)
{
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const unsigned short* s_XwaTieWarheadTypeCapacityPercent = (unsigned short*)0x005B12C0;
	int capacity = (unsigned short)params[0];
	int warheadType = params[Params_EDI];
	int percent = s_XwaTieWarheadTypeCapacityPercent[warheadType];
	percent = percent == 0xffff ? 100 : ((percent * 100 + 0x8000) / 0x10000);

	XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	int playerId = *(int*)0x008C1CC8;
	int objectIndex = XwaPlayers[playerId].ObjectIndex;
	int value = g_modelIndexWarhead.GetStats(objectIndex, warheadType).CapacityPercent;

	if (value != -1)
	{
		percent = value;
	}

	capacity = (capacity * percent + 50) / 100;

	int maxCapacity = GetHangarWarheadMaxCount(xwaObjects[objectIndex].TieFlightGroupIndex, warheadType, capacity);
	if (maxCapacity != -1)
	{
		capacity = min(capacity, maxCapacity);
	}

	return capacity;
}

int WarheadCapacity_00460904_Hook(int* params)
{
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const unsigned short* s_XwaTieWarheadTypeCapacityPercent = (unsigned short*)0x005B12C0;
	int capacity = (unsigned short)params[0];
	int warheadType = params[Params_EAX];
	int percent = s_XwaTieWarheadTypeCapacityPercent[warheadType];
	percent = percent == 0xffff ? 100 : ((percent * 100 + 0x8000) / 0x10000);

	int s_XwaHangarPlayerObjectIndex = *(int*)0x0068BC08;
	int objectIndex = s_XwaHangarPlayerObjectIndex;
	int value = g_modelIndexWarhead.GetStats(objectIndex, warheadType).CapacityPercent;

	if (value != -1)
	{
		percent = value;
	}

	capacity = (capacity * percent + 50) / 100;

	int maxCapacity = GetHangarWarheadMaxCount(xwaObjects[objectIndex].TieFlightGroupIndex, warheadType, capacity);
	if (maxCapacity != -1)
	{
		capacity = min(capacity, maxCapacity);
	}

	return capacity;
}

int WarheadCapacity_004B1426_Hook(int* params)
{
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const unsigned short* s_XwaTieWarheadTypeCapacityPercent = (unsigned short*)0x005B12C0;
	int capacity = (unsigned short)params[0];
	int warheadType = params[Params_EAX];
	int percent = s_XwaTieWarheadTypeCapacityPercent[warheadType];
	percent = percent == 0xffff ? 100 : ((percent * 100 + 0x8000) / 0x10000);

	int objectIndex = (unsigned short)params[9];
	int value = g_modelIndexWarhead.GetStats(objectIndex, warheadType).CapacityPercent;

	if (value != -1)
	{
		percent = value;
	}

	capacity = (capacity * percent + 50) / 100;

	int maxCapacity = GetHangarWarheadMaxCount(xwaObjects[objectIndex].TieFlightGroupIndex, warheadType, capacity);
	if (maxCapacity != -1)
	{
		capacity = min(capacity, maxCapacity);
	}

	return capacity;
}

struct EnergyStats
{
	float SpeedFactor;
};

EnergyStats GetEnergyStats(const std::vector<std::string>& lines, int sourcePlayerIndex, int sourceModelIndex, const std::string& profileName)
{
	//auto lines = GetModelLines(sourceModelIndex, "EnergyStats");

	EnergyStats stats{};
	std::string energyKey;

	if (!profileName.empty())
	{
		energyKey = profileName + "_";
	}

	std::string speedFactorString;
	bool isDefaultValue = true;

	if (sourcePlayerIndex != -1)
	{
		speedFactorString = GetFileKeyValue(lines, energyKey + "SpeedFactorForPlayer");
		isDefaultValue = speedFactorString.empty() || std::stoi(speedFactorString, 0, 0) == -1;
	}

	if (isDefaultValue)
	{
		speedFactorString = GetFileKeyValue(lines, energyKey + "SpeedFactor");
		isDefaultValue = speedFactorString.empty() || std::stoi(speedFactorString, 0, 0) == -1;
	}

	float speedFactor;

	if (isDefaultValue)
	{
		// ModelIndex_007_0_6_TieBomber
		if (sourceModelIndex == 7)
		{
			speedFactor = (float)0x1000 / (float)0x10000;
		}
		// ModelIndex_005_0_4_TieFighter
		else if (sourceModelIndex == 5)
		{
			speedFactor = (float)0x3000 / (float)0x10000;
		}
		else
		{
			speedFactor = (float)0x2000 / (float)0x10000;
		}
	}
	else
	{
		speedFactor = std::stof(speedFactorString) * 0.01f;

		if (speedFactor < 0.0f)
		{
			speedFactor = 0.0f;
		}
		else if (speedFactor > 1.0f)
		{
			speedFactor = 1.0f;
		}
	}

	stats.SpeedFactor = speedFactor;

	return stats;
}

std::string GetEnergyProfileName(int fgIndex)
{
	auto lines = GetMissionLines("EnergyProfiles");

	std::string name = GetFileKeyValue(lines, "EnergyProfile_fg_" + std::to_string(fgIndex));

	if (_stricmp(name.c_str(), "Default") == 0)
	{
		name = std::string();
	}

	return name;
}

class ModelIndexEnergy
{
public:
	const std::vector<std::string>& GetEnergyStatsLines(int sourceModelIndex)
	{
		auto it = this->_energyStatsLines.find(sourceModelIndex);

		if (it == this->_energyStatsLines.end())
		{
			auto modelLines = GetModelLines(sourceModelIndex, "EnergyStats");

			this->_energyStatsLines.insert(std::make_pair(sourceModelIndex, modelLines));
			it = this->_energyStatsLines.find(sourceModelIndex);
		}

		return it->second;
	}

	const EnergyStats& GetStats(int sourceObjectIndex)
	{
		this->Update();

		const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
		const XwaObject* sourceObject = &xwaObjects[sourceObjectIndex];

		int sourcePlayerIndex = sourceObject->PlayerIndex;
		unsigned short sourceModelIndex = sourceObject->ModelIndex;
		int sourceFgIndex = sourceObject->TieFlightGroupIndex;

		auto it = this->_energyStats.find(sourceFgIndex);

		if (it == this->_energyStats.end())
		{
			std::string profileName = GetEnergyProfileName(sourceFgIndex);
			auto& lines = GetEnergyStatsLines(sourceModelIndex);
			EnergyStats stats = GetEnergyStats(lines, sourcePlayerIndex, sourceModelIndex, profileName);
			this->_energyStats.insert(std::make_pair(sourceFgIndex, stats));
			it = this->_energyStats.find(sourceFgIndex);
		}

		return it->second;
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

			this->_energyStats.clear();
			this->_energyStatsLines.clear();
		}
	}

	std::map<int, EnergyStats> _energyStats;
	std::map<int, std::vector<std::string>> _energyStatsLines;
};

ModelIndexEnergy g_modelIndexEnergy;

int EnergySpeedFactorHook(int* params)
{
	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	int objectIndex = params[Params_ESI] / 0x27;

	float factor = g_modelIndexEnergy.GetStats(objectIndex).SpeedFactor;

	factor = factor * 0x10000;

	return (int)factor;
}

struct LinkingStats
{
	int LaserLink;
	bool PreventAILasersIonWhenDisabling;
	bool PreventAIIonWhenNotDisabling;
	bool PreventPlayerLinkedLasersIon;
};

LinkingStats GetLinkingStats(const std::vector<std::string>& lines, int sourceModelIndex, const std::string& profileName)
{
	//auto lines = GetModelLines(sourceModelIndex, "LinkingStats");

	LinkingStats stats{};
	std::string linkingKey;

	if (!profileName.empty())
	{
		linkingKey = profileName + "_";
	}

	stats.LaserLink = GetFileKeyValueInt(lines, linkingKey + "LaserLink", -1);

	int preventAILasersIonWhenDisabling = GetFileKeyValueInt(lines, linkingKey + "PreventAILasersIonWhenDisabling", -1);

	if (preventAILasersIonWhenDisabling == -1)
	{
		stats.PreventAILasersIonWhenDisabling = g_config.PreventAILasersIonWhenDisabling;
	}
	else
	{
		stats.PreventAILasersIonWhenDisabling = preventAILasersIonWhenDisabling != 0;
	}

	int preventAIIonWhenNotDisabling = GetFileKeyValueInt(lines, linkingKey + "PreventAIIonWhenNotDisabling", -1);

	if (preventAIIonWhenNotDisabling == -1)
	{
		stats.PreventAIIonWhenNotDisabling = g_config.PreventAIIonWhenNotDisabling;
	}
	else
	{
		stats.PreventAIIonWhenNotDisabling = preventAIIonWhenNotDisabling != 0;
	}

	int preventPlayerLinkedLasersIon = GetFileKeyValueInt(lines, linkingKey + "PreventPlayerLinkedLasersIon", -1);

	if (preventPlayerLinkedLasersIon == -1)
	{
		stats.PreventPlayerLinkedLasersIon = g_config.PreventPlayerLinkedLasersIon;
	}
	else
	{
		stats.PreventPlayerLinkedLasersIon = preventPlayerLinkedLasersIon != 0;
	}

	return stats;
}

std::string GetLinkingProfileName(int fgIndex)
{
	auto lines = GetMissionLines("LinkingProfiles");

	std::string name = GetFileKeyValue(lines, "LinkingProfile_fg_" + std::to_string(fgIndex));

	if (_stricmp(name.c_str(), "Default") == 0)
	{
		name = std::string();
	}

	return name;
}

class ModelIndexLinking
{
public:
	const std::vector<std::string>& GetLinkingStatsLines(int sourceModelIndex)
	{
		auto it = this->_linkingStatsLines.find(sourceModelIndex);

		if (it == this->_linkingStatsLines.end())
		{
			auto modelLines = GetModelLines(sourceModelIndex, "LinkingStats");

			this->_linkingStatsLines.insert(std::make_pair(sourceModelIndex, modelLines));
			it = this->_linkingStatsLines.find(sourceModelIndex);
		}

		return it->second;
	}

	const LinkingStats& GetStats(int sourceObjectIndex)
	{
		this->Update();

		const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
		const XwaObject* sourceObject = &xwaObjects[sourceObjectIndex];

		unsigned short sourceModelIndex = sourceObject->ModelIndex;
		int sourceFgIndex = sourceObject->TieFlightGroupIndex;

		auto it = this->_linkingStats.find(sourceFgIndex);

		if (it == this->_linkingStats.end())
		{
			std::string profileName = GetLinkingProfileName(sourceFgIndex);
			auto& lines = GetLinkingStatsLines(sourceModelIndex);
			LinkingStats stats = GetLinkingStats(lines, sourceModelIndex, profileName);
			this->_linkingStats.insert(std::make_pair(sourceFgIndex, stats));
			it = this->_linkingStats.find(sourceFgIndex);
		}

		return it->second;
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

			this->_linkingStats.clear();
			this->_linkingStatsLines.clear();
		}
	}

	std::map<int, LinkingStats> _linkingStats;
	std::map<int, std::vector<std::string>> _linkingStatsLines;
};

ModelIndexLinking g_modelIndexLinking;

int LasersLinkAIFighterHook(int* params)
{
	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	int objectIndex = *(int*)0x07CA1A0;
	int laserLink = g_modelIndexLinking.GetStats(objectIndex).LaserLink;

	if (laserLink != -1)
	{
		params[8] = laserLink;
	}

	params[Params_EDX] = *(int*)0x007CA1CC;
	return 0;
}

int LasersLinkPlayerFighterHook(int* params)
{
	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	int objectIndex = params[12];
	int laserLink = g_modelIndexLinking.GetStats(objectIndex).LaserLink;

	if (laserLink == -1)
	{
		laserLink = *(unsigned char*)(params[Params_ECX] + params[Params_EBP] + 0x01B6);
	}

	return laserLink;
}

int LasersLinkPlayerKeyXHook(int* params)
{
	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	int objectIndex = *(int*)(params[Params_EBP] - 0x08) / 0x27;
	int laserLink = g_modelIndexLinking.GetStats(objectIndex).LaserLink;

	if (laserLink != -1)
	{
		*(int*)(params[Params_EBP] - 0x10) = laserLink;
	}

	params[Params_EAX] = *(int*)(params[Params_EBP] - 0x10);
	params[Params_ECX] = 0;

	return 0;
}

int PreventAILasersIonWhenDisablingHook(int* params)
{
	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	int objectIndex = *(int*)0x07CA1A0;
	bool value = g_modelIndexLinking.GetStats(objectIndex).PreventAILasersIonWhenDisabling;

	if (value)
	{
		params[Params_ReturnAddress] = 0x004A7A33;
	}
	else
	{
		int eax = params[Params_EAX];
		int edx = params[Params_EDX];

		if (eax < edx)
		{
			params[Params_ReturnAddress] = 0x004A7A33;
		}
	}

	return 0;
}

int PreventAIIonWhenNotDisablingHook(int* params)
{
	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	int objectIndex = *(int*)0x07CA1A0;
	bool value = g_modelIndexLinking.GetStats(objectIndex).PreventAIIonWhenNotDisabling;

	if (value)
	{
		params[Params_ReturnAddress] = 0x004A79F9;
	}
	else
	{
		params[Params_EDX] = *(int*)0x008BF378;
	}

	return 0;
}

int PreventPlayerLinkedLasersIonHook(int* params)
{
	params[Params_EAX] = *(unsigned char*)(params[Params_EBX] + params[Params_EAX] + 0x01B6);

	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	int objectIndex = *(int*)(params[Params_EBP] - 0x08) / 0x27;
	bool value = g_modelIndexLinking.GetStats(objectIndex).PreventPlayerLinkedLasersIon;

	if (value)
	{
		*(unsigned short*)0x004FC9CB = 0x15EB;
		*(unsigned int*)0x004FC9EB = 0x00033D66;
	}
	else
	{
		*(unsigned short*)0x004FC9CB = 0x1575;
		*(unsigned int*)0x004FC9EB = 0x00043D66;
	}

	return 0;
}

int WarheadLockDistanceHook(int* params)
{
	//int method = 0;

	//if (method == 0)
	//{
	//	params[Params_EBX] = 0x18DAD;

	//	if (params[Params_ESI] < params[Params_EAX])
	//	{
	//		params[Params_ReturnAddress] = 0x0048FE46;
	//	}

	//	return 0;
	//}

	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;

	int index = params[Params_ESI];
	ShipCategoryEnum category = XwaObjects[index].ShipCategory;
	unsigned short modelIndex = XwaObjects[index].ModelIndex;

	int distance = 101805;

	if (category == ShipCategory_Starship
		|| category == ShipCategory_Platform
		|| category == ShipCategory_Container
		|| category == ShipCategory_Freighter)
	{
		distance = 244332;
	}
	else if (category == ShipCategory_Transport)
	{
		const unsigned short ModelIndex_057_0_44_MuurianTransport = 57;
		const unsigned short ModelIndex_058_0_45_CorellianTransport2 = 58;
		const unsigned short ModelIndex_059_0_46_MilleniumFalcon2 = 59;
		const unsigned short ModelIndex_060_0_47_Outrider = 60;
		const unsigned short ModelIndex_065_0_52_FamilyTransport = 65;

		if (modelIndex == ModelIndex_057_0_44_MuurianTransport
			|| modelIndex == ModelIndex_058_0_45_CorellianTransport2
			|| modelIndex == ModelIndex_059_0_46_MilleniumFalcon2
			|| modelIndex == ModelIndex_060_0_47_Outrider
			|| modelIndex == ModelIndex_065_0_52_FamilyTransport)
		{
			distance = 244332;
		}
	}
	else if (category == ShipCategory_Armament)
	{
		const unsigned short ModelIndex_194_0_133_AsteroidLaserBattery = 194;
		const unsigned short ModelIndex_195_0_134_AsteroidWarheadLauncher = 195;

		if (modelIndex == ModelIndex_194_0_133_AsteroidLaserBattery
			|| modelIndex == ModelIndex_195_0_134_AsteroidWarheadLauncher)
		{
			distance = 244332;
		}
	}

	params[Params_EBX] = distance;
	params[Params_ReturnAddress] = 0x0048FE46;

	return 0;
}

int ProjectileTurretThreatHook(int* params)
{
	const auto L004A27A0 = (int(*)(int))0x004A27A0;

	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;

	//int method = 0;

	//if (method == 0)
	//{
	//	params[Params_EAX] = L004A27A0(params[0]);
	//	return 0;
	//}

	int index = params[0];

	if (index != 0xFFFF)
	{
		ShipCategoryEnum category = XwaObjects[index].ShipCategory;

		if (category == ShipCategory_PlayerProjectile
			|| category == ShipCategory_OtherProjectile)
		{
			params[Params_EAX] = 0;
			return 0;
		}
	}

	int result = L004A27A0(index);
	params[Params_EAX] = result;
	return 0;
}

void EnergyLowHighSeparationFunction1(int* params)
{
	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const XwaObject* sourceObject = (XwaObject*)params[Params_ESI];
	const int sourceObjectIndex = sourceObject - xwaObjects;

	const XwaCraft* pCraft = (XwaCraft*)params[Params_EDX];
	const int rackIndex = params[Params_EDI];

	int weaponIndex = params[Params_ECX] / 2;

	const XwaCraftWeaponRack* weaponRacks = (XwaCraftWeaponRack*)((int)pCraft + g_craftConfig.Craft_Offset_2DF);

	if (weaponRacks[rackIndex].Charge >= 0x40)
	{
		weaponIndex--;
	}

	int energyLowHighSeparation = g_modelIndexWeapon.GetStats(sourceObjectIndex, weaponIndex).EnergyLowHighSeparation;

	if (weaponRacks[rackIndex].Charge >= energyLowHighSeparation)
	{
		weaponIndex++;
	}

	params[Params_ECX] = weaponIndex * 2;
}

bool EnergyLowHighSeparationFunction2(int* params)
{
	if (params[Params_ReturnAddress] != 0x004915F6)
	{
		return true;
	}

	XwaCraft* XwaCurrentCraft = *(XwaCraft**)0x00910DFC;

	const int objectIndex = params[0];
	const int rackIndex = params[1];

	XwaCraftWeaponRack* weaponRacks = (XwaCraftWeaponRack*)((int)XwaCurrentCraft + g_craftConfig.Craft_Offset_2DF);

	unsigned short modelIndex = weaponRacks[rackIndex].ModelIndex;

	if (modelIndex == 280 || modelIndex == 282 || modelIndex == 284)
	{
		int energyLowHighSeparation = g_modelIndexWeapon.GetStats(objectIndex, modelIndex - 0x118).EnergyLowHighSeparation;

		if (weaponRacks[rackIndex].Charge >= energyLowHighSeparation)
		{
			modelIndex++;
		}

		params[2] = modelIndex;
		params[Params_ECX] = modelIndex;
		params[Params_EBP] = modelIndex;

		int charge = weaponRacks[rackIndex].Charge;
		int dechargeRate = g_modelIndexWeapon.GetStats(objectIndex, modelIndex - 0x118).DechargeRate;

		if (charge < dechargeRate)
		{
			return false;
		}
	}

	return true;
}

int DrawEnergyBarHook(int* params)
{
	XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	int playerId = *(int*)0x008C1CC8;
	int objectIndex = XwaPlayers[playerId].ObjectIndex;

	XwaCraft* pCraft = (XwaCraft*)params[4];
	int rackIndex = (params[7] - params[4] - 0x02E3) / 0x0E;

	XwaCraftWeaponRack* weaponRacks = (XwaCraftWeaponRack*)((int)pCraft + g_craftConfig.Craft_Offset_2DF);

	unsigned short modelIndex = weaponRacks[rackIndex].ModelIndex;

	int charge = (unsigned short)params[Params_ECX];
	int energyLowHighSeparation = g_modelIndexWeapon.GetStats(objectIndex, modelIndex - 0x118).EnergyLowHighSeparation;

	int ebp;
	int eax;

	if (charge <= energyLowHighSeparation)
	{
		ebp = params[6];
		eax = charge;
	}
	else
	{
		ebp = params[9];
		eax = charge - energyLowHighSeparation;
	}

	int edi = eax / 10;

	if (edi == 0)
	{
		if (charge < 10 || charge > energyLowHighSeparation)
		{
			edi = 1;
		}
	}

	if (edi > 6)
	{
		edi = 6;
	}

	int ebx = 0;

	if (charge > energyLowHighSeparation)
	{
		ebx = 6 - edi;
	}

	params[Params_EBP] = ebp;
	params[Params_EAX] = eax;
	params[Params_EDI] = edi;
	params[Params_EBX] = ebx;

	params[Params_ESI] = 0x01;
	return 0;
}

int SetupLaserChargePositionsHook(int* params)
{
	// 5 weapons => 6 weapons
	*(unsigned char*)0x0046A25F = 0x90;
	*(unsigned char*)0x0046A260 = 0xE9;

	// 6 weapons
	int width = *(short*)0x068C7D4 + *(short*)0x068C7D0;
	*(int*)0x068C7FC = *(short*)0x068C7D0 + width / 2;
	*(int*)0x068C804 = *(short*)0x068C7D4 - width / 2;
	*(int*)0x068C800 = *(int*)0x068C804 + width;

	*(int*)0x068C808 = *(short*)0x068C7D8;
	*(int*)0x068C80C = *(short*)0x068C7DC;

	params[Params_EAX] = *(int*)0x008C1CC8;
	return 0;
}

int SetWeaponsCount1Hook(int* params)
{
	int count = params[Params_EAX] * g_config.WeaponsCount;
	params[Params_ESI] = params[Params_ECX] + count;
	params[Params_EDI] = params[Params_ESI] + g_config.WeaponsCount;

	return 0;
}

int SetWeaponsCount2Hook(int* params)
{
	int count = params[Params_EDI] * g_config.WeaponsCount;
	params[Params_EAX] = count;

	return 0;
}

struct WarheadTypeCount
{
	int SpaceBombs;
	int HeavyRockets;
	int Missiles;
	int ProtonTorpedos;
	int AdvancedMissiles;
	int AdvancedTorpedos;
	int MagPulse;
	int IonPulse;
	int AdvancedMagPulse;
	int ClusterBombs;
};

WarheadTypeCount GetMissionWarheadTypeCount(int fgIndex)
{
	auto lines = GetMissionLines("WarheadTypeCount");

	WarheadTypeCount missionCount{};
	std::string key = "WarheadTypeCount_fg_" + std::to_string(fgIndex);

	missionCount.SpaceBombs = GetFileKeyValueInt(lines, key + "_SpaceBombs", -1);
	missionCount.HeavyRockets = GetFileKeyValueInt(lines, key + "_HeavyRockets", -1);
	missionCount.Missiles = GetFileKeyValueInt(lines, key + "_Missiles", -1);
	missionCount.ProtonTorpedos = GetFileKeyValueInt(lines, key + "_ProtonTorpedos", -1);
	missionCount.AdvancedMissiles = GetFileKeyValueInt(lines, key + "_AdvancedMissiles", -1);
	missionCount.AdvancedTorpedos = GetFileKeyValueInt(lines, key + "_AdvancedTorpedos", -1);
	missionCount.MagPulse = GetFileKeyValueInt(lines, key + "_MagPulse", -1);
	missionCount.IonPulse = GetFileKeyValueInt(lines, key + "_IonPulse", -1);
	missionCount.AdvancedMagPulse = GetFileKeyValueInt(lines, key + "_AdvancedMagPulse", -1);
	missionCount.ClusterBombs = GetFileKeyValueInt(lines, key + "_ClusterBombs", -1);

	return missionCount;
}

class MissionWarheadType
{
public:
	WarheadTypeCount& GetWarheadTypeCount(int fgIndex)
	{
		this->Update();

		auto it = this->_warheadRemaining.find(fgIndex);

		if (it == this->_warheadRemaining.end())
		{
			auto warheadCount = GetMissionWarheadTypeCount(fgIndex);
			this->_warheadRemaining.insert(std::make_pair(fgIndex, warheadCount));
			it = this->_warheadRemaining.find(fgIndex);
		}

		return it->second;
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

			this->_warheadRemaining.clear();
		}
	}

	std::map<int, WarheadTypeCount> _warheadRemaining;
};

MissionWarheadType g_missionWarheadType;

int& GetHangarWarheadTypeCount(WarheadTypeCount& missionCount, int warheadType)
{
	static int _default;

	switch (warheadType)
	{
	case 1:
		return missionCount.SpaceBombs;
	case 2:
		return missionCount.HeavyRockets;
	case 3:
		return missionCount.Missiles;
	case 4:
		return missionCount.ProtonTorpedos;
	case 5:
		return missionCount.AdvancedMissiles;
	case 6:
		return missionCount.AdvancedTorpedos;
	case 7:
		return missionCount.MagPulse;
	case 8:
		return missionCount.IonPulse;
	case 9:
		return missionCount.AdvancedMagPulse;
	case 10:
		return missionCount.ClusterBombs;
	}

	return _default;
}

int GetHangarWarheadMaxCount(int fgIndex, int warheadType, int capacity)
{
	WarheadTypeCount& missionCount = g_missionWarheadType.GetWarheadTypeCount(fgIndex);
	int count = -1;

	if (warheadType >= 1 && warheadType <= 10)
	{
		count = GetHangarWarheadTypeCount(missionCount, warheadType);

		if (count != -1)
		{
			if (capacity < count)
			{
				GetHangarWarheadTypeCount(missionCount, warheadType) -= capacity;
			}
			else
			{
				GetHangarWarheadTypeCount(missionCount, warheadType) = 0;
			}
		}
	}

	return count;
}

int HangarWarheadReloadHook(int* params)
{
	XwaObject* object = (XwaObject*)params[Params_ESI];
	unsigned char fgIndex = object->TieFlightGroupIndex;
	int warheadType = (short)params[Params_EDI];
	int requestedCount = (short)params[Params_EDX];
	unsigned char rackCount = (unsigned char)params[Params_EAX];

	WarheadTypeCount& missionCount = g_missionWarheadType.GetWarheadTypeCount(fgIndex);

	int load = -1;

	if (warheadType >= 1 && warheadType <= 10)
	{
		load = GetHangarWarheadTypeCount(missionCount, warheadType);
	}

	if (load != -1)
	{
		requestedCount = min(requestedCount, rackCount + load);
		params[Params_EDX] = requestedCount;

		if (rackCount < requestedCount)
		{
			if (warheadType >= 1 && warheadType <= 10)
			{
				GetHangarWarheadTypeCount(missionCount, warheadType)--;
			}
		}
	}

	if (rackCount >= requestedCount)
	{
		params[Params_ReturnAddress] = 0x00460A1B;
	}

	return 0;
}

int HangarWarheadCountHook(int* params)
{
	*(int*)0x00686D6C = params[Params_EDX];

	const XwaObject* xwaObjects = *(XwaObject**)0x007B33C4;
	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const int playerId = *(int*)0x008C1CC8;
	const int objectIndex = XwaPlayers[playerId].ObjectIndex;
	const XwaObject* object = &xwaObjects[objectIndex];
	const unsigned char fgIndex = object->TieFlightGroupIndex;

	int warheadType = *(short*)params[Params_EBX];
	unsigned short weaponModelIndex = (unsigned short)params[Params_EBP];
	int itemIndex = params[Params_EDX] - 1;

	if (weaponModelIndex == 0)
	{
		return 0;
	}

	WarheadTypeCount& missionCount = g_missionWarheadType.GetWarheadTypeCount(fgIndex);

	int load = -1;

	if (warheadType >= 1 && warheadType <= 10)
	{
		load = GetHangarWarheadTypeCount(missionCount, warheadType);
	}

	char* name = (char*)(0x00689488 + itemIndex * 0x32);

	if (load != -1)
	{
		strcat_s(name, 50, " (");
		strcat_s(name, 50, std::to_string(load).c_str());
		strcat_s(name, 50, ")");
	}

	return 0;
}
