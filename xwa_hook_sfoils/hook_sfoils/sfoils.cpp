#include "targetver.h"
#include "sfoils.h"
#include "config.h"
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

class Config
{
public:
	Config()
	{
		const auto lines = GetFileLines("hook_sfoils.cfg");

		this->CloseSFoilsAndOpenLangingGearsBeforeEnterHangar = GetFileKeyValueInt(lines, "CloseSFoilsAndOpenLangingGearsBeforeEnterHangar") != 0;
		this->CloseLangingGearsBeforeEnterHyperspace = GetFileKeyValueInt(lines, "CloseLangingGearsBeforeEnterHyperspace") != 0;
	}

	bool CloseSFoilsAndOpenLangingGearsBeforeEnterHangar;
	bool CloseLangingGearsBeforeEnterHyperspace;
};

Config g_config;

#pragma pack(push, 1)

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
	char unk00[221];
	XwaCraft* pCraft;
	char unkE1[4];
};

static_assert(sizeof(XwaMobileObject) == 229, "size of XwaMobileObject must be 229");

struct XwaObject
{
	char unk000[2];
	unsigned short ModelIndex;
	char unk004[1];
	unsigned char TieFlightGroupIndex;
	char unk006[25];
	int PlayerIndex;
	XwaMobileObject* pMobileObject;
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

struct XwaAIData
{
	char unk00[92];
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
};

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
	const std::string ship = g_flightModelsList.GetLstLine(modelIndex);

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
	const std::string ship = g_flightModelsList.GetLstLine(modelIndex);

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

CraftSettings GetSFoilsSettings(int modelIndex)
{
	const std::string ship = g_flightModelsList.GetLstLine(modelIndex);

	auto lines = GetFileLines(ship + "SFoils.txt");

	if (!lines.size())
	{
		lines = GetFileLines(ship + ".ini", "SFoils");
	}

	CraftSettings settings;
	settings.CloseSFoilsInHyperspace = GetFileKeyValueInt(lines, "CloseSFoilsInHyperspace", 0) == 0 ? false : true;
	settings.SFoilsAnimationSpeed = std::max(GetFileKeyValueInt(lines, "SFoilsAnimationSpeed", 1), 1);

	return settings;
}

class ModelIndexSFoils
{
public:
	std::vector<SFoil> GetSFoils(int modelIndex)
	{
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

	CraftSettings GetSettings(int modelIndex)
	{
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

		if (!sfoils.size())
		{
			return false;
		}

		bool ret = true;

		for (unsigned int i = 0; i < sfoils.size(); i++)
		{
			const auto& sfoil = sfoils[i];

			if (craft->MeshRotationAngles[sfoil.meshIndex] != 0)
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

		if (!landingGears.size())
		{
			return true;
		}

		bool ret = true;

		for (unsigned int i = 0; i < landingGears.size(); i++)
		{
			const auto& landingGear = landingGears[i];

			if (craft->MeshRotationAngles[landingGear.meshIndex] != 0)
			{
				ret = false;
				break;
			}
		}

		return ret;
	}

private:
	std::map<int, std::vector<SFoil>> _sfoils;
	std::map<int, std::vector<SFoil>> _landingGears;
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

private:
	std::map<int, int> _time;
};

ObjectIndexTime g_objectIndexTime;

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
		s_StringsMessageLines[StringsMessageLine_MSG_SFOILS_CLOSING] = "\03S-Foils closing";
		s_StringsMessageLines[StringsMessageLine_MSG_SFOILS_OPENING] = "\03S-Foils opening";
		s_StringsMessageLines[StringsMessageLine_MSG_SFOILS_CLOSED] = "\03S-Foils have reached [closed] position";
		s_StringsMessageLines[StringsMessageLine_MSG_SFOILS_OPEN] = "\03S-Foils have reached [open] position";
	}
	else if (g_keyLandingGears)
	{
		s_StringsMessageLines[StringsMessageLine_MSG_SFOILS_CLOSING] = "\03Landing Gears opening";
		s_StringsMessageLines[StringsMessageLine_MSG_SFOILS_OPENING] = "\03Landing Gears closing";
		s_StringsMessageLines[StringsMessageLine_MSG_SFOILS_CLOSED] = "\03Landing Gears have reached [open] position";
		s_StringsMessageLines[StringsMessageLine_MSG_SFOILS_OPEN] = "\03Landing Gears have reached [closed] position";
	}
	else
	{
		s_StringsMessageLines[StringsMessageLine_MSG_SFOILS_CLOSING] = "\03S-Foils closing";
		s_StringsMessageLines[StringsMessageLine_MSG_SFOILS_OPENING] = "\03S-Foils opening";
		s_StringsMessageLines[StringsMessageLine_MSG_SFOILS_CLOSED] = "\03S-Foils have reached [closed] position";
		s_StringsMessageLines[StringsMessageLine_MSG_SFOILS_OPEN] = "\03S-Foils have reached [open] position";
	}
}

int SFoilsFilterHook(int* params)
{
	const XwaObject* object = (XwaObject*)params[0];
	const unsigned short modelIndex = object->ModelIndex;

	const char** s_StringsMessageLines = (const char**)0x009B6400;
	const int StringsMessageLine_MSG_SFOILS_NO_FIRE = 138;
	const int StringsMessageLine_MSG_NOT_EQUIPPED_SFOIL = 293;

	std::vector<SFoil> sfoils;

	// Key_V
	if (params[-1] == 0x4FC470)
	{
		bool areSFoilsOpened = g_modelIndexSFoils.AreSFoilsOpened(object);
		bool areLandingGearsClosed = g_modelIndexSFoils.AreLandingGearsClosed(object);

		s_StringsMessageLines[StringsMessageLine_MSG_SFOILS_NO_FIRE] = "\03Cannons cannot fire with S-Foils closed or Landing Gears opened";

		unsigned short key = *(unsigned short*)0x08053C0;

		// Key_V = 118
		if (key == 118)
		{
			sfoils = g_modelIndexSFoils.GetSFoils(modelIndex);

			if (g_keyLandingGears)
			{
				sfoils.clear();
				s_StringsMessageLines[StringsMessageLine_MSG_NOT_EQUIPPED_SFOIL] = "\03[Landing Gears] are already opening or closing";
			}
			else if (sfoils.size() && !areSFoilsOpened && !areLandingGearsClosed)
			{
				sfoils.clear();
				s_StringsMessageLines[StringsMessageLine_MSG_NOT_EQUIPPED_SFOIL] = "\03Close [Landing Gear] before engaging [S-Foils]";
			}
			else
			{
				s_StringsMessageLines[StringsMessageLine_MSG_NOT_EQUIPPED_SFOIL] = "\03Your craft is not equipped with [S-Foils]";

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
				s_StringsMessageLines[StringsMessageLine_MSG_NOT_EQUIPPED_SFOIL] = "\03[S-Foils] are already opening or closing";
			}
			else if (sfoils.size() && areSFoilsOpened && areLandingGearsClosed)
			{
				sfoils.clear();
				s_StringsMessageLines[StringsMessageLine_MSG_NOT_EQUIPPED_SFOIL] = "\03Close [S-Foils] before engaging [Landing Gears]";
			}
			else
			{
				s_StringsMessageLines[StringsMessageLine_MSG_NOT_EQUIPPED_SFOIL] = "\03Your craft is not equipped with [Landing Gears]";

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

		if (!hasSFoils && hasLandingGears && areLandingGearsClosed)
		{
			sfoils.clear();
		}
	}

	params[0] = sfoils.size() ? 0 : 1;

	return 0;
}

int SFoilsHook1(int* params)
{
	const int modelIndex = params[0];

	XwaCraft* currentCraft = *(XwaCraft**)0x0910DFC;

	const auto sfoils = g_modelIndexSFoils.GetSFoilsAndLandingGears(modelIndex);

	if (!sfoils.size())
	{
		currentCraft->SFoilsState = 0;
		return 0;
	}

	for (unsigned int i = 0; i < sfoils.size(); i++)
	{
		const auto& sfoil = sfoils[i];

		currentCraft->MeshRotationAngles[sfoil.meshIndex] = sfoil.angle;
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

	const auto settings = g_modelIndexSFoils.GetSettings(modelIndex);

	if (!sfoils.size())
	{
		currentCraft->SFoilsState = 0;
		return 0;
	}

	const int timeFrame = 15 * settings.SFoilsAnimationSpeed;
	int time = g_objectIndexTime.Get(objectIndex);
	time += elapsedTime;

	int timeSpeed = 0;
	if (time >= timeFrame)
	{
		timeSpeed = 1;
		time = time % timeFrame;
	}

	g_objectIndexTime.Set(objectIndex, time);

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
				if (currentCraft->MeshRotationAngles[sfoil.meshIndex] < sfoil.angle)
				{
					currentCraft->MeshRotationAngles[sfoil.meshIndex] += sfoil.closingSpeed;

					if (currentCraft->MeshRotationAngles[sfoil.meshIndex] > sfoil.angle)
					{
						currentCraft->MeshRotationAngles[sfoil.meshIndex] = sfoil.angle;
					}

					ret = 1;
				}
			}
			else
			{
				if (currentCraft->MeshRotationAngles[sfoil.meshIndex] >= sfoil.openingSpeed)
				{
					currentCraft->MeshRotationAngles[sfoil.meshIndex] -= sfoil.openingSpeed;

					if (currentCraft->MeshRotationAngles[sfoil.meshIndex] < sfoil.openingSpeed)
					{
						currentCraft->MeshRotationAngles[sfoil.meshIndex] = 0;
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

			if (currentCraft->MeshRotationAngles[sfoil.meshIndex] < sfoil.angle)
			{
				currentCraft->MeshRotationAngles[sfoil.meshIndex] += speed;

				if (currentCraft->MeshRotationAngles[sfoil.meshIndex] > sfoil.angle)
				{
					currentCraft->MeshRotationAngles[sfoil.meshIndex] = sfoil.angle;
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

			if (currentCraft->MeshRotationAngles[sfoil.meshIndex] >= speed)
			{
				currentCraft->MeshRotationAngles[sfoil.meshIndex] -= speed;

				if (currentCraft->MeshRotationAngles[sfoil.meshIndex] < speed)
				{
					currentCraft->MeshRotationAngles[sfoil.meshIndex] = 0;
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

	const auto settings = g_modelIndexSFoils.GetSettings(modelIndex);

	L0049AE20(objectIndex);

	if (settings.CloseSFoilsInHyperspace)
	{
		for (unsigned int i = 0; i < 50; i++)
		{
			currentCraft->MeshRotationAngles[i] = 0;
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

			currentCraft->MeshRotationAngles[sfoil.meshIndex] = sfoil.angle;
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

					currentCraft->MeshRotationAngles[sfoil.meshIndex] = sfoil.angle;
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

	if (!g_config.CloseSFoilsAndOpenLangingGearsBeforeEnterHangar)
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
			s_StringsMessageLines[StringsMessageLine_MSG_HANGAR_TRACTOR] = "\04Close S-Foils and open Landing Gears before enter hangar";
			DisplayMessage(StringsMessageLine_MSG_HANGAR_TRACTOR, currentPlayerId);
		}

		ret = 1;
	}
	else if (hasSFoils && areSFoilsOpened)
	{
		if (playerIndex == currentPlayerId)
		{
			s_StringsMessageLines[StringsMessageLine_MSG_HANGAR_TRACTOR] = "\04Close S-Foils before enter hangar";
			DisplayMessage(StringsMessageLine_MSG_HANGAR_TRACTOR, currentPlayerId);
		}

		ret = 1;
	}
	else if (hasLandingGears && areLandingGearsClosed)
	{
		if (playerIndex == currentPlayerId)
		{
			s_StringsMessageLines[StringsMessageLine_MSG_HANGAR_TRACTOR] = "\04Open Landing Gears before enter hangar";
			DisplayMessage(StringsMessageLine_MSG_HANGAR_TRACTOR, currentPlayerId);
		}

		ret = 1;
	}
	else
	{
		s_StringsMessageLines[StringsMessageLine_MSG_HANGAR_TRACTOR] = "\04Hit [Space] to activate tractor beam and enter hangar";
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

	if (!g_config.CloseLangingGearsBeforeEnterHyperspace)
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
			s_StringsMessageLines[StringsMessageLine_MSG_ASK_ABOUT_HYPER] = "\03Close Landing Gears before enter hyperspace";
			DisplayMessage(StringsMessageLine_MSG_ASK_ABOUT_HYPER, currentPlayerId);
		}

		ret = 1;
	}
	else
	{
		s_StringsMessageLines[StringsMessageLine_MSG_ASK_ABOUT_HYPER] = "\03Press [SPACE] to hyperspace to [*]";
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

	int ret = 0;

	if (hasSFoils && !areSFoilsOpened)
	{
		ret = 1;
	}
	else if (hasLandingGears && !areLandingGearsClosed)
	{
		ret = 1;
	}

	return currentCraft->SFoilsState != 0 || ret;
}
