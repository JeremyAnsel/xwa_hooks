#include "targetver.h"
#include "pilot.h"
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <vector>

std::string GetStringWithoutExtension(const std::string& str)
{
	auto b = str.find_last_of('.');

	return str.substr(0, b);
}

std::vector<std::string> Tokennize(const std::string& str)
{
	const std::string delimiters = ",;";
	std::vector<std::string> tokens;

	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);

	std::string::size_type pos = str.find_first_of(delimiters, lastPos);

	while (std::string::npos != pos || std::string::npos != lastPos)
	{
		std::string value = str.substr(lastPos, pos - lastPos);
		value.erase(std::remove_if(value.begin(), value.end(), std::isspace), value.end());

		tokens.push_back(value);

		lastPos = str.find_first_not_of(delimiters, pos);

		pos = str.find_first_of(delimiters, lastPos);
	}

	return tokens;
}

std::vector<std::vector<std::string>> GetFileListValues(const std::string& path)
{
	std::vector<std::vector<std::string>> values;

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

			values.push_back(Tokennize(line));
		}
	}

	return values;
}

std::string GetFlightModelsLstLine(int modelIndex)
{
	const int xwaObjectStats = 0x05FB240;
	const int dataIndex1 = *(short*)(xwaObjectStats + modelIndex * 0x18 + 0x14);
	const int dataIndex2 = *(short*)(xwaObjectStats + modelIndex * 0x18 + 0x16);

	if (dataIndex1 != 0 && dataIndex1 != 1)
	{
		return std::string();
	}

	std::string lstPath;
	lstPath.append("FlightModels\\");

	std::string lstFilesNames[] =
	{
		"Spacecraft0.LST",
		"Equipment0.LST",
	};

	lstPath.append(lstFilesNames[dataIndex1]);

	std::ifstream lstFile(lstPath);

	if (lstFile)
	{
		std::string line;

		for (int i = 0; i <= dataIndex2; i++)
		{
			std::getline(lstFile, line);
		}

		return GetStringWithoutExtension(line);
	}

	return std::string();
}

#pragma pack(push, 1)

struct XwaCraft
{
	char unk000[608];
	char MeshRotationAngles[50];
	char unk292[359];
};

static_assert(sizeof(XwaCraft) == 1017, "size of XwaCraft must be 1017");

#pragma pack(pop)

struct PilotMesh
{
	int meshIndex;
	int angle;
	int speed;
	int behavior;
};

std::vector<PilotMesh> GetFileListPilotMeshes(const std::string& path)
{
	auto pilotMeshes = GetFileListValues(path);

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

std::vector<PilotMesh> GetDefaultPilotMeshes(int modelIndex)
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
		values.push_back({ 4, 32, 2, 2 });
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
	std::vector<PilotMesh> pilotMeshes;

	std::string pilotPath = GetFlightModelsLstLine(modelIndex);

	if (pilotPath.empty())
	{
		return pilotMeshes;
	}

	pilotPath.append("Pilot.txt");

	if (std::ifstream(pilotPath))
	{
		pilotMeshes = GetFileListPilotMeshes(pilotPath);
	}
	else
	{
		pilotMeshes = GetDefaultPilotMeshes(modelIndex);
	}

	return pilotMeshes;
}

int PilotHook(int* params)
{
	const auto XwaRandom = (unsigned short(*)())0x00494E10;

	XwaCraft* craft = (XwaCraft*)params[0];
	const int modelIndex = params[1];
	const int A8 = params[8];

	auto pilotMeshes = GetPilotMeshes(modelIndex);

	if (pilotMeshes.empty())
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
