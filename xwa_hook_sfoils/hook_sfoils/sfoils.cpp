#include "targetver.h"
#include "sfoils.h"
#include "config.h"
#include "translate.h"
#include <fstream>
#include <map>
#include <algorithm>
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

class SoundsConfig
{
public:
	SoundsConfig()
	{
		this->SoundsCountHookExists = std::ifstream("Hook_Sounds_Count.dll") ? true : false;
		this->SoundEffectIds = this->SoundsCountHookExists ? *(int**)0x00917E80 : (int*)0x00917E80;

		if (this->SoundsCountHookExists)
		{
			auto lines = GetFileLines("Hook_Sounds_Count.txt");

			this->SfxSFoilIndex = GetFileKeyValueInt(lines, "sfx_sfoil_index");
			this->SfxSFoilCount = GetFileKeyValueInt(lines, "sfx_sfoil_count");
		}
	}

	bool SoundsCountHookExists;
	int* SoundEffectIds;
	int SfxSFoilIndex;
	int SfxSFoilCount;
};

SoundsConfig& GetSoundsConfig()
{
	static SoundsConfig g_soundsConfig;
	return g_soundsConfig;
}

class Config
{
public:
	Config()
	{
		auto lines = GetFileLines("hook_sfoils.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_sfoils");
		}

		this->CloseSFoilsAndOpenLandingGearsBeforeEnterHangar = GetFileKeyValueInt(lines, "CloseSFoilsAndOpenLandingGearsBeforeEnterHangar") != 0;
		this->CloseLandingGearsBeforeEnterHyperspace = GetFileKeyValueInt(lines, "CloseLandingGearsBeforeEnterHyperspace") != 0;
		this->AutoCloseSFoils = GetFileKeyValueInt(lines, "AutoCloseSFoils", 1) != 0;
		this->Language = GetFileKeyValue(lines, "Language");
	}

	bool CloseSFoilsAndOpenLandingGearsBeforeEnterHangar;
	bool CloseLandingGearsBeforeEnterHyperspace;
	bool AutoCloseSFoils;
	std::string Language;
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

struct XwaCraft
{
	char unk000[4];
	unsigned short CraftIndex;
	int LeaderObjectIndex;
	char unk00A[29];
	char SFoilsState;
	char unk028[568];
	unsigned char MeshRotationAngles[50];
	char unk292[359];
};

static_assert(sizeof(XwaCraft) == 1017, "size of XwaCraft must be 1017");

struct XwaMobileObject
{
	char unk00[152];
	unsigned char Team;
	char unk99[68];
	XwaCraft* pCraft;
	char unkE1[4];
};

static_assert(sizeof(XwaMobileObject) == 229, "size of XwaMobileObject must be 229");

struct XwaObject
{
	char unk000[2];
	unsigned short ModelIndex;
	ShipCategoryEnum ShipCategory;
	unsigned char TieFlightGroupIndex;
	unsigned char Region;
	int PositionX;
	int PositionY;
	int PositionZ;
	char unk013[12];
	int PlayerIndex;
	XwaMobileObject* pMobileObject;
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

struct XwaAIData
{
	char unk00[63];
	int PositionX;
	int PositionY;
	int PositionZ;
	char unk4B[17];
	unsigned char ManrId;
	unsigned char m5D;
	int m5E;
	int m62;
};

static_assert(sizeof(XwaAIData) == 102, "size of XwaAIData must be 102");

struct XwaPlayer
{
	int ObjectIndex;
	char unk004[78];
	unsigned char m052;
	char unk053[2940];
};

static_assert(sizeof(XwaPlayer) == 3023, "size of XwaPlayer must be 3023");

#pragma pack(pop)

struct SFoil
{
	int meshIndex;
	int angle;
	int closingSpeed;
	int openingSpeed;
};

struct CraftSettings
{
	bool CloseSFoilsInHyperspace;
	int SFoilsAnimationSpeed;
	bool AllowFireWhenSFoilsAreClosed;
	bool ParkOrderSFoilsClosed;
	int HangarDoorsDistance;
};

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

std::string GetShipPath(int modelIndex)
{
	const std::string shipPath = g_flightModelsList.GetLstLine(modelIndex);

	{
		const auto objectLines = GetCustomFileLines("Objects");
		const std::string objectValue = GetFileKeyValue(objectLines, shipPath + ".opt");

		if (!objectValue.empty() && std::ifstream(objectValue))
		{
			return GetStringWithoutExtension(objectValue);
		}
	}

	{
		const auto objectLines = GetCustomFileLines("HangarObjects");
		const std::string objectValue = GetFileKeyValue(objectLines, shipPath + ".opt");

		if (!objectValue.empty() && std::ifstream(objectValue))
		{
			return GetStringWithoutExtension(objectValue);
		}
	}

	return shipPath;
}

std::vector<SFoil> GetFileListSFoils(const std::vector<std::string>& lines)
{
	const auto sfoils = GetFileListValues(lines);

	std::vector<SFoil> values;

	for (unsigned int i = 0; i < sfoils.size(); i++)
	{
		const auto& value = sfoils[i];

		if (value.size() < 4)
		{
			continue;
		}

		SFoil sfoil;
		sfoil.meshIndex = std::stoi(value[0], 0, 0);
		sfoil.angle = std::stoi(value[1], 0, 0);
		sfoil.closingSpeed = std::stoi(value[2], 0, 0);
		sfoil.openingSpeed = std::stoi(value[3], 0, 0);

		values.push_back(sfoil);
	}

	return values;
}

std::vector<SFoil> GetDefaultSFoils(int modelIndex)
{
	std::vector<SFoil> values;

	switch (modelIndex)
	{
	case 1: // ModelIndex_001_0_0_Xwing
		values.push_back({ 1, 8, 1, 1 });
		values.push_back({ 2, 12, 1, 1 });
		values.push_back({ 3, 12, 1, 1 });
		values.push_back({ 4, 8, 1, 1 });
		break;

	case 4: // ModelIndex_004_0_3_Bwing
		values.push_back({ 0, 64, 2, 2 });
		values.push_back({ 3, 64, 2, 2 });
		values.push_back({ 5, 64, 2, 2 });
		values.push_back({ 6, 64, 2, 2 });
		break;

	case 50: // ModelIndex_050_0_37_Shuttle
		values.push_back({ 1, 96, 2, 2 });
		values.push_back({ 2, 96, 2, 2 });
		break;
	}

	return values;
}

std::vector<SFoil> GetSFoilsList(int modelIndex)
{
	const std::string ship = GetShipPath(modelIndex);

	auto lines = GetFileLines(ship + "SFoils.txt");

	if (!lines.size())
	{
		lines = GetFileLines(ship + ".ini", "SFoils");
	}

	std::vector<SFoil> sfoils;

	if (lines.size())
	{
		sfoils = GetFileListSFoils(lines);
	}
	else
	{
		sfoils = GetDefaultSFoils(modelIndex);
	}

	return sfoils;
}

std::vector<SFoil> GetLandingGearsList(int modelIndex)
{
	const std::string ship = GetShipPath(modelIndex);

	auto lines = GetFileLines(ship + "SFoilsLandingGears.txt");

	if (!lines.size())
	{
		lines = GetFileLines(ship + ".ini", "SFoilsLandingGears");
	}

	const auto sfoils = GetFileListValues(lines);

	std::vector<SFoil> values;

	for (unsigned int i = 0; i < sfoils.size(); i++)
	{
		const auto& value = sfoils[i];

		if (value.size() < 4)
		{
			continue;
		}

		SFoil sfoil;
		sfoil.meshIndex = std::stoi(value[0], 0, 0);
		sfoil.angle = std::stoi(value[1], 0, 0);
		sfoil.closingSpeed = std::stoi(value[2], 0, 0);
		sfoil.openingSpeed = std::stoi(value[3], 0, 0);

		values.push_back(sfoil);
	}

	return values;
}

std::vector<SFoil> GetHangarDoorsList(int modelIndex)
{
	const std::string ship = GetShipPath(modelIndex);

	auto lines = GetFileLines(ship + "SFoilsHangarDoors.txt");

	if (!lines.size())
	{
		lines = GetFileLines(ship + ".ini", "SFoilsHangarDoors");
	}

	const auto sfoils = GetFileListValues(lines);

	std::vector<SFoil> values;

	for (unsigned int i = 0; i < sfoils.size(); i++)
	{
		const auto& value = sfoils[i];

		if (value.size() < 4)
		{
			continue;
		}

		SFoil sfoil;
		sfoil.meshIndex = std::stoi(value[0], 0, 0);
		sfoil.angle = std::stoi(value[1], 0, 0);
		sfoil.closingSpeed = std::stoi(value[2], 0, 0);
		sfoil.openingSpeed = std::stoi(value[3], 0, 0);

		values.push_back(sfoil);
	}

	return values;
}

std::vector<SFoil> GetHatchesList(int modelIndex)
{
	const std::string ship = GetShipPath(modelIndex);

	auto lines = GetFileLines(ship + "SFoilsHatches.txt");

	if (!lines.size())
	{
		lines = GetFileLines(ship + ".ini", "SFoilsHatches");
	}

	const auto sfoils = GetFileListValues(lines);

	std::vector<SFoil> values;

	for (unsigned int i = 0; i < sfoils.size(); i++)
	{
		const auto& value = sfoils[i];

		if (value.size() < 4)
		{
			continue;
		}

		SFoil sfoil;
		sfoil.meshIndex = std::stoi(value[0], 0, 0);
		sfoil.angle = std::stoi(value[1], 0, 0);
		sfoil.closingSpeed = std::stoi(value[2], 0, 0);
		sfoil.openingSpeed = std::stoi(value[3], 0, 0);

		values.push_back(sfoil);
	}

	return values;
}

CraftSettings GetSFoilsSettings(int modelIndex)
{
	const std::string ship = GetShipPath(modelIndex);

	auto lines = GetFileLines(ship + "SFoils.txt");

	if (!lines.size())
	{
		lines = GetFileLines(ship + ".ini", "SFoils");
	}

	CraftSettings settings;
	settings.CloseSFoilsInHyperspace = GetFileKeyValueInt(lines, "CloseSFoilsInHyperspace", 0) == 0 ? false : true;
	settings.SFoilsAnimationSpeed = std::max(GetFileKeyValueInt(lines, "SFoilsAnimationSpeed", 1), 1);
	settings.AllowFireWhenSFoilsAreClosed = GetFileKeyValueInt(lines, "AllowFireWhenSFoilsAreClosed", 0) == 0 ? false : true;
	settings.ParkOrderSFoilsClosed = GetFileKeyValueInt(lines, "ParkOrderSFoilsClosed", 0) == 0 ? false : true;
	settings.HangarDoorsDistance = GetFileKeyValueInt(lines, "HangarDoorsDistance", 0x4000);

	return settings;
}

class ModelIndexSFoils
{
public:
	std::vector<SFoil> GetSFoils(int modelIndex)
	{
		this->UpdateIfChanged();

		auto it = this->_sfoils.find(modelIndex);

		if (it != this->_sfoils.end())
		{
			return it->second;
		}
		else
		{
			auto value = GetSFoilsList(modelIndex);
			this->_sfoils.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

	std::vector<SFoil> GetLandingGears(int modelIndex)
	{
		this->UpdateIfChanged();

		auto it = this->_landingGears.find(modelIndex);

		if (it != this->_landingGears.end())
		{
			return it->second;
		}
		else
		{
			auto value = GetLandingGearsList(modelIndex);
			this->_landingGears.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

	std::vector<SFoil> GetSFoilsAndLandingGears(int modelIndex)
	{
		auto sfoils = this->GetSFoils(modelIndex);
		auto landingGears = this->GetLandingGears(modelIndex);

		sfoils.insert(sfoils.end(), landingGears.begin(), landingGears.end());
		return sfoils;
	}

	std::vector<SFoil> GetHangarDoors(int modelIndex)
	{
		this->UpdateIfChanged();

		auto it = this->_hangarDoors.find(modelIndex);

		if (it != this->_hangarDoors.end())
		{
			return it->second;
		}
		else
		{
			auto value = GetHangarDoorsList(modelIndex);
			this->_hangarDoors.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

	std::vector<SFoil> GetHatches(int modelIndex)
	{
		this->UpdateIfChanged();

		auto it = this->_hatches.find(modelIndex);

		if (it != this->_hatches.end())
		{
			return it->second;
		}
		else
		{
			auto value = GetHatchesList(modelIndex);
			this->_hatches.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

	CraftSettings GetSettings(int modelIndex)
	{
		this->UpdateIfChanged();

		auto it = this->_settings.find(modelIndex);

		if (it != this->_settings.end())
		{
			return it->second;
		}
		else
		{
			auto value = GetSFoilsSettings(modelIndex);
			this->_settings.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

	bool AreSFoilsOpened(const XwaObject* object)
	{
		const XwaCraft* craft = object->pMobileObject->pCraft;
		const unsigned short modelIndex = object->ModelIndex;
		const auto sfoils = this->GetSFoils(modelIndex);

		const unsigned char* craftMeshRotationAngles = g_craftConfig.MeshesCount == 0 ? craft->MeshRotationAngles : (unsigned char*)((int)craft + g_craftConfig.Craft_Offset_260);

		if (!sfoils.size())
		{
			return false;
		}

		bool ret = true;

		for (unsigned int i = 0; i < sfoils.size(); i++)
		{
			const auto& sfoil = sfoils[i];

			if (craftMeshRotationAngles[sfoil.meshIndex] != 0)
			{
				ret = false;
				break;
			}
		}

		return ret;
	}

	bool AreLandingGearsClosed(const XwaObject* object)
	{
		const XwaCraft* craft = object->pMobileObject->pCraft;
		const unsigned short modelIndex = object->ModelIndex;
		const auto landingGears = this->GetLandingGears(modelIndex);

		const unsigned char* craftMeshRotationAngles = g_craftConfig.MeshesCount == 0 ? craft->MeshRotationAngles : (unsigned char*)((int)craft + g_craftConfig.Craft_Offset_260);

		if (!landingGears.size())
		{
			return true;
		}

		bool ret = true;

		for (unsigned int i = 0; i < landingGears.size(); i++)
		{
			const auto& landingGear = landingGears[i];

			if (craftMeshRotationAngles[landingGear.meshIndex] != 0)
			{
				ret = false;
				break;
			}
		}

		return ret;
	}

private:
	void UpdateIfChanged()
	{
		static std::string _mission;

		const char* xwaMissionFileName = (const char*)0x06002E8;

		if (_mission != xwaMissionFileName)
		{
			_mission = xwaMissionFileName;

			this->_sfoils.clear();
			this->_landingGears.clear();
			this->_hangarDoors.clear();
			this->_hatches.clear();
			this->_settings.clear();
		}
	}

	std::map<int, std::vector<SFoil>> _sfoils;
	std::map<int, std::vector<SFoil>> _landingGears;
	std::map<int, std::vector<SFoil>> _hangarDoors;
	std::map<int, std::vector<SFoil>> _hatches;
	std::map<int, CraftSettings> _settings;
};

ModelIndexSFoils g_modelIndexSFoils;

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

	int RetrieveTimeSpeed(int modelIndex, int objectIndex, int elapsedTime)
	{
		const auto settings = g_modelIndexSFoils.GetSettings(modelIndex);

		const int timeFrame = 15 * settings.SFoilsAnimationSpeed;
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

ObjectIndexTime g_objectIndexTime;
ObjectIndexTime g_hangarDoorsObjectIndexTime;
ObjectIndexTime g_hatchesObjectIndexTime;

bool g_keySFoils = false;
bool g_keyLandingGears = false;

void SetSFoilsText()
{
	const char** s_StringsMessageLines = (const char**)0x009B6400;
	const int StringsMessageLine_MSG_SFOILS_OPENING = 134;
	const int StringsMessageLine_MSG_SFOILS_CLOSING = 135;
	const int StringsMessageLine_MSG_SFOILS_OPEN = 136;
	const int StringsMessageLine_MSG_SFOILS_CLOSED = 137;

	if (g_keySFoils)
	{
		s_StringsMessageLines[StringsMessageLine_MSG_SFOILS_CLOSING] = Translate(g_config.Language, ID_MSG_SFOILS_CLOSING_1); // "\03S-Foils closing";
		s_StringsMessageLines[StringsMessageLine_MSG_SFOILS_OPENING] = Translate(g_config.Language, ID_MSG_SFOILS_OPENING_1); // "\03S-Foils opening";
		s_StringsMessageLines[StringsMessageLine_MSG_SFOILS_CLOSED] = Translate(g_config.Language, ID_MSG_SFOILS_CLOSED_1); // "\03S-Foils have reached [closed] position";
		s_StringsMessageLines[StringsMessageLine_MSG_SFOILS_OPEN] = Translate(g_config.Language, ID_MSG_SFOILS_OPEN_1); // "\03S-Foils have reached [open] position";
	}
	else if (g_keyLandingGears)
	{
		s_StringsMessageLines[StringsMessageLine_MSG_SFOILS_CLOSING] = Translate(g_config.Language, ID_MSG_SFOILS_CLOSING_2); // "\03Landing Gears opening";
		s_StringsMessageLines[StringsMessageLine_MSG_SFOILS_OPENING] = Translate(g_config.Language, ID_MSG_SFOILS_OPENING_2); // "\03Landing Gears closing";
		s_StringsMessageLines[StringsMessageLine_MSG_SFOILS_CLOSED] = Translate(g_config.Language, ID_MSG_SFOILS_CLOSED_2); // "\03Landing Gears have reached [open] position";
		s_StringsMessageLines[StringsMessageLine_MSG_SFOILS_OPEN] = Translate(g_config.Language, ID_MSG_SFOILS_OPEN_2); // "\03Landing Gears have reached [closed] position";
	}
	else
	{
		s_StringsMessageLines[StringsMessageLine_MSG_SFOILS_CLOSING] = Translate(g_config.Language, ID_MSG_SFOILS_CLOSING_1); // "\03S-Foils closing";
		s_StringsMessageLines[StringsMessageLine_MSG_SFOILS_OPENING] = Translate(g_config.Language, ID_MSG_SFOILS_OPENING_1); // "\03S-Foils opening";
		s_StringsMessageLines[StringsMessageLine_MSG_SFOILS_CLOSED] = Translate(g_config.Language, ID_MSG_SFOILS_CLOSED_1); // "\03S-Foils have reached [closed] position";
		s_StringsMessageLines[StringsMessageLine_MSG_SFOILS_OPEN] = Translate(g_config.Language, ID_MSG_SFOILS_OPEN_1); // "\03S-Foils have reached [open] position";
	}
}

int SFoilsFilterHook(int* params)
{
	const XwaObject* object = (XwaObject*)params[0];

	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const XwaPlayer* xwaPlayers = (XwaPlayer*)0x08B94E0;
	const int currentPlayerId = *(int*)0x08C1CC8;
	const unsigned short modelIndex = object->ModelIndex;
	const int objectIndex = object - xwaObjects;
	const int playerObjectIndex = xwaPlayers[currentPlayerId].ObjectIndex;

	const char** s_StringsMessageLines = (const char**)0x009B6400;
	const int StringsMessageLine_MSG_SFOILS_NO_FIRE = 138;
	const int StringsMessageLine_MSG_NOT_EQUIPPED_SFOIL = 293;

	// player hangar exit
	if (params[-1] == 0x459522)
	{
		XwaCraft* currentCraft = object->pMobileObject->pCraft;
		unsigned char* currentCraftMeshRotationAngles = g_craftConfig.MeshesCount == 0 ? currentCraft->MeshRotationAngles : (unsigned char*)((int)currentCraft + g_craftConfig.Craft_Offset_260);

		const auto hatches = g_modelIndexSFoils.GetHatches(modelIndex);

		for (unsigned int i = 0; i < hatches.size(); i++)
		{
			const auto& hatch = hatches[i];

			currentCraftMeshRotationAngles[hatch.meshIndex] = 0;
		}
	}

	std::vector<SFoil> sfoils;

	// Key_V
	if (params[-1] == 0x4FC470)
	{
		bool areSFoilsOpened = g_modelIndexSFoils.AreSFoilsOpened(object);
		bool areLandingGearsClosed = g_modelIndexSFoils.AreLandingGearsClosed(object);

		s_StringsMessageLines[StringsMessageLine_MSG_SFOILS_NO_FIRE] = Translate(g_config.Language, ID_MSG_SFOILS_NO_FIRE); // "\03Cannons cannot fire with S-Foils closed or Landing Gears opened";

		unsigned short key = *(unsigned short*)0x08053C0;

		// Key_V = 118
		if (key == 118)
		{
			sfoils = g_modelIndexSFoils.GetSFoils(modelIndex);

			if (g_keyLandingGears)
			{
				sfoils.clear();
				s_StringsMessageLines[StringsMessageLine_MSG_NOT_EQUIPPED_SFOIL] = Translate(g_config.Language, ID_MSG_NOT_EQUIPPED_SFOIL_2); // "\03[Landing Gears] are already opening or closing";
			}
			else if (sfoils.size() && !areSFoilsOpened && !areLandingGearsClosed)
			{
				sfoils.clear();
				s_StringsMessageLines[StringsMessageLine_MSG_NOT_EQUIPPED_SFOIL] = Translate(g_config.Language, ID_MSG_NOT_EQUIPPED_SFOIL_3); // "\03Close [Landing Gear] before engaging [S-Foils]";
			}
			else
			{
				s_StringsMessageLines[StringsMessageLine_MSG_NOT_EQUIPPED_SFOIL] = Translate(g_config.Language, ID_MSG_NOT_EQUIPPED_SFOIL_1); // "\03Your craft is not equipped with [S-Foils]";

				if (sfoils.size())
				{
					g_keySFoils = true;

					if (areSFoilsOpened)
					{
						object->pMobileObject->pCraft->SFoilsState = 0;
					}
					else
					{
						object->pMobileObject->pCraft->SFoilsState = 2;
					}
				}
			}
		}
		// Key_Control_L = 282
		else if (key == 282)
		{
			sfoils = g_modelIndexSFoils.GetLandingGears(modelIndex);

			if (g_keySFoils)
			{
				sfoils.clear();
				s_StringsMessageLines[StringsMessageLine_MSG_NOT_EQUIPPED_SFOIL] = Translate(g_config.Language, ID_MSG_NOT_EQUIPPED_LANDINGGEARS_2); // "\03[S-Foils] are already opening or closing";
			}
			else if (sfoils.size() && areSFoilsOpened && areLandingGearsClosed)
			{
				sfoils.clear();
				s_StringsMessageLines[StringsMessageLine_MSG_NOT_EQUIPPED_SFOIL] = Translate(g_config.Language, ID_MSG_NOT_EQUIPPED_LANDINGGEARS_3); // "\03Close [S-Foils] before engaging [Landing Gears]";
			}
			else
			{
				s_StringsMessageLines[StringsMessageLine_MSG_NOT_EQUIPPED_SFOIL] = Translate(g_config.Language, ID_MSG_NOT_EQUIPPED_LANDINGGEARS_1); // "\03Your craft is not equipped with [Landing Gears]";

				if (sfoils.size())
				{
					g_keyLandingGears = true;

					if (areLandingGearsClosed)
					{
						object->pMobileObject->pCraft->SFoilsState = 0;
					}
					else
					{
						object->pMobileObject->pCraft->SFoilsState = 2;
					}
				}
			}
		}

		SetSFoilsText();
	}
	else
	{
		sfoils = g_modelIndexSFoils.GetSFoilsAndLandingGears(modelIndex);

		bool hasSFoils = g_modelIndexSFoils.GetSFoils(modelIndex).size() != 0;
		bool areSFoilsOpened = g_modelIndexSFoils.AreSFoilsOpened(object);
		bool hasLandingGears = g_modelIndexSFoils.GetLandingGears(modelIndex).size() != 0;
		bool areLandingGearsClosed = g_modelIndexSFoils.AreLandingGearsClosed(object);

		const auto settings = g_modelIndexSFoils.GetSettings(modelIndex);

		if (!hasSFoils && hasLandingGears && areLandingGearsClosed)
		{
			sfoils.clear();
		}

		if (objectIndex == playerObjectIndex && !g_config.AutoCloseSFoils)
		{
			sfoils.clear();
		}
		else if (params[-1] == 0x508DE6)
		{
			// player enter hyperspace

			if (!settings.CloseSFoilsInHyperspace)
			{
				sfoils.clear();
			}
		}
		else if (params[-1] == 0x403AF2)
		{
			// player exit hyperspace

			if (object->pMobileObject->pCraft->SFoilsState == 0)
			{
				sfoils.clear();
			}
			else
			{
				object->pMobileObject->pCraft->SFoilsState = 2;
			}
		}
	}

	params[0] = sfoils.size() ? 0 : 1;

	return 0;
}

int SFoilsHook1(int* params)
{
	const int modelIndex = params[0];

	XwaCraft* currentCraft = *(XwaCraft**)0x0910DFC;

	unsigned char* currentCraftMeshRotationAngles = g_craftConfig.MeshesCount == 0 ? currentCraft->MeshRotationAngles : (unsigned char*)((int)currentCraft + g_craftConfig.Craft_Offset_260);

	// hangar start
	if (*(int*)0x0686B94 == 0)
	{
		const auto hatches = g_modelIndexSFoils.GetHatches(modelIndex);

		for (unsigned int i = 0; i < hatches.size(); i++)
		{
			const auto& hatch = hatches[i];

			currentCraftMeshRotationAngles[hatch.meshIndex] = hatch.angle;
		}
	}

	const auto sfoils = g_modelIndexSFoils.GetSFoilsAndLandingGears(modelIndex);

	if (!sfoils.size())
	{
		currentCraft->SFoilsState = 0;
		return 0;
	}

	for (unsigned int i = 0; i < sfoils.size(); i++)
	{
		const auto& sfoil = sfoils[i];

		currentCraftMeshRotationAngles[sfoil.meshIndex] = sfoil.angle;
	}

	currentCraft->SFoilsState = 2;

	return 0;
}

int SFoilsHook2(int* params)
{
	const int modelIndex = params[0];
	const int objectIndex = params[25];

	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	XwaCraft* currentCraft = *(XwaCraft**)0x0910DFC;
	short elapsedTime = *(short*)0x08C1640;
	XwaAIData* aiData = (XwaAIData*)((int)currentCraft + 0x28);
	int currentPlayerId = *(int*)0x008C1CC8;

	unsigned char* currentCraftMeshRotationAngles = g_craftConfig.MeshesCount == 0 ? currentCraft->MeshRotationAngles : (unsigned char*)((int)currentCraft + g_craftConfig.Craft_Offset_260);

	std::vector<SFoil> sfoils;

	if (xwaObjects[objectIndex].PlayerIndex == currentPlayerId && (g_keySFoils || g_keyLandingGears))
	{
		if (g_keySFoils)
		{
			sfoils = g_modelIndexSFoils.GetSFoils(modelIndex);
		}
		else if (g_keyLandingGears)
		{
			sfoils = g_modelIndexSFoils.GetLandingGears(modelIndex);
		}
	}
	else
	{
		if (currentCraft->SFoilsState == 3 && (aiData->ManrId == 21 || xwaObjects[objectIndex].PlayerIndex == currentPlayerId))
		{
			sfoils = g_modelIndexSFoils.GetSFoils(modelIndex);
		}
		else
		{
			sfoils = g_modelIndexSFoils.GetSFoilsAndLandingGears(modelIndex);
		}
	}

	if (!sfoils.size())
	{
		currentCraft->SFoilsState = 0;
		return 0;
	}

	int timeSpeed = g_objectIndexTime.RetrieveTimeSpeed(modelIndex, objectIndex, elapsedTime);

	int ret = 0;

	for (unsigned int i = 0; i < sfoils.size(); i++)
	{
		auto sfoil = sfoils[i];

		sfoil.closingSpeed *= timeSpeed;
		sfoil.openingSpeed *= timeSpeed;

		if ((currentCraft->SFoilsState & 1) != 0)
		{
			if ((currentCraft->SFoilsState & 2) != 0)
			{
				if (currentCraftMeshRotationAngles[sfoil.meshIndex] < sfoil.angle)
				{
					currentCraftMeshRotationAngles[sfoil.meshIndex] += sfoil.closingSpeed;

					if (currentCraftMeshRotationAngles[sfoil.meshIndex] > sfoil.angle)
					{
						currentCraftMeshRotationAngles[sfoil.meshIndex] = sfoil.angle;
					}

					ret = 1;
				}
			}
			else
			{
				if (currentCraftMeshRotationAngles[sfoil.meshIndex] >= sfoil.openingSpeed)
				{
					currentCraftMeshRotationAngles[sfoil.meshIndex] -= sfoil.openingSpeed;

					if (currentCraftMeshRotationAngles[sfoil.meshIndex] < sfoil.openingSpeed)
					{
						currentCraftMeshRotationAngles[sfoil.meshIndex] = 0;
					}

					ret = 1;
				}
			}
		}
	}

	if ((currentCraft->SFoilsState & 1) != 0 && xwaObjects[objectIndex].PlayerIndex == currentPlayerId && ret == 0)
	{
		SetSFoilsText();

		g_keySFoils = false;
		g_keyLandingGears = false;
	}

	return ret;
}

int SFoilsHangarShuttleHook(int* params)
{
	if (params[2] == -1)
	{
		return 0;
	}

	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;

	const int objectIndex = params[10];
	const unsigned short modelIndex = xwaObjects[objectIndex].ModelIndex;
	XwaCraft* currentCraft = (XwaCraft*)params[0];
	int time = params[1];
	bool closing = params[2] != 0;

	unsigned char* currentCraftMeshRotationAngles = g_craftConfig.MeshesCount == 0 ? currentCraft->MeshRotationAngles : (unsigned char*)((int)currentCraft + g_craftConfig.Craft_Offset_260);

	auto sfoils = g_modelIndexSFoils.GetSFoilsAndLandingGears(modelIndex);

	if (!sfoils.size())
	{
		return 0;
	}

	if (closing)
	{
		for (unsigned int i = 0; i < sfoils.size(); i++)
		{
			const auto& sfoil = sfoils[i];
			int speed = (sfoil.closingSpeed * time + 1) / 2;

			if (currentCraftMeshRotationAngles[sfoil.meshIndex] < sfoil.angle)
			{
				currentCraftMeshRotationAngles[sfoil.meshIndex] += speed;

				if (currentCraftMeshRotationAngles[sfoil.meshIndex] > sfoil.angle)
				{
					currentCraftMeshRotationAngles[sfoil.meshIndex] = sfoil.angle;
				}
			}
		}
	}
	else
	{
		for (unsigned int i = 0; i < sfoils.size(); i++)
		{
			const auto& sfoil = sfoils[i];
			int speed = (sfoil.openingSpeed * time + 1) / 2;

			if (currentCraftMeshRotationAngles[sfoil.meshIndex] >= speed)
			{
				currentCraftMeshRotationAngles[sfoil.meshIndex] -= speed;

				if (currentCraftMeshRotationAngles[sfoil.meshIndex] < speed)
				{
					currentCraftMeshRotationAngles[sfoil.meshIndex] = 0;
				}
			}
		}
	}

	return 0;
}

int SFoilsAIOutOfHyperspace1Hook(int* params)
{
	const int objectIndex = params[0];

	const auto L0049AE20 = (int(*)(int))0x0049AE20;

	XwaCraft* currentCraft = *(XwaCraft**)0x0910DFC;
	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const unsigned short modelIndex = xwaObjects[objectIndex].ModelIndex;

	unsigned char* currentCraftMeshRotationAngles = g_craftConfig.MeshesCount == 0 ? currentCraft->MeshRotationAngles : (unsigned char*)((int)currentCraft + g_craftConfig.Craft_Offset_260);

	const auto settings = g_modelIndexSFoils.GetSettings(modelIndex);

	L0049AE20(objectIndex);

	if (settings.CloseSFoilsInHyperspace)
	{
		for (unsigned int i = 0; i < 50; i++)
		{
			currentCraftMeshRotationAngles[i] = 0;
		}

		const auto sfoils = g_modelIndexSFoils.GetSFoils(modelIndex);

		if (!sfoils.size())
		{
			currentCraft->SFoilsState = 0;
			return 0;
		}

		for (unsigned int i = 0; i < sfoils.size(); i++)
		{
			const auto& sfoil = sfoils[i];

			currentCraftMeshRotationAngles[sfoil.meshIndex] = sfoil.angle;
		}

		currentCraft->SFoilsState = 2;
	}

	return 0;
}

int SFoilsAIOutOfHyperspace2Hook(int* params)
{
	const int pLeaderCraft = params[0];

	const auto GetCraftAIData = (int(*)(int))0x004A5040;

	const XwaObject* pObject = *(XwaObject**)(0x07CA1A0 + 0x04);
	XwaCraft* pCraft = *(XwaCraft**)(0x07CA1A0 + 0x0C);
	const XwaAIData* pAIData = *(XwaAIData**)(0x07CA1A0 + 0x10);
	const unsigned short modelIndex = pObject->ModelIndex;

	const auto settings = g_modelIndexSFoils.GetSettings(modelIndex);

	if (settings.CloseSFoilsInHyperspace)
	{
		if (pAIData->m5E != 0)
		{
			if (pAIData->m5E <= 0x2AC && pCraft->SFoilsState != 0)
			{
				pCraft->SFoilsState = 1;
			}
		}
	}

	GetCraftAIData(pLeaderCraft);

	return 0;
}

int SFoilsAIIntoHyperspace1Hook(int* params)
{
	const auto L004A3660 = (void(*)())0x004A3660;

	const XwaObject* pObject = *(XwaObject**)(0x07CA1A0 + 0x04);
	XwaCraft* pCraft = *(XwaCraft**)(0x07CA1A0 + 0x0C);
	const unsigned short modelIndex = pObject->ModelIndex;

	const auto settings = g_modelIndexSFoils.GetSettings(modelIndex);

	L004A3660();

	if (settings.CloseSFoilsInHyperspace)
	{
		pCraft->SFoilsState = 3;
	}

	return 0;
}

int SFoilsAIHyperspaceOrderHook(int* params)
{
	const int param0 = params[0];
	const int param1 = params[1];

	const auto L004B64F0 = (void(*)(int, int))0x004A3660;

	const XwaObject* pObject = *(XwaObject**)(0x07CA1A0 + 0x04);
	const unsigned short modelIndex = pObject->ModelIndex;
	XwaCraft* currentCraft = *(XwaCraft**)0x0910DFC;

	const auto settings = g_modelIndexSFoils.GetSettings(modelIndex);

	L004B64F0(param0, param1);

	if (settings.CloseSFoilsInHyperspace)
	{
		currentCraft->SFoilsState = 3;
	}

	return 0;
}

int InitSFoilsLandingGearsHook(int* params)
{
	static std::string _mission;
	static std::vector<std::vector<std::string>> _lines;

	const int modelIndex = params[0];
	const int objectIndex = (unsigned short)params[11];

	const auto XwaOptGetMeshesCount = (int(*)(int))0x00488960;

	const char* xwaMissionFileName = (const char*)0x06002E8;
	XwaCraft* currentCraft = *(XwaCraft**)0x0910DFC;
	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;

	unsigned char* currentCraftMeshRotationAngles = g_craftConfig.MeshesCount == 0 ? currentCraft->MeshRotationAngles : (unsigned char*)((int)currentCraft + g_craftConfig.Craft_Offset_260);

	if (_mission != xwaMissionFileName)
	{
		_mission = xwaMissionFileName;

		const std::string path = GetStringWithoutExtension(xwaMissionFileName);
		auto file = GetFileLines(path + ".txt");

		if (!file.size())
		{
			file = GetFileLines(path + ".ini", "mission_tie");
		}

		_lines = GetFileListValues(file);
	}

	unsigned char objectFlightGroupIndex = xwaObjects[objectIndex].TieFlightGroupIndex;

	for (const auto& line : _lines)
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

			if (fg != objectFlightGroupIndex)
			{
				continue;
			}

			const auto& element = line[2];

			std::vector<SFoil> sfoils;

			if (_stricmp(element.c_str(), "close_SFoils") == 0)
			{
				int value = std::stoi(line[3]);

				if (value != 1)
				{
					continue;
				}

				sfoils = g_modelIndexSFoils.GetSFoils(modelIndex);
			}
			else if (_stricmp(element.c_str(), "open_LandingGears") == 0)
			{
				int value = std::stoi(line[3]);

				if (value != 1)
				{
					continue;
				}

				sfoils = g_modelIndexSFoils.GetLandingGears(modelIndex);
			}

			if (sfoils.size())
			{
				for (unsigned int i = 0; i < sfoils.size(); i++)
				{
					const auto& sfoil = sfoils[i];

					currentCraftMeshRotationAngles[sfoil.meshIndex] = sfoil.angle;
				}
			}
		}
	}

	return XwaOptGetMeshesCount(modelIndex);
}

int EnterHangarHook(int* params)
{
	const int playerIndex = params[18];

	const auto DisplayMessage = (void(*)(int, int))0x00497D40;

	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const XwaPlayer* xwaPlayers = (XwaPlayer*)0x08B94E0;
	const int currentPlayerId = *(int*)0x08C1CC8;

	const char** s_StringsMessageLines = (const char**)0x009B6400;
	const int StringsMessageLine_MSG_HANGAR_TRACTOR = 279;

	const XwaObject* object = &xwaObjects[xwaPlayers[playerIndex].ObjectIndex];

	if (!g_config.CloseSFoilsAndOpenLandingGearsBeforeEnterHangar)
	{
		return xwaPlayers[playerIndex].m052;
	}

	if (xwaPlayers[playerIndex].m052 != 0)
	{
		return 1;
	}

	bool hasSFoils = g_modelIndexSFoils.GetSFoils(object->ModelIndex).size() != 0;
	bool areSFoilsOpened = g_modelIndexSFoils.AreSFoilsOpened(object);
	bool hasLandingGears = g_modelIndexSFoils.GetLandingGears(object->ModelIndex).size() != 0;
	bool areLandingGearsClosed = g_modelIndexSFoils.AreLandingGearsClosed(object);

	int ret;

	if (hasSFoils && areSFoilsOpened && hasLandingGears && areLandingGearsClosed)
	{
		if (playerIndex == currentPlayerId)
		{
			s_StringsMessageLines[StringsMessageLine_MSG_HANGAR_TRACTOR] = Translate(g_config.Language, ID_MSG_HANGAR_TRACTOR_1); // "\04Close S-Foils and open Landing Gears before enter hangar";
			DisplayMessage(StringsMessageLine_MSG_HANGAR_TRACTOR, currentPlayerId);
		}

		ret = 1;
	}
	else if (hasSFoils && areSFoilsOpened)
	{
		if (playerIndex == currentPlayerId)
		{
			s_StringsMessageLines[StringsMessageLine_MSG_HANGAR_TRACTOR] = Translate(g_config.Language, ID_MSG_HANGAR_TRACTOR_2); // "\04Close S-Foils before enter hangar";
			DisplayMessage(StringsMessageLine_MSG_HANGAR_TRACTOR, currentPlayerId);
		}

		ret = 1;
	}
	else if (hasLandingGears && areLandingGearsClosed)
	{
		if (playerIndex == currentPlayerId)
		{
			s_StringsMessageLines[StringsMessageLine_MSG_HANGAR_TRACTOR] = Translate(g_config.Language, ID_MSG_HANGAR_TRACTOR_3); // "\04Open Landing Gears before enter hangar";
			DisplayMessage(StringsMessageLine_MSG_HANGAR_TRACTOR, currentPlayerId);
		}

		ret = 1;
	}
	else
	{
		s_StringsMessageLines[StringsMessageLine_MSG_HANGAR_TRACTOR] = Translate(g_config.Language, ID_MSG_HANGAR_TRACTOR_4); // "\04Hit [Space] to activate tractor beam and enter hangar";
		ret = 0;
	}

	return ret;
}

int EnterHyperspaceHook(int* params)
{
	const int playerIndex = params[18];

	const auto DisplayMessage = (void(*)(int, int))0x00497D40;

	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const XwaPlayer* xwaPlayers = (XwaPlayer*)0x08B94E0;
	const int currentPlayerId = *(int*)0x08C1CC8;

	const char** s_StringsMessageLines = (const char**)0x009B6400;
	const int StringsMessageLine_MSG_ASK_ABOUT_HYPER = 121;

	const XwaObject* object = &xwaObjects[xwaPlayers[playerIndex].ObjectIndex];

	if (!g_config.CloseLandingGearsBeforeEnterHyperspace)
	{
		return xwaPlayers[playerIndex].m052;
	}

	if (xwaPlayers[playerIndex].m052 != 0)
	{
		return 1;
	}

	bool hasSFoils = g_modelIndexSFoils.GetSFoils(object->ModelIndex).size() != 0;
	bool areSFoilsOpened = g_modelIndexSFoils.AreSFoilsOpened(object);
	bool hasLandingGears = g_modelIndexSFoils.GetLandingGears(object->ModelIndex).size() != 0;
	bool areLandingGearsClosed = g_modelIndexSFoils.AreLandingGearsClosed(object);

	int ret;

	if (hasLandingGears && !areLandingGearsClosed)
	{
		if (playerIndex == currentPlayerId)
		{
			s_StringsMessageLines[StringsMessageLine_MSG_ASK_ABOUT_HYPER] = Translate(g_config.Language, ID_MSG_ASK_ABOUT_HYPER_1); // "\03Close Landing Gears before enter hyperspace";
			DisplayMessage(StringsMessageLine_MSG_ASK_ABOUT_HYPER, currentPlayerId);
		}

		ret = 1;
	}
	else
	{
		s_StringsMessageLines[StringsMessageLine_MSG_ASK_ABOUT_HYPER] = Translate(g_config.Language, ID_MSG_ASK_ABOUT_HYPER_2); // "\03Press [SPACE] to hyperspace to [*]";
		ret = 0;
	}

	return ret;
}

int NoFireMessageHook(int* params)
{
	const int objectIndex = params[13];

	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const XwaCraft* currentCraft = *(XwaCraft**)0x0910DFC;
	const XwaObject* object = &xwaObjects[objectIndex];

	params[0] = currentCraft->CraftIndex;

	bool hasSFoils = g_modelIndexSFoils.GetSFoils(object->ModelIndex).size() != 0;
	bool areSFoilsOpened = g_modelIndexSFoils.AreSFoilsOpened(object);
	bool hasLandingGears = g_modelIndexSFoils.GetLandingGears(object->ModelIndex).size() != 0;
	bool areLandingGearsClosed = g_modelIndexSFoils.AreLandingGearsClosed(object);
	const auto settings = g_modelIndexSFoils.GetSettings(object->ModelIndex);

	int ret = 0;

	if (hasSFoils && !areSFoilsOpened)
	{
		ret = settings.AllowFireWhenSFoilsAreClosed ? 0 : 1;
	}

	if (hasLandingGears && !areLandingGearsClosed)
	{
		ret = 1;
	}

	return (currentCraft->SFoilsState & 1) != 0 || ret;
}

int AILookForParkOrderHook(int* params)
{
	const int eax = params[0];

	XwaObject* pObject = *(XwaObject**)(0x07CA1A0 + 0x04);
	XwaCraft* pCraft = *(XwaCraft**)(0x07CA1A0 + 0x0C);
	XwaAIData* pAIData = *(XwaAIData**)(0x07CA1A0 + 0x10);
	unsigned char* currentCraftMeshRotationAngles = g_craftConfig.MeshesCount == 0 ? pCraft->MeshRotationAngles : (unsigned char*)((int)pCraft + g_craftConfig.Craft_Offset_260);

	pAIData->PositionZ = eax;

	const auto settings = g_modelIndexSFoils.GetSettings(pObject->ModelIndex);
	const auto sfoils = g_modelIndexSFoils.GetSFoilsAndLandingGears(pObject->ModelIndex);

	if (settings.ParkOrderSFoilsClosed)
	{
		if (!sfoils.size())
		{
			pCraft->SFoilsState = 0;
		}
		else
		{
			pCraft->SFoilsState = 3;
		}
	}

	return 0;
}

int AIParkManrHook(int* params)
{
	XwaObject* pObject = *(XwaObject**)(0x07CA1A0 + 0x04);
	XwaCraft* pCraft = *(XwaCraft**)(0x07CA1A0 + 0x0C);
	XwaAIData* pAIData = *(XwaAIData**)(0x07CA1A0 + 0x10);
	unsigned char* currentCraftMeshRotationAngles = g_craftConfig.MeshesCount == 0 ? pCraft->MeshRotationAngles : (unsigned char*)((int)pCraft + g_craftConfig.Craft_Offset_260);

	const auto settings = g_modelIndexSFoils.GetSettings(pObject->ModelIndex);
	const auto sfoils = g_modelIndexSFoils.GetSFoilsAndLandingGears(pObject->ModelIndex);

	if (settings.ParkOrderSFoilsClosed)
	{
		if (!sfoils.size())
		{
			pCraft->SFoilsState = 0;
		}
		else
		{
			for (unsigned int i = 0; i < sfoils.size(); i++)
			{
				const auto& sfoil = sfoils[i];

				currentCraftMeshRotationAngles[sfoil.meshIndex] = sfoil.angle;
			}

			pCraft->SFoilsState = 2;
		}
	}

	return *(unsigned char*)0x08053F7;
}

int AIParkManrFunctionHook(int* params)
{
	XwaObject* pObject = *(XwaObject**)(0x07CA1A0 + 0x04);
	XwaCraft* pCraft = *(XwaCraft**)(0x07CA1A0 + 0x0C);
	XwaAIData* pAIData = *(XwaAIData**)(0x07CA1A0 + 0x10);

	pAIData->m5D++;

	const auto settings = g_modelIndexSFoils.GetSettings(pObject->ModelIndex);
	const auto sfoils = g_modelIndexSFoils.GetSFoilsAndLandingGears(pObject->ModelIndex);

	if (settings.ParkOrderSFoilsClosed)
	{
		if (!sfoils.size())
		{
			pCraft->SFoilsState = 0;
		}
		else
		{
			pCraft->SFoilsState = 1;
		}
	}

	return 0;
}

int PlaySFoilSoundHook(int* params)
{
	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const XwaPlayer* xwaPlayers = (XwaPlayer*)0x08B94E0;
	const int currentPlayerId = *(int*)0x08C1CC8;
	const int* xwaSoundEffectsBufferId = GetSoundsConfig().SoundEffectIds;

	const auto playSound = (int(*)(int, int, int))0x0043BF90;
	const auto stopSound = (unsigned char(*)(int))0x004DC400;

	const unsigned short modelIndex = xwaObjects[xwaPlayers[currentPlayerId].ObjectIndex].ModelIndex;

	const auto& soundConfig = GetSoundsConfig();

	stopSound(xwaSoundEffectsBufferId[0x78]);

	int slot = 0x78;

	if (soundConfig.SoundsCountHookExists && soundConfig.SfxSFoilCount)
	{
		for (int i = soundConfig.SfxSFoilIndex; i < soundConfig.SfxSFoilIndex + soundConfig.SfxSFoilCount; i++)
		{
			stopSound(xwaSoundEffectsBufferId[i]);
		}

		if (modelIndex < soundConfig.SfxSFoilCount)
		{
			slot = soundConfig.SfxSFoilIndex + modelIndex;
		}
	}

	int ret = playSound(slot, 0xFFFF, currentPlayerId);

	return ret;
}

int Distance(int objectIndex1, int objectIndex2)
{
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;

	int x = std::abs(xwaObjects[objectIndex1].PositionX - xwaObjects[objectIndex2].PositionX);
	int y = std::abs(xwaObjects[objectIndex1].PositionY - xwaObjects[objectIndex2].PositionY);
	int z = std::abs(xwaObjects[objectIndex1].PositionZ - xwaObjects[objectIndex2].PositionZ);

	int distance;

	if (x > y && x > z)
	{
		distance = x + y / 4 + z / 4;
	}
	else if (y > x && y > z)
	{
		distance = x / 4 + y + z / 4;
	}
	else
	{
		distance = x / 4 + y / 4 + z;
	}

	return distance;
}

int HangarDoorsHook(int* params)
{
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	short elapsedTime = *(short*)0x08C1640;

	const auto L00408DC0 = (void(*)())0x00408DC0;

	L00408DC0();

	for (int currentObjectIndex = *(int*)0x08BF378; currentObjectIndex < *(int*)0x08D9628; currentObjectIndex++)
	{
		XwaObject* currentObject = &xwaObjects[currentObjectIndex];
		unsigned short currentModelIndex = currentObject->ModelIndex;

		if (currentModelIndex == 0)
		{
			continue;
		}

		if (currentObject->ShipCategory != ShipCategory_Starship && currentObject->ShipCategory != ShipCategory_Platform && currentObject->ShipCategory != ShipCategory_Freighter)
		{
			continue;
		}

		XwaMobileObject* currentMobileObject = currentObject->pMobileObject;

		if (currentMobileObject == nullptr)
		{
			continue;
		}

		XwaCraft* currentCraft = currentMobileObject->pCraft;

		if (currentCraft == nullptr)
		{
			continue;
		}

		const auto sfoils = g_modelIndexSFoils.GetHangarDoors(currentModelIndex);

		if (!sfoils.size())
		{
			continue;
		}

		int currentDistanceSameTeam = 0x7fffffff;
		int currentDistanceOtherTeams = 0x7fffffff;

		for (int objectIndex = *(int*)0x08BF378; objectIndex < *(int*)0x08D9628; objectIndex++)
		{
			if (objectIndex == currentObjectIndex)
			{
				continue;
			}

			XwaObject* object = &xwaObjects[objectIndex];
			unsigned short modelIndex = object->ModelIndex;

			if (modelIndex == 0)
			{
				continue;
			}

			if (object->Region != currentObject->Region)
			{
				continue;
			}

			if (object->ShipCategory != ShipCategory_Starfighter && object->ShipCategory != ShipCategory_UtilityVehicle && object->ShipCategory != ShipCategory_Transport && object->ShipCategory != ShipCategory_Droid)
			{
				continue;
			}

			XwaMobileObject* mobileObject = object->pMobileObject;

			if (mobileObject == nullptr)
			{
				continue;
			}

			XwaCraft* craft = mobileObject->pCraft;

			if (craft == nullptr)
			{
				continue;
			}

			int distance = Distance(currentObjectIndex, objectIndex);

			if (mobileObject->Team == currentMobileObject->Team)
			{
				if (distance < currentDistanceSameTeam)
				{
					currentDistanceSameTeam = distance;
				}
			}
			else
			{
				if (distance < currentDistanceOtherTeams)
				{
					currentDistanceOtherTeams = distance;
				}
			}
		}

		const auto settings = g_modelIndexSFoils.GetSettings(currentModelIndex);
		unsigned char* currentCraftMeshRotationAngles = g_craftConfig.MeshesCount == 0 ? currentCraft->MeshRotationAngles : (unsigned char*)((int)currentCraft + g_craftConfig.Craft_Offset_260);
		int timeSpeed = g_hangarDoorsObjectIndexTime.RetrieveTimeSpeed(currentModelIndex, currentObjectIndex, elapsedTime);
		bool openDoors = currentDistanceSameTeam < settings.HangarDoorsDistance && currentDistanceOtherTeams >= settings.HangarDoorsDistance;

		for (unsigned int i = 0; i < sfoils.size(); i++)
		{
			auto sfoil = sfoils[i];

			sfoil.closingSpeed *= timeSpeed;
			sfoil.openingSpeed *= timeSpeed;

			if (openDoors)
			{
				if (currentCraftMeshRotationAngles[sfoil.meshIndex] < sfoil.angle)
				{
					currentCraftMeshRotationAngles[sfoil.meshIndex] += sfoil.closingSpeed;

					if (currentCraftMeshRotationAngles[sfoil.meshIndex] > sfoil.angle)
					{
						currentCraftMeshRotationAngles[sfoil.meshIndex] = sfoil.angle;
					}
				}
			}
			else
			{
				if (currentCraftMeshRotationAngles[sfoil.meshIndex] >= sfoil.openingSpeed)
				{
					currentCraftMeshRotationAngles[sfoil.meshIndex] -= sfoil.openingSpeed;

					if (currentCraftMeshRotationAngles[sfoil.meshIndex] < sfoil.openingSpeed)
					{
						currentCraftMeshRotationAngles[sfoil.meshIndex] = 0;
					}
				}
			}
		}
	}

	return 0;
}

int ShuttleHatchHook(int* params)
{
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;

	const int step = *(int*)0x68BBCC;
	const short elapsedTime = *(short*)0x08C1640;
	const int shuttleObjectIndex = *(int*)0x068BBC8;
	const unsigned short modelIndex = xwaObjects[shuttleObjectIndex].ModelIndex;
	XwaCraft* currentCraft = xwaObjects[shuttleObjectIndex].pMobileObject->pCraft;

	unsigned char* currentCraftMeshRotationAngles = g_craftConfig.MeshesCount == 0 ? currentCraft->MeshRotationAngles : (unsigned char*)((int)currentCraft + g_craftConfig.Craft_Offset_260);
	int timeSpeed = g_hatchesObjectIndexTime.RetrieveTimeSpeed(modelIndex, shuttleObjectIndex, elapsedTime);

	auto hatches = g_modelIndexSFoils.GetHatches(modelIndex);

	switch (step)
	{
	case 0:
	{
		for (unsigned int i = 0; i < hatches.size(); i++)
		{
			const auto& hatch = hatches[i];
			currentCraftMeshRotationAngles[hatch.meshIndex] = hatch.angle;
		}

		break;
	}

	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	{
		for (unsigned int i = 0; i < hatches.size(); i++)
		{
			auto hatch = hatches[i];

			hatch.closingSpeed *= timeSpeed;
			hatch.openingSpeed *= timeSpeed;

			if (currentCraftMeshRotationAngles[hatch.meshIndex] >= hatch.openingSpeed)
			{
				currentCraftMeshRotationAngles[hatch.meshIndex] -= hatch.openingSpeed;

				if (currentCraftMeshRotationAngles[hatch.meshIndex] < hatch.openingSpeed)
				{
					currentCraftMeshRotationAngles[hatch.meshIndex] = 0;
				}
			}
		}

		break;
	}

	case 9:
	{
		for (unsigned int i = 0; i < hatches.size(); i++)
		{
			auto hatch = hatches[i];

			hatch.closingSpeed *= timeSpeed;
			hatch.openingSpeed *= timeSpeed;

			if (currentCraftMeshRotationAngles[hatch.meshIndex] < hatch.angle)
			{
				currentCraftMeshRotationAngles[hatch.meshIndex] += hatch.closingSpeed;

				if (currentCraftMeshRotationAngles[hatch.meshIndex] > hatch.angle)
				{
					currentCraftMeshRotationAngles[hatch.meshIndex] = hatch.angle;
				}
			}
		}

		break;
	}
	}

	return *(int*)0x8B94D4;
}

int PlayerCraftHatchHook(int* params)
{
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;

	const int step = *(int*)0x0686B94;
	const short elapsedTime = *(short*)0x08C1640;

	const int playerObjectIndex = *(int*)0x68BC08;
	const unsigned short modelIndex = xwaObjects[playerObjectIndex].ModelIndex;
	XwaCraft* currentCraft = xwaObjects[playerObjectIndex].pMobileObject->pCraft;

	unsigned char* currentCraftMeshRotationAngles = g_craftConfig.MeshesCount == 0 ? currentCraft->MeshRotationAngles : (unsigned char*)((int)currentCraft + g_craftConfig.Craft_Offset_260);
	int timeSpeed = g_hatchesObjectIndexTime.RetrieveTimeSpeed(modelIndex, playerObjectIndex, elapsedTime);

	auto hatches = g_modelIndexSFoils.GetHatches(modelIndex);

	switch (step)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	{
		for (unsigned int i = 0; i < hatches.size(); i++)
		{
			auto hatch = hatches[i];

			hatch.closingSpeed *= timeSpeed;
			hatch.openingSpeed *= timeSpeed;

			if (currentCraftMeshRotationAngles[hatch.meshIndex] >= hatch.openingSpeed)
			{
				currentCraftMeshRotationAngles[hatch.meshIndex] -= hatch.openingSpeed;

				if (currentCraftMeshRotationAngles[hatch.meshIndex] < hatch.openingSpeed)
				{
					currentCraftMeshRotationAngles[hatch.meshIndex] = 0;
				}
			}
		}

		break;
	}

	case 7:
	case 8:
	{
		for (unsigned int i = 0; i < hatches.size(); i++)
		{
			auto hatch = hatches[i];

			hatch.closingSpeed *= timeSpeed;
			hatch.openingSpeed *= timeSpeed;

			if (currentCraftMeshRotationAngles[hatch.meshIndex] < hatch.angle)
			{
				currentCraftMeshRotationAngles[hatch.meshIndex] += hatch.closingSpeed;

				if (currentCraftMeshRotationAngles[hatch.meshIndex] > hatch.angle)
				{
					currentCraftMeshRotationAngles[hatch.meshIndex] = hatch.angle;
				}
			}
		}

		break;
	}
	}

	return *(int*)0x68BC08;
}
