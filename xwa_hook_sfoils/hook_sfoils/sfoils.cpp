#include "targetver.h"
#include "sfoils.h"
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
	char unk000[39];
	char SFoilsState;
	char unk028[568];
	char MeshRotationAngles[50];
	char unk292[359];
};

static_assert(sizeof(XwaCraft) == 1017, "size of XwaCraft must be 1017");

#pragma pack(pop)

struct SFoil
{
	int meshIndex;
	int angle;
	int closingSpeed;
	int openingSpeed;
};

std::vector<SFoil> GetFileListSFoils(const std::string& path)
{
	auto sfoils = GetFileListValues(path);

	std::vector<SFoil> values;

	for (unsigned int i = 0; i < sfoils.size(); i++)
	{
		const auto& value = sfoils[i];

		if (value.size() < 4)
		{
			continue;
		}

		SFoil sfoil;
		sfoil.meshIndex = std::stoi(value[0], 0, 0);
		sfoil.angle = std::stoi(value[1], 0, 0);
		sfoil.closingSpeed = std::stoi(value[2], 0, 0);
		sfoil.openingSpeed = std::stoi(value[3], 0, 0);

		values.push_back(sfoil);
	}

	return values;
}

std::vector<SFoil> GetDefaultSFoils(int modelIndex)
{
	std::vector<SFoil> values;

	switch (modelIndex)
	{
	case 1: // ModelIndex_001_0_0_Xwing
		values.push_back({ 1, 8, 1, 1 });
		values.push_back({ 2, 12, 1, 1 });
		values.push_back({ 3, 12, 1, 1 });
		values.push_back({ 4, 8, 1, 1 });
		break;

	case 4: // ModelIndex_004_0_3_Bwing
		values.push_back({ 0, 64, 2, 2 });
		values.push_back({ 3, 64, 2, 2 });
		values.push_back({ 5, 64, 2, 2 });
		values.push_back({ 6, 64, 2, 2 });
		break;

	case 50: // ModelIndex_050_0_37_Shuttle
		values.push_back({ 1, 96, 2, 2 });
		values.push_back({ 2, 96, 2, 2 });
		break;
	}

	return values;
}

std::vector<SFoil> GetSFoils(int modelIndex)
{
	std::vector<SFoil> sfoils;

	std::string sfoilsPath = GetFlightModelsLstLine(modelIndex);

	if (sfoilsPath.empty())
	{
		return sfoils;
	}

	sfoilsPath.append("SFoils.txt");

	if (std::ifstream(sfoilsPath))
	{
		sfoils = GetFileListSFoils(sfoilsPath);
	}
	else
	{
		sfoils = GetDefaultSFoils(modelIndex);
	}

	return sfoils;
}

int SFoilsHook1(int* params)
{
	const int modelIndex = params[0];

	XwaCraft* currentCraft = *(XwaCraft**)0x0910DFC;

	auto sfoils = GetSFoils(modelIndex);

	if (sfoils.empty())
	{
		currentCraft->SFoilsState = 0;
		return 0;
	}

	for (unsigned int i = 0; i < sfoils.size(); i++)
	{
		const auto& sfoil = sfoils[i];

		currentCraft->MeshRotationAngles[sfoil.meshIndex] = sfoil.angle;
	}

	currentCraft->SFoilsState = 2;

	return 0;
}

int SFoilsHook2(int* params)
{
	const int modelIndex = params[0];

	XwaCraft* currentCraft = *(XwaCraft**)0x0910DFC;

	auto sfoils = GetSFoils(modelIndex);

	if (sfoils.empty())
	{
		currentCraft->SFoilsState = 0;
		return 0;
	}

	int ret = 0;

	for (unsigned int i = 0; i < sfoils.size(); i++)
	{
		const auto& sfoil = sfoils[i];

		if ((currentCraft->SFoilsState & 1) != 0)
		{
			if ((currentCraft->SFoilsState & 2) != 0)
			{
				if (currentCraft->MeshRotationAngles[sfoil.meshIndex] < sfoil.angle)
				{
					currentCraft->MeshRotationAngles[sfoil.meshIndex] += sfoil.closingSpeed;

					if (currentCraft->MeshRotationAngles[sfoil.meshIndex] > sfoil.angle)
					{
						currentCraft->MeshRotationAngles[sfoil.meshIndex] = sfoil.angle;
					}

					ret = 1;
				}
			}
			else
			{
				if (currentCraft->MeshRotationAngles[sfoil.meshIndex] > 0)
				{
					currentCraft->MeshRotationAngles[sfoil.meshIndex] -= sfoil.openingSpeed;

					if (currentCraft->MeshRotationAngles[sfoil.meshIndex] < 0)
					{
						currentCraft->MeshRotationAngles[sfoil.meshIndex] = 0;
					}

					ret = 1;
				}
			}
		}
	}

	return ret;
}
