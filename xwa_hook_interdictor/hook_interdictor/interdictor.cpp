#include "targetver.h"
#include "interdictor.h"
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

int IsShipInterdictor(int modelIndex)
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

	lines = GetFileLines(shipPath + "Interdictor.txt");

	if (!lines.size())
	{
		lines = GetFileLines(shipPath + ".ini", "Interdictor");
	}

	if (lines.size())
	{
		return GetFileKeyValueInt(lines, "IsInterdictor");
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
		this->Update();

		auto it = this->_interdictor.find(modelIndex);

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

			this->_interdictor.clear();
		}
	}

	std::map<int, int> _interdictor;
};

ModelIndexInterdictor g_modelIndexInterdictor;

int InterdictorHook(int* params)
{
	const unsigned short modelIndex = (unsigned short)params[0];

	if (modelIndex == 0)
	{
		return 0;
	}

	return g_modelIndexInterdictor.IsInterdictor(modelIndex);
}
