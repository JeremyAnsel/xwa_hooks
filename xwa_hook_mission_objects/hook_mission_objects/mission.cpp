#include "targetver.h"
#include "mission.h"
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

#pragma pack(push, 1)

struct ExeEnableEntry
{
	char unk00[8];
	void* pData1;
	void* pData2;
	char unk10[2];
	short CraftIndex;
	char unk14[4];
};

static_assert(sizeof(ExeEnableEntry) == 24, "size of ExeEnableEntry must be 24");

struct ExeCraftEntry
{
	char Unk0000[594];
	short TurretOptModelId[2];
	char Unk0256[389];
};

static_assert(sizeof(ExeCraftEntry) == 987, "size of ExeCraftEntry must be 987");

struct OptModelMeshesInfo
{
	int NumOfMeshes;
	int MeshesType[50];
	void* MeshesDescriptor[50];
};
static_assert(sizeof(OptModelMeshesInfo) == 404, "size of S0x08D9760 must be 404");

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

#pragma pack(pop)

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

	const std::string path = "FlightModels\\";
	lines = GetFileLines(path + name + ".txt");

	return lines;
}

int MissionObjectsHook(int* params)
{
	const char* argOpt = (char*)params[0];

	const auto OptLoad = (int(*)(const char*))0x004CC940;

	const auto lines = GetCustomFileLines("Objects");
	const std::string value = GetFileKeyValue(lines, argOpt);

	if (!value.empty() && std::ifstream(value))
	{
		return OptLoad(value.c_str());
	}

	return OptLoad(argOpt);
}

void TurretOptReload(int gunnerModelIndex, int playerModelIndex, int turretIndex)
{
	const auto OptUnload = (void(*)(unsigned short))0x004CCA60;
	const auto OptLoad = (short(*)(const char*))0x004CC940;
	const auto Lock_Handle = (void*(*)(short))0x0050E2F0;
	const auto L004328B0 = (void(*)(int, int))0x004328B0;
	const auto OptGetNumOfMeshes = (int(*)(int))0x00485190;
	const auto OptGetMeshType = (int(*)(int, int))0x00485A70;
	const auto OptGetMeshDescriptor = (void*(*)(int, int))0x004859B0;

	int& s_V0x077333C = *(int*)0x0077333C;
	unsigned short* OptModelFileMemHandles = (unsigned short*)0x007CA6E0;
	ExeEnableEntry* ExeEnableTable = (ExeEnableEntry*)0x005FB240;
	OptModelMeshesInfo* OptModelMeshesInfos = (OptModelMeshesInfo*)0x008D9760;

	if (OptModelFileMemHandles[gunnerModelIndex] != 0)
	{
		OptUnload(OptModelFileMemHandles[gunnerModelIndex]);

		OptModelFileMemHandles[gunnerModelIndex] = 0;
		ExeEnableTable[gunnerModelIndex].pData1 = nullptr;
		ExeEnableTable[gunnerModelIndex].pData2 = nullptr;
	}

	FlightModelsList g_flightModelsList;
	std::string ship = g_flightModelsList.GetLstLine(playerModelIndex);

	switch (turretIndex)
	{
	case 1:
		ship.append("Gunner.opt");
		break;

	case 2:
		if (std::ifstream(ship + "Gunner2.opt"))
		{
			ship.append("Gunner2.opt");
		}
		else
		{
			ship.append("Gunner.opt");
		}

		break;

	default:
		ship.append("Gunner.opt");
		break;
	}

	if (!std::ifstream(ship))
	{
		ship = "FlightModels\\CorellianTransportGunner.opt";
	}

	s_V0x077333C = 1;

	short handle = OptLoad(ship.c_str());
	ExeEnableTable[gunnerModelIndex].pData1 = Lock_Handle(handle);
	OptModelFileMemHandles[gunnerModelIndex] = handle;

	L004328B0(ExeEnableTable[gunnerModelIndex].CraftIndex, gunnerModelIndex);

	int numMeshes = OptGetNumOfMeshes(gunnerModelIndex);
	OptModelMeshesInfos[gunnerModelIndex].NumOfMeshes = numMeshes;

	for (int i = 0; i < numMeshes; i++)
	{
		OptModelMeshesInfos[gunnerModelIndex].MeshesType[i] = OptGetMeshType(gunnerModelIndex, i);
		OptModelMeshesInfos[gunnerModelIndex].MeshesDescriptor[i] = OptGetMeshDescriptor(gunnerModelIndex, i);
	}

	s_V0x077333C = 0;
}

int CraftTurretHook(int* params)
{
	const int arg4 = params[1];
	const int arg8 = params[2];

	const auto L004201E0 = (int(*)(int, int, int))0x004201E0;
	const auto L00497D40 = (void(*)(int, int))0x00497D40;

	const ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;
	XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	const int XwaCurrentPlayerId = *(int*)0x008C1CC8;
	char& V0x077129C = *(char*)0x0077129C;
	const int V0x07827E4 = *(int*)0x007827E4;

	int playerObjectIndex = XwaPlayers[arg4].ObjectIndex;

	XwaCraft* playerCraft = nullptr;
	short playerCraftIndex = 0;
	int playerModelIndex = 0;

	if (playerObjectIndex != 0xFFFF)
	{
		playerCraft = XwaObjects[playerObjectIndex].pMobileObject->pCraft;
		playerCraftIndex = playerCraft->CraftIndex;
		playerModelIndex = XwaObjects[playerObjectIndex].ModelIndex;
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
			bool call_L004201E0 = false;

			if (XwaPlayers[arg4].XwaPlayer_mBCE != 0)
			{
				XwaPlayers[arg4].TurretIndex++;

				if (XwaPlayers[arg4].TurretIndex > turretsCount)
				{
					XwaPlayers[arg4].TurretIndex = turretsCount - 1;
				}

				call_L004201E0 = true;
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

					call_L004201E0 = true;
				}
			}

			int turretModelIndex;

			switch (XwaPlayers[arg4].TurretIndex)
			{
			case 1:
				turretModelIndex = ExeCraftTable[playerCraftIndex].TurretOptModelId[0];
				break;

			case 2:
				turretModelIndex = ExeCraftTable[playerCraftIndex].TurretOptModelId[1];
				break;

			default:
				turretModelIndex = ExeCraftTable[playerCraftIndex].TurretOptModelId[0];
				break;
			}

			TurretOptReload(turretModelIndex, playerModelIndex, XwaPlayers[arg4].TurretIndex);

			if (call_L004201E0)
			{
				L004201E0(arg4, playerObjectIndex, 0);
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
