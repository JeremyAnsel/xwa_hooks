#include "targetver.h"
#include "dock.h"
#include "config.h"
#include <map>
#include <utility>
#include <algorithm>

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
		auto lines = GetFileLines("hook_dock.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_dock");
		}

		this->ForceDockElevation = GetFileKeyValueInt(lines, "ForceDockElevation", -1);
	}

	int ForceDockElevation;
};

Config g_config;

#pragma pack(push, 1)

struct ExeCraftEntry
{
	char Unk0000[606];
	int DockPositionY;
	int DockFromSmallPositionZ;
	int DockFromBigPositionZ;
	int DockToSmallPositionZ;
	int DockToBigPositionZ;
	char unk0272[361];
};

static_assert(sizeof(ExeCraftEntry) == 987, "size of ExeCraftEntry must be 987");

struct XwaCraft
{
	char Unk0000[4];
	short CraftIndex;
	char Unk0006[222];
	int m00E4;
	int m00E8;
	int m00EC;
	char Unk00F0[777];
};

static_assert(sizeof(XwaCraft) == 1017, "size of XwaCraft must be 1017");

struct XwaMobileObject
{
	char Unk0000[221];
	XwaCraft* pCraft;
	char Unk00E5[4];
};

static_assert(sizeof(XwaMobileObject) == 229, "size of XwaMobileObject must be 229");

struct XwaObject
{
	char Unk0000[2];
	short ModelIndex;
	char Unk0004[15];
	short HeadingXY;
	short HeadingZ;
	short m17;
	short m19;
	short m1B;
	char Unk001D[6];
	XwaMobileObject* pMobileObject;
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

#pragma pack(pop)

int GetShipDockElevation(int modelIndex)
{
	const std::string shipPath = g_flightModelsList.GetLstLine(modelIndex);

	auto lines = GetFileLines(shipPath + "Dock.txt");

	if (!lines.size())
	{
		lines = GetFileLines(shipPath + ".ini", "Dock");
	}

	if (lines.size())
	{
		return GetFileKeyValueInt(lines, "DockElevation", 0x3A4);
	}
	else
	{
		// ModelIndex_170_0_119_SpaceColony2
		if (modelIndex == 0xAA)
		{
			return 0;
		}
	}

	return 0x3A4;
}

class ModelIndexDock
{
public:
	int GetDockElevation(int modelIndex)
	{
		auto it = this->_dockElevation.find(modelIndex);

		if (it != this->_dockElevation.end())
		{
			return it->second;
		}
		else
		{
			int value = GetShipDockElevation(modelIndex);
			this->_dockElevation.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

private:
	std::map<int, int> _dockElevation;
};

ModelIndexDock g_modelIndexDock;

int DockReadPositionHook(int* params)
{
	const int A4 = (unsigned short)params[1047];
	const int positionZ = params[5];

	ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;

	ExeCraftTable[A4].DockToSmallPositionZ = positionZ;
	ExeCraftTable[A4].DockToBigPositionZ = positionZ;

	return 0;
}

int AIDockPickUpHook(int* params)
{
	const int A4 = params[0];
	const int A8 = params[1];
	const int AC = params[2];
	const int A10 = params[3];

	const auto L004A2FB0 = (void(*)(int, int, int, int))0x004A2FB0;

	const ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;
	const XwaCraft* s_pXwaCurrentCraft = *(XwaCraft**)0x00910DFC;

	int positionY = A10 - ExeCraftTable[s_pXwaCurrentCraft->CraftIndex].DockPositionY;

	L004A2FB0(A4, A8, AC, positionY);

	return 0;
}

int AIDockPickedUpHook(int* params)
{
	const int A4 = params[0];
	const int A8 = params[1];
	const int AC = params[2];
	const int A10 = params[3];
	const XwaCraft* ebp10 = (XwaCraft*)params[15];

	const auto L004A2FB0 = (void(*)(int, int, int, int))0x004A2FB0;

	const ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;

	int positionY = ExeCraftTable[ebp10->CraftIndex].DockPositionY - A10;

	L004A2FB0(A4, A8, AC, positionY);

	return 0;
}

int PlayerDockHook(int* params)
{
	const int A4 = params[0];
	const int A8 = params[1];
	const int AC = params[2];
	const int A10 = params[3];

	const auto L004A2FB0 = (void(*)(int, int, int, int))0x004A2FB0;

	int& s_V0x080DB64 = *(int*)0x080DB64;

	L004A2FB0(A4, A8, AC, A10);

	return 0;
}

int PLayerDockElevationHook(int* params)
{
	const int esp24 = (short)params[10];

	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	const ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;
	const XwaCraft* s_pXwaCurrentCraft = *(XwaCraft**)0x00910DFC;

	int elevation;

	if (g_config.ForceDockElevation >= 0)
	{
		elevation = g_config.ForceDockElevation;
	}
	else
	{
		short modelIndex = XwaObjects[esp24].ModelIndex;
		int elevationSetting = g_modelIndexDock.GetDockElevation(modelIndex);

		int elevationExeBottom = ExeCraftTable[XwaObjects[esp24].pMobileObject->pCraft->CraftIndex].DockFromBigPositionZ;
		int elevationExeTop = ExeCraftTable[s_pXwaCurrentCraft->CraftIndex].DockToBigPositionZ;
		int elevationExe = elevationExeBottom - elevationExeTop;

		elevation = std::min(elevationSetting, elevationExe);
	}

	params[0] = elevation / 2;

	return 0;
}

int PLayerDockOrientationHook(int* params)
{
	const int esp2C = (short)params[11];
	const int esp24 = (short)params[9];

	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;

	XwaObjects[esp2C].pMobileObject->pCraft->m00E4 = 0;
	XwaObjects[esp2C].pMobileObject->pCraft->m00E8 = 0;
	XwaObjects[esp2C].pMobileObject->pCraft->m00EC = 0;

	XwaObjects[esp2C].HeadingXY = XwaObjects[esp24].HeadingXY;
	XwaObjects[esp2C].HeadingZ = XwaObjects[esp24].HeadingZ;
	XwaObjects[esp2C].m17 = XwaObjects[esp24].m17;

	return 0;
}
