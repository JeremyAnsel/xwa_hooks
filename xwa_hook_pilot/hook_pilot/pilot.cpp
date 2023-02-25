#include "targetver.h"
#include "pilot.h"
#include "config.h"
#include <fstream>
#include <map>
#include <utility>

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

		this->ProvingGroundHangarModel = GetFileKeyValue(lines, "ProvingGroundHangarModel");
	}

	std::string ProvingGroundHangarModel;
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
	char unk028[355];
	unsigned char m18B;
	char unk18C[162];
	char XwaCraft_m22E[50];
	char MeshRotationAngles[50];
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

struct XwaPlayer
{
	int ObjectIndex;
	char m004[79];
	short m053;
	char m055[420];
	unsigned char CockpitDisplayed;
	char unk1FA[2382];
	int CameraPositionX;
	int CameraPositionY;
	int CameraPositionZ;
	char unkB54[123];
};

static_assert(sizeof(XwaPlayer) == 3023, "size of XwaPlayer must be 3023");

#pragma pack(pop)

struct PilotMesh
{
	int meshIndex;
	int angle;
	int speed;
	int behavior;
};

std::vector<std::string> GetCustomFileLinesBase(const std::string& name, bool append)
{
	static std::vector<std::string> _lines;
	static std::string _name;
	static std::string _mission;

	const char* xwaMissionFileName = (const char*)0x06002E8;

	if (_name != name || _mission != xwaMissionFileName)
	{
		_name = name;
		_mission = xwaMissionFileName;
		_lines.clear();

		const std::string mission = GetStringWithoutExtension(xwaMissionFileName);
		CombineFileLines(_lines, GetFileLines(mission + "_" + name + ".txt"), append);
		CombineFileLines(_lines, GetFileLines(mission + ".ini", name), append);
		CombineFileLines(_lines, GetFileLines(std::string("FlightModels\\") + name + ".txt"), append);
		CombineFileLines(_lines, GetFileLines("FlightModels\\default.ini", name), append);
	}

	return _lines;
}

std::string GetCommandShipLstLine()
{
	const unsigned short* exeSpecies = (unsigned short*)0x05B0F70;
	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const TieFlightGroupEx* xwaTieFlightGroups = (TieFlightGroupEx*)0x080DC80;
	const XwaPlayer* xwaPlayers = (XwaPlayer*)0x08B94E0;
	//const int playerObjectIndex = *(int*)0x068BC08;
	const int playerObjectIndex = *(int*)0x068BB98;
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
	//const int playerObjectIndex = *(int*)0x068BC08;
	const int playerObjectIndex = *(int*)0x068BB98;
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

std::string GetShipPath(int modelIndex)
{
	const std::string shipPath = g_flightModelsList.GetLstLine(modelIndex);

	{
		const auto objectLines = GetCustomFileLines("Objects", false);
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

std::vector<PilotMesh> GetFileListPilotMeshes(const std::vector<std::string>& lines)
{
	const auto pilotMeshes = GetFileListValues(lines);

	std::vector<PilotMesh> values;

	for (unsigned int i = 0; i < pilotMeshes.size(); i++)
	{
		const auto& value = pilotMeshes[i];

		if (value.size() < 4)
		{
			continue;
		}

		PilotMesh pilotMesh{};
		pilotMesh.meshIndex = std::stoi(value[0], 0, 0);
		pilotMesh.angle = std::stoi(value[1], 0, 0);
		pilotMesh.speed = std::stoi(value[2], 0, 0);
		pilotMesh.behavior = std::stoi(value[3], 0, 0);

		values.push_back(pilotMesh);
	}

	return values;
}

std::vector<PilotMesh> GetDefaultPilotMeshes(int modelIndex, bool isCockpit)
{
	std::vector<PilotMesh> values;

	switch (modelIndex)
	{
	case 1: // ModelIndex_001_0_0_Xwing:
		values.push_back({ 6, 32, 2, 1 });
		values.push_back({ 9, 32, 2, 2 });
		break;

	case 2: // ModelIndex_002_0_1_Ywing:
		values.push_back({ 8, 32, 2, 1 });
		values.push_back({ 13, 32, 2, 2 });
		break;

	case 3: // ModelIndex_003_0_2_Awing:
		if (isCockpit)
		{
		}
		else
		{
			values.push_back({ 4, 32, 2, 2 });
		}

		break;

	case 4: // ModelIndex_004_0_3_Bwing:
		break;

	case 58: // ModelIndex_058_0_45_CorellianTransport2:
		values.push_back({ 19, 32, 2, 1 });
		values.push_back({ 17, 32, 2, 2 });
		break;

	case 59: // ModelIndex_059_0_46_MilleniumFalcon2:
		values.push_back({ 21, 32, 2, 1 });
		values.push_back({ 19, 32, 2, 2 });
		break;

	case 65: // ModelIndex_065_0_52_FamilyTransport:
		values.push_back({ 13, 32, 2, 1 });
		values.push_back({ 9, 32, 2, 2 });
		break;

	case 203: // ModelIndex_203_1_51_RebelPilot:
	case 204: // ModelIndex_204_1_52_ImperialPilot:
		values.push_back({ 1, 32, 2, 2 });
		values.push_back({ 2, 32, 2, 2 });
		values.push_back({ 3, 32, 2, 2 });
		values.push_back({ 4, 32, 2, 2 });
		values.push_back({ 5, 32, 2, 2 });
		break;

	case 311: // ModelIndex_311_1_33_HangarDroid
		values.push_back({ 3, 0, 0, 3 });
		break;

	case 312: // ModelIndex_312_1_34_HangarDroid2
		values.push_back({ 1, 0, 0, 4 });
		values.push_back({ 2, 0, 0, 5 });
		break;
	}

	return values;
}

std::vector<PilotMesh> GetPilotMeshes(int modelIndex)
{
	const std::string pilot = GetShipPath(modelIndex);

	auto lines = GetFileLines(pilot + "Pilot.txt");

	if (!lines.size())
	{
		lines = GetFileLines(pilot + ".ini", "Pilot");
	}

	std::vector<PilotMesh> pilotMeshes;

	if (lines.size())
	{
		pilotMeshes = GetFileListPilotMeshes(lines);
	}
	else if (_stricmp(pilot.c_str(), g_flightModelsList.GetLstLine(modelIndex).c_str()) != 0)
	{
	}
	else
	{
		pilotMeshes = GetDefaultPilotMeshes(modelIndex, false);
	}

	return pilotMeshes;
}

std::vector<PilotMesh> GetPilotCockpitMeshes(int modelIndex)
{
	const std::string pilot = GetShipPath(modelIndex);

	auto lines = GetFileLines(pilot + "PilotCockpit.txt");

	if (!lines.size())
	{
		lines = GetFileLines(pilot + ".ini", "PilotCockpit");
	}

	if (!lines.size())
	{
		lines = GetFileLines(pilot + "Pilot.txt");
	}

	if (!lines.size())
	{
		lines = GetFileLines(pilot + ".ini", "Pilot");
	}


	std::vector<PilotMesh> pilotMeshes;

	if (lines.size())
	{
		pilotMeshes = GetFileListPilotMeshes(lines);
	}
	else if (_stricmp(pilot.c_str(), g_flightModelsList.GetLstLine(modelIndex).c_str()) != 0)
	{
	}
	else
	{
		pilotMeshes = GetDefaultPilotMeshes(modelIndex, true);
	}

	return pilotMeshes;
}

class ModelIndexPilotMeshes
{
public:
	std::vector<PilotMesh> GetMeshes(int modelIndex)
	{
		this->UpdateIfChanged();

		auto it = this->_meshes.find(modelIndex);

		if (it != this->_meshes.end())
		{
			return it->second;
		}
		else
		{
			auto value = GetPilotMeshes(modelIndex);
			this->_meshes.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

	std::vector<PilotMesh> GetMeshesCockpit(int modelIndex)
	{
		this->UpdateIfChanged();

		auto it = this->_meshesCockpit.find(modelIndex);

		if (it != this->_meshesCockpit.end())
		{
			return it->second;
		}
		else
		{
			auto value = GetPilotCockpitMeshes(modelIndex);
			this->_meshesCockpit.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

private:
	void UpdateIfChanged()
	{
		static std::string _mission;

		const char* xwaMissionFileName = (const char*)0x06002E8;

		if (_mission != xwaMissionFileName)
		{
			_mission = xwaMissionFileName;

			this->_meshes.clear();
			this->_meshesCockpit.clear();
		}
	}

	std::map<int, std::vector<PilotMesh>> _meshes;
	std::map<int, std::vector<PilotMesh>> _meshesCockpit;
};

ModelIndexPilotMeshes g_modelIndexPilotMeshes;

int PilotHook(int* params)
{
	const auto XwaRandom = (unsigned short(*)())0x00494E10;

	XwaCraft* craft = (XwaCraft*)params[0];
	const int modelIndex = params[1];
	const int objectIndex = params[7];
	const int A8 = params[8];

	char* craftMeshRotationAngles = g_craftConfig.MeshesCount == 0 ? craft->MeshRotationAngles : (char*)((int)craft + g_craftConfig.Craft_Offset_260);

	const XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	const XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;

	const int playerIndex = XwaObjects[objectIndex].PlayerIndex;
	const bool isCockpitDisplayed = playerIndex != -1 && XwaPlayers[playerIndex].CockpitDisplayed != 0;

	const auto pilotMeshes = isCockpitDisplayed ? g_modelIndexPilotMeshes.GetMeshesCockpit(modelIndex) : g_modelIndexPilotMeshes.GetMeshes(modelIndex);

	if (!pilotMeshes.size())
	{
		return 0;
	}

	for (unsigned int i = 0; i < pilotMeshes.size(); i++)
	{
		const auto& pilot = pilotMeshes[i];

		char& meshRotation = craftMeshRotationAngles[pilot.meshIndex];

		switch (pilot.behavior)
		{
		case 0:
			if (A8 != 0)
			{
				meshRotation = 0;
			}
			else
			{
				if ((meshRotation & 1) != 0)
				{
					meshRotation += pilot.speed;

					if (meshRotation > pilot.angle + 1)
					{
						meshRotation = pilot.angle + 1;
					}

					if (meshRotation == pilot.angle + 1)
					{
						meshRotation ^= 1;
					}
				}
				else
				{
					meshRotation -= pilot.speed;

					if (meshRotation < -pilot.angle)
					{
						meshRotation = -pilot.angle;
					}

					if (meshRotation == -pilot.angle)
					{
						meshRotation ^= 1;
					}
				}
			}

			break;

		case 1:
			if (A8 != 0)
			{
				meshRotation = 0;
			}
			else
			{
				if ((meshRotation & 1) != 0)
				{
					meshRotation += pilot.speed;

					if (meshRotation > pilot.angle + 1)
					{
						meshRotation = pilot.angle + 1;
					}

					if (meshRotation == pilot.angle + 1 || XwaRandom() < 0x300)
					{
						meshRotation ^= 1;
					}
				}
				else
				{
					meshRotation -= pilot.speed;

					if (meshRotation < -pilot.angle)
					{
						meshRotation = -pilot.angle;
					}

					if (meshRotation == -pilot.angle || XwaRandom() < 0x300)
					{
						meshRotation ^= 1;
					}
				}
			}

			break;

		case 2:
			if (A8 != 0)
			{
				meshRotation = 0;
			}
			else
			{
				if ((meshRotation & 1) != 0)
				{
					meshRotation -= pilot.speed;

					if (meshRotation < -pilot.angle + 1)
					{
						meshRotation = -pilot.angle + 1;
					}

					if (meshRotation == -pilot.angle / 2 + 1 && XwaRandom() > 0x300)
					{
						meshRotation += pilot.speed;
					}

					if (meshRotation == -pilot.angle + 1 || XwaRandom() < 0x300)
					{
						meshRotation ^= 1;
					}
				}
				else
				{
					meshRotation += pilot.speed;

					if (meshRotation > pilot.angle)
					{
						meshRotation = pilot.angle;
					}

					if (meshRotation == pilot.angle / 2 && XwaRandom() > 0x300)
					{
						meshRotation -= pilot.speed;
					}

					if (meshRotation == pilot.angle || XwaRandom() < 0x300)
					{
						meshRotation ^= 1;
					}
				}
			}

			break;
		}
	}

	return 0;
}

void ApplyHangarDroidPilot(XwaCraft* craft, int objectIndex)
{
	const auto XwaRandom = (unsigned short(*)())0x00494E10;

	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	XwaObject* object = &XwaObjects[objectIndex];
	int modelIndex = object->ModelIndex;

	char* craftMeshRotationAngles = g_craftConfig.MeshesCount == 0 ? craft->MeshRotationAngles : (char*)((int)craft + g_craftConfig.Craft_Offset_260);

	const auto pilotMeshes = g_modelIndexPilotMeshes.GetMeshes(modelIndex);

	if (!pilotMeshes.size())
	{
		return;
	}

	for (unsigned int i = 0; i < pilotMeshes.size(); i++)
	{
		const auto& pilot = pilotMeshes[i];

		char& meshRotation = craftMeshRotationAngles[pilot.meshIndex];

		switch (pilot.behavior)
		{
		case 3:
			if ((meshRotation & 1) != 0)
			{
				meshRotation -= 0x02;

				if (meshRotation == 0xF7 && XwaRandom() > 0x200)
				{
					meshRotation += 0x02;
				}

				if (meshRotation == 0xE1)
				{
					meshRotation = 0xE0;
				}
			}
			else
			{
				meshRotation += 0x02;

				if (meshRotation == 0x04 && XwaRandom() > 0x200)
				{
					meshRotation -= 0x02;
				}

				if (meshRotation == 0x20)
				{
					meshRotation = 0x21;
				}
			}

			break;

		case 4:
			meshRotation += *(int*)0x009C67D8 * 0x04;

			break;

		case 5:
			if (*(int*)0x009C67B2 == 0)
			{
				meshRotation++;

			}
			else
			{
				meshRotation += 0x04;
			}

			break;
		}
	}
}

int HangarDroid1Hook(int* params)
{
	int objectIndex = params[Params_EBX];

	params[Params_ReturnAddress] = 0x0045E286;

	XwaCraft* craft = (XwaCraft*)params[Params_ECX];
	*(XwaCraft**)0x00910DFC = craft;

	ApplyHangarDroidPilot(craft, objectIndex);

	return 0;
}

int HangarDroid2Hook(int* params)
{
	const auto XwaRandom = (unsigned short(*)())0x00494E10;

	int objectIndex = params[Params_EBX];

	params[Params_ReturnAddress] = 0x0045E286;

	XwaCraft* craft = (XwaCraft*)params[Params_EAX];
	*(XwaCraft**)0x00910DFC = craft;

	if ((XwaRandom() % 0x64) < 0x04)
	{
		*(int*)0x009C67D8 = (XwaRandom() % 0x02) - 0x01;
	}

	ApplyHangarDroidPilot(craft, objectIndex);

	return 0;
}
