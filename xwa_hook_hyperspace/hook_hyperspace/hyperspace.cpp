#include "targetver.h"
#include "hyperspace.h"
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

class Config
{
public:
	Config()
	{
		auto lines = GetFileLines("hook_hyperspace.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_hyperspace");
		}

		this->ShortHyperspaceEffect = GetFileKeyValueInt(lines, "ShortHyperspaceEffect", 0);

		if (this->ShortHyperspaceEffect < 0 || this->ShortHyperspaceEffect > 1)
		{
			this->ShortHyperspaceEffect = 0;
		}
	}

	int ShortHyperspaceEffect;
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

		this->SfxEnterHyperspaceIndex = GetFileKeyValueInt(lines, "sfx_enterhyperspace_index");
		this->SfxEnterHyperspaceCount = GetFileKeyValueInt(lines, "sfx_enterhyperspace_count");
		this->SfxExitHyperspaceIndex = GetFileKeyValueInt(lines, "sfx_exithyperspace_index");
		this->SfxExitHyperspaceCount = GetFileKeyValueInt(lines, "sfx_exithyperspace_count");
	}

	bool SoundsCountHookExists;
	int* SoundEffectIds;
	int SfxEnterHyperspaceIndex;
	int SfxEnterHyperspaceCount;
	int SfxExitHyperspaceIndex;
	int SfxExitHyperspaceCount;
};

SoundsConfig& GetSoundsConfig()
{
	static SoundsConfig g_soundsConfig;
	return g_soundsConfig;
}

class MissionSettings
{
public:
	int GetShortHyperspaceEffect()
	{
		this->UpdateIfChanged();
		return this->ShortHyperspaceEffect;
	}

private:
	void UpdateIfChanged()
	{
		if (this->HasChanged())
		{
			std::vector<std::string> lines;

			const std::string mission = GetStringWithoutExtension(lastMissionFileName);
			lines = GetFileLines(mission + "_Hyperspace.txt");

			if (!lines.size())
			{
				lines = GetFileLines(mission + ".ini", "Hyperspace");
			}

			this->ShortHyperspaceEffect = GetFileKeyValueInt(lines, "ShortHyperspaceEffect", -1);

			if (this->ShortHyperspaceEffect < -1 || this->ShortHyperspaceEffect > 1)
			{
				this->ShortHyperspaceEffect = -1;
			}
		}
	}

	int ShortHyperspaceEffect = -1;

	bool HasChanged()
	{
		const char* xwaMissionFileName = (const char*)0x06002E8;
		const int missionFileNameIndex = *(int*)0x06002E4;

		if (missionFileNameIndex == 0 ? (this->lastMissionFileName != xwaMissionFileName) : (this->lastMissionFileNameIndex != missionFileNameIndex))
		{
			this->lastMissionFileName = xwaMissionFileName;
			this->lastMissionFileNameIndex = missionFileNameIndex;
			return true;
		}

		return false;
	}

	std::string lastMissionFileName;
	int lastMissionFileNameIndex = 0;
};

MissionSettings g_missionSettings;

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

enum PloEnum : unsigned char
{
	Plo_Null = 0,
	Plo_StationaryLdr = 1,
	Plo_StationaryFlw = 2,
	Plo_FormLdr1 = 3,
	Plo_FlyHome = 4,
	Plo_Variable = 5,
	Plo_FormFlw1 = 6,
	Plo_FollowHome = 7,
	Plo_FormEvadeLdr1 = 8,
	Plo_FlyHomeEvade = 9,
	Plo_FormEvadeFlw1 = 10,
	Plo_FollowHomeEvade = 11,
	Plo_CapLdr1 = 12,
	Plo_CapLdr2 = 13,
	Plo_CapLdr5 = 14,
	Plo_CapEscortersLdr1 = 15,
	Plo_CapRespondLdr1 = 16,
	Plo_CapLdr4 = 17,
	Plo_CapLdr3 = 18,
	Plo_CapFlw1 = 19,
	Plo_CapFlw2 = 20,
	Plo_CapFlw5 = 21,
	Plo_CapFlw4 = 22,
	Plo_CapFlw3 = 23,
	Plo_DisableLdr1 = 24,
	Plo_EscortLdr1 = 25,
	Plo_EscortLdr2 = 26,
	Plo_EscortLdr4 = 27,
	Plo_EscortLdr3 = 28,
	Plo_EscortFlw1 = 29,
	Plo_EscortFlw2 = 30,
	Plo_EscortFlw4 = 31,
	Plo_EscortFlw3 = 32,
	Plo_BoardToGive = 33,
	Plo_Board2 = 34,
	Plo_BoardToTake = 35,
	Plo_BoardToExchange = 36,
	Plo_BoardToCapture = 37,
	Plo_BoardToDestroy = 38,
	Plo_BoardToPickup = 39,
	Plo_BoardToContact = 40,
	Plo_Board3 = 41,
	Plo_DropOffLdr1 = 42,
	Plo_DropOffLdr2 = 43,
	Plo_RenderVous1 = 44,
	Plo_RendezVous2 = 45,
	Plo_RendezVousFlw1 = 46,
	Plo_Disabled = 47,
	Plo_WaitForBoard = 48,
	Plo_CraftWaitForGo = 49,
	Plo_IntoHyperspace = 50,
	Plo_EnterHangar = 51,
	Plo_HomeViaHyperspace = 52,
	Plo_ExitHangar = 53,
	Plo_OutOfHyperspace = 54,
	Plo_StarshipIntoHyper = 55,
	Plo_StarshipStat = 56,
	Plo_StarshipFollowHome = 57,
	Plo_StarshipForm = 58,
	Plo_StarshipFollow = 59,
	Plo_StarshipWaitReturn = 60,
	Plo_StarshipWaitCreate = 61,
	Plo_StarshipProtect = 62,
	Plo_StarshipEscort = 63,
	Plo_StarshipAttack = 64,
	Plo_StarshipDisable = 65,
	Plo_StarshipWaitForGo = 66,
	Plo_Wait = 67,
	Plo_SelfDestroy = 68,
	Plo_BoardToRepair = 69,
	Plo_CapFreeLdr1 = 70,
	Plo_Kamikaze1 = 71,
	Plo_Kamikaze2 = 72,
	Plo_Kamikaze3 = 73,
	Plo_Hyperspace = 74,
	Plo_TransferCargo = 75,
	Plo_Orbit = 76,
	Plo_SelfCapture = 77,
	Plo_Release1 = 78,
	Plo_Release2 = 79,
	Plo_Deliver = 80,
	Plo_ChangeSides = 81,
	Plo_StartOver = 82,
	Plo_Backup = 83,
	Plo_HyperBuoy = 84,
	Plo_Disappear = 85,
	Plo_RepairOneself = 86,
	Plo_InspectLdr1 = 87,
	Plo_InspectLdr2 = 88,
	Plo_InspectLdr3 = 89,
	Plo_PlayerCapLdr1 = 90,
	Plo_PlayerCapLdr2 = 91,
	Plo_ResumeMission = 92,
	Plo_PlayerCapLdr4 = 93,
	Plo_playerCapLdr3 = 94,
	Plo_PlayerCapLdr5 = 95,
	Plo_PlayerFollow = 96,
	Plo_PlayerInspectLdr1 = 97,
	Plo_PlayerInspectLdr2 = 98,
	Plo_PlayerInspectLdr3 = 99,
	Plo_PlayerDisableLdr2 = 100,
	Plo_PlayerBoardToRepair = 101,
	Plo_PlayerBoardToCapture = 102,
	Plo_PlayerBoardToPickup = 103,
	Plo_PlayerBoardToDestroy = 104,
	Plo_PlayerBoardToDefuse = 105,
	Plo_PlayerBoard2 = 106,
	Plo_PlayerBoard3 = 107,
	Plo_Homing1 = 108,
	Plo_Homing2 = 109,
	Plo_Park1 = 110,
	Plo_Park2 = 111,
	Plo_WorkOn1 = 112,
	Plo_WorkOn2 = 113,
	Plo_WorkOn3 = 114,
	Plo_DeathStarFollow = 115,
	Plo_FollowTarget1 = 116,
	Plo_FollowTarget2 = 117,
	Plo_FollowTarget3 = 118,
};

struct XwaPloItem
{
	char Name[80];
	unsigned char IsSet;
	int DataOffset;
};

static_assert(sizeof(XwaPloItem) == 85, "size of XwaPloItem must be 85");

struct XwaAIData
{
	char unk00[42];
	PloEnum Pln1;
	PloEnum Pln2;
	char unk2C[1];
	PloEnum m2D;
	char unk2E[17];
	int PositionX;
	int PositionY;
	int PositionZ;
	int m4B;
	char unk4F[15];
	int m5E;
	int m62;
};

static_assert(sizeof(XwaAIData) == 102, "size of XwaAIData must be 102");

struct XwaCraft
{
	char unk000[4];
	unsigned short CraftIndex;
	int LeaderObjectIndex;
	char unk00A[30];
	XwaAIData AIData;
	char unk08E[54];
	short Speed;
	char unk0C6[819];
};

static_assert(sizeof(XwaCraft) == 1017, "size of XwaCraft must be 1017");

struct XwaMobileObject
{
	char unk00[10];
	int PositionX;
	int PositionY;
	int PositionZ;
	char unk16[111];
	short Speed;
	char unk87[56];
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
	char unkD9[4];
	XwaCraft* pCraft;
	char unkE1[4];
};

static_assert(sizeof(XwaMobileObject) == 229, "size of XwaMobileObject must be 229");

struct XwaObject
{
	char unk00[2];
	unsigned short ModelIndex;
	ShipCategoryEnum ShipCategory;
	unsigned char TieFlightGroupIndex;
	unsigned char Region;
	int PositionX;
	int PositionY;
	int PositionZ;
	short HeadingXY;
	short HeadingZ;
	char unk17[8];
	int PlayerIndex;
	XwaMobileObject* pMobileObject;
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

struct XwaPlayer
{
	int ObjectIndex;
	char Unk0004[6];
	short Iff;
	short Team;
	char Unk000E[2];
	unsigned char Region;
	char Unk0011[3006];
};

static_assert(sizeof(XwaPlayer) == 3023, "size of XwaPlayer must be 3023");

#pragma pack(pop)

struct Point3D
{
	int X;
	int Y;
	int Z;
};

const short g_AIOutOfHyperspaceSpeed = 0x7000;
const short g_AIIntoHyperspaceSpeed = 0x7000;

int GetShortHyperspaceEffectType()
{
	int missionSetting = g_missionSettings.GetShortHyperspaceEffect();

	if (missionSetting == -1)
	{
		missionSetting = g_config.ShortHyperspaceEffect;
	}

	return missionSetting;
}

int GetShipSetting(int modelIndex, const std::string& key)
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

	lines = GetFileLines(shipPath + "Hyperspace.txt");

	if (!lines.size())
	{
		lines = GetFileLines(shipPath + ".ini", "Hyperspace");
	}

	int value = GetFileKeyValueInt(lines, key, -1);
	return value;
}

int GetShipOutOfHyperspaceSpeed(int modelIndex)
{
	return GetShipSetting(modelIndex, "OutOfHyperspaceSpeed");
}

int GetShipOutOfHyperspaceSetPosition(int modelIndex)
{
	return GetShipSetting(modelIndex, "OutOfHyperspaceSetPosition");
}

int GetShipIntoHyperspaceSpeed(int modelIndex)
{
	return GetShipSetting(modelIndex, "IntoHyperspaceSpeed");
}

class ModelIndexHyperspace
{
public:
	int GetOutOfHyperspaceSpeed(int modelIndex)
	{
		this->Update();

		auto it = this->_outOfSpeed.find(modelIndex);

		if (it != this->_outOfSpeed.end())
		{
			return it->second;
		}
		else
		{
			int value = GetShipOutOfHyperspaceSpeed(modelIndex);
			this->_outOfSpeed.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

	int GetOutOfHyperspaceSetPosition(int modelIndex)
	{
		this->Update();

		auto it = this->_outOfSetPosition.find(modelIndex);

		if (it != this->_outOfSetPosition.end())
		{
			return it->second;
		}
		else
		{
			int value = GetShipOutOfHyperspaceSetPosition(modelIndex);
			this->_outOfSetPosition.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

	int GetIntoHyperspaceSpeed(int modelIndex)
	{
		this->Update();

		auto it = this->_intoSpeed.find(modelIndex);

		if (it != this->_intoSpeed.end())
		{
			return it->second;
		}
		else
		{
			int value = GetShipIntoHyperspaceSpeed(modelIndex);
			this->_outOfSpeed.insert(std::make_pair(modelIndex, value));
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

			this->_outOfSpeed.clear();
			this->_outOfSetPosition.clear();
			this->_intoSpeed.clear();
		}
	}

	std::map<int, int> _outOfSpeed;
	std::map<int, int> _outOfSetPosition;
	std::map<int, int> _intoSpeed;
};

ModelIndexHyperspace g_modelIndexHyperspace;

int GetObjectRegion(int fgIndex, bool set, int region)
{
	static std::map<int, int> _regions;

	if (set)
	{
		if (_regions.find(fgIndex) == _regions.end())
		{
			_regions.insert(std::make_pair(fgIndex, region));
		}
		else
		{
			_regions[fgIndex] = region;
		}
	}
	else
	{
		auto it = _regions.find(fgIndex);

		if (it != _regions.end())
		{
			region = it->second;
		}
	}

	return region;
}

int AIOutOfHyperspaceInitSpeedHook(int* params)
{
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	int objectIndex = *(int*)0x007CA1A0;
	XwaObject* object = &xwaObjects[objectIndex];
	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const int currentPlayerId = *(int*)0x08C1CC8;
	const XwaPlayer* currentPlayer = &XwaPlayers[currentPlayerId];

	int effectType = GetShortHyperspaceEffectType();
	XwaMobileObject* pMobileObject = (XwaMobileObject*)params[Params_EAX];

	int hyperSpeed = g_AIOutOfHyperspaceSpeed;
	int speedSetting = g_modelIndexHyperspace.GetOutOfHyperspaceSpeed(object->ModelIndex);

	if (speedSetting != -1)
	{
		hyperSpeed = speedSetting;
	}

	switch (effectType)
	{
	case 1:
		pMobileObject->Speed = hyperSpeed;
		break;

	default:
		pMobileObject->Speed = 0x0E10;
		break;
	}

	return 0;
}

int AIOutOfHyperspaceInitDelayHook(int* params)
{
	int effectType = GetShortHyperspaceEffectType();
	XwaAIData* pAIData = (XwaAIData*)params[Params_EAX];

	switch (effectType)
	{
	case 1:
		pAIData->m62 = 0;
		break;

	default:
		pAIData->m62 = 0xEC;
		break;
	}

	return 0;
}

int AIOutOfHyperspaceInitTimeHook(int* params)
{
	int effectType = GetShortHyperspaceEffectType();
	XwaAIData* pAIData = (XwaAIData*)params[Params_EDX];

	switch (effectType)
	{
	case 1:
		pAIData->m5E = 0xEC;
		break;

	default:
		pAIData->m5E = 0x0A24;
		break;
	}

	return 0;
}

int AIOutOfHyperspaceStepSpeedHook(int* params)
{
	int effectType = GetShortHyperspaceEffectType();
	XwaMobileObject* pMobileObject = (XwaMobileObject*)params[Params_EDX];
	const short speed = (short)params[Params_EAX];

	int objectIndex = *(int*)0x007CA1A0;
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	unsigned short modelIndex = xwaObjects[objectIndex].ModelIndex;

	switch (effectType)
	{
	case 1:
		//pMobileObject->Speed = g_AIOutOfHyperspaceSpeed;
		break;

	default:
		pMobileObject->Speed = speed;
		break;
	}

	return 0;
}

Point3D GetOrAddObjectOffset(int objectIndex, bool removeValue, int offsetX, int offsetY, int offsetZ)
{
	static std::map<int, Point3D> _offsets;

	auto it = _offsets.find(objectIndex);

	if (it != _offsets.end())
	{
		Point3D value = it->second;

		if (removeValue)
		{
			_offsets.erase(objectIndex);
		}

		return value;
	}
	else
	{
		Point3D value{ offsetX, offsetY, offsetZ };

		if (!removeValue)
		{
			_offsets.insert(std::make_pair(objectIndex, value));
		}

		return value;
	}
}

void GetAngles(int offsetX, int offsetY, int offsetZ, short* headingXY, short* headingZ)
{
	const auto L004EAC30 = (void(*)(int, int, int))0x004EAC30;

	L004EAC30(offsetX, offsetY, offsetZ);

	*headingXY = *(short*)0x07CAB58;
	*headingZ = *(short*)0x0910DE4;
}

int AIOutOfHyperspaceDistanceCheckHook(int* params)
{
	const auto XwaModelGetMaxPositionY = (int(*)(unsigned short))0x004856E0;

	int effectType = GetShortHyperspaceEffectType();

	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	int objectIndex = *(int*)0x007CA1A0;
	XwaObject* object = &xwaObjects[objectIndex];
	XwaAIData* pAIData = (XwaAIData*)params[Params_EAX];
	const XwaPloItem* xwaPloItems = (XwaPloItem*)0x007FFDA0;

	bool setPosition = true;
	int setPositionSetting = g_modelIndexHyperspace.GetOutOfHyperspaceSetPosition(object->ModelIndex);

	if (setPositionSetting != -1)
	{
		setPosition = setPositionSetting != 0;
	}

	XwaCraft* XwaCurrentCraft = *(XwaCraft**)0x00910DFC;

	int distance = *(int*)0x007D4B60;

	float offsetX = (float)pAIData->PositionX - (float)object->PositionX;
	float offsetY = (float)pAIData->PositionY - (float)object->PositionY;
	float offsetZ = (float)pAIData->PositionZ - (float)object->PositionZ;
	distance = (int)sqrtf(offsetX * offsetX + offsetY * offsetY + offsetZ * offsetZ);

	bool isCheck = false;
	int checkDistance;

	switch (effectType)
	{
	case 1:
		checkDistance = object->ShipCategory == ShipCategory_Starship ? 0xAAAA : 0x10000;
		break;

	default:
		checkDistance = 0;
		break;
	}

	int speed = object->pMobileObject->Speed;

	if (distance < speed * 3)
	{
		speed = distance / 3;
	}

	distance -= object->pMobileObject->Speed;

	if (distance >= pAIData->m4B || distance <= checkDistance)
	{
		isCheck = true;

		speed = 0;
	}
	else
	{
		pAIData->m4B = distance;
	}

	object->pMobileObject->Speed = speed;

	if (effectType == 1)
	{
		if (object->pMobileObject->pCraft->LeaderObjectIndex != -1)
		{
			XwaObject* leader = &xwaObjects[object->pMobileObject->pCraft->LeaderObjectIndex];

			Point3D value = GetOrAddObjectOffset(
				objectIndex,
				false,
				object->PositionX - leader->PositionX,
				object->PositionY - leader->PositionY,
				object->PositionZ - leader->PositionZ);
		}

		if (!isCheck && object->pMobileObject->pCraft->LeaderObjectIndex == -1)
		{
			short headingXY;
			short headingZ;

			GetAngles(
				pAIData->PositionX - object->PositionX,
				pAIData->PositionY - object->PositionY,
				pAIData->PositionZ - object->PositionZ,
				&headingXY,
				&headingZ);

			if (setPosition)
			{
				object->HeadingXY = headingXY;
				object->HeadingZ = headingZ;

				object->pMobileObject->RecalculateForwardVector = true;
				object->pMobileObject->RecalculateTransformMatrix = true;
			}
		}

		if (isCheck)
		{
			if (object->pMobileObject->pCraft->LeaderObjectIndex == -1)
			{
				if (setPosition)
				{
					object->PositionX = pAIData->PositionX;
					object->PositionY = pAIData->PositionY;
					object->PositionZ = pAIData->PositionZ;
				}
			}
			else
			{
				XwaObject* leader = &xwaObjects[object->pMobileObject->pCraft->LeaderObjectIndex];

				Point3D value = GetOrAddObjectOffset(objectIndex, true, 0, 0, 0);

				// todo
				//object->PositionX = leader->PositionX + value.X;
				//object->PositionY = leader->PositionY + value.Y;
				//object->PositionZ = leader->PositionZ + value.Z;
			}
		}
	}

	if (isCheck)
	{
		const auto playSound = (int(*)(int, int, int))0x0043BF90;

		const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
		const int currentPlayerId = *(int*)0x08C1CC8;
		const XwaPlayer* currentPlayer = &XwaPlayers[currentPlayerId];

		int region = GetObjectRegion(object->TieFlightGroupIndex, false, object->Region);

		if (/*effectType != 0 &&*/ region == currentPlayer->Region)
		{
			const int* xwaSoundEffectsBufferId = GetSoundsConfig().SoundEffectIds;
			const auto& soundConfig = GetSoundsConfig();

			if (soundConfig.SoundsCountHookExists && soundConfig.SfxExitHyperspaceCount)
			{
				if (object->ModelIndex < soundConfig.SfxExitHyperspaceCount)
				{
					int slot = soundConfig.SfxExitHyperspaceIndex + object->ModelIndex;

					playSound(slot, 0xFFFF, currentPlayerId);
				}
			}
		}
	}

	params[Params_EBX] = isCheck ? 1 : 0;

	return 0;
}

int AIIntoHyperspacePlaySoundHook(int* params)
{
	const int A4 = params[0];
	const int A8 = params[1];
	const int AC = params[2];

	const auto L00498D10 = (void(*)(int, int, int))0x00498D10;
	const auto playSound = (int(*)(int, int, int))0x0043BF90;

	L00498D10(A4, A8, AC);

	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	int objectIndex = *(int*)0x007CA1A0;
	XwaObject* object = &xwaObjects[objectIndex];
	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const int currentPlayerId = *(int*)0x08C1CC8;
	const XwaPlayer* currentPlayer = &XwaPlayers[currentPlayerId];

	short tieFlightGroupIndex = *(short*)(0x007CA1A0 + 0x1C);
	unsigned char orderIndex = *(unsigned char*)(0x007CA1A0 + 0x1E);
	unsigned char orderRegion = *(unsigned char*)(0x007CA1A0 + 0x1F);
	unsigned char region = *(unsigned char*)(0x0080DC80 + tieFlightGroupIndex * 0xE42 + 0xCA + (orderIndex + orderRegion * 4) * 0x94 + 0x02);

	GetObjectRegion(object->TieFlightGroupIndex, true, region);

	int effectType = GetShortHyperspaceEffectType();

	if (/*effectType != 0 &&*/ object->Region == currentPlayer->Region)
	{
		const int* xwaSoundEffectsBufferId = GetSoundsConfig().SoundEffectIds;
		const auto& soundConfig = GetSoundsConfig();

		if (soundConfig.SoundsCountHookExists && soundConfig.SfxEnterHyperspaceCount)
		{
			if (object->ModelIndex < soundConfig.SfxEnterHyperspaceCount)
			{
				int slot = soundConfig.SfxEnterHyperspaceIndex + object->ModelIndex;

				playSound(slot, 0xFFFF, currentPlayerId);
			}
		}
	}

	return 0;
}

bool IsPloHyperspace(PloEnum plo)
{
	switch (plo)
	{
	case Plo_IntoHyperspace:
	case Plo_HomeViaHyperspace:
	case Plo_OutOfHyperspace:
	case Plo_StarshipIntoHyper:
	case Plo_Hyperspace:
		return true;
	}

	return false;
}

int AIIntoHyperspaceSpeedHook(int* params)
{
	XwaMobileObject* pMobileObject = (XwaMobileObject*)params[Params_EAX];
	const short objectIndex = (short)params[6];

	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	XwaObject* pObject = &xwaObjects[objectIndex];

	int effectType = GetShortHyperspaceEffectType();

	if (effectType == 0 || pObject->PlayerIndex != -1 || !IsPloHyperspace(pMobileObject->pCraft->AIData.Pln1))
	{
		if (pMobileObject->Speed > 0xE10)
		{
			pMobileObject->Speed = 0xE10;
		}

		return 0;
	}

	int speed = pMobileObject->Speed;
	speed += speed;

	int hyperSpeed = g_AIIntoHyperspaceSpeed;
	int speedSetting = g_modelIndexHyperspace.GetIntoHyperspaceSpeed(pObject->ModelIndex);

	if (speedSetting != -1)
	{
		hyperSpeed = speedSetting;
	}

	if (speed > hyperSpeed)
	{
		speed = hyperSpeed;
	}

	pMobileObject->Speed = (short)speed;

	return 0;
}
