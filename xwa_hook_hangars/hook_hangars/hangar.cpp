#include "targetver.h"
#include "hangar.h"
#include "config.h"
#include <fstream>
#include <map>
#include <utility>
#include <algorithm>

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

unsigned int GetFileKeyValueUnsignedInt(const std::vector<std::string>& lines, const std::string& key, unsigned int defaultValue = 0)
{
	std::string value = GetFileKeyValue(lines, key);

	if (value.empty())
	{
		return defaultValue;
	}

	return std::stoul(value, 0, 16);
}

void CombineFileLines(std::vector<std::string>& a, const std::vector<std::string>& b, bool append)
{
	if (append)
	{
		a.insert(std::end(a), std::begin(b), std::end(b));
	}
	else
	{
		if (a.empty())
		{
			a.insert(std::end(a), std::begin(b), std::end(b));
		}
	}
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
	}

	int MeshesCount;
	int Craft_Size;
	int Craft_Offset_22E;
	int Craft_Offset_260;
	int Craft_Offset_292;
};

CraftConfig g_craftConfig;

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

		this->SfxHangarAmbientIndex = GetFileKeyValueInt(lines, "sfx_hangarambient_index");
		this->SfxHangarAmbientCount = GetFileKeyValueInt(lines, "sfx_hangarambient_count");
	}

	bool SoundsCountHookExists;
	int* SoundEffectIds;
	int SfxHangarAmbientIndex;
	int SfxHangarAmbientCount;
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
		auto lines = GetFileLines("hook_hangars.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_hangars");
		}

		this->SelectionMode = GetFileKeyValue(lines, "SelectionMode");
		this->ProvingGroundHangarModel = GetFileKeyValue(lines, "ProvingGroundHangarModel");
		this->DrawShadows = GetFileKeyValueInt(lines, "DrawShadows", 1) != 0;
		this->ShadowLod = GetFileKeyValueInt(lines, "ShadowLod", 0) != 0;
	}

	std::string SelectionMode;
	std::string ProvingGroundHangarModel;
	bool DrawShadows;
	bool ShadowLod;
};

Config g_config;

#pragma pack(push, 1)

struct XwaPlayer
{
	int ObjectIndex;
	char m004[79];
	short m053;
	char m055[2803];
	int CameraPositionX;
	int CameraPositionY;
	int CameraPositionZ;
	char mB54[123];
};

static_assert(sizeof(XwaPlayer) == 3023, "size of XwaPlayer must be 3023");

struct ExeEnableEntry
{
	unsigned char m00; // flags
	unsigned char m01; // flags
	char unk02[6];
	void* pData1;
	void* pData2;
	char unk10[2];
	short CraftIndex;
	char unk14[4];
};

static_assert(sizeof(ExeEnableEntry) == 24, "size of ExeEnableEntry must be 24");

struct XwaCraft
{
	char unk000[4];
	unsigned short CraftIndex;
	int LeaderObjectIndex;
	char unk00A[29];
	char SFoilsState;
	char unk028[355];
	unsigned char m18B;
	char unk18C[162];
	char XwaCraft_m22E[50];
	unsigned char MeshRotationAngles[50];
	char XwaCraft_m292[50];
	char unk2C4[309];
};

static_assert(sizeof(XwaCraft) == 1017, "size of XwaCraft must be 1017");

struct XwaMobileObject
{
	char Unk0000[10];
	int PositionX;
	int PositionY;
	int PositionZ;
	char Unk0016[127];
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
	char unk000[107];
	unsigned char CraftId;
	char unk06C[4];
	unsigned char Iff;
	char unk071[81];
	unsigned char m0C2;
	unsigned char m0C3;
	unsigned char m0C4;
	unsigned char m0C5;
	unsigned char m0C6;
	unsigned char m0C7;
	unsigned char m0C8;
	unsigned char m0C9;
	char unk0CA[3444];
	int PlayerIndex;
};

static_assert(sizeof(TieFlightGroupEx) == 3650, "size of TieFlightGroupEx must be 3650");

struct S0x09C6780
{
	int ObjectIndex;
	int m04;
	int m08;
	int m0C;
	int m10;
	int TargetPositionX;
	int TargetPositionY;
	int TargetPositionZ;
	short m20;
	float m22;
	int m26;
	int m2A;
};

static_assert(sizeof(S0x09C6780) == 46, "size of S0x09C6780 must be 46");

#pragma pack(pop)

std::vector<unsigned short> g_hangarMapModelIndices;

std::vector<unsigned short> GetDefaultCraftSelectionTransports()
{
	std::vector<unsigned short> selection;

	// ModelIndex_058_0_45_CorellianTransport2
	selection.push_back(58);

	// ModelIndex_065_0_52_FamilyTransport
	selection.push_back(65);

	// ModelIndex_059_0_46_MilleniumFalcon2
	selection.push_back(59);

	return selection;
}

std::vector<unsigned short> GetDefaultCraftSelectionFighters()
{
	std::vector<unsigned short> selection;

	// ModelIndex_001_0_0_Xwing
	selection.push_back(1);

	// ModelIndex_002_0_1_Ywing
	selection.push_back(2);

	// ModelIndex_003_0_2_Awing
	selection.push_back(3);

	// ModelIndex_004_0_3_Bwing
	selection.push_back(4);

	// ModelIndex_014_0_13_Z_95
	selection.push_back(14);

	return selection;
}

std::vector<unsigned short> GetDefaultCraftSelectionCrafts()
{
	std::vector<unsigned short> selection;

	// ModelIndex_001_0_0_Xwing
	selection.push_back(1);

	// ModelIndex_002_0_1_Ywing
	selection.push_back(2);

	// ModelIndex_003_0_2_Awing
	selection.push_back(3);

	// ModelIndex_004_0_3_Bwing
	selection.push_back(4);

	// ModelIndex_014_0_13_Z_95
	selection.push_back(14);

	// ModelIndex_058_0_45_CorellianTransport2
	selection.push_back(58);

	// ModelIndex_065_0_52_FamilyTransport
	selection.push_back(65);

	// ModelIndex_059_0_46_MilleniumFalcon2
	selection.push_back(59);

	return selection;
}

class CraftSelectionValues
{
public:
	CraftSelectionValues()
	{
		if (std::ifstream("CraftSelectionTransports.txt"))
		{
			const auto lines = GetFileLines("CraftSelectionTransports.txt");
			this->_transportsSelection = GetFileListUnsignedShortValues(lines);
		}
		else
		{
			this->_transportsSelection = GetDefaultCraftSelectionTransports();
		}

		if (std::ifstream("CraftSelectionFighters.txt"))
		{
			const auto lines = GetFileLines("CraftSelectionFighters.txt");
			this->_fightersSelection = GetFileListUnsignedShortValues(lines);
		}
		else
		{
			this->_fightersSelection = GetDefaultCraftSelectionFighters();
		}

		if (std::ifstream("CraftSelectionCrafts.txt"))
		{
			const auto lines = GetFileLines("CraftSelectionCrafts.txt");
			this->_craftsSelection = GetFileListUnsignedShortValues(lines);
		}
		else
		{
			this->_craftsSelection = GetDefaultCraftSelectionCrafts();
		}
	}

	std::vector<unsigned short> _transportsSelection;
	std::vector<unsigned short> _fightersSelection;
	std::vector<unsigned short> _craftsSelection;
};

CraftSelectionValues g_craftSelectionValues;

std::vector<std::string> GetCustomFileLinesBase(const std::string& name, bool append)
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
		_lines.clear();

		const std::string mission = GetStringWithoutExtension(xwaMissionFileName);
		CombineFileLines(_lines, GetFileLines(mission + "_" + name + ".txt"), append);
		CombineFileLines(_lines, GetFileLines(mission + ".ini", name), append);
		CombineFileLines(_lines, GetFileLines(std::string("FlightModels\\") + name + ".txt"), append);
		CombineFileLines(_lines, GetFileLines("FlightModels\\default.ini", name), append);
	}

	return _lines;
}

int GetCommandShipModelIndex()
{
	const unsigned short* exeSpecies = (unsigned short*)0x05B0F70;
	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const TieFlightGroupEx* xwaTieFlightGroups = (TieFlightGroupEx*)0x080DC80;
	const XwaPlayer* xwaPlayers = (XwaPlayer*)0x08B94E0;
	//const int hangarPlayerObjectIndex = *(int*)0x068BC08;
	//const int playerObjectIndex = *(int*)0x068BB98;
	const int playerObjectIndex = *(int*)0x068BC08;
	const int currentPlayerId = *(int*)0x08C1CC8;

	if (playerObjectIndex == 0xffff)
	{
		return -1;
	}

	const short mothershipObjectIndex = xwaPlayers[currentPlayerId].m053;

	if (mothershipObjectIndex == -1)
	{
		return -1;
	}

	if (mothershipObjectIndex == 0)
	{
		const int playerFlightGroupIndex = xwaObjects[playerObjectIndex].TieFlightGroupIndex;

		int commandShipModelIndex = -1;

		if (xwaTieFlightGroups[playerFlightGroupIndex].m0C3)
		{
			const int commandShipCraftId = xwaTieFlightGroups[xwaTieFlightGroups[playerFlightGroupIndex].m0C2].CraftId;
			commandShipModelIndex = exeSpecies[commandShipCraftId];
		}
		else if (xwaTieFlightGroups[playerFlightGroupIndex].m0C5)
		{
			const int commandShipCraftId = xwaTieFlightGroups[xwaTieFlightGroups[playerFlightGroupIndex].m0C4].CraftId;
			commandShipModelIndex = exeSpecies[commandShipCraftId];
		}

		return commandShipModelIndex;
	}

	const int mothershipModelIndex = xwaObjects[mothershipObjectIndex].ModelIndex;

	return mothershipModelIndex;
}

std::string GetCommandShipLstLine()
{
	const unsigned short* exeSpecies = (unsigned short*)0x05B0F70;
	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const TieFlightGroupEx* xwaTieFlightGroups = (TieFlightGroupEx*)0x080DC80;
	const XwaPlayer* xwaPlayers = (XwaPlayer*)0x08B94E0;
	//const int hangarPlayerObjectIndex = *(int*)0x068BC08;
	//const int playerObjectIndex = *(int*)0x068BB98;
	const int playerObjectIndex = *(int*)0x068BC08;
	const int currentPlayerId = *(int*)0x08C1CC8;

	if (playerObjectIndex == 0xffff)
	{
		return std::string();
	}

	const short mothershipObjectIndex = xwaPlayers[currentPlayerId].m053;

	if (mothershipObjectIndex == -1)
	{
		return std::string();
	}

	std::string lstLine;

	if (mothershipObjectIndex == 0)
	{
		const int playerFlightGroupIndex = xwaObjects[playerObjectIndex].TieFlightGroupIndex;

		int commandShipModelIndex = -1;

		if (xwaTieFlightGroups[playerFlightGroupIndex].m0C3)
		{
			const int commandShipCraftId = xwaTieFlightGroups[xwaTieFlightGroups[playerFlightGroupIndex].m0C2].CraftId;
			commandShipModelIndex = exeSpecies[commandShipCraftId];
		}
		else if (xwaTieFlightGroups[playerFlightGroupIndex].m0C5)
		{
			const int commandShipCraftId = xwaTieFlightGroups[xwaTieFlightGroups[playerFlightGroupIndex].m0C4].CraftId;
			commandShipModelIndex = exeSpecies[commandShipCraftId];
		}

		if (commandShipModelIndex == -1)
		{
			return std::string();
		}

		lstLine = g_flightModelsList.GetLstLine(commandShipModelIndex);
	}
	else
	{
		const int mothershipModelIndex = xwaObjects[mothershipObjectIndex].ModelIndex;

		lstLine = g_flightModelsList.GetLstLine(mothershipModelIndex);
	}

	{
		const auto objectLines = GetCustomFileLinesBase("Objects", false);
		const std::string objectValue = GetFileKeyValue(objectLines, lstLine + ".opt");

		if (!objectValue.empty() && std::ifstream(objectValue))
		{
			lstLine = GetStringWithoutExtension(objectValue);
		}
	}

	return lstLine;
}

unsigned char GetCommandShipIff()
{
	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const TieFlightGroupEx* xwaTieFlightGroups = (TieFlightGroupEx*)0x080DC80;
	const XwaPlayer* xwaPlayers = (XwaPlayer*)0x08B94E0;
	//const int hangarPlayerObjectIndex = *(int*)0x068BC08;
	//const int playerObjectIndex = *(int*)0x068BB98;
	const int playerObjectIndex = *(int*)0x068BC08;
	const int currentPlayerId = *(int*)0x08C1CC8;

	if (playerObjectIndex == 0xffff)
	{
		return 0;
	}

	const short mothershipObjectIndex = xwaPlayers[currentPlayerId].m053;

	if (mothershipObjectIndex == -1)
	{
		return 0;
	}

	if (mothershipObjectIndex == 0)
	{
		const int playerFlightGroupIndex = xwaObjects[playerObjectIndex].TieFlightGroupIndex;

		if (xwaTieFlightGroups[playerFlightGroupIndex].m0C3)
		{
			return xwaTieFlightGroups[xwaTieFlightGroups[playerFlightGroupIndex].m0C2].Iff;
		}
		else if (xwaTieFlightGroups[playerFlightGroupIndex].m0C5)
		{
			return xwaTieFlightGroups[xwaTieFlightGroups[playerFlightGroupIndex].m0C4].Iff;
		}

		return 0;
	}

	const unsigned char mothershipFlightGroupIndex = xwaObjects[mothershipObjectIndex].TieFlightGroupIndex;

	if (mothershipFlightGroupIndex == 0xff)
	{
		return 0;
	}

	return xwaTieFlightGroups[mothershipFlightGroupIndex].Iff;
}

std::string GetCustomFilePath(const std::string& name)
{
	const bool isProvingGround = *(unsigned char*)(0x08053E0 + 0x05) != 0;
	const char* xwaMissionFileName = (const char*)0x06002E8;

	if (isProvingGround)
	{
		std::string ship = g_config.ProvingGroundHangarModel;

		if (!ship.empty())
		{
			ship = GetStringWithoutExtension(ship);

			if (_stricmp(name.c_str(), "Hangar.opt") == 0 && _stricmp(ship.c_str(), "FlightModels\\FamilyBase") == 0)
			{
				ship.append(".opt");
			}
			else
			{
				ship.append(name);
			}

			if (std::ifstream(ship))
			{
				return ship;
			}
		}
	}
	else
	{
		std::string mission = GetStringWithoutExtension(xwaMissionFileName);

		if (!mission.empty())
		{
			mission.append("_");
			mission.append(name);

			if (std::ifstream(mission))
			{
				return mission;
			}
		}

		std::string ship = GetCommandShipLstLine();

		if (!ship.empty())
		{
			if (_stricmp(name.c_str(), "Hangar.opt") == 0 && _stricmp(ship.c_str(), "FlightModels\\FamilyBase") == 0)
			{
				ship.append(".opt");
			}
			else
			{
				ship.append(name);
			}

			if (std::ifstream(ship))
			{
				return ship;
			}
		}
	}

	std::string path = "FlightModels\\";
	path.append(name);

	return path;
}

std::vector<std::string> GetCustomFileLines(const std::string& name, bool append)
{
	const bool isProvingGround = *(unsigned char*)(0x08053E0 + 0x05) != 0;
	const char* xwaMissionFileName = (const char*)0x06002E8;
	std::vector<std::string> lines;

	if (isProvingGround)
	{
		std::string ship = g_config.ProvingGroundHangarModel;

		if (!ship.empty())
		{
			ship = GetStringWithoutExtension(ship);

			CombineFileLines(lines, GetFileLines(ship + name + ".txt"), append);
			CombineFileLines(lines, GetFileLines(ship + ".ini", name), append);
		}
	}
	else
	{
		const std::string mission = GetStringWithoutExtension(xwaMissionFileName);
		const std::string ship = GetCommandShipLstLine();
		const unsigned char shipIff = GetCommandShipIff();

		if (!ship.empty())
		{
			const std::string shipName = GetFileNameWithoutExtension(ship);

			CombineFileLines(lines, GetFileLines(mission + "_" + name + "_" + shipName + "_" + std::to_string(shipIff) + ".txt"), append);
			CombineFileLines(lines, GetFileLines(mission + ".ini", name + "_" + shipName + "_" + std::to_string(shipIff)), append);
			CombineFileLines(lines, GetFileLines(mission + "_" + name + "_" + shipName + ".txt"), append);
			CombineFileLines(lines, GetFileLines(mission + ".ini", name + "_" + shipName), append);
		}

		CombineFileLines(lines, GetFileLines(mission + "_" + name + ".txt"), append);
		CombineFileLines(lines, GetFileLines(mission + ".ini", name), append);

		if (!ship.empty())
		{
			const std::string nameIff = name + std::to_string(shipIff);

			CombineFileLines(lines, GetFileLines(ship + nameIff + ".txt"), append);
			CombineFileLines(lines, GetFileLines(ship + ".ini", nameIff), append);
			CombineFileLines(lines, GetFileLines(ship + name + ".txt"), append);
			CombineFileLines(lines, GetFileLines(ship + ".ini", name), append);
		}
	}

	const std::string path = "FlightModels\\";

	CombineFileLines(lines, GetFileLines(path + name + ".txt"), append);
	CombineFileLines(lines, GetFileLines(path + "default.ini", name), append);

	return lines;
}

struct PlayerCraft
{
	unsigned short ModelIndex;
	int OffsetX;
	int OffsetY;
	int OffsetZ;
};

class CustomFileLinesHangarObjects
{
public:
	std::vector<std::string> GetLines()
	{
		this->UpdateIfChanged();
		return this->_lines;
	}

	bool GetLoadShuttle()
	{
		this->UpdateIfChanged();
		return this->LoadShuttle;
	}

	unsigned short GetShuttleModelIndex()
	{
		this->UpdateIfChanged();
		return this->ShuttleModelIndex;
	}

	int GetShuttleMarkings()
	{
		this->UpdateIfChanged();
		return this->ShuttleMarkings;
	}

	std::string GetShuttleObjectProfile()
	{
		this->UpdateIfChanged();
		return this->ShuttleObjectProfile;
	}

	int GetShuttlePositionX()
	{
		this->UpdateIfChanged();
		return this->ShuttlePositionX;
	}

	int GetShuttlePositionY()
	{
		this->UpdateIfChanged();
		return this->ShuttlePositionY;
	}

	int GetShuttlePositionZ()
	{
		this->UpdateIfChanged();
		return this->ShuttlePositionZ;
	}

	unsigned short GetShuttleOrientation()
	{
		this->UpdateIfChanged();
		return this->ShuttleOrientation;
	}

	bool GetIsShuttleFloorInverted()
	{
		this->UpdateIfChanged();
		return this->IsShuttleFloorInverted;
	}

	std::string GetShuttleAnimation()
	{
		this->UpdateIfChanged();
		return this->ShuttleAnimation;
	}

	int GetShuttleAnimationStraightLine()
	{
		this->UpdateIfChanged();
		return this->ShuttleAnimationStraightLine;
	}

	int GetShuttleAnimationElevation()
	{
		this->UpdateIfChanged();
		return this->ShuttleAnimationElevation;
	}

	bool GetLoadDroids()
	{
		this->UpdateIfChanged();
		return this->LoadDroids;
	}

	int GetDroidsPositionZ()
	{
		this->UpdateIfChanged();
		return this->DroidsPositionZ;
	}

	bool GetIsDroidsFloorInverted()
	{
		this->UpdateIfChanged();
		return this->IsDroidsFloorInverted;
	}

	bool GetLoadDroid1()
	{
		this->UpdateIfChanged();
		return this->LoadDroids && this->LoadDroid1;
	}

	int GetDroid1PositionZ()
	{
		this->UpdateIfChanged();
		return this->Droid1PositionZ;
	}

	bool GetIsDroid1FloorInverted()
	{
		this->UpdateIfChanged();
		return this->IsDroidsFloorInverted || this->IsDroid1FloorInverted;
	}

	bool GetDroid1Update()
	{
		this->UpdateIfChanged();
		return GetLoadDroid1() && this->Droid1Update;
	}

	unsigned short GetDroid1ModelIndex()
	{
		this->UpdateIfChanged();
		return this->Droid1ModelIndex;
	}

	int GetDroid1Markings()
	{
		this->UpdateIfChanged();
		return this->Droid1Markings;
	}

	std::string GetDroid1ObjectProfile()
	{
		this->UpdateIfChanged();
		return this->Droid1ObjectProfile;
	}

	bool GetLoadDroid2()
	{
		this->UpdateIfChanged();
		return this->LoadDroids && this->LoadDroid2;
	}

	int GetDroid2PositionZ()
	{
		this->UpdateIfChanged();
		return this->Droid2PositionZ;
	}

	bool GetIsDroid2FloorInverted()
	{
		this->UpdateIfChanged();
		return this->IsDroidsFloorInverted || this->IsDroid2FloorInverted;
	}

	bool GetDroid2Update()
	{
		this->UpdateIfChanged();
		return GetLoadDroid2() && this->Droid2Update;
	}

	unsigned short GetDroid2ModelIndex()
	{
		this->UpdateIfChanged();
		return this->Droid2ModelIndex;
	}

	int GetDroid2Markings()
	{
		this->UpdateIfChanged();
		return this->Droid2Markings;
	}

	std::string GetDroid2ObjectProfile()
	{
		this->UpdateIfChanged();
		return this->Droid2ObjectProfile;
	}

	int GetHangarRoofCranePositionX()
	{
		this->UpdateIfChanged();
		return this->HangarRoofCranePositionX;
	}

	int GetHangarRoofCranePositionY()
	{
		this->UpdateIfChanged();
		return this->HangarRoofCranePositionY;
	}

	int GetHangarRoofCranePositionZ()
	{
		this->UpdateIfChanged();
		return this->HangarRoofCranePositionZ;
	}

	int GetHangarRoofCraneAxis()
	{
		this->UpdateIfChanged();
		return this->HangarRoofCraneAxis;
	}

	int GetHangarRoofCraneLowOffset()
	{
		this->UpdateIfChanged();
		return this->HangarRoofCraneLowOffset;
	}

	int GetHangarRoofCraneHighOffset()
	{
		this->UpdateIfChanged();
		return this->HangarRoofCraneHighOffset;
	}

	bool GetIsHangarFloorInverted()
	{
		this->UpdateIfChanged();
		return this->IsHangarFloorInverted;
	}

	int GetHangarFloorInvertedHeight()
	{
		this->UpdateIfChanged();
		return this->HangarFloorInvertedHeight;
	}

	unsigned char GetHangarIff()
	{
		this->UpdateIfChanged();
		return this->HangarIff;
	}

	bool GetDrawShadows()
	{
		this->UpdateIfChanged();
		return this->DrawShadows;
	}

	int GetPlayerAnimationElevation()
	{
		this->UpdateIfChanged();
		return this->PlayerAnimationElevation;
	}

	int GetPlayerAnimationInvertedElevation()
	{
		this->UpdateIfChanged();
		return this->PlayerAnimationInvertedElevation;
	}

	int GetPlayerAnimationStraightLine()
	{
		this->UpdateIfChanged();
		return this->PlayerAnimationStraightLine;
	}

	PlayerCraft GetPlayerCraft(unsigned short modelIndex)
	{
		this->UpdateIfChanged();

		for (const PlayerCraft& player : this->PlayerCrafts)
		{
			if (player.ModelIndex == modelIndex)
			{
				return player;
			}
		}

		if (this->GetIsPlayerModelIndexFloorInverted(modelIndex))
		{
			PlayerCraft player{};
			player.ModelIndex = modelIndex;
			player.OffsetX = this->PlayerInvertedOffsetX;
			player.OffsetY = this->PlayerInvertedOffsetY;
			player.OffsetZ = this->PlayerInvertedOffsetZ;

			return player;
		}
		else
		{
			PlayerCraft player{};
			player.ModelIndex = modelIndex;
			player.OffsetX = this->PlayerOffsetX;
			player.OffsetY = this->PlayerOffsetY;
			player.OffsetZ = this->PlayerOffsetZ;

			return player;
		}
	}

	//bool GetIsPlayerFloorInverted()
	//{
	//	this->UpdateIfChanged();
	//	return this->IsPlayerFloorInverted;
	//}

	//const std::vector<int>& GetPlayerFloorInvertedModelIndices()
	//{
	//	this->UpdateIfChanged();
	//	return this->PlayerFloorInvertedModelIndices;
	//}

	bool GetIsPlayerModelIndexFloorInverted(unsigned short modelIndex)
	{
		this->UpdateIfChanged();

		if (this->IsPlayerFloorInverted)
		{
			return true;
		}

		const std::vector<int>& indices = this->PlayerFloorInvertedModelIndices;
		return std::find(indices.begin(), indices.end(), modelIndex) != indices.end();
	}

	unsigned int GetLightColorIntensity()
	{
		this->UpdateIfChanged();
		return this->LightColorIntensity;
	}

	unsigned int GetLightColorRgb()
	{
		this->UpdateIfChanged();
		return this->LightColorRgb;
	}

private:
	void UpdateIfChanged()
	{
		if (this->HasChanged())
		{
			const auto lines = GetCustomFileLines("HangarObjects", true);
			this->_lines = lines;
			this->LoadShuttle = GetFileKeyValueInt(lines, "LoadShuttle", 1) == 1;
			this->ShuttleModelIndex = (unsigned short)GetFileKeyValueInt(lines, "ShuttleModelIndex");
			this->ShuttleMarkings = GetFileKeyValueInt(lines, "ShuttleMarkings");
			this->ShuttleObjectProfile = GetFileKeyValue(lines, "ShuttleObjectProfile");

			if (this->ShuttleObjectProfile.empty())
			{
				this->ShuttleObjectProfile = "Default";
			}

			this->ShuttlePositionX = GetFileKeyValueInt(lines, "ShuttlePositionX", 0x467);
			this->ShuttlePositionY = GetFileKeyValueInt(lines, "ShuttlePositionY", 0x3BF);
			this->ShuttlePositionZ = GetFileKeyValueInt(lines, "ShuttlePositionZ", 0);
			this->ShuttleOrientation = (unsigned short)GetFileKeyValueInt(lines, "ShuttleOrientation", 0xA880);
			this->IsShuttleFloorInverted = GetFileKeyValueInt(lines, "IsShuttleFloorInverted", 0) != 0;
			this->ShuttleAnimation = GetFileKeyValue(lines, "ShuttleAnimation");
			this->ShuttleAnimationStraightLine = GetFileKeyValueInt(lines, "ShuttleAnimationStraightLine", 0);
			this->ShuttleAnimationElevation = GetFileKeyValueInt(lines, "ShuttleAnimationElevation", 0);
			this->LoadDroids = GetFileKeyValueInt(lines, "LoadDroids", 1) == 1;
			this->DroidsPositionZ = GetFileKeyValueInt(lines, "DroidsPositionZ", 0);
			this->IsDroidsFloorInverted = GetFileKeyValueInt(lines, "IsDroidsFloorInverted", 0) != 0;
			this->LoadDroid1 = GetFileKeyValueInt(lines, "LoadDroid1", 1) == 1;
			this->Droid1PositionZ = GetFileKeyValueInt(lines, "Droid1PositionZ", this->DroidsPositionZ);
			this->IsDroid1FloorInverted = GetFileKeyValueInt(lines, "IsDroid1FloorInverted", 0) != 0;
			this->Droid1Update = GetFileKeyValueInt(lines, "Droid1Update", 1) != 0;
			this->Droid1ModelIndex = (unsigned short)GetFileKeyValueInt(lines, "Droid1ModelIndex", 311); // ModelIndex_311_1_33_HangarDroid
			this->Droid1Markings = GetFileKeyValueInt(lines, "Droid1Markings", 0);
			this->Droid1ObjectProfile = GetFileKeyValue(lines, "Droid1ObjectProfile");

			if (this->Droid1ObjectProfile.empty())
			{
				this->Droid1ObjectProfile = "Default";
			}

			this->LoadDroid2 = GetFileKeyValueInt(lines, "LoadDroid2", 1) == 1;
			this->Droid2PositionZ = GetFileKeyValueInt(lines, "Droid2PositionZ", this->DroidsPositionZ);
			this->IsDroid2FloorInverted = GetFileKeyValueInt(lines, "IsDroid2FloorInverted", 0) != 0;
			this->Droid2Update = GetFileKeyValueInt(lines, "Droid2Update", 1) != 0;
			this->Droid2ModelIndex = (unsigned short)GetFileKeyValueInt(lines, "Droid2ModelIndex", 312); // ModelIndex_312_1_34_HangarDroid2
			this->Droid2Markings = GetFileKeyValueInt(lines, "Droid2Markings", 0);
			this->Droid2ObjectProfile = GetFileKeyValue(lines, "Droid2ObjectProfile");

			if (this->Droid2ObjectProfile.empty())
			{
				this->Droid2ObjectProfile = "Default";
			}

			this->HangarRoofCranePositionX = GetFileKeyValueInt(lines, "HangarRoofCranePositionX", -1400);
			this->HangarRoofCranePositionY = GetFileKeyValueInt(lines, "HangarRoofCranePositionY", 786);
			this->HangarRoofCranePositionZ = GetFileKeyValueInt(lines, "HangarRoofCranePositionZ", -282);
			this->HangarRoofCraneAxis = GetFileKeyValueInt(lines, "HangarRoofCraneAxis", 0);
			this->HangarRoofCraneLowOffset = GetFileKeyValueInt(lines, "HangarRoofCraneLowOffset", -1400 + 1400);
			this->HangarRoofCraneHighOffset = GetFileKeyValueInt(lines, "HangarRoofCraneHighOffset", 1400 + 1400);
			this->IsHangarFloorInverted = GetFileKeyValueInt(lines, "IsHangarFloorInverted", 0) != 0;
			this->HangarFloorInvertedHeight = GetFileKeyValueInt(lines, "HangarFloorInvertedHeight", 0);
			this->HangarIff = (unsigned char)GetFileKeyValueInt(lines, "HangarIff", -1);
			this->DrawShadows = GetFileKeyValueInt(lines, "DrawShadows", this->IsHangarFloorInverted ? 0 : 1) != 0;
			this->PlayerAnimationElevation = GetFileKeyValueInt(lines, "PlayerAnimationElevation", 0);
			this->PlayerAnimationInvertedElevation = GetFileKeyValueInt(lines, "PlayerAnimationInvertedElevation", this->PlayerAnimationElevation);
			this->PlayerAnimationStraightLine = GetFileKeyValueInt(lines, "PlayerAnimationStraightLine", 0);
			this->PlayerOffsetX = GetFileKeyValueInt(lines, "PlayerOffsetX", 0);
			this->PlayerOffsetY = GetFileKeyValueInt(lines, "PlayerOffsetY", 0);
			this->PlayerOffsetZ = GetFileKeyValueInt(lines, "PlayerOffsetZ", 0);
			this->PlayerInvertedOffsetX = GetFileKeyValueInt(lines, "PlayerInvertedOffsetX", this->PlayerOffsetX);
			this->PlayerInvertedOffsetY = GetFileKeyValueInt(lines, "PlayerInvertedOffsetY", this->PlayerOffsetY);
			this->PlayerInvertedOffsetZ = GetFileKeyValueInt(lines, "PlayerInvertedOffsetZ", this->PlayerOffsetZ);

			std::vector<std::string> playerModelIndices = Tokennize(GetFileKeyValue(lines, "PlayerModelIndices"));
			std::vector<std::string> playerOffsetsX = Tokennize(GetFileKeyValue(lines, "PlayerOffsetsX"));
			std::vector<std::string> playerOffsetsY = Tokennize(GetFileKeyValue(lines, "PlayerOffsetsY"));
			std::vector<std::string> playerOffsetsZ = Tokennize(GetFileKeyValue(lines, "PlayerOffsetsZ"));
			this->PlayerCrafts = std::vector<PlayerCraft>();
			if (playerModelIndices.size() == playerOffsetsX.size()
				&& playerModelIndices.size() == playerOffsetsY.size()
				&& playerModelIndices.size() == playerOffsetsZ.size())
			{
				for (unsigned int i = 0; i < playerModelIndices.size(); i++)
				{
					PlayerCraft player{};
					player.ModelIndex = (unsigned short)std::stoi(playerModelIndices[i], 0, 0);
					player.OffsetX = std::stoi(playerOffsetsX[i], 0, 0);
					player.OffsetY = std::stoi(playerOffsetsY[i], 0, 0);
					player.OffsetZ = std::stoi(playerOffsetsZ[i], 0, 0);
					this->PlayerCrafts.push_back(player);
				}
			}

			this->IsPlayerFloorInverted = GetFileKeyValueInt(lines, "IsPlayerFloorInverted", 0) != 0;

			std::vector<std::string> playerFloorInvertedModelIndices = Tokennize(GetFileKeyValue(lines, "PlayerFloorInvertedModelIndices"));
			this->PlayerFloorInvertedModelIndices = std::vector<int>();
			for (const std::string& value : playerFloorInvertedModelIndices)
			{
				int index = std::stoi(value, 0, 0);
				this->PlayerFloorInvertedModelIndices.push_back(index);
			}

			this->LightColorIntensity = GetFileKeyValueUnsignedInt(lines, "LightColorIntensity", 0xC0);
			this->LightColorRgb = GetFileKeyValueUnsignedInt(lines, "LightColorRgb", 0xFFFFFF);
		}
	}

	std::vector<std::string> _lines;
	bool LoadShuttle = false;
	unsigned short ShuttleModelIndex = 0;
	int ShuttleMarkings = 0;
	std::string ShuttleObjectProfile;
	int ShuttlePositionX = 0;
	int ShuttlePositionY = 0;
	int ShuttlePositionZ = 0;
	unsigned short ShuttleOrientation = 0;
	bool IsShuttleFloorInverted = false;
	std::string ShuttleAnimation;
	int ShuttleAnimationStraightLine = 0;
	int ShuttleAnimationElevation = 0;
	bool LoadDroids = false;
	int DroidsPositionZ = 0;
	bool IsDroidsFloorInverted = 0;
	bool LoadDroid1 = false;
	int Droid1PositionZ = 0;
	bool IsDroid1FloorInverted = 0;
	bool Droid1Update = false;
	unsigned short Droid1ModelIndex = 0;
	std::string Droid1ObjectProfile;
	int Droid1Markings = 0;
	bool LoadDroid2 = false;
	int Droid2PositionZ = 0;
	bool IsDroid2FloorInverted = 0;
	bool Droid2Update = false;
	unsigned short Droid2ModelIndex = 0;
	int Droid2Markings = 0;
	std::string Droid2ObjectProfile;
	int HangarRoofCranePositionX = 0;
	int HangarRoofCranePositionY = 0;
	int HangarRoofCranePositionZ = 0;
	int HangarRoofCraneAxis = 0;
	int HangarRoofCraneLowOffset = 0;
	int HangarRoofCraneHighOffset = 0;
	bool IsHangarFloorInverted = false;
	int HangarFloorInvertedHeight = 0;
	unsigned char HangarIff = 0;
	bool DrawShadows = true;
	int PlayerAnimationElevation = 0;
	int PlayerAnimationInvertedElevation = 0;
	int PlayerAnimationStraightLine = 0;
	int PlayerOffsetX = 0;
	int PlayerOffsetY = 0;
	int PlayerOffsetZ = 0;
	int PlayerInvertedOffsetX = 0;
	int PlayerInvertedOffsetY = 0;
	int PlayerInvertedOffsetZ = 0;
	std::vector<PlayerCraft> PlayerCrafts;
	bool IsPlayerFloorInverted = false;
	std::vector<int> PlayerFloorInvertedModelIndices;
	unsigned int LightColorIntensity = 0;
	unsigned int LightColorRgb = 0;

	bool HasChanged()
	{
		if (!this->isInit)
		{
			this->lastIsProvingGround = *(unsigned char*)(0x08053E0 + 0x05) != 0;

			if (!this->lastIsProvingGround)
			{
				this->lastMissionFileName = (const char*)0x06002E8;
				this->lastMissionFileNameIndex = *(int*)0x06002E4;
				this->lastCommandShip = GetCommandShipModelIndex();
				this->lastCommandShipIff = GetCommandShipIff();
			}

			this->isInit = true;
			return true;
		}

		const bool isProvingGround = *(unsigned char*)(0x08053E0 + 0x05) != 0;
		if (this->lastIsProvingGround != isProvingGround)
		{
			this->lastIsProvingGround = isProvingGround;
			return true;
		}

		if (isProvingGround)
		{
			return false;
		}

		const char* xwaMissionFileName = (const char*)0x06002E8;
		const int missionFileNameIndex = *(int*)0x06002E4;
		if (missionFileNameIndex == 0 ? (this->lastMissionFileName != xwaMissionFileName) : (this->lastMissionFileNameIndex != missionFileNameIndex))
		{
			this->lastMissionFileName = xwaMissionFileName;
			this->lastMissionFileNameIndex = missionFileNameIndex;
			return true;
		}

		const int ship = GetCommandShipModelIndex();
		if (this->lastCommandShip != ship)
		{
			this->lastCommandShip = ship;
			return true;
		}

		const unsigned char shipIff = GetCommandShipIff();
		if (this->lastCommandShipIff != shipIff)
		{
			this->lastCommandShipIff = shipIff;
			return true;
		}

		return false;
	}

	bool isInit = false;
	bool lastIsProvingGround = false;
	std::string lastMissionFileName;
	int lastMissionFileNameIndex = 0;
	int lastCommandShip = -1;
	unsigned char lastCommandShipIff = 0;
};

CustomFileLinesHangarObjects g_hangarObjects;

bool g_isHangarFloorInverted = false;
int g_hangarFloorInvertedHeight = 0;
bool g_hangarDrawShadows = true;
bool g_isShuttleFloorInverted = false;
bool g_isPlayerFloorInverted = false;

void ReadIsHangarFloorInverted()
{
	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const int hangarPlayerObjectIndex = *(int*)0x068BC08;

	g_isHangarFloorInverted = g_hangarObjects.GetIsHangarFloorInverted();
	g_hangarFloorInvertedHeight = g_hangarObjects.GetHangarFloorInvertedHeight();
	g_hangarDrawShadows = g_hangarObjects.GetDrawShadows();
	g_isShuttleFloorInverted = g_hangarObjects.GetIsShuttleFloorInverted();
	g_isPlayerFloorInverted = g_hangarObjects.GetIsPlayerModelIndexFloorInverted(xwaObjects[hangarPlayerObjectIndex].ModelIndex);
}

int GetCraftElevation(unsigned short modelIndex, bool isHangarFloorInverted)
{
	const auto ModelGetSizeZ = (int(*)(unsigned int))0x0485820;

	const std::string optSize = g_flightModelsList.GetLstLine(modelIndex);
	auto optSizeLines = GetFileLines(optSize + "Size.txt");

	if (!optSizeLines.size())
	{
		optSizeLines = GetFileLines(optSize + ".ini", "Size");
	}

	int elevation;

	if (optSizeLines.size())
	{
		if (!isHangarFloorInverted)
		{
			elevation = GetFileKeyValueInt(optSizeLines, "ClosedSFoilsElevation");
		}
		else
		{
			elevation = GetFileKeyValueInt(optSizeLines, "ClosedSFoilsElevationInverted");

			if (elevation == 0)
			{
				elevation = GetFileKeyValueInt(optSizeLines, "ClosedSFoilsElevation");
			}
		}
	}
	else
	{
		switch (modelIndex)
		{
		case 4: // BWing
			elevation = 0x32;
			break;

		case 50: // Shuttle
			elevation = 0x8A;
			break;

		default:
			elevation = ModelGetSizeZ(modelIndex) / 2;
			break;
		}
	}

	return elevation;
}

class ModelIndexHangar
{
public:
	int GetClosedSFoilsElevation(int modelIndex)
	{
		auto it = this->_closedSFoilsElevation.find(modelIndex);

		if (it != this->_closedSFoilsElevation.end())
		{
			return it->second;
		}
		else
		{
			int value = GetCraftElevation(modelIndex, false);
			this->_closedSFoilsElevation.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

	int GetClosedSFoilsElevationInverted(int modelIndex)
	{
		auto it = this->_closedSFoilsElevationInverted.find(modelIndex);

		if (it != this->_closedSFoilsElevationInverted.end())
		{
			return it->second;
		}
		else
		{
			int value = GetCraftElevation(modelIndex, true);
			this->_closedSFoilsElevationInverted.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

private:
	std::map<int, int> _closedSFoilsElevation;
	std::map<int, int> _closedSFoilsElevationInverted;
};

ModelIndexHangar g_modelIndexHangar;

void SetXwaTempString()
{
	char* s_XwaTempString = (char*)0x00ABD680;

	if (*(int*)0x0068BBB8 != 0)
	{
		s_XwaTempString[0] = 0;
		s_XwaTempString[254] = 0;
		s_XwaTempString[255] = 0;
		return;
	}

	s_XwaTempString[0] = 0;
	s_XwaTempString[254] = 1;
	s_XwaTempString[255] = 0;

	const bool isProvingGround = *(unsigned char*)(0x08053E0 + 0x05) != 0;

	if (isProvingGround)
	{
		std::string ship = g_config.ProvingGroundHangarModel;

		if (!ship.empty())
		{
			ship = GetStringWithoutExtension(ship);
			strcpy_s(s_XwaTempString, 254, ship.c_str());
			return;
		}
	}
	else
	{
		const std::string ship = GetCommandShipLstLine();
		const unsigned char shipIff = GetCommandShipIff();

		if (!ship.empty())
		{
			strcpy_s(s_XwaTempString, 254, ship.c_str());
			s_XwaTempString[255] = shipIff;
			return;
		}
	}

	const std::string path = "FlightModels\\";
	strcpy_s(s_XwaTempString, 254, path.c_str());
}

int HangarOptLoadHook(int* params)
{
	SetXwaTempString();

	const char* argOpt = (char*)params[0];
	const unsigned short argModelIndex = (unsigned short)params[0x60];
	unsigned short* OptModelFileMemHandles = (unsigned short*)0x007CA6E0;

	const auto OptLoad = (int(*)(const char*))0x004CC940;

	if (std::find(g_hangarMapModelIndices.begin(), g_hangarMapModelIndices.end(), argModelIndex) != g_hangarMapModelIndices.end())
	{
		return OptModelFileMemHandles[argModelIndex];
	}

	g_hangarMapModelIndices.push_back(argModelIndex);

	std::string opt;

	if (strcmp(argOpt, "FlightModels\\") == 0)
	{
		opt = g_flightModelsList.GetLstLine(argModelIndex);
		opt.append(".opt");
	}
	else
	{
		opt = argOpt;
	}

	const auto lines = g_hangarObjects.GetLines();
	const std::string value = GetFileKeyValue(lines, opt);

	if (!value.empty())
	{
		std::string opt_base_exterior = GetStringWithoutExtension(opt);
		opt_base_exterior.append("Exterior.opt");

		if (_stricmp(opt_base_exterior.c_str(), value.c_str()) != 0)
		{
			if (std::ifstream(value))
			{
				return OptLoad(value.c_str());
			}
		}
	}

	if (_stricmp(opt.c_str(), "FlightModels\\Hangar.opt") == 0)
	{
		const std::string hangar = GetCustomFilePath("Hangar.opt");

		if (std::ifstream(hangar))
		{
			return OptLoad(hangar.c_str());
		}
	}

	return OptLoad(opt.c_str());
}

int HangarOptReloadHook(int* params)
{
	const unsigned short modelIndex = (unsigned short)params[0];

	const auto HangarOptLoad = (int(*)(unsigned short))0x00456FA0;
	const auto OptUnload = (void(*)(unsigned short))0x004CCA60;

	unsigned short* OptModelFileMemHandles = (unsigned short*)0x007CA6E0;
	ExeEnableEntry* ExeEnableTable = (ExeEnableEntry*)0x005FB240;

	if (OptModelFileMemHandles[modelIndex] != 0)
	{
		OptUnload(OptModelFileMemHandles[modelIndex]);

		OptModelFileMemHandles[modelIndex] = 0;
		ExeEnableTable[modelIndex].pData1 = nullptr;
		ExeEnableTable[modelIndex].pData2 = nullptr;
	}

	HangarOptLoad(modelIndex);
	return 0;
}

int HangarObjectCreateHook(int* params)
{
	const unsigned short modelIndex = (unsigned short)params[0];

	const auto OptUnload = (void(*)(unsigned short))0x004CCA60;

	if (std::find(g_hangarMapModelIndices.begin(), g_hangarMapModelIndices.end(), modelIndex) != g_hangarMapModelIndices.end())
	{
		return 0;
	}

	unsigned short* OptModelFileMemHandles = (unsigned short*)0x007CA6E0;
	ExeEnableEntry* ExeEnableTable = (ExeEnableEntry*)0x005FB240;

	if (OptModelFileMemHandles[modelIndex] != 0)
	{
		OptUnload(OptModelFileMemHandles[modelIndex]);

		OptModelFileMemHandles[modelIndex] = 0;
		ExeEnableTable[modelIndex].pData1 = nullptr;
		ExeEnableTable[modelIndex].pData2 = nullptr;
	}

	return 0;
}

void HangarReloadHookReleaseObjects()
{
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const int hangarObjectIndex = *(int*)0x068BCC4;
	const unsigned char hangarTieFlightGroupIndex = xwaObjects[hangarObjectIndex].TieFlightGroupIndex;

	const auto ReleaseObject = (void(*)(int))0x0041E7F0;

	for (int eax = 0; eax < *(int*)0x07FFD80; eax++)
	{
		XwaObject* object = &xwaObjects[eax];

		if (object->ModelIndex == 0)
			continue;

		if (object->TieFlightGroupIndex != hangarTieFlightGroupIndex)
			continue;

		// Hangar or FamilyBase
		if (object->ModelIndex == 0x134 || object->ModelIndex == 0xB3)
			continue;

		if (object->m00 != 0)
		{
			ReleaseObject(eax);
			object->m00 = 0;
		}
	}
}

int HangarReloadHook(int* params)
{
	const short objectIndex = (short)params[2];

	const auto AddObject = (short(*)(unsigned short, int, int, int, unsigned short, unsigned short))0x00456AE0;
	const auto XwaRandom = (unsigned short(*)())0x00494E10;
	const auto SetSFoils = (void(*)(int))0x004016B0;
	const auto SetShipCategoryObjectsRanges = (void(*)(int))0x004154A0;

	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const int hangarObjectIndex = *(int*)0x068BCC4;
	unsigned short& hangarModelIndex = *(unsigned short*)0x009C6754;
	int& V0x068BC10 = *(int*)0x068BC10;
	S0x09C6780* V0x09C6780 = (S0x09C6780*)0x09C6780;

	if (objectIndex == -1)
	{
		return *(int*)0x0077330C;
	}

	unsigned short modelIndex = xwaObjects[objectIndex].ModelIndex;

	int xwaMission = *(int*)0x09EB8E0;
	unsigned char missionType = *(unsigned char*)(xwaMission + 0xAF7DC + 0x23A6);
	hangarModelIndex = (missionType == 7 && SelectHangarTypeHook(nullptr) == 1) ? 0xB3 : 0x134;
	xwaObjects[hangarObjectIndex].ModelIndex = hangarModelIndex;

	unsigned char iff = g_hangarObjects.GetHangarIff();

	if (iff == 0xff)
	{
		iff = GetCommandShipIff();
	}

	xwaObjects[hangarObjectIndex].pMobileObject->Markings = iff;

	if (hangarModelIndex == 0x134)
	{
		int HangarOptReloadHookParams[] = { 0x134 };
		HangarOptReloadHook(HangarOptReloadHookParams);
	}

	HangarReloadHookReleaseObjects();

	SetShipCategoryObjectsRanges(xwaObjects[hangarObjectIndex].Region);

	V0x068BC10 = 0;

	if (hangarModelIndex == 0x134)
	{
		HangarMapHook(nullptr);
		HangarLoadDroidsHook(nullptr);

		S0x09C6780* s_V0x068BBC8 = (S0x09C6780*)0x068BBC8;
		int HangarLoadShuttleHookParams[] = { 0x32, 0x467, 0x3BF, -0x2E5, 0xA880, 0 }; //ModelIndex_050_0_37_Shuttle
		s_V0x068BBC8->ObjectIndex = HangarLoadShuttleHook(HangarLoadShuttleHookParams);
		SetSFoils(s_V0x068BBC8->ObjectIndex);
		s_V0x068BBC8->m04 = 0;
		s_V0x068BBC8->m26 = XwaRandom() / 2 + 0x2710;
		s_V0x068BBC8->m22 = 0;
		s_V0x068BBC8->m08 = 0;

		S0x09C6780* s_V0x0686D38 = (S0x09C6780*)0x0686D38;
		s_V0x0686D38->ObjectIndex = HangarLoadHangarRoofCraneHook(nullptr);
		s_V0x0686D38->m04 = 0;
		s_V0x0686D38->m26 = XwaRandom() / 2;
		s_V0x0686D38->m22 = 0;
		s_V0x0686D38->m08 = 0;
	}
	else
	{
		FamHangarMapHook(nullptr);

		V0x09C6780[V0x068BC10].ObjectIndex = AddObject(0x1E9, 0x4B8, -0x1B47, -0x12DF, 0, 0); // ModelIndex_489_1_157_WorkDroid1
		V0x09C6780[V0x068BC10].m04 = 0;
		V0x09C6780[V0x068BC10].m26 = 0;
		V0x09C6780[V0x068BC10].m22 = 0;
		V0x09C6780[V0x068BC10].m08 = 0;
		V0x09C6780[V0x068BC10].m0C = 0;
		V0x09C6780[V0x068BC10].m10 = 0;
		V0x09C6780[V0x068BC10].m2A = 0;
		V0x068BC10++;
	}

	return *(int*)0x0077330C;
}

int SwitchCraftHook(int* params)
{
	params[Params_EAX] = *(int*)0x0068BC08;

	int& missionFileNameIndex = *(int*)0x06002E4;

	if (missionFileNameIndex != 0)
	{
		missionFileNameIndex++;
	}

	return 0;
}

int HangarCameraPositionHook(int* params)
{
	const short A4 = (short)params[8];
	int& positionX = params[0];
	int& positionY = params[1];
	int& positionZ = params[2];

	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const unsigned short hangarModelIndex = *(unsigned short*)0x09C6754;
	const int playerObjectIndex = *(int*)0x068BC08;
	//const int playerObjectIndex = *(int*)0x068BB98;
	const int V0x0686B94 = *(int*)0x0686B94;

	const auto ModelGetSizeX = (int(*)(unsigned int))0x04857A0;
	const auto ModelGetSizeY = (int(*)(unsigned int))0x04857E0;
	const auto ModelGetSizeZ = (int(*)(unsigned int))0x0485820;

	const auto CockpitOptReadInfos = (void(*)())0x004314B0;
	const auto ExteriorOptReadInfos = (void(*)())0x00431960;

	if (A4 == 0)
	{
		CockpitOptReadInfos();
	}
	else
	{
		ExteriorOptReadInfos();
	}

	if (hangarModelIndex == 0x134)
	{
		// hangar
		// Key 0 is the cockpit view.
		// Keys 1, 2, 3, 6, 9 are fixed views.
		// Keys 4 and 5 are related to the hangar droids.
		// Key 7 is either the same as key 3, either related to the player model.
		// Key 8 is related to the hangar roof crane.

		const auto cameraLines = GetCustomFileLines("HangarCamera", false);

		switch (A4)
		{
		case 1:
			if (cameraLines.size())
			{
				positionX += GetFileKeyValueInt(cameraLines, "Key1_X");
				positionY += GetFileKeyValueInt(cameraLines, "Key1_Y");
				positionZ += GetFileKeyValueInt(cameraLines, "Key1_Z");
			}
			else
			{
				positionX += 0x46A;
				positionY += -0x910;
				positionZ += -0x12C;
			}
			break;

		case 2:
			if (cameraLines.size())
			{
				positionX += GetFileKeyValueInt(cameraLines, "Key2_X");
				positionY += GetFileKeyValueInt(cameraLines, "Key2_Y");
				positionZ += GetFileKeyValueInt(cameraLines, "Key2_Z");
			}
			else
			{
				positionX += 0x4D8;
				positionY += -0x14A;
				positionZ += -0x2BC;
			}
			break;

		case 3:
			if (cameraLines.size())
			{
				positionX += GetFileKeyValueInt(cameraLines, "Key3_X");
				positionY += GetFileKeyValueInt(cameraLines, "Key3_Y");
				positionZ += GetFileKeyValueInt(cameraLines, "Key3_Z");
			}
			else
			{
				positionX += -0x460;
				positionY += 0x550;
				positionZ += -0x316;
			}
			break;

		case 4:
		{
			const S0x09C6780* V0x09C6780 = (S0x09C6780*)0x09C6780;
			int& V0x068BCC0 = *(int*)0x068BCC0;

			bool value = g_hangarObjects.GetLoadDroid1();

			if (value)
			{
				V0x068BCC0 = V0x09C6780[0].ObjectIndex;

				positionX = xwaObjects[V0x068BCC0].PositionX;
				positionY = xwaObjects[V0x068BCC0].PositionY;
				positionZ = xwaObjects[V0x068BCC0].PositionZ + 0x1E;
			}
			else
			{
				if (cameraLines.size())
				{
					positionX += GetFileKeyValueInt(cameraLines, "Key3_X");
					positionY += GetFileKeyValueInt(cameraLines, "Key3_Y");
					positionZ += GetFileKeyValueInt(cameraLines, "Key3_Z");
				}
				else
				{
					positionX += -0x460;
					positionY += 0x550;
					positionZ += -0x316;
				}
			}

			break;
		}

		case 5:
		{
			const S0x09C6780* V0x09C6780 = (S0x09C6780*)0x09C6780;
			int& V0x068BCC0 = *(int*)0x068BCC0;

			bool value = g_hangarObjects.GetLoadDroid2();

			if (value)
			{
				V0x068BCC0 = V0x09C6780[1].ObjectIndex;

				positionX = xwaObjects[V0x068BCC0].PositionX;
				positionY = xwaObjects[V0x068BCC0].PositionY;
				positionZ = xwaObjects[V0x068BCC0].PositionZ + 0x1E;
			}
			else
			{
				if (cameraLines.size())
				{
					positionX += GetFileKeyValueInt(cameraLines, "Key3_X");
					positionY += GetFileKeyValueInt(cameraLines, "Key3_Y");
					positionZ += GetFileKeyValueInt(cameraLines, "Key3_Z");
				}
				else
				{
					positionX += -0x460;
					positionY += 0x550;
					positionZ += -0x316;
				}
			}

			break;
		}

		case 6:
			if (cameraLines.size())
			{
				positionX += GetFileKeyValueInt(cameraLines, "Key6_X");
				positionY += GetFileKeyValueInt(cameraLines, "Key6_Y");
				positionZ += GetFileKeyValueInt(cameraLines, "Key6_Z");
			}
			else
			{
				positionX += -0x4B0;
				positionY += -0x5FA;
				positionZ += -0x352;
			}
			break;

		case 8:
		{
			const S0x09C6780& V0x0686D38 = *(S0x09C6780*)0x0686D38;
			int& V0x068BCC0 = *(int*)0x068BCC0;

			V0x068BCC0 = V0x0686D38.ObjectIndex;

			positionX = xwaObjects[V0x068BCC0].PositionX;
			positionY = xwaObjects[V0x068BCC0].PositionY + 0x28E;
			positionZ = xwaObjects[V0x068BCC0].PositionZ - 0x08;

			break;
		}

		case 9:
			if (cameraLines.size())
			{
				positionX += GetFileKeyValueInt(cameraLines, "Key9_X");
				positionY += GetFileKeyValueInt(cameraLines, "Key9_Y");
				positionZ += GetFileKeyValueInt(cameraLines, "Key9_Z");
			}
			else
			{
				positionX += 0x42E;
				positionY += 0x1220;
				positionZ += -0x82;
			}
			break;

		case 7:
			if (playerObjectIndex == 0xffff || V0x0686B94 != 0)
			{
				if (cameraLines.size())
				{
					positionX += GetFileKeyValueInt(cameraLines, "Key3_X");
					positionY += GetFileKeyValueInt(cameraLines, "Key3_Y");
					positionZ += GetFileKeyValueInt(cameraLines, "Key3_Z");
				}
				else
				{
					positionX += -0x460;
					positionY += 0x550;
					positionZ += -0x316;
				}
			}
			else
			{
				const unsigned short playerModelIndex = xwaObjects[playerObjectIndex].ModelIndex;
				const int playerPositionX = xwaObjects[playerObjectIndex].PositionX;
				const int playerPositionY = xwaObjects[playerObjectIndex].PositionY;
				const int playerPositionZ = xwaObjects[playerObjectIndex].PositionZ;

				const std::string optCamera = g_flightModelsList.GetLstLine(playerModelIndex);
				auto optCameraLines = GetFileLines(optCamera + "Camera.txt");

				if (!optCameraLines.size())
				{
					optCameraLines = GetFileLines(optCamera + ".ini", "Camera");
				}

				if (optCameraLines.size())
				{
					int x;
					int y;
					int z;

					if (g_hangarObjects.GetIsPlayerModelIndexFloorInverted(playerModelIndex))
					{
						x = GetFileKeyValueInt(optCameraLines, "InvertedX");
						y = GetFileKeyValueInt(optCameraLines, "InvertedY");
						z = GetFileKeyValueInt(optCameraLines, "InvertedZ");

						if (x == 0 && y == 0 && z == 0)
						{
							x = GetFileKeyValueInt(optCameraLines, "X");
							y = GetFileKeyValueInt(optCameraLines, "Y");
							z = GetFileKeyValueInt(optCameraLines, "Z");
						}
					}
					else
					{
						x = GetFileKeyValueInt(optCameraLines, "X");
						y = GetFileKeyValueInt(optCameraLines, "Y");
						z = GetFileKeyValueInt(optCameraLines, "Z");
					}

					positionX = playerPositionX + x;
					positionY = playerPositionY + y;
					positionZ = playerPositionZ + z;
				}
				else
				{
					switch (playerModelIndex)
					{
					case 2:
						// YWing
						positionX += -0x118;
						positionY += -0x87;
						positionZ += -0x2E5;
						break;

					case 4:
						// BWing
						positionX += 0x1D6;
						positionY += -0x28A;
						positionZ += -0x302;
						break;

					case 14:
						// Z-95
						positionX += 0x96;
						positionY += -0x1EC;
						positionZ += -0x331;
						break;

					case 50:
						// Shuttle
						positionX += -0x118;
						positionY += -0x87;
						positionZ += -0x2E5;
						break;

					case 58:
						// CorellianTransport2
						positionX += 0x1D6;
						positionY += -0x1F4;
						positionZ += -0x2C6;
						break;

					case 59:
						// MilleniumFalcon2
						positionX += 0x1D6;
						positionY += -0x1F4;
						positionZ += -0x2C6;
						break;

					case 65:
						// FamilyTransport
						positionX += 0x50;
						positionY += 0x3C;
						positionZ += -0x24E;
						break;

					default:
						positionX = playerPositionX - 0x5A;
						positionY = playerPositionY + ModelGetSizeY(playerModelIndex) / 2 + 0x2D;
						positionZ = playerPositionZ + ModelGetSizeZ(playerModelIndex) / 2 + 0x50;
						break;
					}
				}
			}

			break;
		}
	}
	else
	{
		// family base
		// Key 0 is the cockpit view.
		// Keys 1, 2, 3, 6, 7, 8, 9 are fixed views.
		// Key 4 is related to the work droid 1.
		// Key 5 is related to the player model.

		const auto cameraLines = GetCustomFileLines("FamHangarCamera", false);

		switch (A4)
		{
		case 1:
			if (cameraLines.size())
			{
				positionX += GetFileKeyValueInt(cameraLines, "FamKey1_X");
				positionY += GetFileKeyValueInt(cameraLines, "FamKey1_Y");
				positionZ += GetFileKeyValueInt(cameraLines, "FamKey1_Z");
			}
			else
			{
				positionX += 0x30C;
				positionY += -0x1947;
				positionZ += -0x1371;
			}
			break;

		case 2:
			if (cameraLines.size())
			{
				positionX += GetFileKeyValueInt(cameraLines, "FamKey2_X");
				positionY += GetFileKeyValueInt(cameraLines, "FamKey2_Y");
				positionZ += GetFileKeyValueInt(cameraLines, "FamKey2_Z");
			}
			else
			{
				positionX += -0x7B2;
				positionY += -0x226A;
				positionZ += -0x1263;
			}
			break;

		case 3:
			if (cameraLines.size())
			{
				positionX += GetFileKeyValueInt(cameraLines, "FamKey3_X");
				positionY += GetFileKeyValueInt(cameraLines, "FamKey3_Y");
				positionZ += GetFileKeyValueInt(cameraLines, "FamKey3_Z");
			}
			else
			{
				positionX += 0x9CE;
				positionY += -0x150F;
				positionZ += -0x13CB;
			}
			break;

		case 6:
			if (cameraLines.size())
			{
				positionX += GetFileKeyValueInt(cameraLines, "FamKey6_X");
				positionY += GetFileKeyValueInt(cameraLines, "FamKey6_Y");
				positionZ += GetFileKeyValueInt(cameraLines, "FamKey6_Z");
			}
			else
			{
				positionX += 0x6CC;
				positionY += -0x210D;
				positionZ += -0x13B7;
			}
			break;

		case 7:
			if (cameraLines.size())
			{
				positionX += GetFileKeyValueInt(cameraLines, "FamKey7_X");
				positionY += GetFileKeyValueInt(cameraLines, "FamKey7_Y");
				positionZ += GetFileKeyValueInt(cameraLines, "FamKey7_Z");
			}
			else
			{
				positionX += 0xC6C;
				positionY += 0xA45;
				positionZ += -0xEC1;
			}
			break;

		case 8:
			if (cameraLines.size())
			{
				positionX += GetFileKeyValueInt(cameraLines, "FamKey8_X");
				positionY += GetFileKeyValueInt(cameraLines, "FamKey8_Y");
				positionZ += GetFileKeyValueInt(cameraLines, "FamKey8_Z");
			}
			else
			{
				positionX += 0x2032;
				positionY += 0x19C8;
				positionZ += 0x0A;
			}
			break;

		case 9:
			if (cameraLines.size())
			{
				positionX += GetFileKeyValueInt(cameraLines, "FamKey9_X");
				positionY += GetFileKeyValueInt(cameraLines, "FamKey9_Y");
				positionZ += GetFileKeyValueInt(cameraLines, "FamKey9_Z");
			}
			else
			{
				positionX += -0x3430;
				positionY += 0x88CB;
				positionZ += -0x1989;
			}
			break;

		case 5:
			if (playerObjectIndex == 0xffff)
			{
				if (cameraLines.size())
				{
					positionX += GetFileKeyValueInt(cameraLines, "FamKey3_X");
					positionY += GetFileKeyValueInt(cameraLines, "FamKey3_Y");
					positionZ += GetFileKeyValueInt(cameraLines, "FamKey3_Z");
				}
				else
				{
					positionX += 0x9CE;
					positionY += -0x150F;
					positionZ += -0x13CB;
				}
			}
			else
			{
				const unsigned short playerModelIndex = xwaObjects[playerObjectIndex].ModelIndex;
				const int playerPositionX = xwaObjects[playerObjectIndex].PositionX;
				const int playerPositionY = xwaObjects[playerObjectIndex].PositionY;
				const int playerPositionZ = xwaObjects[playerObjectIndex].PositionZ;

				const std::string optCamera = g_flightModelsList.GetLstLine(playerModelIndex);
				auto optCameraLines = GetFileLines(optCamera + "Camera.txt");

				if (!optCameraLines.size())
				{
					optCameraLines = GetFileLines(optCamera + ".ini", "Camera");
				}

				if (optCameraLines.size())
				{
					int x;
					int y;
					int z;

					if (g_hangarObjects.GetIsPlayerModelIndexFloorInverted(playerModelIndex))
					{
						x = GetFileKeyValueInt(optCameraLines, "FamInvertedX");
						y = GetFileKeyValueInt(optCameraLines, "FamInvertedY");
						z = GetFileKeyValueInt(optCameraLines, "FamInvertedZ");

						if (x == 0 && y == 0 && z == 0)
						{
							x = GetFileKeyValueInt(optCameraLines, "FamX");
							y = GetFileKeyValueInt(optCameraLines, "FamY");
							z = GetFileKeyValueInt(optCameraLines, "FamZ");

							if (x == 0 && y == 0 && z == 0)
							{
								x = GetFileKeyValueInt(optCameraLines, "InvertedX");
								y = GetFileKeyValueInt(optCameraLines, "InvertedY");
								z = GetFileKeyValueInt(optCameraLines, "InvertedZ");

								if (x == 0 && y == 0 && z == 0)
								{
									x = GetFileKeyValueInt(optCameraLines, "X");
									y = GetFileKeyValueInt(optCameraLines, "Y");
									z = GetFileKeyValueInt(optCameraLines, "Z");
								}
							}
						}
					}
					else
					{
						x = GetFileKeyValueInt(optCameraLines, "FamX");
						y = GetFileKeyValueInt(optCameraLines, "FamY");
						z = GetFileKeyValueInt(optCameraLines, "FamZ");

						if (x == 0 && y == 0 && z == 0)
						{
							x = GetFileKeyValueInt(optCameraLines, "X");
							y = GetFileKeyValueInt(optCameraLines, "Y");
							z = GetFileKeyValueInt(optCameraLines, "Z");
						}
					}

					positionX = playerPositionX + x;
					positionY = playerPositionY + y;
					positionZ = playerPositionZ + z;
				}
				else
				{
					switch (playerModelIndex)
					{
					case 58:
						// CorellianTransport2
						positionX += 0x262;
						positionY += -0x1950;
						positionZ += -0x1388;
						break;

					case 59:
						// MilleniumFalcon2
						positionX += 0x262;
						positionY += -0x1950;
						positionZ += -0x1388;
						break;

					case 65:
						// FamilyTransport
						positionX += 0x190;
						positionY += -0x152C;
						positionZ += -0x1306;
						break;

					default:
						positionX = playerPositionX - 0x5A;
						positionY = playerPositionY + ModelGetSizeY(playerModelIndex) / 2 + 0x2D;
						positionZ = playerPositionZ + ModelGetSizeZ(playerModelIndex) / 2 + 0x50;
						break;
					}
				}
			}

			break;
		}
	}

	return 0;
}

std::string GetShipPath(int modelIndex)
{
	const std::string shipPath = g_flightModelsList.GetLstLine(modelIndex);

	{
		const auto objectLines = GetCustomFileLinesBase("Objects", false);
		const std::string objectValue = GetFileKeyValue(objectLines, shipPath + ".opt");

		if (!objectValue.empty() && std::ifstream(objectValue))
		{
			return GetStringWithoutExtension(objectValue);
		}
	}

	{
		const auto objectLines = GetCustomFileLines("HangarObjects", true);
		const std::string objectValue = GetFileKeyValue(objectLines, shipPath + ".opt");

		if (!objectValue.empty() && std::ifstream(objectValue))
		{
			return GetStringWithoutExtension(objectValue);
		}
	}

	return shipPath;
}

void ApplyObjectProfile(unsigned short modelIndex, std::string objectProfile, XwaCraft* pCraft)
{
	const ExeEnableEntry* ExeEnableTable = (ExeEnableEntry*)0x005FB240;

	if (ExeEnableTable[modelIndex].CraftIndex == -1)
	{
		return;
	}

	int meshesCount;
	char* m292;
	char* m22E;

	if (g_craftConfig.MeshesCount == 0)
	{
		meshesCount = 49;
		m292 = pCraft->XwaCraft_m292;
		m22E = pCraft->XwaCraft_m22E;
	}
	else
	{
		meshesCount = g_craftConfig.MeshesCount - 1;
		m292 = (char*)((int)pCraft + g_craftConfig.Craft_Offset_292);
		m22E = (char*)((int)pCraft + g_craftConfig.Craft_Offset_22E);
	}

	std::string shipPath = GetShipPath(modelIndex);

	auto lines = GetFileLines(shipPath + "ObjectProfiles.txt");

	if (!lines.size())
	{
		lines = GetFileLines(shipPath + ".ini", "ObjectProfiles");
	}

	const auto values = Tokennize(GetFileKeyValue(lines, objectProfile));

	for (int index = 0; index < meshesCount; index++)
	{
		m292[index] = (char)0xFF;
		m22E[index] = 0;
	}

	for (const std::string& value : values)
	{
		int index = std::stoi(value);

		if (index < 0 || index >= meshesCount)
		{
			continue;
		}

		m292[index] = 0;
		m22E[index] = 4;
	}
}

int HangarLoadShuttleHook(int* params)
{
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const auto AddObject = (short(*)(unsigned short, int, int, int, unsigned short, unsigned short))0x00456AE0;
	const auto SetSFoils = (void(*)(int))0x004016B0;

	//unsigned short a0 = (unsigned short)params[0];
	//const int a1 = params[1];
	//const int a2 = params[2];
	//const int a3 = params[3];
	//const unsigned short a4 = (unsigned short)params[4];
	//const unsigned short a5 = (unsigned short)params[5];

	unsigned short a0 = 0x32;
	const int a1 = g_hangarObjects.GetShuttlePositionX();
	const int a2 = g_hangarObjects.GetShuttlePositionY();
	const int a3 = 0x7FFFFFFF;
	const unsigned short a4 = g_hangarObjects.GetShuttleOrientation();
	const unsigned short a5 = 0;

	const bool value = g_hangarObjects.GetLoadShuttle();
	const unsigned short shuttleModelIndex = g_hangarObjects.GetShuttleModelIndex();
	const int shuttleMarkings = g_hangarObjects.GetShuttleMarkings();
	const int shuttlePositionZ = g_hangarObjects.GetShuttlePositionZ();

	if (shuttleModelIndex != 0)
	{
		a0 = shuttleModelIndex;
	}

	if (value)
	{
		bool isHangarFloorInverted = g_isHangarFloorInverted;
		g_isHangarFloorInverted = g_isShuttleFloorInverted;

		short objectIndex = AddObject(a0, a1, a2, a3, a4, a5);

		SetSFoils(objectIndex);

		g_isHangarFloorInverted = isHangarFloorInverted;

		xwaObjects[objectIndex].pMobileObject->Markings = shuttleMarkings;
		ApplyObjectProfile(shuttleModelIndex, g_hangarObjects.GetShuttleObjectProfile(), xwaObjects[objectIndex].pMobileObject->pCraft);

		xwaObjects[objectIndex].PositionZ += shuttlePositionZ;
		xwaObjects[objectIndex].pMobileObject->PositionZ = xwaObjects[objectIndex].PositionZ;

		return objectIndex;
	}

	return 0;
}

int HangarShuttleUpdateHook(int* params)
{
	const auto UpdateShuttle = (void(*)(int))0x0045EC70;

	const int a0 = params[0];

	const bool value = g_hangarObjects.GetLoadShuttle();

	if (value)
	{
		UpdateShuttle(a0);
	}

	return 0;
}

int HangarShuttleReenterPositionHook(int* params)
{
	ReadIsHangarFloorInverted();

	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const int hangarPlayerObjectIndex = *(int*)0x068BC08;
	int& positionX = xwaObjects[hangarPlayerObjectIndex].PositionX;
	int& positionY = xwaObjects[hangarPlayerObjectIndex].PositionY;
	int& positionZ = xwaObjects[hangarPlayerObjectIndex].PositionZ;
	const int hangarObjectIndex = *(int*)0x068BCC4;
	const int V0x068BC38 = *(int*)0x068BC38;

	positionX = xwaObjects[hangarObjectIndex].PositionX;
	positionY = xwaObjects[hangarObjectIndex].PositionY;
	positionZ = V0x068BC38;

	xwaObjects[hangarPlayerObjectIndex].m13 = 0;
	xwaObjects[hangarPlayerObjectIndex].m15 = 0x4000;
	xwaObjects[hangarPlayerObjectIndex].m17 = 0;

	const bool value = g_hangarObjects.GetLoadShuttle();
	const int shuttlePositionX = g_hangarObjects.GetShuttlePositionX();
	const int shuttlePositionZ = g_hangarObjects.GetShuttlePositionZ();
	const int shuttleAnimationStraightLine = g_hangarObjects.GetShuttleAnimationStraightLine();
	const int shuttleAnimationElevation = g_hangarObjects.GetShuttleAnimationElevation();

	if (value)
	{
		positionX += shuttlePositionX;
		positionY += 0x2328 + shuttleAnimationStraightLine;

		if (!g_isShuttleFloorInverted)
		{
			positionZ += 0x161 + shuttleAnimationElevation;
		}
		else
		{
			positionZ -= 0x161 + shuttleAnimationElevation;
		}

		positionZ += shuttlePositionZ;

		xwaObjects[hangarPlayerObjectIndex].m13 = -0x8000;
		xwaObjects[hangarPlayerObjectIndex].m15 = 0x4000;
		xwaObjects[hangarPlayerObjectIndex].m17 = 0;
	}

	return 0;
}

int HangarShuttleOptReadInfosHook(int* params)
{
	const auto CockpitOptReadInfos = (void(*)())0x004314B0;
	const auto ExteriorOptReadInfos = (void(*)())0x00431960;

	const bool value = g_hangarObjects.GetLoadShuttle();

	if (value)
	{
		CockpitOptReadInfos();
		ExteriorOptReadInfos();
	}

	return 0;
}

void HangarLoadDroidsSetPositionZ(int objectIndex, int positionZ)
{
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;

	xwaObjects[objectIndex].PositionZ += positionZ;
	xwaObjects[objectIndex].pMobileObject->PositionZ = xwaObjects[objectIndex].PositionZ;
}

int HangarLoadDroidsHook(int* params)
{
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const auto AddObject = (short(*)(unsigned short, int, int, int, unsigned short, unsigned short))0x00456AE0;

	int& V0x068BC10 = *(int*)0x068BC10;
	S0x09C6780* V0x09C6780 = (S0x09C6780*)0x09C6780;

	const bool value = g_hangarObjects.GetLoadDroids();

	if (value)
	{
		bool isHangarFloorInverted = g_isHangarFloorInverted;

		// ModelIndex_311_1_33_HangarDroid
		if (g_hangarObjects.GetLoadDroid1())
		{
			g_isHangarFloorInverted = g_hangarObjects.GetIsDroid1FloorInverted();
			V0x09C6780[V0x068BC10].ObjectIndex = AddObject(g_hangarObjects.GetDroid1ModelIndex(), 0xE3, 0x15F, 0x7FFFFFFF, 0xE570, 0);
			V0x09C6780[V0x068BC10].m04 = 0;
			V0x09C6780[V0x068BC10].m26 = 0;
			V0x09C6780[V0x068BC10].m22 = 0;
			V0x09C6780[V0x068BC10].m08 = 0;
			HangarLoadDroidsSetPositionZ(V0x09C6780[V0x068BC10].ObjectIndex, g_hangarObjects.GetDroid1PositionZ());
			xwaObjects[V0x09C6780[V0x068BC10].ObjectIndex].pMobileObject->Markings = g_hangarObjects.GetDroid1Markings();
			ApplyObjectProfile(g_hangarObjects.GetDroid1ModelIndex(), g_hangarObjects.GetDroid1ObjectProfile(), xwaObjects[V0x09C6780[V0x068BC10].ObjectIndex].pMobileObject->pCraft);
			V0x068BC10++;
		}
		else
		{
			V0x09C6780[V0x068BC10].ObjectIndex = 0;
			V0x068BC10++;
		}

		// ModelIndex_312_1_34_HangarDroid2
		if (g_hangarObjects.GetLoadDroid2())
		{
			g_isHangarFloorInverted = g_hangarObjects.GetIsDroid2FloorInverted();
			V0x09C6780[V0x068BC10].ObjectIndex = AddObject(g_hangarObjects.GetDroid2ModelIndex(), 0xE3, 0x15F, 0x7FFFFFFF, 0xE570, 0);
			V0x09C6780[V0x068BC10].m04 = 0;
			V0x09C6780[V0x068BC10].m26 = 0;
			V0x09C6780[V0x068BC10].m22 = 0;
			V0x09C6780[V0x068BC10].m08 = 0;
			HangarLoadDroidsSetPositionZ(V0x09C6780[V0x068BC10].ObjectIndex, g_hangarObjects.GetDroid2PositionZ());
			xwaObjects[V0x09C6780[V0x068BC10].ObjectIndex].pMobileObject->Markings = g_hangarObjects.GetDroid2Markings();
			ApplyObjectProfile(g_hangarObjects.GetDroid2ModelIndex(), g_hangarObjects.GetDroid2ObjectProfile(), xwaObjects[V0x09C6780[V0x068BC10].ObjectIndex].pMobileObject->pCraft);
			V0x068BC10++;
		}
		else
		{
			V0x09C6780[V0x068BC10].ObjectIndex = 0;
			V0x068BC10++;
		}

		g_isHangarFloorInverted = isHangarFloorInverted;
	}

	return 0;
}

int HangarDroidsUpdateHook(int* params)
{
	int& edx = params[0];
	int& ebp = params[1];
	int& eax = params[2];
	int& ecx = params[3];
	int& esi = params[4];
	int& ebx = params[5];
	int& esp10 = params[6 + 4];
	int& esp20 = params[6 + 8];

	edx = *(int*)0x00686B94;
	ebp = *(int*)0x007B33C4;
	eax = esp10;
	ecx = *(int*)0x0068BC08;
	esi = esp10;
	esp20 = *(int*)(0x009C6780 + esp10 * 0x2E);
	ebx = esp20;

	switch (esp10)
	{
	case 0:
		return g_hangarObjects.GetDroid1Update() ? 1 : 0;

	case 1:
		return g_hangarObjects.GetDroid2Update() ? 1 : 0;
	}

	return 0;
}

int HangarLoadHangarRoofCraneHook(int* params)
{
	const auto AddObject = (short(*)(unsigned short, int, int, int, unsigned short, unsigned short))0x00456AE0;

	const int positionX = g_hangarObjects.GetHangarRoofCranePositionX();
	const int positionY = g_hangarObjects.GetHangarRoofCranePositionY();
	const int positionZ = g_hangarObjects.GetHangarRoofCranePositionZ();

	// ModelIndex_316_1_38_HangarRoofCrane
	return AddObject(316, positionX, positionY, positionZ, 0, 0);
}

int HangarRoofCraneInitHook(int* params)
{
	const int edi = params[0];

	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	S0x09C6780* s_V0x0686D38 = (S0x09C6780*)0x0686D38;
	const int hangarObjectIndex = *(int*)0x068BCC4;

	int axis = g_hangarObjects.GetHangarRoofCraneAxis();
	int lowOffset = g_hangarObjects.GetHangarRoofCraneLowOffset();
	int highOffset = g_hangarObjects.GetHangarRoofCraneHighOffset();

	int edx;
	int eax;
	int init;

	switch (axis)
	{
	case 0:
	default:
		edx = xwaObjects[s_V0x0686D38->ObjectIndex].PositionX;
		eax = xwaObjects[hangarObjectIndex].PositionX;
		init = g_hangarObjects.GetHangarRoofCranePositionX();
		break;

	case 1:
		edx = xwaObjects[s_V0x0686D38->ObjectIndex].PositionY;
		eax = xwaObjects[hangarObjectIndex].PositionY;
		init = g_hangarObjects.GetHangarRoofCranePositionY();
		break;

	case 2:
		edx = xwaObjects[s_V0x0686D38->ObjectIndex].PositionZ;
		eax = xwaObjects[hangarObjectIndex].PositionZ;
		init = g_hangarObjects.GetHangarRoofCranePositionZ();
		break;
	}

	int ecx;

	if (edx + edi > eax + init + highOffset)
	{
		ecx = -edi;
	}
	else if (edx + edi < eax + init + lowOffset)
	{
		ecx = -edi;
	}
	else
	{
		ecx = edi;
	}

	ecx = std::min(ecx, eax + init + highOffset - edx);
	ecx = std::max(ecx, eax + init + lowOffset - edx);

	switch (axis)
	{
	case 0:
	default:
		s_V0x0686D38->TargetPositionX = edx + ecx;
		break;

	case 1:
		s_V0x0686D38->TargetPositionY = edx + ecx;
		break;

	case 2:
		s_V0x0686D38->TargetPositionZ = edx + ecx;
		break;
	}

	s_V0x0686D38->m04 = 1;

	return 0;
}

int HangarRoofCraneUpdateHook(int* params)
{
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	S0x09C6780* s_V0x0686D38 = (S0x09C6780*)0x0686D38;

	int axis = g_hangarObjects.GetHangarRoofCraneAxis();

	int edi;
	int ecx;

	switch (axis)
	{
	case 0:
	default:
		edi = (int)&xwaObjects[s_V0x0686D38->ObjectIndex].PositionX;
		ecx = s_V0x0686D38->TargetPositionX;
		break;

	case 1:
		edi = (int)&xwaObjects[s_V0x0686D38->ObjectIndex].PositionY;
		ecx = s_V0x0686D38->TargetPositionY;
		break;

	case 2:
		edi = (int)&xwaObjects[s_V0x0686D38->ObjectIndex].PositionZ;
		ecx = s_V0x0686D38->TargetPositionZ;
		break;
	}

	params[0] = edi;
	params[1] = ecx;

	return 0;
}

int HangarRoofCraneCameraHook(int* params)
{
	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	XwaPlayer* xwaPlayers = (XwaPlayer*)0x08B94E0;
	const int currentPlayerId = *(int*)0x08C1CC8;
	const int V0x068BCC0 = *(int*)0x068BCC0;

	xwaPlayers[currentPlayerId].CameraPositionX = xwaObjects[V0x068BCC0].PositionX;
	xwaPlayers[currentPlayerId].CameraPositionY = xwaObjects[V0x068BCC0].PositionY + 0x28E;
	xwaPlayers[currentPlayerId].CameraPositionZ = xwaObjects[V0x068BCC0].PositionZ - 0x08;

	return 0;
}

int HangarMapHook(int* params)
{
	g_hangarMapModelIndices.clear();

	ReadIsHangarFloorInverted();

	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const auto AddObject = (short(*)(unsigned short, int, int, int, unsigned short, unsigned short))0x00456AE0;

	const auto lines = GetCustomFileLines("HangarMap", false);

	if (lines.size())
	{
		const auto values = GetFileListValues(lines);

		for (unsigned int i = 0; i < values.size(); i++)
		{
			const auto& value = values[i];

			if (value.size() < 6)
			{
				continue;
			}

			unsigned short modelIndex;
			int markings;
			int positionX;
			int positionY;
			int positionZ;
			short orientationXY;
			short orientationZ;
			std::string objectProfile;
			bool isObjectFloorInverted;

			if (value.size() == 9)
			{
				modelIndex = static_cast<unsigned short>(std::stoi(value[0], 0, 0));
				markings = std::stoi(value[1], 0, 0);
				positionX = std::stoi(value[2], 0, 0);
				positionY = std::stoi(value[3], 0, 0);
				positionZ = std::stoi(value[4], 0, 0);
				orientationXY = static_cast<short>(std::stoi(value[5], 0, 0));
				orientationZ = static_cast<short>(std::stoi(value[6], 0, 0));
				objectProfile = value[7];
				isObjectFloorInverted = std::stoi(value[8], 0, 0) != 0;
			}
			else if (value.size() == 8)
			{
				modelIndex = static_cast<unsigned short>(std::stoi(value[0], 0, 0));
				markings = std::stoi(value[1], 0, 0);
				positionX = std::stoi(value[2], 0, 0);
				positionY = std::stoi(value[3], 0, 0);
				positionZ = std::stoi(value[4], 0, 0);
				orientationXY = static_cast<short>(std::stoi(value[5], 0, 0));
				orientationZ = static_cast<short>(std::stoi(value[6], 0, 0));
				objectProfile = value[7];
				isObjectFloorInverted = g_isHangarFloorInverted;
			}
			else if (value.size() == 7)
			{
				modelIndex = static_cast<unsigned short>(std::stoi(value[0], 0, 0));
				markings = std::stoi(value[1], 0, 0);
				positionX = std::stoi(value[2], 0, 0);
				positionY = std::stoi(value[3], 0, 0);
				positionZ = std::stoi(value[4], 0, 0);
				orientationXY = static_cast<short>(std::stoi(value[5], 0, 0));
				orientationZ = static_cast<short>(std::stoi(value[6], 0, 0));
				objectProfile = "Default";
				isObjectFloorInverted = g_isHangarFloorInverted;
			}
			else if (value.size() == 6)
			{
				modelIndex = static_cast<unsigned short>(std::stoi(value[0], 0, 0));
				markings = 0;
				positionX = std::stoi(value[1], 0, 0);
				positionY = std::stoi(value[2], 0, 0);
				positionZ = std::stoi(value[3], 0, 0);
				orientationXY = static_cast<short>(std::stoi(value[4], 0, 0));
				orientationZ = static_cast<short>(std::stoi(value[5], 0, 0));
				objectProfile = "Default";
				isObjectFloorInverted = g_isHangarFloorInverted;
			}
			else
			{
				continue;
			}

			bool isHangarFloorInverted = g_isHangarFloorInverted;
			g_isHangarFloorInverted = isObjectFloorInverted;

			short objectIndex = AddObject(modelIndex, positionX, positionY, positionZ, orientationXY, orientationZ);

			xwaObjects[objectIndex].pMobileObject->Markings = markings;
			ApplyObjectProfile(modelIndex, objectProfile, xwaObjects[objectIndex].pMobileObject->pCraft);

			switch (i)
			{
			case 0:
				*(int*)0x068BC34 = objectIndex;
				break;

			case 1:
				*(int*)0x068BCA8 = objectIndex;
				break;

			case 2:
				*(int*)0x068BBBC = objectIndex;
				break;

			case 3:
				*(int*)0x068BBB4 = objectIndex;
				*(int*)0x068BBB0 = *(int*)0x068BBB4;
				break;
			}

			g_isHangarFloorInverted = isHangarFloorInverted;
		}
	}
	else
	{
		*(int*)0x068BC34 = AddObject(314, -0x423, 0x521, 0x7FFFFFFF, 0x96C8, 0); // ModelIndex_314_1_36_HangarMonitor
		*(int*)0x068BCA8 = AddObject(309, 0x2D5, -0x1A0, 0x7FFFFFFF, 0x5BCC, 0); // ModelIndex_309_1_31_HangarCrane
		*(int*)0x068BBBC = AddObject(313, -0x548, 0x5A0, 0x7FFFFFFF, 0x218, 0); // ModelIndex_313_1_35_HangarGenerator
		*(int*)0x068BBB4 = AddObject(315, -0x429, -0x38, 0x7FFFFFFF, 0xBA78, 0); // ModelIndex_315_1_37_HangarWorkStand
		*(int*)0x068BBB0 = *(int*)0x068BBB4;

		AddObject(315, 0x81F, 0x2DD, 0x7FFFFFFF, 0x1900, 0); // ModelIndex_315_1_37_HangarWorkStand
		AddObject(315, -0x9B8, 0x4BF, 0x7FFFFFFF, 0x2E58, 0); // ModelIndex_315_1_37_HangarWorkStand
		AddObject(310, 0x31A, -0x2EE, -0x350, 0x1A90, 0x30D4); // ModelIndex_310_1_32_HangarCrate
		AddObject(310, -0x323, -0x5B8, 0x7FFFFFFF, 0xC030, 0); // ModelIndex_310_1_32_HangarCrate
		AddObject(310, -0x61B, 0x3B0, 0x7FFFFFFF, 0xC8EC, 0); // ModelIndex_310_1_32_HangarCrate
		AddObject(83, -0x1FA, -0x858, -0x2D2, 0x8044, 0x4060); // ModelIndex_083_0_65_ContainerBox
		AddObject(83, -0x349, -0x65C, 0x7FFFFFFF, 0xCF6C, 0); // ModelIndex_083_0_65_ContainerBox
		AddObject(83, 0x4D8, -0x832, -0x2D2, 0x110, 0x4060); // ModelIndex_083_0_65_ContainerBox
		AddObject(84, -0x342, -0x7F2, 0x7FFFFFFF, 0x7E50, 0); // ModelIndex_084_0_66_ContainerSphere
		AddObject(80, -0xD92, -0x113, 0x7FFFFFFF, 0x72FC, 0); // ModelIndex_080_0_62_CargoCanister
		AddObject(2, -0x40E, -0x31E, 0x7FFFFFFF, 0x1E14, 0); // ModelIndex_002_0_1_Ywing
		AddObject(50, 0x47A, -0x508, -0x2E5, 0xC568, 0); // ModelIndex_050_0_37_Shuttle
		AddObject(315, 0x334, -0x299, 0x7FFFFFFF, 0xBFF0, 0); // ModelIndex_315_1_37_HangarWorkStand
		AddObject(310, -0x369, -0x5B0, -0x351, 0xEC38, 0x4FB0); // ModelIndex_310_1_32_HangarCrate
		AddObject(310, -0xC17, -0x250, 0x7FFFFFFF, 0xC8EC, 0); // ModelIndex_310_1_32_HangarCrate
		AddObject(83, -0x6D7, -0x7CC, 0x7FFFFFFF, 0x79C4, 0); // ModelIndex_083_0_65_ContainerBox
		AddObject(83, 0x3D3, -0x837, 0x7FFFFFFF, 0x90AC, 0); // ModelIndex_083_0_65_ContainerBox
		AddObject(84, -0x55E, -0x814, 0x7FFFFFFF, 0x9E58, 0); // ModelIndex_084_0_66_ContainerSphere
		AddObject(80, 0xAF6, 0x54D, 0x7FFFFFFF, 0x5038, 0); // ModelIndex_080_0_62_CargoCanister
		AddObject(80, -0xC3, -0x776, 0x7FFFFFFF, 0xD464, 0); // ModelIndex_080_0_62_CargoCanister
		AddObject(3, 0x882, 0x1FA, -0x33E, 0x9C64, 0x5DC); // ModelIndex_003_0_2_Awing
		AddObject(2, 0x44C, -0x1F7, 0x7FFFFFFF, 0xBF8C, 0); // ModelIndex_002_0_1_Ywing
		AddObject(1, -0x3EB, 0x118, -0x339, 0x40D8, 0); // ModelIndex_001_0_0_Xwing
	}

	return 0;
}

int FamHangarMapHook(int* params)
{
	g_hangarMapModelIndices.clear();

	ReadIsHangarFloorInverted();

	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const auto AddObject = (short(*)(unsigned short, int, int, int, unsigned short, unsigned short))0x00456AE0;

	const auto lines = GetCustomFileLines("FamHangarMap", false);

	if (lines.size())
	{
		const auto values = GetFileListValues(lines);

		for (unsigned int i = 0; i < values.size(); i++)
		{
			const auto& value = values[i];

			if (value.size() < 6)
			{
				continue;
			}

			unsigned short modelIndex;
			int markings;
			int positionX;
			int positionY;
			int positionZ;
			short orientationXY;
			short orientationZ;
			std::string objectProfile;
			bool isObjectFloorInverted;

			if (value.size() == 9)
			{
				modelIndex = static_cast<unsigned short>(std::stoi(value[0], 0, 0));
				markings = std::stoi(value[1], 0, 0);
				positionX = std::stoi(value[2], 0, 0);
				positionY = std::stoi(value[3], 0, 0);
				positionZ = std::stoi(value[4], 0, 0);
				orientationXY = static_cast<short>(std::stoi(value[5], 0, 0));
				orientationZ = static_cast<short>(std::stoi(value[6], 0, 0));
				objectProfile = value[7];
				isObjectFloorInverted = std::stoi(value[8], 0, 0) != 0;
			}
			else if (value.size() == 8)
			{
				modelIndex = static_cast<unsigned short>(std::stoi(value[0], 0, 0));
				markings = std::stoi(value[1], 0, 0);
				positionX = std::stoi(value[2], 0, 0);
				positionY = std::stoi(value[3], 0, 0);
				positionZ = std::stoi(value[4], 0, 0);
				orientationXY = static_cast<short>(std::stoi(value[5], 0, 0));
				orientationZ = static_cast<short>(std::stoi(value[6], 0, 0));
				objectProfile = value[7];
				isObjectFloorInverted = g_isHangarFloorInverted;
			}
			else if (value.size() == 7)
			{
				modelIndex = static_cast<unsigned short>(std::stoi(value[0], 0, 0));
				markings = std::stoi(value[1], 0, 0);
				positionX = std::stoi(value[2], 0, 0);
				positionY = std::stoi(value[3], 0, 0);
				positionZ = std::stoi(value[4], 0, 0);
				orientationXY = static_cast<short>(std::stoi(value[5], 0, 0));
				orientationZ = static_cast<short>(std::stoi(value[6], 0, 0));
				objectProfile = "Default";
				isObjectFloorInverted = g_isHangarFloorInverted;
			}
			else if (value.size() == 6)
			{
				modelIndex = static_cast<unsigned short>(std::stoi(value[0], 0, 0));
				markings = 0;
				positionX = std::stoi(value[1], 0, 0);
				positionY = std::stoi(value[2], 0, 0);
				positionZ = std::stoi(value[3], 0, 0);
				orientationXY = static_cast<short>(std::stoi(value[4], 0, 0));
				orientationZ = static_cast<short>(std::stoi(value[5], 0, 0));
				objectProfile = "Default";
				isObjectFloorInverted = g_isHangarFloorInverted;
			}
			else
			{
				continue;
			}

			bool isHangarFloorInverted = g_isHangarFloorInverted;
			g_isHangarFloorInverted = isObjectFloorInverted;

			short objectIndex = AddObject(modelIndex, positionX, positionY, positionZ, orientationXY, orientationZ);

			xwaObjects[objectIndex].pMobileObject->Markings = markings;
			ApplyObjectProfile(modelIndex, objectProfile, xwaObjects[objectIndex].pMobileObject->pCraft);

			switch (i)
			{
			case 0:
				*(int*)0x068BBB4 = objectIndex;
				*(int*)0x068BBB0 = *(int*)0x068BBB4;
				break;

			case 1:
				*(int*)0x068BC34 = objectIndex;
				break;

			case 2:
				*(int*)0x068BCA8 = objectIndex;
				break;

			case 3:
				*(int*)0x068BBBC = objectIndex;
				break;
			}

			g_isHangarFloorInverted = isHangarFloorInverted;
		}
	}
	else
	{
		*(int*)0x068BBB4 = AddObject(315, 0x335, -0x19DA, 0x7FFFFFFF, 0x7788, 0); // ModelIndex_315_1_37_HangarWorkStand
		*(int*)0x068BBB0 = *(int*)0x068BBB4;
		*(int*)0x068BC34 = AddObject(314, 0x97F, -0x1547, 0x7FFFFFFF, 0xF060, 0); // ModelIndex_314_1_36_HangarMonitor
		*(int*)0x068BCA8 = AddObject(309, 0x272, -0x18FD, 0x7FFFFFFF, 0x2E0, 0); // ModelIndex_309_1_31_HangarCrane
		*(int*)0x068BBBC = AddObject(313, 0x629, -0x1A07, 0x7FFFFFFF, 0x1F4, 0); // ModelIndex_313_1_35_HangarGenerator

		AddObject(80, -0x7A2, -0x1D15, 0x7FFFFFFF, 0x7A04, 0); // ModelIndex_080_0_62_CargoCanister
		AddObject(80, -0x792, -0x1D15, -0x1385, 0x7A04, 0); // ModelIndex_080_0_62_CargoCanister
		AddObject(83, -0x4CF, -0x2160, -0x138B, 0x7FE0, 0x4000); // ModelIndex_083_0_65_ContainerBox
		AddObject(83, -0x5C9, -0x20FC, -0x138B, 0xB2A8, 0x4000); // ModelIndex_083_0_65_ContainerBox
		AddObject(83, -0x71D, -0x2110, -0x138B, 0x7FE0, 0x4000); // ModelIndex_083_0_65_ContainerBox
		AddObject(84, -0x72A, -0x180A, 0x7FFFFFFF, 0xA6F0, 0); // ModelIndex_084_0_66_ContainerSphere
		AddObject(84, 0x793, -0x1F4D, 0x7FFFFFFF, 0x7C38, 0); // ModelIndex_084_0_66_ContainerSphere
		AddObject(80, -0x7A2, -0x1D15, -0x1318, 0x7A04, 0); // ModelIndex_080_0_62_CargoCanister
		AddObject(80, -0x7BA, -0x1CDA, -0x12AD, 0x7BF8, 0); // ModelIndex_080_0_62_CargoCanister
		AddObject(310, 0x5CC, -0x2093, -0x140D, 0x7404, 0x4FB0); // ModelIndex_310_1_32_HangarCrate
		AddObject(310, 0x568, -0x2093, 0x7FFFFFFF, 0x7D24, 0); // ModelIndex_310_1_32_HangarCrate
		AddObject(83, -0x80D, -0x2098, -0x138B, 0x7FE0, 0x4000); // ModelIndex_083_0_65_ContainerBox
		AddObject(83, -0x5E7, -0x1A4E, 0x7FFFFFFF, 0x6C58, 0x8000); // ModelIndex_083_0_65_ContainerBox
	}

	return 0;
}

int SelectHangarTypeHook(int* params)
{
	// apply to family mission type

	const auto& mode = g_config.SelectionMode;
	const auto missionDirectoryId = *(int*)0x0AE2A8A;
	const auto missionDescriptionId = ((int*)0x0AE2A8E)[missionDirectoryId];

	int hangarType;

	if (mode == "ForceHangar")
	{
		hangarType = 0;
	}
	else if (mode == "BasedOnModelIndex")
	{
		hangarType = 1;
	}
	else // BasedOnModelIndexAzzameenCampaign
	{
		hangarType = missionDescriptionId < 7 ? 1 : 0;
	}

	return hangarType;
}

int SelectHangarInsideAnimation(int* params)
{
	const unsigned short modelIndex = (unsigned short)params[0];

	const auto& mode = g_config.SelectionMode;

	bool inside;

	if (mode == "ForceHangar")
	{
		inside = true;
	}
	else // BasedOnModelIndex or BasedOnModelIndexAzzameenCampaign
	{
		// 179 = FamilyBase
		inside = modelIndex == 179 ? false : true;
	}

	return inside ? 0 : 1;
}

int SelectHangarModelIndex(int* params)
{
	const unsigned short modelIndex = (unsigned short)params[0];

	const auto& mode = g_config.SelectionMode;

	bool isFamilyBase;

	if (mode == "ForceHangar")
	{
		isFamilyBase = false;
	}
	else // BasedOnModelIndex or BasedOnModelIndexAzzameenCampaign
	{
		// 179 = FamilyBase
		isFamilyBase = modelIndex == 179 ? true : false;
	}

	return isFamilyBase ? 1 : 0;
}

int CraftElevationHook(int* params)
{
	const unsigned short modelIndex = (unsigned short)params[0];

	return g_modelIndexHangar.GetClosedSFoilsElevation(modelIndex);
}

int CraftSelectionMissionHook(int* params)
{
	unsigned short* craftSelection = (unsigned short*)0x09C6960;
	int& craftSelectionCount = params[0];
	const unsigned short playerModelIndex = (unsigned short)params[1];

	bool isTransport = false;
	for (const auto modelIndex : g_craftSelectionValues._transportsSelection)
	{
		if (playerModelIndex == modelIndex)
		{
			isTransport = true;
			break;
		}
	}

	if (isTransport)
	{
		for (const auto modelIndex : g_craftSelectionValues._transportsSelection)
		{
			if (modelIndex != playerModelIndex)
			{
				craftSelection[craftSelectionCount] = modelIndex;
				craftSelectionCount++;
			}
		}
	}
	else
	{
		for (const auto modelIndex : g_craftSelectionValues._fightersSelection)
		{
			if (modelIndex != playerModelIndex)
			{
				craftSelection[craftSelectionCount] = modelIndex;
				craftSelectionCount++;
			}
		}
	}

	return 0;
}

int CraftSelectionMeleeHook(int* params)
{
	unsigned short* craftSelection = (unsigned short*)0x09C6960;
	int& craftSelectionCount = params[0];
	const unsigned short playerModelIndex = (unsigned short)params[1];
	const unsigned char type = (unsigned char)params[2];

	switch (type)
	{
	case 1:
		for (const auto modelIndex : g_craftSelectionValues._craftsSelection)
		{
			if (modelIndex != playerModelIndex)
			{
				craftSelection[craftSelectionCount] = modelIndex;
				craftSelectionCount++;
			}
		}

		break;

	case 2:
		for (const auto modelIndex : g_craftSelectionValues._fightersSelection)
		{
			if (modelIndex != playerModelIndex)
			{
				craftSelection[craftSelectionCount] = modelIndex;
				craftSelectionCount++;
			}
		}

		break;

	case 3:
		for (const auto modelIndex : g_craftSelectionValues._transportsSelection)
		{
			if (modelIndex != playerModelIndex)
			{
				craftSelection[craftSelectionCount] = modelIndex;
				craftSelectionCount++;
			}
		}

		break;
	}

	return 0;
}

int HangarLaunchAnimation1Hook(int* params)
{
	const int esp1C = params[7];
	const unsigned short s_XwaHangarModelIndex = *(unsigned short*)0x009C6754;
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const int hangarPlayerObjectIndex = *(int*)0x068BC08;
	int& positionZ = xwaObjects[hangarPlayerObjectIndex].PositionZ;
	const PlayerCraft player = g_hangarObjects.GetPlayerCraft(xwaObjects[hangarPlayerObjectIndex].ModelIndex);

	if (!g_isPlayerFloorInverted)
	{
		positionZ += esp1C;
	}
	else
	{
		positionZ -= esp1C;
	}

	return 0;
}

int HangarLaunchAnimation2Hook(int* params)
{
	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const int hangarPlayerObjectIndex = *(int*)0x068BC08;
	const int positionZ = xwaObjects[hangarPlayerObjectIndex].PositionZ;
	const int hangarFloorPositionZ = *(int*)0x068BC38;

	const int elevation = 0xF7 + g_hangarObjects.GetPlayerAnimationElevation();
	const int elevationInverted = 0xF7 + g_hangarObjects.GetPlayerAnimationInvertedElevation();
	const PlayerCraft player = g_hangarObjects.GetPlayerCraft(xwaObjects[hangarPlayerObjectIndex].ModelIndex);

	int ret;

	if (!g_isPlayerFloorInverted)
	{
		ret = positionZ > hangarFloorPositionZ + player.OffsetZ + elevation ? 1 : 0;
	}
	else
	{
		ret = positionZ < hangarFloorPositionZ + g_hangarFloorInvertedHeight + player.OffsetZ - elevationInverted ? 1 : 0;
	}

	return ret;
}

int HangarLaunchAnimation3Hook(int* params)
{
	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const int hangarObjectIndex = *(int*)0x068BCC4;
	const unsigned short hangarModelIndex = *(unsigned short*)0x09C6754;

	const int straightLine = g_hangarObjects.GetPlayerAnimationStraightLine();

	int ret;

	if (hangarModelIndex == 0x134)
	{
		ret = 0x9C4;
	}
	else
	{
		ret = -0x384;
	}

	ret += straightLine;

	return ret;
}

int HangarObjectsElevationHook(int* params)
{
	const int objectIndex = params[0];
	//const int objectElevation = params[1];

	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	int& positionZ = xwaObjects[objectIndex].PositionZ;
	const int hangarFloorPositionZ = *(int*)0x068BC38;

	if (!g_isHangarFloorInverted)
	{
		int objectElevation = g_modelIndexHangar.GetClosedSFoilsElevation(xwaObjects[objectIndex].ModelIndex);
		positionZ = hangarFloorPositionZ + objectElevation;
	}
	else
	{
		int objectElevation = g_modelIndexHangar.GetClosedSFoilsElevationInverted(xwaObjects[objectIndex].ModelIndex);
		positionZ = hangarFloorPositionZ + g_hangarFloorInvertedHeight - objectElevation;
	}

	return 0;
}

int HangarFloorElevationHook(int* params)
{
	ReadIsHangarFloorInverted();

	//const int elevation = params[0];

	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const int hangarPlayerObjectIndex = *(int*)0x068BC08;
	const int positionZ = xwaObjects[hangarPlayerObjectIndex].PositionZ;
	int& hangarFloorPositionZ = *(int*)0x068BC38;

	const PlayerCraft player = g_hangarObjects.GetPlayerCraft(xwaObjects[hangarPlayerObjectIndex].ModelIndex);

	int elevation;
	if (!g_isPlayerFloorInverted)
	{
		elevation = g_modelIndexHangar.GetClosedSFoilsElevation(xwaObjects[hangarPlayerObjectIndex].ModelIndex);
	}
	else
	{
		elevation = g_modelIndexHangar.GetClosedSFoilsElevationInverted(xwaObjects[hangarPlayerObjectIndex].ModelIndex);
	}

	if (!g_isPlayerFloorInverted)
	{
		hangarFloorPositionZ = positionZ - player.OffsetZ - elevation;
	}
	else
	{
		hangarFloorPositionZ = positionZ - g_hangarFloorInvertedHeight - player.OffsetZ + elevation;
	}

	return 0;
}

int HangarPositionXYHook(int* params)
{
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const int hangarObjectIndex = *(int*)0x068BCC4;
	const int hangarPlayerObjectIndex = *(int*)0x068BC08;
	const PlayerCraft player = g_hangarObjects.GetPlayerCraft(xwaObjects[hangarPlayerObjectIndex].ModelIndex);

	xwaObjects[hangarObjectIndex].PositionX -= player.OffsetX;
	xwaObjects[hangarObjectIndex].PositionY -= player.OffsetY;

	return *(unsigned short*)0x09C6754;
}

int HangarPlayerCraftElevationHook(int* params)
{
	ReadIsHangarFloorInverted();

	//const int elevation = params[0];

	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const XwaPlayer* xwaPlayers = (XwaPlayer*)0x08B94E0;
	const int currentPlayerId = *(int*)0x08C1CC8;
	const int hangarFloorPositionZ = *(int*)0x068BC38;

	const PlayerCraft player = g_hangarObjects.GetPlayerCraft(xwaObjects[xwaPlayers[currentPlayerId].ObjectIndex].ModelIndex);

	if (!g_isPlayerFloorInverted)
	{
		int elevation = g_modelIndexHangar.GetClosedSFoilsElevation(xwaObjects[xwaPlayers[currentPlayerId].ObjectIndex].ModelIndex);
		xwaObjects[xwaPlayers[currentPlayerId].ObjectIndex].PositionZ = hangarFloorPositionZ + player.OffsetZ + elevation;
	}
	else
	{
		int elevation = g_modelIndexHangar.GetClosedSFoilsElevationInverted(xwaObjects[xwaPlayers[currentPlayerId].ObjectIndex].ModelIndex);
		xwaObjects[xwaPlayers[currentPlayerId].ObjectIndex].PositionZ = hangarFloorPositionZ + g_hangarFloorInvertedHeight + player.OffsetZ - elevation;
	}

	return 0;
}

int g_hangarPlayerPositionY;

int HangarReenterInitPositionZHook(int* params)
{
	ReadIsHangarFloorInverted();

	const int hangarFloorPositionZ = *(int*)0x068BC38;

	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const int hangarPlayerObjectIndex = *(int*)0x068BC08;

	const int elevation = 0x223 + g_hangarObjects.GetPlayerAnimationElevation();
	const int elevationInverted = 0x223 + g_hangarObjects.GetPlayerAnimationInvertedElevation();
	const int straightLine = g_hangarObjects.GetPlayerAnimationStraightLine();
	const PlayerCraft player = g_hangarObjects.GetPlayerCraft(xwaObjects[hangarPlayerObjectIndex].ModelIndex);

	xwaObjects[hangarPlayerObjectIndex].PositionX += player.OffsetX;
	xwaObjects[hangarPlayerObjectIndex].PositionY += player.OffsetY + straightLine;

	g_hangarPlayerPositionY = xwaObjects[hangarPlayerObjectIndex].PositionY;

	int positionZ;

	if (!g_isPlayerFloorInverted)
	{
		positionZ = hangarFloorPositionZ + player.OffsetZ + elevation;
	}
	else
	{
		positionZ = hangarFloorPositionZ + g_hangarFloorInvertedHeight + player.OffsetZ - elevationInverted;
	}

	return positionZ;
}

int HangarReenterAnimation51Hook(int* params)
{
	const int esp1C = params[7];

	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const int hangarPlayerObjectIndex = *(int*)0x068BC08;
	const int positionY = xwaObjects[hangarPlayerObjectIndex].PositionY;
	const int positionZ = xwaObjects[hangarPlayerObjectIndex].PositionZ;
	const int hangarFloorPositionZ = *(int*)0x068BC38;
	const unsigned short hangarModelIndex = *(unsigned short*)0x09C6754;
	float& s_V0x068BCA4 = *(float*)0x068BCA4;

	const int elevation = 0x93 + g_hangarObjects.GetPlayerAnimationElevation();
	const int elevationInverted = 0x93 + g_hangarObjects.GetPlayerAnimationInvertedElevation();
	const int straightLine = g_hangarObjects.GetPlayerAnimationStraightLine();
	const PlayerCraft player = g_hangarObjects.GetPlayerCraft(xwaObjects[hangarPlayerObjectIndex].ModelIndex);

	if (positionY > g_hangarPlayerPositionY - player.OffsetY - straightLine)
	{
		if (hangarModelIndex == 308)
		{
			s_V0x068BCA4 += esp1C * 0.16f;
		}
		else
		{
			s_V0x068BCA4 += esp1C * 0.085f;
		}

		return 0;
	}

	int ret;

	if (!g_isPlayerFloorInverted)
	{
		ret = positionZ > hangarFloorPositionZ + player.OffsetZ + elevation ? 1 : 0;
	}
	else
	{
		ret = positionZ < hangarFloorPositionZ + g_hangarFloorInvertedHeight + player.OffsetZ - elevationInverted ? 1 : 0;
	}

	return ret;
}

int HangarReenterAnimation52Hook(int* params)
{
	//const int elevation = params[0];

	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const int hangarPlayerObjectIndex = *(int*)0x068BC08;
	const int positionZ = xwaObjects[hangarPlayerObjectIndex].PositionZ;
	const int hangarFloorPositionZ = *(int*)0x068BC38;

	const PlayerCraft player = g_hangarObjects.GetPlayerCraft(xwaObjects[hangarPlayerObjectIndex].ModelIndex);

	int ret;

	if (!g_isPlayerFloorInverted)
	{
		int elevation = g_modelIndexHangar.GetClosedSFoilsElevation(xwaObjects[hangarPlayerObjectIndex].ModelIndex);
		ret = positionZ > hangarFloorPositionZ + player.OffsetZ + elevation ? 1 : 0;
	}
	else
	{
		int elevation = g_modelIndexHangar.GetClosedSFoilsElevationInverted(xwaObjects[hangarPlayerObjectIndex].ModelIndex);
		ret = positionZ < hangarFloorPositionZ + g_hangarFloorInvertedHeight + player.OffsetZ - elevation ? 1 : 0;
	}

	return ret;
}

int HangarReenterAnimation53Hook(int* params)
{
	const int delta = params[0];

	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const int hangarPlayerObjectIndex = *(int*)0x068BC08;
	int& positionZ = xwaObjects[hangarPlayerObjectIndex].PositionZ;

	if (!g_isPlayerFloorInverted)
	{
		positionZ -= delta;
	}
	else
	{
		positionZ += delta;
	}

	return 0;
}

int HangarReenterAnimation5YCheckHook(int* params)
{
	const unsigned short s_XwaHangarModelIndex = *(unsigned short*)0x009C6754;
	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const int hangarPlayerObjectIndex = *(int*)0x068BC08;
	const PlayerCraft player = g_hangarObjects.GetPlayerCraft(xwaObjects[hangarPlayerObjectIndex].ModelIndex);

	int deltaY;

	if (s_XwaHangarModelIndex == 308)
	{
		deltaY = -0x320;
	}
	else
	{
		deltaY = -0x1A90;
	}

	deltaY += player.OffsetY;

	return deltaY;
}

int HangarReenterAnimation6Hook(int* params)
{
	//const int elevation = params[0];
	const int esp1C = params[8];

	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const int hangarPlayerObjectIndex = *(int*)0x068BC08;
	int& positionZ = xwaObjects[hangarPlayerObjectIndex].PositionZ;
	const int hangarFloorPositionZ = *(int*)0x068BC38;

	const PlayerCraft player = g_hangarObjects.GetPlayerCraft(xwaObjects[hangarPlayerObjectIndex].ModelIndex);

	if (!g_isPlayerFloorInverted)
	{
		int elevation = g_modelIndexHangar.GetClosedSFoilsElevation(xwaObjects[hangarPlayerObjectIndex].ModelIndex);

		if (positionZ > hangarFloorPositionZ + player.OffsetZ + elevation)
		{
			positionZ -= esp1C;
		}
	}
	else
	{
		int elevation = g_modelIndexHangar.GetClosedSFoilsElevationInverted(xwaObjects[hangarPlayerObjectIndex].ModelIndex);

		if (positionZ < hangarFloorPositionZ + g_hangarFloorInvertedHeight + player.OffsetZ - elevation)
		{
			positionZ += esp1C;
		}
	}

	return 0;
}

int HangarReenterAnimation71Hook(int* params)
{
	const int esp1C = params[7];

	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const int hangarPlayerObjectIndex = *(int*)0x068BC08;
	int& positionZ = xwaObjects[hangarPlayerObjectIndex].PositionZ;

	if (!g_isPlayerFloorInverted)
	{
		positionZ -= esp1C;
	}
	else
	{
		positionZ += esp1C;
	}

	return 0;
}

int HangarReenterAnimation72Hook(int* params)
{
	//const int elevation = params[0];

	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const int hangarPlayerObjectIndex = *(int*)0x068BC08;
	const int positionZ = xwaObjects[hangarPlayerObjectIndex].PositionZ;
	const int hangarFloorPositionZ = *(int*)0x068BC38;

	const PlayerCraft player = g_hangarObjects.GetPlayerCraft(xwaObjects[hangarPlayerObjectIndex].ModelIndex);

	int ret;

	if (!g_isPlayerFloorInverted)
	{
		int elevation = g_modelIndexHangar.GetClosedSFoilsElevation(xwaObjects[hangarPlayerObjectIndex].ModelIndex);
		ret = positionZ <= hangarFloorPositionZ + player.OffsetZ + elevation ? 1 : 0;
	}
	else
	{
		int elevation = g_modelIndexHangar.GetClosedSFoilsElevationInverted(xwaObjects[hangarPlayerObjectIndex].ModelIndex);
		ret = positionZ >= hangarFloorPositionZ + g_hangarFloorInvertedHeight + player.OffsetZ - elevation ? 1 : 0;
	}

	return ret;
}

int HangarReenterAnimation73Hook(int* params)
{
	const int modelIndex = params[0];

	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const int hangarPlayerObjectIndex = *(int*)0x068BC08;
	const int hangarObjectIndex = *(int*)0x068BCC4;

	const PlayerCraft player = g_hangarObjects.GetPlayerCraft(xwaObjects[hangarPlayerObjectIndex].ModelIndex);

	xwaObjects[hangarPlayerObjectIndex].PositionX += player.OffsetX;

	//xwaObjects[hangarObjectIndex].PositionY += player.OffsetY;

	int deltaY = HangarReenterAnimation5YCheckHook(nullptr);
	xwaObjects[hangarPlayerObjectIndex].PositionY = xwaObjects[hangarObjectIndex].PositionY + deltaY;

	int elevation;

	if (!g_isPlayerFloorInverted)
	{
		elevation = g_modelIndexHangar.GetClosedSFoilsElevation(modelIndex);
	}
	else
	{
		elevation = g_hangarFloorInvertedHeight - g_modelIndexHangar.GetClosedSFoilsElevationInverted(modelIndex);
	}

	return player.OffsetZ + elevation;
}

int HangarShuttleLaunchReenterAnimations4And6Hook(int* params)
{
	const int A4 = params[0];

	const S0x09C6780* s_V0x068BBC8 = (S0x09C6780*)0x068BBC8;
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	XwaObject* object = &xwaObjects[s_V0x068BBC8->ObjectIndex];

	std::string value = g_hangarObjects.GetShuttleAnimation();

	object->m17 += A4 * 20;

	int step = (int)(sin(object->m13 * 9.587372E-05f) * s_V0x068BBC8->m22) * A4;

	if (_stricmp(value.c_str(), "Right") == 0)
	{
		object->m17 -= A4 * 20;
		object->PositionX += step;
	}
	else if (_stricmp(value.c_str(), "Top") == 0)
	{
		object->PositionZ += step;
	}
	else if (_stricmp(value.c_str(), "Bottom") == 0)
	{
		object->PositionZ -= step;
	}
	else
	{
		// Right
		object->m17 -= A4 * 20;
		object->PositionX += step;
	}

	return 0;
}

int HangarShuttleLaunchReenterAnimation3CheckHook(int* params)
{
	int& offset = params[0];

	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const int hangarObjectIndex = *(int*)0x068BCC4;
	int positionY = xwaObjects[hangarObjectIndex].PositionY;

	int value = g_hangarObjects.GetShuttleAnimationStraightLine();

	offset = positionY + value;

	return *(int*)0x07B33C4;
}

int HangarShuttleLaunchReenterAnimation1UpdateHook(int* params)
{
	const int A4 = params[0];

	const S0x09C6780* s_V0x068BBC8 = (S0x09C6780*)0x068BBC8;
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	XwaObject* object = &xwaObjects[s_V0x068BBC8->ObjectIndex];

	if (!g_isShuttleFloorInverted)
	{
		object->PositionZ += A4 * 2;
	}
	else
	{
		object->PositionZ -= A4 * 2;
	}

	return 0;
}

int HangarShuttleLaunchReenterAnimation1CheckHook(int* params)
{
	const S0x09C6780* s_V0x068BBC8 = (S0x09C6780*)0x068BBC8;
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	XwaObject* object = &xwaObjects[s_V0x068BBC8->ObjectIndex];

	int shuttlePositionZ = g_hangarObjects.GetShuttlePositionZ();
	const int shuttleAnimationElevation = g_hangarObjects.GetShuttleAnimationElevation();

	int positionZ = *(int*)0x068BC38;

	if (!g_isShuttleFloorInverted)
	{
		positionZ += 0x161 + shuttleAnimationElevation;
	}
	else
	{
		positionZ -= 0x161 + shuttleAnimationElevation;
	}

	positionZ += shuttlePositionZ;

	int ret;

	if (!g_isShuttleFloorInverted)
	{
		ret = object->PositionZ > positionZ ? 0 : 1;
	}
	else
	{
		ret = object->PositionZ < positionZ ? 0 : 1;
	}

	return ret;
}

int HangarShuttleLaunchReenterAnimation7And10CheckHook(int* params)
{
	const int A4 = params[0];
	int& offset = params[0];

	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	int positionY = g_hangarObjects.GetShuttlePositionY();

	offset = xwaObjects[A4].PositionY + positionY;

	return 0;
}

int HangarShuttleLaunchReenterAnimation7And10SetHook(int* params)
{
	const S0x09C6780* s_V0x068BBC8 = (S0x09C6780*)0x068BBC8;
	const int hangarObjectIndex = *(int*)0x068BCC4;
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;

	xwaObjects[s_V0x068BBC8->ObjectIndex].PositionX = xwaObjects[hangarObjectIndex].PositionX + g_hangarObjects.GetShuttlePositionX();
	xwaObjects[s_V0x068BBC8->ObjectIndex].PositionY = xwaObjects[hangarObjectIndex].PositionY + g_hangarObjects.GetShuttlePositionY();
	xwaObjects[s_V0x068BBC8->ObjectIndex].m17 = 0;

	return 0;
}

int HangarShuttleLaunchReenterAnimation9UpdateHook(int* params)
{
	const int A4 = params[0];

	const S0x09C6780* s_V0x068BBC8 = (S0x09C6780*)0x068BBC8;
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	XwaObject* object = &xwaObjects[s_V0x068BBC8->ObjectIndex];

	if (!g_isShuttleFloorInverted)
	{
		object->PositionZ -= A4 * 2;
	}
	else
	{
		object->PositionZ += A4 * 2;
	}

	return 0;
}

int HangarShuttleLaunchReenterAnimation9CheckHook(int* params)
{
	const S0x09C6780* s_V0x068BBC8 = (S0x09C6780*)0x068BBC8;
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	XwaObject* object = &xwaObjects[s_V0x068BBC8->ObjectIndex];

	int shuttlePositionZ = g_hangarObjects.GetShuttlePositionZ();

	int positionZ = *(int*)0x068BC38;

	if (!g_isShuttleFloorInverted)
	{
		int elevation = g_modelIndexHangar.GetClosedSFoilsElevation(object->ModelIndex);
		positionZ += elevation;
	}
	else
	{
		int elevation = g_modelIndexHangar.GetClosedSFoilsElevationInverted(object->ModelIndex);
		positionZ -= elevation;
	}

	positionZ += shuttlePositionZ;

	int ret;

	if (!g_isShuttleFloorInverted)
	{
		ret = object->PositionZ <= positionZ ? 0 : 1;
	}
	else
	{
		ret = object->PositionZ >= positionZ ? 0 : 1;
	}

	return ret;
}

int g_shuttleHeadingXY;

int HangarShuttleLaunchReenterAnimation2UpdateHook(int* params)
{
	const int A4 = params[10];

	const S0x09C6780* s_V0x068BBC8 = (S0x09C6780*)0x068BBC8;
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	XwaObject* object = &xwaObjects[s_V0x068BBC8->ObjectIndex];

	//object->m13 += A4 * 0x32;

	int headingXY = (unsigned short)object->m13;

	if (headingXY >= 0x8000)
	{
		headingXY += A4 * 0x32;
	}
	else
	{
		headingXY -= A4 * 0x32;
	}

	g_shuttleHeadingXY = headingXY;
	object->m13 = (unsigned short)headingXY;

	return 0;
}

int HangarShuttleLaunchReenterAnimation2CheckHook(int* params)
{
	const int A4 = params[10];

	const S0x09C6780* s_V0x068BBC8 = (S0x09C6780*)0x068BBC8;
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	XwaObject* object = &xwaObjects[s_V0x068BBC8->ObjectIndex];

	//int ret = (unsigned short)object->m13 < 0x4E20 ? 0 : 1;

	int ret = (g_shuttleHeadingXY <= 0 || g_shuttleHeadingXY >= 0x10000) ? 0 : 1;

	return ret;
}

int HangarShuttleLaunchReenterAnimation8UpdateHook(int* params)
{
	const int A4 = params[10];

	const S0x09C6780* s_V0x068BBC8 = (S0x09C6780*)0x068BBC8;
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	XwaObject* object = &xwaObjects[s_V0x068BBC8->ObjectIndex];

	//object->m13 += A4 * 0x32;

	int headingXY = (unsigned short)object->m13;
	int orientation = g_hangarObjects.GetShuttleOrientation();

	if (orientation == 0)
	{
		orientation = 0x10000;
	}

	if (orientation >= 0x8000)
	{
		headingXY += A4 * 0x32;
	}
	else
	{
		headingXY -= A4 * 0x32;
	}

	g_shuttleHeadingXY = headingXY;
	object->m13 = (unsigned short)headingXY;

	return 0;
}

int HangarShuttleLaunchReenterAnimation8CheckHook(int* params)
{
	const int A4 = params[10];

	const S0x09C6780* s_V0x068BBC8 = (S0x09C6780*)0x068BBC8;
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	XwaObject* object = &xwaObjects[s_V0x068BBC8->ObjectIndex];

	//int headingXY = (unsigned short)(object->m13 - A4 * 0x32) + A4 * 0x32;
	int orientation = g_hangarObjects.GetShuttleOrientation();

	if (orientation == 0)
	{
		orientation = 0x10000;
	}

	//int ret = headingXY >= orientation ? 0 : 1;

	int ret;

	if (orientation >= 0x8000)
	{
		ret = g_shuttleHeadingXY >= orientation ? 0 : 1;
	}
	else
	{
		ret = g_shuttleHeadingXY <= orientation ? 0 : 1;
	}

	return ret;
}

int HangarShuttleAnimationSound1Hook(int* params)
{
	// TODO
	return 0xD7;
}

int HangarShuttleAnimationSound3Hook(int* params)
{
	// TODO
	return 0x5A1;
}

int HangarShuttleAnimationSound7Hook(int* params)
{
	// TODO
	return 0x5A1;
}

int HangarShuttleAnimationSound9Hook(int* params)
{
	// TODO
	return 0xD7;
}

int HangarShuttleAnimationSound10Hook(int* params)
{
	// TODO
	return 0x5A1;
}

int HangarGetCraftIndexHook(int* params)
{
	const int modelIndex = params[0];
	const auto GetCraftIndex = (int(*)(int))0x004DCE30;

	int craftIndex = GetCraftIndex(modelIndex);

	char* esp18 = (char*)&params[7];

	if (craftIndex == 0xFFFF)
	{
		esp18[0] = 0;
		craftIndex = 0;
	}
	else
	{
		const char* yourCraftString = *(const char**)0x009C6DF4;
		const char* craftName = *(const char**)(0x005BB484 + craftIndex * 0x3DB);

		strcpy_s(esp18, 52, yourCraftString);
		strcat_s(esp18, 52, craftName);
	}

	return craftIndex;
}

int HangarDisableShadowHook(int* params)
{
	const int A4 = params[0];

	const auto L004836F0 = (void(*)(int))0x004836F0;

	int& s_XwaOptCurrentLodDistance = *(int*)0x007D4F8C;

	if (g_config.DrawShadows && g_hangarDrawShadows)
	{
		int lodDistance = s_XwaOptCurrentLodDistance;

		if (g_config.ShadowLod)
		{
			s_XwaOptCurrentLodDistance = 0x7fffffff;
		}

		L004836F0(A4);

		s_XwaOptCurrentLodDistance = lodDistance;
	}

	return 0;
}

int HangarFoldOutsideHook(int* params)
{
	const auto L004A3660 = (void(*)())0x004A3660;

	L004A3660();

	int distance = *(int*)0x07D4B60;

	if (distance >= 0x1800)
	{
		return 0;
	}

	const ExeEnableEntry* ExeEnableTable = (ExeEnableEntry*)0x005FB240;
	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;

	for (int objectIndex = *(int*)0x08BF378; objectIndex < *(int*)0x07CA3B8; objectIndex++)
	{
		if (xwaObjects[objectIndex].ModelIndex == 0)
		{
			continue;
		}

		if (ExeEnableTable[xwaObjects[objectIndex].ModelIndex].CraftIndex == -1)
		{
			continue;
		}

		if (xwaObjects[objectIndex].TieFlightGroupIndex != *(int*)(0x07CA1A0 + 0x1C))
		{
			continue;
		}

		XwaCraft* craft = xwaObjects[objectIndex].pMobileObject->pCraft;

		if (craft->SFoilsState != 0x02)
		{
			craft->SFoilsState = 0x03;
		}
	}

	return 0;
}

int HangarFoldInsideHook(int* params)
{
	const ExeEnableEntry* ExeEnableTable = (ExeEnableEntry*)0x005FB240;
	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;

	for (int objectIndex = *(int*)0x08BF378; objectIndex < *(int*)0x07CA3B8; objectIndex++)
	{
		if (xwaObjects[objectIndex].ModelIndex == 0)
		{
			continue;
		}

		if (ExeEnableTable[xwaObjects[objectIndex].ModelIndex].CraftIndex == -1)
		{
			continue;
		}

		if (xwaObjects[objectIndex].TieFlightGroupIndex != *(int*)(0x07CA1A0 + 0x1C))
		{
			continue;
		}

		XwaCraft* craft = xwaObjects[objectIndex].pMobileObject->pCraft;

		if (craft->SFoilsState != 0x02)
		{
			craft->SFoilsState = 0x03;
		}
	}

	return 0;
}

int HangarAmbientSoundHook(int* params)
{
	const int selectedSoundIndex = params[Params_EDX];
	int hangarShipModelIndex = GetCommandShipModelIndex();
	const auto& soundConfig = GetSoundsConfig();

	if (soundConfig.SoundsCountHookExists && soundConfig.SfxHangarAmbientCount)
	{
		if (hangarShipModelIndex != -1 && hangarShipModelIndex < soundConfig.SfxHangarAmbientCount / 10)
		{
			int soundIndex = soundConfig.SfxHangarAmbientIndex + hangarShipModelIndex * 10 + selectedSoundIndex;
			params[Params_EDX] = soundIndex;

			return 0;
		}
	}

	params[Params_EDX] = 0x97 + selectedSoundIndex;

	return 0;
}

int HangarLightHook(int* params)
{
	const int positionX = params[0];
	const int positionY = params[1];
	const int positionZ = params[2];

	const auto XwaAddGlobalLight = (void(*)(int, int, int, float, float, float, float))0x00438F10;

	const unsigned short hangarModelIndex = *(unsigned short*)0x09C6754;

	unsigned int colorIntensity = g_hangarObjects.GetLightColorIntensity();
	float colorI = (colorIntensity & 0xFFU) / 255.0f;

	unsigned int colorRgb = g_hangarObjects.GetLightColorRgb();
	float colorR = ((colorRgb >> 16) & 0xFFU) / 255.0f;
	float colorG = ((colorRgb >> 8) & 0xFFU) / 255.0f;
	float colorB = (colorRgb & 0xFFU) / 255.0f;

	XwaAddGlobalLight(positionX, positionY, positionZ, colorI, colorR, colorG, colorB);

	return 0;
}

std::vector<unsigned short> GetHangarObjects()
{
	std::vector<unsigned short> objects;

	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const int hangarObjectIndex = *(int*)0x068BCC4;
	const unsigned char hangarTieFlightGroupIndex = xwaObjects[hangarObjectIndex].TieFlightGroupIndex;

	for (int eax = 0; eax < *(int*)0x07FFD80; eax++)
	{
		XwaObject* object = &xwaObjects[eax];

		if (object->ModelIndex == 0)
			continue;

		if (object->TieFlightGroupIndex != hangarTieFlightGroupIndex)
			continue;

		// Hangar or FamilyBase
		if (object->ModelIndex == 0x134 || object->ModelIndex == 0xB3)
			continue;

		objects.push_back(object->ModelIndex);
	}

	return objects;
}

int HangarExitHook(int* params)
{
	*(int*)0x0068BBB8 = 1;

	g_hangarMapModelIndices.clear();

	const auto HangarOptLoad = (int(*)(unsigned short))0x00456FA0;
	const auto OptUnload = (void(*)(unsigned short))0x004CCA60;
	const auto CockpitOptReadInfos = (void(*)())0x004314B0;
	const auto ExteriorOptReadInfos = (void(*)())0x00431960;

	unsigned short* OptModelFileMemHandles = (unsigned short*)0x007CA6E0;
	const int OptModelFileMemHandlesCount = 557;
	ExeEnableEntry* ExeEnableTable = (ExeEnableEntry*)0x005FB240;

	bool modelIndexes[OptModelFileMemHandlesCount];
	memset(modelIndexes, 0, sizeof(modelIndexes));

	std::vector<unsigned short> hangarObjects = GetHangarObjects();

	for (unsigned short modelIndex : hangarObjects)
	{
		modelIndexes[modelIndex] = true;
	}

	for (unsigned short modelIndex = 0; modelIndex < OptModelFileMemHandlesCount; modelIndex++)
	{
		if (!modelIndexes[modelIndex] || OptModelFileMemHandles[modelIndex] == 0)
		{
			continue;
		}

		OptUnload(OptModelFileMemHandles[modelIndex]);

		OptModelFileMemHandles[modelIndex] = 0;
		ExeEnableTable[modelIndex].pData1 = nullptr;
		ExeEnableTable[modelIndex].pData2 = nullptr;

		if ((ExeEnableTable[modelIndex].m01 & 0x18) == 0)
		{
			continue;
		}

		HangarOptLoad(modelIndex);
	}

	CockpitOptReadInfos();
	ExteriorOptReadInfos();

	return 0;
}
