#include "targetver.h"
#include "hangar.h"
#include "config.h"
#include <fstream>

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

struct XwaPlayer
{
	int ObjectIndex;
	char m004[79];
	short m053;
	char m055[2938];
};

static_assert(sizeof(XwaPlayer) == 3023, "size of XwaPlayer must be 3023");

struct ExeEnableEntry
{
	char unk00[8];
	void* pData1;
	void* pData2;
	char unk10[8];
};

static_assert(sizeof(ExeEnableEntry) == 24, "size of ExeEnableEntry must be 24");

struct XwaMobileObject
{
	char Unk0000[149];
	unsigned short ModelIndex;
	char Unk0097[2];
	unsigned char Markings;
	char Unk009A[75];
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
	char unk06C[86];
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
	int m14;
	int m18;
	int m1C;
	short m20;
	float m22;
	int m26;
	int m2A;
};

static_assert(sizeof(S0x09C6780) == 46, "size of S0x09C6780 must be 46");

#pragma pack(pop)

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

std::string GetCommandShipLstLine()
{
	const unsigned short* exeSpecies = (unsigned short*)0x05B0F70;
	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const TieFlightGroupEx* xwaTieFlightGroups = (TieFlightGroupEx*)0x080DC80;
	const XwaPlayer* xwaPlayers = (XwaPlayer*)0x08B94E0;
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

	if (mothershipObjectIndex == 0)
	{
		const int playerFlightGroupIndex = xwaObjects[playerObjectIndex].TieFlightGroupIndex;
		const int commandShipFlightGroupIndex = xwaTieFlightGroups[playerFlightGroupIndex].m0C4;
		const int commandShipEnabled = xwaTieFlightGroups[playerFlightGroupIndex].m0C5;

		if (commandShipEnabled)
		{
			const int commandShipCraftId = xwaTieFlightGroups[commandShipFlightGroupIndex].CraftId;
			const int commandShipModelIndex = exeSpecies[commandShipCraftId];

			std::string lstLine = g_flightModelsList.GetLstLine(commandShipModelIndex);
			return lstLine;
		}

		return std::string();
	}

	const int mothershipModelIndex = xwaObjects[mothershipObjectIndex].ModelIndex;

	std::string lstLine = g_flightModelsList.GetLstLine(mothershipModelIndex);
	return lstLine;
}

std::string GetCustomFilePath(const std::string& name)
{
	const char* xwaMissionFileName = (const char*)0x06002E8;

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
		ship.append(name);

		if (std::ifstream(ship))
		{
			return ship;
		}
	}

	std::string path = "FlightModels\\";
	path.append(name);

	return path;
}

std::vector<std::string> GetCustomFileLines(const std::string& name)
{
	const char* xwaMissionFileName = (const char*)0x06002E8;
	std::vector<std::string> lines;

	const std::string mission = GetStringWithoutExtension(xwaMissionFileName);
	lines = GetFileLines(mission + "_" + name + ".txt");

	if (!lines.size())
	{
		lines = GetFileLines(mission + ".ini", name);
	}

	if (lines.size())
	{
		return lines;
	}

	const std::string ship = GetCommandShipLstLine();

	if (!ship.empty())
	{
		lines = GetFileLines(ship + name + ".txt");

		if (!lines.size())
		{
			lines = GetFileLines(ship + ".ini", name);
		}

		if (lines.size())
		{
			return lines;
		}
	}

	const std::string path = "FlightModels\\";
	lines = GetFileLines(path + name + ".txt");

	return lines;
}

int HangarOptLoadHook(int* params)
{
	const char* argOpt = (char*)params[0];
	const unsigned short argModelIndex = (unsigned short)params[0x60];

	const auto OptLoad = (int(*)(const char*))0x004CC940;

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

	const auto lines = GetCustomFileLines("HangarObjects");
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

	if (hangarModelIndex == 0x134)
	{
		int HangarOptReloadHookParams[] = { 0x134 };
		HangarOptReloadHook(HangarOptReloadHookParams);
	}

	HangarReloadHookReleaseObjects();

	SetShipCategoryObjectsRanges(xwaObjects[hangarObjectIndex].Region);

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
		s_V0x0686D38->ObjectIndex = AddObject(0x13C, -0x578, 0x312, -0x11A, 0, 0); // ModelIndex_316_1_38_HangarRoofCrane
		s_V0x0686D38->m04 = 0;
		s_V0x0686D38->m26 = XwaRandom() / 2;
		s_V0x0686D38->m22 = 0;
		s_V0x0686D38->m08 = 0;
	}
	else
	{
		FamHangarMapHook(nullptr);

		V0x068BC10 = 0;

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

int HangarCameraPositionHook(int* params)
{
	const short A4 = (short)params[8];
	int& positionX = params[0];
	int& positionY = params[1];
	int& positionZ = params[2];

	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const unsigned short hangarModelIndex = *(unsigned short*)0x09C6754;
	const int playerObjectIndex = *(int*)0x068BC08;
	const int V0x0686B94 = *(int*)0x0686B94;

	const auto ModelGetSizeX = (int(*)(unsigned int))0x04857A0;
	const auto ModelGetSizeY = (int(*)(unsigned int))0x04857E0;
	const auto ModelGetSizeZ = (int(*)(unsigned int))0x0485820;

	if (hangarModelIndex == 0x134)
	{
		// hangar
		// Key 0 is the cockpit view.
		// Keys 1, 2, 3, 6, 9 are fixed views.
		// Keys 4 and 5 are related to the hangar droids.
		// Key 7 is either the same as key 3, either related to the player model.
		// Key 8 is related to the hangar roof crane.

		const auto cameraLines = GetCustomFileLines("HangarCamera");

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

			const auto lines = GetCustomFileLines("HangarObjects");
			int value = GetFileKeyValueInt(lines, "LoadDroids", 1);

			if (value == 1)
			{
				V0x068BCC0 = V0x09C6780[0].ObjectIndex;

				positionX = xwaObjects[V0x068BCC0].PositionX;
				positionY = xwaObjects[V0x068BCC0].PositionY;
				positionZ = xwaObjects[V0x068BCC0].PositionZ + 0x1E;
			}

			break;
		}

		case 5:
		{
			const S0x09C6780* V0x09C6780 = (S0x09C6780*)0x09C6780;
			int& V0x068BCC0 = *(int*)0x068BCC0;

			const auto lines = GetCustomFileLines("HangarObjects");
			int value = GetFileKeyValueInt(lines, "LoadDroids", 1);

			if (value == 1)
			{
				V0x068BCC0 = V0x09C6780[1].ObjectIndex;

				positionX = xwaObjects[V0x068BCC0].PositionX;
				positionY = xwaObjects[V0x068BCC0].PositionY;
				positionZ = xwaObjects[V0x068BCC0].PositionZ + 0x1E;
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
					positionX = playerPositionX + GetFileKeyValueInt(optCameraLines, "X");
					positionY = playerPositionY + GetFileKeyValueInt(optCameraLines, "Y");
					positionZ = playerPositionZ + GetFileKeyValueInt(optCameraLines, "Z");
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

		const auto cameraLines = GetCustomFileLines("FamHangarCamera");

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
					int x = GetFileKeyValueInt(optCameraLines, "FamX");
					int y = GetFileKeyValueInt(optCameraLines, "FamY");
					int z = GetFileKeyValueInt(optCameraLines, "FamZ");

					if (x == 0 && y == 0 && z == 0)
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

int HangarLoadShuttleHook(int* params)
{
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const auto AddObject = (short(*)(unsigned short, int, int, int, unsigned short, unsigned short))0x00456AE0;

	unsigned short a0 = (unsigned short)params[0];
	const int a1 = params[1];
	const int a2 = params[2];
	const int a3 = params[3];
	const unsigned short a4 = (unsigned short)params[4];
	const unsigned short a5 = (unsigned short)params[5];

	const auto lines = GetCustomFileLines("HangarObjects");
	const int value = GetFileKeyValueInt(lines, "LoadShuttle", 1);
	const unsigned short shuttleModelIndex = (unsigned short)GetFileKeyValueInt(lines, "ShuttleModelIndex");
	const int shuttleMarkings = GetFileKeyValueInt(lines, "ShuttleMarkings");

	if (shuttleModelIndex != 0)
	{
		a0 = shuttleModelIndex;
	}

	if (value == 1)
	{
		short objectIndex = AddObject(a0, a1, a2, a3, a4, a5);

		xwaObjects[objectIndex].pMobileObject->Markings = shuttleMarkings;

		return objectIndex;
	}

	return 0;
}

int HangarShuttleUpdateHook(int* params)
{
	const auto UpdateShuttle = (void(*)(int))0x0045EC70;

	const int a0 = params[0];

	const auto lines = GetCustomFileLines("HangarObjects");
	const int value = GetFileKeyValueInt(lines, "LoadShuttle", 1);

	if (value == 1)
	{
		UpdateShuttle(a0);
	}

	return 0;
}

int HangarShuttleCameraHook(int* params)
{
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
	xwaObjects[hangarPlayerObjectIndex].m15 = 0;
	xwaObjects[hangarPlayerObjectIndex].m17 = 0;

	const auto lines = GetCustomFileLines("HangarObjects");
	const int value = GetFileKeyValueInt(lines, "LoadShuttle", 1);

	if (value == 1)
	{
		positionX += 0x467;
		positionY += 0x2328;
		positionZ += 0x161;

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

	const auto lines = GetCustomFileLines("HangarObjects");
	const int value = GetFileKeyValueInt(lines, "LoadShuttle", 1);

	if (value == 1)
	{
		CockpitOptReadInfos();
		ExteriorOptReadInfos();
	}

	return 0;
}

int HangarLoadDroidsHook(int* params)
{
	const auto AddObject = (short(*)(unsigned short, int, int, int, unsigned short, unsigned short))0x00456AE0;

	int& V0x068BC10 = *(int*)0x068BC10;
	S0x09C6780* V0x09C6780 = (S0x09C6780*)0x09C6780;

	const auto lines = GetCustomFileLines("HangarObjects");
	const int value = GetFileKeyValueInt(lines, "LoadDroids", 1);

	if (value == 1)
	{
		// ModelIndex_311_1_33_HangarDroid
		V0x09C6780[V0x068BC10].ObjectIndex = AddObject(311, 0xE3, 0x15F, 0x7FFFFFFF, 0xE570, 0);
		V0x09C6780[V0x068BC10].m04 = 0;
		V0x09C6780[V0x068BC10].m26 = 0;
		V0x09C6780[V0x068BC10].m22 = 0;
		V0x09C6780[V0x068BC10].m08 = 0;
		V0x068BC10++;

		// ModelIndex_312_1_34_HangarDroid2
		V0x09C6780[V0x068BC10].ObjectIndex = AddObject(312, 0xE3, 0x15F, 0x7FFFFFFF, 0xE570, 0);
		V0x09C6780[V0x068BC10].m04 = 0;
		V0x09C6780[V0x068BC10].m26 = 0;
		V0x09C6780[V0x068BC10].m22 = 0;
		V0x09C6780[V0x068BC10].m08 = 0;
		V0x068BC10++;
	}

	return 0;
}

int HangarMapHook(int* params)
{
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const auto AddObject = (short(*)(unsigned short, int, int, int, unsigned short, unsigned short))0x00456AE0;

	const auto lines = GetCustomFileLines("HangarMap");

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

			if (value.size() == 7)
			{
				modelIndex = static_cast<unsigned short>(std::stoi(value[0], 0, 0));
				markings = std::stoi(value[1], 0, 0);
				positionX = std::stoi(value[2], 0, 0);
				positionY = std::stoi(value[3], 0, 0);
				positionZ = std::stoi(value[4], 0, 0);
				orientationXY = static_cast<short>(std::stoi(value[5], 0, 0));
				orientationZ = static_cast<short>(std::stoi(value[6], 0, 0));
			}
			else
			{
				modelIndex = static_cast<unsigned short>(std::stoi(value[0], 0, 0));
				markings = 0;
				positionX = std::stoi(value[1], 0, 0);
				positionY = std::stoi(value[2], 0, 0);
				positionZ = std::stoi(value[3], 0, 0);
				orientationXY = static_cast<short>(std::stoi(value[4], 0, 0));
				orientationZ = static_cast<short>(std::stoi(value[5], 0, 0));
			}

			short objectIndex = AddObject(modelIndex, positionX, positionY, positionZ, orientationXY, orientationZ);

			xwaObjects[objectIndex].pMobileObject->Markings = markings;

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
	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const auto AddObject = (short(*)(unsigned short, int, int, int, unsigned short, unsigned short))0x00456AE0;

	const auto lines = GetCustomFileLines("FamHangarMap");

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

			if (value.size() == 7)
			{
				modelIndex = static_cast<unsigned short>(std::stoi(value[0], 0, 0));
				markings = std::stoi(value[1], 0, 0);
				positionX = std::stoi(value[2], 0, 0);
				positionY = std::stoi(value[3], 0, 0);
				positionZ = std::stoi(value[4], 0, 0);
				orientationXY = static_cast<short>(std::stoi(value[5], 0, 0));
				orientationZ = static_cast<short>(std::stoi(value[6], 0, 0));
			}
			else
			{
				modelIndex = static_cast<unsigned short>(std::stoi(value[0], 0, 0));
				markings = 0;
				positionX = std::stoi(value[1], 0, 0);
				positionY = std::stoi(value[2], 0, 0);
				positionZ = std::stoi(value[3], 0, 0);
				orientationXY = static_cast<short>(std::stoi(value[4], 0, 0));
				orientationZ = static_cast<short>(std::stoi(value[5], 0, 0));
			}

			short objectIndex = AddObject(modelIndex, positionX, positionY, positionZ, orientationXY, orientationZ);

			xwaObjects[objectIndex].pMobileObject->Markings = markings;

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

	const auto lines = GetFileLines("hook_hangars.cfg");
	const auto mode = GetFileKeyValue(lines, "SelectionMode");
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

	const auto lines = GetFileLines("hook_hangars.cfg");
	const auto mode = GetFileKeyValue(lines, "SelectionMode");

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

	const auto lines = GetFileLines("hook_hangars.cfg");
	const auto mode = GetFileKeyValue(lines, "SelectionMode");

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
	const auto ModelGetSizeZ = (int(*)(unsigned int))0x0485820;

	const unsigned short modelIndex = (unsigned short)params[0];

	const std::string optSize = g_flightModelsList.GetLstLine(modelIndex);
	auto optSizeLines = GetFileLines(optSize + "Size.txt");

	if (!optSizeLines.size())
	{
		optSizeLines = GetFileLines(optSize + ".ini", "Size");
	}

	if (optSizeLines.size())
	{
		return GetFileKeyValueInt(optSizeLines, "ClosedSFoilsElevation");
	}
	else
	{
		if (modelIndex == 4) // BWing
		{
			return 0x32;
		}
		else
		{
			return ModelGetSizeZ(modelIndex) / 2;
		}
	}
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
