#include "targetver.h"
#include "mission.h"
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <vector>

std::string GetStringWithoutExtension(const std::string& str)
{
	auto b = str.find_last_of('.');

	return str.substr(0, b);
}

std::string GetFileKeyValue(const std::string& path, const std::string& key)
{
	std::ifstream file(path);

	if (file)
	{
		std::string line;

		while (std::getline(file, line))
		{
			if (!line.length())
			{
				continue;
			}

			if (line[0] == '#' || line[0] == ';' || (line[0] == '/' && line[1] == '/'))
			{
				continue;
			}

			int pos = line.find("=");

			if (pos == -1)
			{
				continue;
			}

			std::string name = line.substr(0, pos);
			name.erase(std::remove_if(name.begin(), name.end(), std::isspace), name.end());

			std::string value = line.substr(pos + 1);
			value.erase(std::remove_if(value.begin(), value.end(), std::isspace), value.end());

			if (!name.length() || !value.length())
			{
				continue;
			}

			if (_stricmp(name.c_str(), key.c_str()) == 0)
			{
				return value;
			}
		}
	}

	return std::string();
}

std::vector<std::string> GetFileLines(const std::string& path)
{
	std::vector<std::string> values;

	std::ifstream file(path);

	if (file)
	{
		std::string line;

		while (std::getline(file, line))
		{
			if (!line.length())
			{
				continue;
			}

			if (line[0] == '#' || line[0] == ';' || (line[0] == '/' && line[1] == '/'))
			{
				continue;
			}

			values.push_back(line);
		}
	}

	return values;
}

class FlightModelsList
{
public:
	FlightModelsList()
	{
		for (auto& line : GetFileLines("FlightModels\\Spacecraft0.LST"))
		{
			this->_spacecraftList.push_back(GetStringWithoutExtension(line));
		}

		for (auto& line : GetFileLines("FlightModels\\Equipment0.LST"))
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

	std::string path = "FlightModels\\";
	path.append(name);

	return path;
}

#pragma pack(push, 1)

struct ExeCraftEntry
{
	char Unk0000[594];
	short TurretOptModelId[2];
	char Unk0256[389];
};

static_assert(sizeof(ExeCraftEntry) == 987, "size of ExeCraftEntry must be 987");

struct XwaCraftWeaponRack
{
	char Unk0000[2];
	char Sequence;
	char Unk0003[7];
	short ObjectIndex;
	char Unk000C[2];
};

static_assert(sizeof(XwaCraftWeaponRack) == 14, "size of XwaCraftWeaponRack must be 14");

struct XwaCraft
{
	char Unk0000[4];
	short CraftIndex;
	char Unk0006[424];
	char WeaponRacksCount;
	char Unk01AF[304];
	XwaCraftWeaponRack WeaponRacks[16];
	char Unk03BF[58];
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
	short ModelIndex;
	char Unk0004[31];
	XwaMobileObject* pMobileObject;
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

struct XwaPlayer
{
	int ObjectIndex;
	char Unk0004[10];
	short XwaPlayer_m00E;
	char Unk0010[1];
	char XwaPlayer_m011;
	char Unk0012[33];
	char XwaPlayer_m033;
	char XwaPlayer_m034;
	char Unk0035[455];
	char XwaPlayer_m1FC;
	short MousePositionX;
	short MousePositionY;
	char Unk0201[24];
	short TurretIndex;
	short XwaPlayer_m21B;
	char Unk021D[2481];
	char XwaPlayer_mBCE;
};

static_assert(sizeof(XwaPlayer) == 3023, "size of XwaPlayer must be 3023");

struct TieFlightGroupEx
{
	char Unk0000[107];
	unsigned char CraftId;
	char Unk006C[3538];
	int PlayerIndex;
};

static_assert(sizeof(TieFlightGroupEx) == 3650, "size of TieFlightGroupEx must be 3650");

#pragma pack(pop)

int GetPlayerModelIndex(int playerId)
{
	const unsigned short* exeSpecies = (unsigned short*)0x05B0F70;
	TieFlightGroupEx* XwaTieFlightGroups = (TieFlightGroupEx*)0x0080DC80;
	int XwaTieFlightGroupsCount = *(short*)0x007B4C00;

	int playerFlightGroupIndex = -1;
	for (int index = 0; index < XwaTieFlightGroupsCount; index++)
	{
		if (XwaTieFlightGroups[playerFlightGroupIndex].PlayerIndex == playerId)
		{
			playerFlightGroupIndex = index;
			break;
		}
	}

	if (playerFlightGroupIndex == -1)
	{
		return -1;
	}

	int playerCraftId = XwaTieFlightGroups[playerFlightGroupIndex].CraftId;
	int playerModelIndex = exeSpecies[playerCraftId];
	return playerModelIndex;
}

int MissionObjectsHook(int* params)
{
	const char* argOpt = (char*)params[0];

	const auto OptLoad = (int(*)(const char*))0x004CC940;

	std::string txtPath = GetCustomFilePath("Objects.txt");
	std::string value = GetFileKeyValue(txtPath, argOpt);

	if (!value.empty() && std::ifstream(value))
	{
		return OptLoad(value.c_str());
	}

	if (_stricmp(argOpt, "FlightModels\\CorellianTransportGunner.opt") == 0)
	{
		int XwaCurrentPlayerId = *(int*)0x008C1CC8;
		int playerModelIndex = GetPlayerModelIndex(XwaCurrentPlayerId);

		if (playerModelIndex != -1)
		{
			FlightModelsList g_flightModelsList;
			std::string ship = GetStringWithoutExtension(g_flightModelsList.GetLstLine(playerModelIndex));
			ship.append("Gunner.opt");

			if (std::ifstream(ship))
			{
				return OptLoad(ship.c_str());
			}
		}
	}

	return OptLoad(argOpt);
}

int CraftTurretHook(int* params)
{
	const int arg4 = params[1];
	const int arg8 = params[2];

	const auto L004201E0 = (int(*)(int, int, int))0x004201E0;
	const auto L00497D40 = (void(*)(int, int))0x00497D40;

	ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;
	XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	int XwaCurrentPlayerId = *(int*)0x008C1CC8;
	char& V0x077129C = *(char*)0x0077129C;
	int V0x07827E4 = *(int*)0x007827E4;

	int playerObjectIndex = XwaPlayers[arg4].ObjectIndex;

	XwaCraft* playerCraft = nullptr;
	short playerCraftIndex = 0;

	if (playerObjectIndex != 0xFFFF)
	{
		playerCraft = XwaObjects[playerObjectIndex].pMobileObject->pCraft;
		playerCraftIndex = playerCraft->CraftIndex;
	}

	if (V0x07827E4 == 0 && playerObjectIndex != 0xFFFF)
	{
		if (XwaPlayers[arg4].XwaPlayer_m1FC != 0)
		{
			XwaPlayers[arg4].XwaPlayer_m1FC = 0;
		}

		int turretsCount = 0;

		if (ExeCraftTable[playerCraftIndex].TurretOptModelId[0] != 0)
		{
			turretsCount++;

			if (ExeCraftTable[playerCraftIndex].TurretOptModelId[1] != 0)
			{
				turretsCount++;
			}
		}

		if (turretsCount != 0)
		{
			if (XwaPlayers[arg4].XwaPlayer_mBCE != 0)
			{
				XwaPlayers[arg4].TurretIndex++;

				if (XwaPlayers[arg4].TurretIndex > turretsCount)
				{
					XwaPlayers[arg4].TurretIndex = turretsCount - 1;
				}

				L004201E0(arg4, playerObjectIndex, 0);
			}
			else
			{
				int ecx = -1;

				for (int index = 0; index < 8; index++)
				{
					if (index == arg4)
						continue;

					if (XwaPlayers[index].XwaPlayer_m011 == 0)
						continue;

					if (XwaPlayers[index].XwaPlayer_mBCE == 0)
						continue;

					if (XwaPlayers[index].XwaPlayer_m00E == XwaPlayers[arg4].XwaPlayer_m00E)
					{
						ecx = index;
						break;
					}
				}

				if (ecx == -1)
				{
					XwaPlayers[arg4].TurretIndex++;

					if (arg8 != 0 || XwaPlayers[arg4].TurretIndex > turretsCount)
					{
						XwaPlayers[arg4].TurretIndex = 0;
					}

					L004201E0(arg4, playerObjectIndex, 0);
				}

			}
		}

		if (turretsCount != 0)
		{
			if (XwaPlayers[arg4].TurretIndex != 0)
			{
				XwaPlayers[arg4].XwaPlayer_m21B = 1;

				// MSG_GUNNER
				L00497D40(519, arg4);
			}
			else
			{
				XwaPlayers[arg4].XwaPlayer_m21B = 0;

				// MSG_PILOTING
				L00497D40(520, arg4);
			}

			for (int index = 0; index < playerCraft->WeaponRacksCount; index++)
			{
				if (playerCraft->WeaponRacks[index].Sequence >= 4)
				{
					playerCraft->WeaponRacks[index].ObjectIndex = -1;
				}
			}
		}
	}

	if (XwaPlayers[arg4].TurretIndex > 0)
	{
		XwaPlayers[arg4].XwaPlayer_m033 = 0;
		XwaPlayers[arg4].XwaPlayer_m034 = 0;
		XwaPlayers[arg4].MousePositionX = 0;
		XwaPlayers[arg4].MousePositionY = 0;

		if (arg4 == XwaCurrentPlayerId)
		{
			V0x077129C = 0;
		}
	}

	return 0;
}
