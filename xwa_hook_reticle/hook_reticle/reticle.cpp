#include "targetver.h"
#include "reticle.h"
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

struct XwaObject
{
	char unk000[2];
	unsigned short ModelIndex;
	char unk004[35];
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

struct XwaPlayer
{
	int ObjectIndex;
	char unk004[3019];
};

static_assert(sizeof(XwaPlayer) == 3023, "size of XwaPlayer must be 3023");

struct XwaSpeciesTMInfo
{
	void* pData;
	unsigned short Width;
	unsigned short Height;
	unsigned int Color;
	unsigned char MipmapLevel;
};

static_assert(sizeof(XwaSpeciesTMInfo) == 13, "size of XwaSpeciesTMInfo must be 13");

#pragma pack(pop)

const int g_imageIndices[] = { 5, 6, 7, 8, 9, 10, 15, 16, 17, 18, 19, 20, 21, 22 };
const int g_inL0046BD00[] = { 5, 6, 7, 8, 9, 10 };
const int g_inL00477E50[] = { 15, 16, 17, 18, 19, 20, 21, 22 };
const int g_imageSizes[] = { 0, 0, 0, 0, 0, 64, 64, 64, 64, 8, 8, 0, 0, 0, 0, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0 };

std::vector<int> GetReticles(int modelIndex)
{
	std::vector<int> values;

	for (int i = 0; i < 25; i++)
	{
		values.push_back(i);
	}

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

	lines = GetFileLines(shipPath + "Reticle.txt");

	if (!lines.size())
	{
		lines = GetFileLines(shipPath + ".ini", "Reticle");
	}

	if (lines.size())
	{
		for (int index : g_imageIndices)
		{
			values[index] = GetFileKeyValueInt(lines, "Reticle_" + std::to_string(index), index);
		}
	}

	return values;
}

class ModelIndexReticle
{
public:
	int ReticleIndex(int modelIndex, int imageIndex)
	{
		this->Update();

		auto it = this->_reticles.find(modelIndex);

		if (it != this->_reticles.end())
		{
			if ((size_t)imageIndex >= it->second.size())
			{
				return -1;
			}

			return it->second[imageIndex];
		}
		else
		{
			auto value = GetReticles(modelIndex);
			this->_reticles.insert(std::make_pair(modelIndex, value));

			if ((size_t)imageIndex >= value.size())
			{
				return -1;
			}

			return value[imageIndex];
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

			this->_reticles.clear();
		}
	}

	std::map<int, std::vector<int>> _reticles;
};

ModelIndexReticle g_modelIndexReticle;

int SetReticleHook(int* params)
{
	const short imageModelIndex = (short)params[0];
	const short imageIndex = (short)params[1];
	const short AC = (short)params[2];

	const auto L00432750 = (void(*)(short, short, short))0x00432750;

	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	const XwaPlayer* xwaPlayers = (XwaPlayer*)0x08B94E0;
	const int currentPlayerId = *(int*)0x08C1CC8;

	unsigned short modelIndex = xwaObjects[xwaPlayers[currentPlayerId].ObjectIndex].ModelIndex;

	int reticleIndex = g_modelIndexReticle.ReticleIndex(modelIndex, imageIndex);

	if (reticleIndex == -1)
	{
		reticleIndex = imageIndex;
	}

	L00432750(imageModelIndex, reticleIndex, AC);

	const int xwaObjectStats = 0x05FB240;
	const XwaSpeciesTMInfo* pData1 = *(XwaSpeciesTMInfo**)(xwaObjectStats + imageModelIndex * 0x18 + 0x08);

	int size;

	if (pData1 == nullptr)
	{
		size = (int)(0x100 * *(float*)0x06002B8);
	}
	else
	{
		int oldSize = g_imageSizes[imageIndex];
		int newSize = pData1->Width;

		size = 0x100 * oldSize / newSize;
		size = (int)(size * *(float*)0x06002B8);
	}

	if (std::find(std::begin(g_inL0046BD00), std::end(g_inL0046BD00), imageIndex) != std::end(g_inL0046BD00))
	{
		// esp2C + 0x0E
		*(short*)((int)params + 0x0C + 0x2C + 0x0E) = (short)size;
	}
	else if (std::find(std::begin(g_inL00477E50), std::end(g_inL00477E50), imageIndex) != std::end(g_inL00477E50))
	{
		// esp10 + 0x0E
		*(short*)((int)params + 0x0C + 0x10 + 0x0E) = (short)size;
	}

	return 0;
}
