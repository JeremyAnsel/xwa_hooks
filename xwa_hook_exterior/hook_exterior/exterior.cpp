#include "targetver.h"
#include "exterior.h"
#include "config.h"
#include <fstream>
#include <map>
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

#pragma pack(push, 1)

struct S0x08B94E0_B48
{
	char Unk0000[65];
	short IsExteriorView;
	char Unk0043[10];
};

static_assert(sizeof(S0x08B94E0_B48) == 77, "size of S0x08B94E0_B48 must be 77");

struct XwaPlayer
{
	int ObjectIndex;
	char Unk004[19];
	unsigned char HyperspacePhase;
	char Unk018[13];
	short XwaPlayer_m025;
	char Unk027[61];
	unsigned char IsHudVisible;
	unsigned char XwaPlayer_m065;
	unsigned char XwaPlayer_m066;
	unsigned char IsHudMfd1Visible;
	unsigned char IsHudMfd2Visible;
	unsigned char XwaPlayer_m069;
	unsigned char XwaPlayer_m06A;
	char Unk06B[402];
	short MousePositionX;
	short MousePositionY;
	char Unk201[2375];
	S0x08B94E0_B48 XwaPlayer_mB48;
	char UnkB95[58];
};

static_assert(sizeof(XwaPlayer) == 3023, "size of XwaPlayer must be 3023");

struct XwaObject
{
	char unk00[2];
	unsigned short ModelIndex;
	char unk04[35];
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

#pragma pack(pop)

int GetShipMfdLodDistance(int modelIndex)
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

	lines = GetFileLines(shipPath + "Exterior.txt");

	if (!lines.size())
	{
		lines = GetFileLines(shipPath + ".ini", "Exterior");
	}

	if (lines.size())
	{
		return GetFileKeyValueInt(lines, "LodDistance");
	}

	return 0;
}

class ModelIndexLodDistance
{
public:
	int GetLodDistance(int modelIndex)
	{
		this->Update();

		auto it = this->_lodDistance.find(modelIndex);

		if (it != this->_lodDistance.end())
		{
			return it->second;
		}
		else
		{
			int value = GetShipMfdLodDistance(modelIndex);
			this->_lodDistance.insert(std::make_pair(modelIndex, value));
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

			this->_lodDistance.clear();
		}
	}

	std::map<int, int> _lodDistance;
};

ModelIndexLodDistance g_modelIndexLodDistance;

int Render3DHook(int* params)
{
	XwaPlayer* xwaPlayers = (XwaPlayer*)0x08B94E0;
	const int currentPlayerId = *(int*)0x08C1CC8;

	// 3d render scene
	((void(*)())0x004EFE00)();

	if (xwaPlayers[currentPlayerId].XwaPlayer_mB48.IsExteriorView == 1)
	{
		xwaPlayers[currentPlayerId].XwaPlayer_mB48.IsExteriorView = 0;

		unsigned char hyper = xwaPlayers[currentPlayerId].HyperspacePhase;
		xwaPlayers[currentPlayerId].HyperspacePhase = 0;

		// force the game to recompute the reticle position
		xwaPlayers[currentPlayerId].MousePositionX = 1;
		xwaPlayers[currentPlayerId].MousePositionY = 1;

		// render MFDs
		((void(*)())0x00465B30)();
		((void(*)())0x00489690)();

		// render MFDs text
		((void(*)())0x0046D610)();

		// render target miniature
		if (xwaPlayers[currentPlayerId].IsHudVisible != 0 && *(unsigned char*)0x05B5334 != 0 && xwaPlayers[currentPlayerId].XwaPlayer_m025 != -1)
		{
			float s_0x008B94CC = *(float*)0x008B94CC;
			float s_0x005B46B4 = *(float*)0x005B46B4;
			float s_0x008C1600 = *(float*)0x008C1600;
			float s_0x00686ACC = *(float*)0x00686ACC;
			float s_0x0080ACF8 = *(float*)0x0080ACF8;
			float s_0x007B33C0 = *(float*)0x007B33C0;
			float s_0x0064D1AC = *(float*)0x0064D1AC;
			float s_0x007B4BEC = *(float*)0x007B4BEC;
			float s_0x007B6FF8 = *(float*)0x007B6FF8;
			float s_0x007B33DC = *(float*)0x007B33DC;
			float s_0x007B4BE8 = *(float*)0x007B4BE8;
			float s_0x007B6FF0 = *(float*)0x007B6FF0;
			float s_0x007B33D8 = *(float*)0x007B33D8;
			float s_0x007B4BF4 = *(float*)0x007B4BF4;
			float s_0x007B33D4 = *(float*)0x007B33D4;
			float s_0x007B4BE4 = *(float*)0x007B4BE4;

			short di = *(short*)0x068C5BC;
			short ebx = *(short*)0x068C5C0;
			short esi = *(short*)0x07D4B6C - *(short*)0x068C5C0;
			short cx = *(short*)0x068C520 - *(short*)0x068C5BC / 2;

			((void(*)(int, int, int, int, int))0x00478490)(cx, esi, di, ebx, 0x01);

			*(float*)0x008B94CC = s_0x008B94CC;
			*(float*)0x005B46B4 = s_0x005B46B4;
			*(float*)0x008C1600 = s_0x008C1600;
			*(float*)0x00686ACC = s_0x00686ACC;
			*(float*)0x0080ACF8 = s_0x0080ACF8;
			*(float*)0x007B33C0 = s_0x007B33C0;
			*(float*)0x0064D1AC = s_0x0064D1AC;
			*(float*)0x007B4BEC = s_0x007B4BEC;
			*(float*)0x007B6FF8 = s_0x007B6FF8;
			*(float*)0x007B33DC = s_0x007B33DC;
			*(float*)0x007B4BE8 = s_0x007B4BE8;
			*(float*)0x007B6FF0 = s_0x007B6FF0;
			*(float*)0x007B33D8 = s_0x007B33D8;
			*(float*)0x007B4BF4 = s_0x007B4BF4;
			*(float*)0x007B33D4 = s_0x007B33D4;
			*(float*)0x007B4BE4 = s_0x007B4BE4;
		}

		xwaPlayers[currentPlayerId].HyperspacePhase = hyper;
		xwaPlayers[currentPlayerId].XwaPlayer_mB48.IsExteriorView = 1;
	}

	return 0;
}

int SetHudVisibilityHook(int* params)
{
	const int callback = params[0];
	const int playerIndex = params[1];
	const unsigned char visibility = (unsigned char)params[2];

	XwaPlayer* xwaPlayers = (XwaPlayer*)0x08B94E0;

	if (callback == 0x0045D2B5 || callback == 0x00500C24 || callback == 0x004603B9)
	{
		xwaPlayers[playerIndex].IsHudVisible = visibility;
	}
	else
	{
		xwaPlayers[playerIndex].IsHudVisible = xwaPlayers[playerIndex].XwaPlayer_m065;
	}

	return 0;
}

int LodDistanceHook(int* params)
{
	const int A4 = params[0];
	const int A8 = params[1];
	const int AC = params[2];
	const unsigned short objectIndex = (unsigned short)params[7];

	const auto XwaTransformMatrixUp = (int(*)(int, int, int))0x004EA360;

	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	unsigned short modelIndex = xwaObjects[objectIndex].ModelIndex;

	int lodDistance = g_modelIndexLodDistance.GetLodDistance(modelIndex);

	if (lodDistance == 0)
	{
		lodDistance = XwaTransformMatrixUp(A4, A8, AC);
	}

	//lodDistance = 0x7fffffff;

	return lodDistance;
}
