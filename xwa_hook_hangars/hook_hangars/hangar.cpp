#include "targetver.h"
#include "hangar.h"
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

int GetFileKeyValueInt(const std::string& path, const std::string& key)
{
	std::string value = GetFileKeyValue(path, key);

	if (value.empty())
	{
		return 0;
	}

	return std::stoi(value, 0, 0);
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

FlightModelsList g_flightModelsList;

std::string GetCommandShipLstLine()
{
	const unsigned short* exeSpecies = (unsigned short*)0x05B0F70;
	const int xwaObjects = *(int*)0x07B33C4;
	const int xwaTieFlightGroups = 0x080DC80;
	const int playerObjectIndex = *(int*)0x068BC08;

	if (playerObjectIndex != 0xffff)
	{
		const int playerFlightGroupIndex = *(unsigned char*)(xwaObjects + playerObjectIndex * 0x27 + 0x05);
		const int commandShipFlightGroupIndex = *(unsigned char*)(xwaTieFlightGroups + playerFlightGroupIndex * 0xE42 + 0x0C4);
		const int commandShipEnabled = *(unsigned char*)(xwaTieFlightGroups + playerFlightGroupIndex * 0xE42 + 0x0C5);

		if (commandShipEnabled)
		{
			const int commandShipCraftId = *(unsigned char*)(xwaTieFlightGroups + commandShipFlightGroupIndex * 0xE42 + 0x06B);
			const int commandShipModelIndex = exeSpecies[commandShipCraftId];

			return g_flightModelsList.GetLstLine(commandShipModelIndex);
		}
	}

	return std::string();
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

	std::string txtPath = GetCustomFilePath("HangarObjects.txt");

	std::string value = GetFileKeyValue(txtPath, opt);

	if (!value.empty() && std::ifstream(value))
	{
		return OptLoad(value.c_str());
	}

	if (_stricmp(opt.c_str(), "FlightModels\\Hangar.opt") == 0)
	{
		std::string hangar = GetCustomFilePath("Hangar.opt");

		if (std::ifstream(hangar))
		{
			return OptLoad(hangar.c_str());
		}
	}

	return OptLoad(opt.c_str());
}

int HangarCameraPositionHook(int* params)
{
	short A4 = (short)params[8];
	int& positionX = params[0];
	int& positionY = params[1];
	int& positionZ = params[2];

	const int xwaObjects = *(int*)0x07B33C4;
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

		std::string txtPath = GetCustomFilePath("HangarCamera.txt");

		switch (A4)
		{
		case 1:
			if (std::ifstream(txtPath))
			{
				positionX += GetFileKeyValueInt(txtPath, "Key1_X");
				positionY += GetFileKeyValueInt(txtPath, "Key1_Y");
				positionZ += GetFileKeyValueInt(txtPath, "Key1_Z");
			}
			else
			{
				positionX += 0x46A;
				positionY += -0x910;
				positionZ += -0x12C;
			}
			break;

		case 2:
			if (std::ifstream(txtPath))
			{
				positionX += GetFileKeyValueInt(txtPath, "Key2_X");
				positionY += GetFileKeyValueInt(txtPath, "Key2_Y");
				positionZ += GetFileKeyValueInt(txtPath, "Key2_Z");
			}
			else
			{
				positionX += 0x4D8;
				positionY += -0x14A;
				positionZ += -0x2BC;
			}
			break;

		case 3:
			if (std::ifstream(txtPath))
			{
				positionX += GetFileKeyValueInt(txtPath, "Key3_X");
				positionY += GetFileKeyValueInt(txtPath, "Key3_Y");
				positionZ += GetFileKeyValueInt(txtPath, "Key3_Z");
			}
			else
			{
				positionX += -0x460;
				positionY += 0x550;
				positionZ += -0x316;
			}
			break;

		case 6:
			if (std::ifstream(txtPath))
			{
				positionX += GetFileKeyValueInt(txtPath, "Key6_X");
				positionY += GetFileKeyValueInt(txtPath, "Key6_Y");
				positionZ += GetFileKeyValueInt(txtPath, "Key6_Z");
			}
			else
			{
				positionX += -0x4B0;
				positionY += -0x5FA;
				positionZ += -0x352;
			}
			break;

		case 9:
			if (std::ifstream(txtPath))
			{
				positionX += GetFileKeyValueInt(txtPath, "Key9_X");
				positionY += GetFileKeyValueInt(txtPath, "Key9_Y");
				positionZ += GetFileKeyValueInt(txtPath, "Key9_Z");
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
				if (std::ifstream(txtPath))
				{
					positionX += GetFileKeyValueInt(txtPath, "Key3_X");
					positionY += GetFileKeyValueInt(txtPath, "Key3_Y");
					positionZ += GetFileKeyValueInt(txtPath, "Key3_Z");
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
				const unsigned short playerModelIndex = *(unsigned short*)(xwaObjects + playerObjectIndex * 0x27 + 0x02);
				const int playerPositionX = *(int*)(xwaObjects + playerObjectIndex * 0x27 + 0x07);
				const int playerPositionY = *(int*)(xwaObjects + playerObjectIndex * 0x27 + 0x0B);
				const int playerPositionZ = *(int*)(xwaObjects + playerObjectIndex * 0x27 + 0x0F);

				std::string optCamera = g_flightModelsList.GetLstLine(playerModelIndex);
				optCamera.append("Camera.txt");

				if (std::ifstream(optCamera))
				{
					positionX = playerPositionX + GetFileKeyValueInt(optCamera, "X");
					positionY = playerPositionY + GetFileKeyValueInt(optCamera, "Y");
					positionZ = playerPositionZ + GetFileKeyValueInt(optCamera, "Z");
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

		std::string txtPath = GetCustomFilePath("FamHangarCamera.txt");

		switch (A4)
		{
		case 1:
			if (std::ifstream(txtPath))
			{
				positionX += GetFileKeyValueInt(txtPath, "FamKey1_X");
				positionY += GetFileKeyValueInt(txtPath, "FamKey1_Y");
				positionZ += GetFileKeyValueInt(txtPath, "FamKey1_Z");
			}
			else
			{
				positionX += 0x30C;
				positionY += -0x1947;
				positionZ += -0x1371;
			}
			break;

		case 2:
			if (std::ifstream(txtPath))
			{
				positionX += GetFileKeyValueInt(txtPath, "FamKey2_X");
				positionY += GetFileKeyValueInt(txtPath, "FamKey2_Y");
				positionZ += GetFileKeyValueInt(txtPath, "FamKey2_Z");
			}
			else
			{
				positionX += -0x7B2;
				positionY += -0x226A;
				positionZ += -0x1263;
			}
			break;

		case 3:
			if (std::ifstream(txtPath))
			{
				positionX += GetFileKeyValueInt(txtPath, "FamKey3_X");
				positionY += GetFileKeyValueInt(txtPath, "FamKey3_Y");
				positionZ += GetFileKeyValueInt(txtPath, "FamKey3_Z");
			}
			else
			{
				positionX += 0x9CE;
				positionY += -0x150F;
				positionZ += -0x13CB;
			}
			break;

		case 6:
			if (std::ifstream(txtPath))
			{
				positionX += GetFileKeyValueInt(txtPath, "FamKey6_X");
				positionY += GetFileKeyValueInt(txtPath, "FamKey6_Y");
				positionZ += GetFileKeyValueInt(txtPath, "FamKey6_Z");
			}
			else
			{
				positionX += 0x6CC;
				positionY += -0x210D;
				positionZ += -0x13B7;
			}
			break;

		case 7:
			if (std::ifstream(txtPath))
			{
				positionX += GetFileKeyValueInt(txtPath, "FamKey7_X");
				positionY += GetFileKeyValueInt(txtPath, "FamKey7_Y");
				positionZ += GetFileKeyValueInt(txtPath, "FamKey7_Z");
			}
			else
			{
				positionX += 0xC6C;
				positionY += 0xA45;
				positionZ += -0xEC1;
			}
			break;

		case 8:
			if (std::ifstream(txtPath))
			{
				positionX += GetFileKeyValueInt(txtPath, "FamKey8_X");
				positionY += GetFileKeyValueInt(txtPath, "FamKey8_Y");
				positionZ += GetFileKeyValueInt(txtPath, "FamKey8_Z");
			}
			else
			{
				positionX += 0x2032;
				positionY += 0x19C8;
				positionZ += 0x0A;
			}
			break;

		case 9:
			if (std::ifstream(txtPath))
			{
				positionX += GetFileKeyValueInt(txtPath, "FamKey9_X");
				positionY += GetFileKeyValueInt(txtPath, "FamKey9_Y");
				positionZ += GetFileKeyValueInt(txtPath, "FamKey9_Z");
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
				if (std::ifstream(txtPath))
				{
					positionX += GetFileKeyValueInt(txtPath, "FamKey3_X");
					positionY += GetFileKeyValueInt(txtPath, "FamKey3_Y");
					positionZ += GetFileKeyValueInt(txtPath, "FamKey3_Z");
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
				const unsigned short playerModelIndex = *(unsigned short*)(xwaObjects + playerObjectIndex * 0x27 + 0x02);
				const int playerPositionX = *(int*)(xwaObjects + playerObjectIndex * 0x27 + 0x07);
				const int playerPositionY = *(int*)(xwaObjects + playerObjectIndex * 0x27 + 0x0B);
				const int playerPositionZ = *(int*)(xwaObjects + playerObjectIndex * 0x27 + 0x0F);

				std::string optCamera = g_flightModelsList.GetLstLine(playerModelIndex);
				optCamera.append("Camera.txt");

				if (std::ifstream(optCamera))
				{
					positionX = playerPositionX + GetFileKeyValueInt(optCamera, "X");
					positionY = playerPositionY + GetFileKeyValueInt(optCamera, "Y");
					positionZ = playerPositionZ + GetFileKeyValueInt(optCamera, "Z");
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

int HangarMapHook(int* params)
{
	const auto AddObject = (short(*)(unsigned short, int, int, int, unsigned short, unsigned short))0x00456AE0;

	std::string txtPath = GetCustomFilePath("HangarMap.txt");

	if (std::ifstream(txtPath))
	{
		auto values = GetFileListValues(txtPath);

		for (unsigned int i = 0; i < values.size(); i++)
		{
			auto& value = values[i];

			if (value.size() < 6)
			{
				continue;
			}

			unsigned short modelIndex = static_cast<unsigned short>(std::stoi(value[0], 0, 0));
			int positionX = std::stoi(value[1], 0, 0);
			int positionY = std::stoi(value[2], 0, 0);
			int positionZ = std::stoi(value[3], 0, 0);
			short orientationXY = static_cast<short>(std::stoi(value[4], 0, 0));
			short orientationZ = static_cast<short>(std::stoi(value[5], 0, 0));

			short objectIndex = AddObject(modelIndex, positionX, positionY, positionZ, orientationXY, orientationZ);

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
	const auto AddObject = (short(*)(unsigned short, int, int, int, unsigned short, unsigned short))0x00456AE0;

	std::string txtPath = GetCustomFilePath("FamHangarMap.txt");

	if (std::ifstream(txtPath))
	{
		auto values = GetFileListValues(txtPath);

		for (unsigned int i = 0; i < values.size(); i++)
		{
			auto& value = values[i];

			if (value.size() < 6)
			{
				continue;
			}

			unsigned short modelIndex = static_cast<unsigned short>(std::stoi(value[0], 0, 0));
			int positionX = std::stoi(value[1], 0, 0);
			int positionY = std::stoi(value[2], 0, 0);
			int positionZ = std::stoi(value[3], 0, 0);
			short orientationXY = static_cast<short>(std::stoi(value[4], 0, 0));
			short orientationZ = static_cast<short>(std::stoi(value[5], 0, 0));

			short objectIndex = AddObject(modelIndex, positionX, positionY, positionZ, orientationXY, orientationZ);

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

	const auto mode = GetFileKeyValue("hook_hangars.cfg", "SelectionMode");
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
	short modelIndex = (short)params[0];

	const auto mode = GetFileKeyValue("hook_hangars.cfg", "SelectionMode");

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
	short modelIndex = (short)params[0];

	const auto mode = GetFileKeyValue("hook_hangars.cfg", "SelectionMode");

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

	short modelIndex = (short)params[0];

	std::string optSize = g_flightModelsList.GetLstLine(modelIndex);
	optSize.append("Size.txt");

	if (std::ifstream(optSize))
	{
		return GetFileKeyValueInt(optSize, "ClosedSFoilsElevation");
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
