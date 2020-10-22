#include "targetver.h"
#include "pilot.h"
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

struct XwaCraft
{
	char unk000[608];
	char MeshRotationAngles[50];
	char unk292[359];
};

static_assert(sizeof(XwaCraft) == 1017, "size of XwaCraft must be 1017");

struct XwaObject
{
	char Unk0000[31];
	int PlayerIndex;
	char Unk0023[4];
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

struct XwaPlayer
{
	char Unk0000[505];
	unsigned char CockpitDisplayed;
	char Unk01FA[2517];
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

		PilotMesh pilotMesh;
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

		char& meshRotation = craft->MeshRotationAngles[pilot.meshIndex];

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
