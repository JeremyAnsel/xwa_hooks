#include "targetver.h"
#include "slam.h"
#include "config.h"
#include <fstream>
#include <map>
#include <utility>
#include <fstream>

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
		auto lines = GetFileLines("hook_slam.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_slam");
		}

		this->SlamEnablePenalty = GetFileKeyValueInt(lines, "SlamEnablePenalty", 4);
	}

	int SlamEnablePenalty;
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
		}
	}

	bool SoundsCountHookExists;
	int* SoundEffectIds;
};

SoundsConfig& GetSoundsConfig()
{
	static SoundsConfig g_soundsConfig;
	return g_soundsConfig;
}

#pragma pack(push, 1)

struct XwaCraftWeaponRack
{
	unsigned short ModelIndex;
	unsigned char Sequence;
	unsigned char LaserIndex;
	unsigned char m04;
	unsigned char Count;
	unsigned short FireRatio;
	unsigned char MeshId;
	unsigned char HardpointId;
	short ObjectIndex;
	short m0C;
};

static_assert(sizeof(XwaCraftWeaponRack) == 14, "size of XwaCraftWeaponRack must be 14");

struct XwaCraft
{
	char unk000[11];
	char m00B;
	char unk00C[234];
	short IsSlamEnabled;
	char unk0F8[182];
	unsigned char WeaponRacksCount;
	char unk1AF[304];
	XwaCraftWeaponRack WeaponRacks[16];
	char unk3BF[58];
};

static_assert(sizeof(XwaCraft) == 1017, "size of XwaCraft must be 1017");

struct XwaMobileObject
{
	char Unk0000[221];
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
	char Unk0006[29];
	XwaMobileObject* pMobileObject;
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

#pragma pack(pop)

int HasShipSlam(int objectIndex)
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

	lines = GetFileLines(shipPath + "Slam.txt");

	if (!lines.size())
	{
		lines = GetFileLines(shipPath + ".ini", "Slam");
	}

	if (lines.size())
	{
		int hasSlam = GetFileKeyValueInt(lines, "HasSlam");
		hasSlam = GetFileKeyValueInt(lines, "HasSlam_" + profile, hasSlam);
		return hasSlam;
	}
	else
	{
		// ModelIndex_012_0_11_MissileBoat
		if (object->ModelIndex == 0x0C)
		{
			return 1;
		}
	}

	return 0;
}

class ModelIndexSlam
{
public:
	int HasSlam(int objectIndex)
	{
		this->Update();

		XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
		XwaObject* object = &XwaObjects[objectIndex];

		auto it = this->_slam.find(object->TieFlightGroupIndex);

		if (it != this->_slam.end())
		{
			return it->second;
		}
		else
		{
			int value = HasShipSlam(objectIndex);
			this->_slam.insert(std::make_pair(object->TieFlightGroupIndex, value));
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

			this->_slam.clear();
		}
	}

	std::map<int, int> _slam;
};

ModelIndexSlam g_modelIndexSlam;

int SlamHook(int* params)
{
	static bool soundsDefined = false;
	static bool useOverdriveSounds;
	static unsigned short powerupSoundIndex;
	static unsigned short powerdnSoundIndex;

	params[-1] = 0x004FDBFF;

	const int playerIndex = params[76];
	const int objectIndex = params[69];

	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	const auto ShowMessage = (void(*)(int, int))0x00497D40;
	const auto PlaySound = (int(*)(int, int, int))0x0043BF90;
	const auto LoadSfxLst = (short(*)(const char*, unsigned short, const char*))0x0043A150;

	const unsigned short modelIndex = XwaObjects[objectIndex].ModelIndex;
	int hasSlam = modelIndex == 0 ? 0 : g_modelIndexSlam.HasSlam(objectIndex);

	if (hasSlam)
	{
		if (!soundsDefined)
		{
			soundsDefined = true;

			if (std::ifstream("wave\\overdrive.lst"))
			{
				useOverdriveSounds = true;
				powerupSoundIndex = 1; // powerup.wav
				powerdnSoundIndex = 2; // powerdn.wav
			}
			else
			{
				useOverdriveSounds = false;
				powerupSoundIndex = 115; // HyperstartImp.wav
				powerdnSoundIndex = 117; // Hyperend.wav
			}

			*(int*)0x004909A1 = powerdnSoundIndex;
		}

		if (useOverdriveSounds)
		{
			if (GetSoundsConfig().SoundEffectIds[1] == -1)
			{
				LoadSfxLst("wave\\overdrive.lst", 1, "wave\\overdrive\\");
			}
		}
	}

	XwaCraft* craft = XwaObjects[objectIndex].pMobileObject->pCraft;

	if (hasSlam == 0)
	{
		craft->IsSlamEnabled = 0;

		// MSG_NOT_EQUIPPED_SLAM
		ShowMessage(294, playerIndex);
	}
	else
	{
		if (craft->IsSlamEnabled)
		{
			craft->IsSlamEnabled = 0;

			// MSG_OVERDRIVE_OFF
			ShowMessage(372, playerIndex);
			PlaySound(powerdnSoundIndex, 0xFFFF, playerIndex);
		}
		else
		{
			XwaCraftWeaponRack* weaponRacks = (XwaCraftWeaponRack*)((int)craft + g_craftConfig.Craft_Offset_2DF);

			int penalty = g_config.SlamEnablePenalty;
			bool hasEnergy = false;

			for (int i = 0; i < craft->WeaponRacksCount; i++)
			{
				if (weaponRacks[i].m04 > penalty)
				{
					hasEnergy = true;
					break;
				}
			}

			if (hasEnergy)
			{
				craft->IsSlamEnabled = 1;

				for (int i = 0; i < craft->WeaponRacksCount; i++)
				{
					if (weaponRacks[i].m04 > penalty)
					{
						weaponRacks[i].m04 -= penalty;
					}
					else
					{
						weaponRacks[i].m04 = 0;
					}
				}

				// MSG_OVERDRIVE_ON
				ShowMessage(371, playerIndex);
				PlaySound(powerupSoundIndex, 0xFFFF, playerIndex);
			}
			else
			{
				// MSG_OVERDRIVE_UNABLE
				ShowMessage(373, playerIndex);
			}
		}
	}

	return 0;
}

int EngineGlowLengthHook(int* params)
{
	XwaCraft* currentCraft = *(XwaCraft**)0x0910DFC;

	if (currentCraft->m00B == 0x05 || currentCraft->m00B == 0x06)
		return 0;

	if (currentCraft->IsSlamEnabled)
		return 0;

	return 1;
}
