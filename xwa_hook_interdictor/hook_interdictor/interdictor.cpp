#include "targetver.h"
#include "interdictor.h"
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <vector>
#include <map>
#include <utility>

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

int IsShipInterdictor(int modelIndex)
{
	std::string shipPath = g_flightModelsList.GetLstLine(modelIndex);

	if (!shipPath.empty())
	{
		shipPath.append("Interdictor.txt");
	}

	if (!shipPath.empty() && std::ifstream(shipPath))
	{
		return GetFileKeyValueInt(shipPath, "IsInterdictor");
	}
	else
	{
		// ModelIndex_137_0_91_Interdictor2, ModelIndex_141_0_95_ModStrikeCruiser
		if (modelIndex == 0x89 || modelIndex == 0x8D)
		{
			return 1;
		}
	}

	return 0;
}

class ModelIndexInterdictor
{
public:
	int IsInterdictor(int modelIndex)
	{
		const auto it = this->_interdictor.find(modelIndex);

		if (it != this->_interdictor.end())
		{
			return it->second;
		}
		else
		{
			int value = IsShipInterdictor(modelIndex);
			this->_interdictor.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

private:
	std::map<int, int> _interdictor;
};

ModelIndexInterdictor g_modelIndexInterdictor;

int InterdictorHook(int* params)
{
	unsigned short modelIndex = (unsigned short)params[0];

	if (modelIndex == 0)
	{
		return 0;
	}

	return g_modelIndexInterdictor.IsInterdictor(modelIndex);
}
