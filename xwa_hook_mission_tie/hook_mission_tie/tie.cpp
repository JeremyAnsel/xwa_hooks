#include "targetver.h"
#include "tie.h"
#include "config.h"

#pragma pack(push, 1)

struct XwaMobileObject
{
	char Unk0000[149];
	unsigned short ModelIndex;
	char Unk0097[2];
	unsigned char Markings;
	char Unk009A[75];
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
	char unk000[112];
	unsigned char Iff;
	char unk071[2];
	char Markings;
	char unk074[3465];
	char PilotVoice[20];
	char unkE11[49];
};

static_assert(sizeof(TieFlightGroupEx) == 3650, "size of TieFlightGroupEx must be 3650");

#pragma pack(pop)

TieFlightGroupEx* s_XwaTieFlightGroups = (TieFlightGroupEx*)0x80DC80;

std::vector<std::vector<std::string>> g_tieLines;

int TieHook(int* params)
{
	// init music state
	// fix a bug where a mission starts with wait state instead of intro state
	*(unsigned char*)0x0069406C = 0;

	g_tieLines.clear();

	const auto LoadMission = (int(*)(const char*))0x420300;
	const char* fileName = (const char*)params[0];

	if (LoadMission(fileName) == 0)
	{
		return 0;
	}

	const std::string path = GetStringWithoutExtension(fileName);
	auto file = GetFileLines(path + ".txt");

	if (!file.size())
	{
		file = GetFileLines(path + ".ini", "mission_tie");
	}

	if (!file.size())
	{
		return 1;
	}

	const auto lines = GetFileListValues(file);

	if (!lines.size())
	{
		return 1;
	}

	g_tieLines = lines;

	for (const auto& line : lines)
	{
		const auto& group = line[0];

		if (_stricmp(group.c_str(), "fg") == 0)
		{
			if (line.size() < 4)
			{
				continue;
			}

			int fg = std::stoi(line[1]);

			if (fg < 0 || fg > 192)
			{
				continue;
			}

			const auto& element = line[2];

			if (_stricmp(element.c_str(), "markings") == 0)
			{
				int value = std::stoi(line[3]);
				s_XwaTieFlightGroups[fg].Markings = value;
			}
			else if (_stricmp(element.c_str(), "iff") == 0)
			{
				int value = std::stoi(line[3]);
				s_XwaTieFlightGroups[fg].Iff = value;
			}
			else if (_stricmp(element.c_str(), "pilotvoice") == 0)
			{
				std::string value = line[3];
				size_t len = value.copy(s_XwaTieFlightGroups[fg].PilotVoice, sizeof(s_XwaTieFlightGroups[fg].PilotVoice) - 1);
				s_XwaTieFlightGroups[fg].PilotVoice[len] = 0;
			}
		}
	}

	return 1;
}

int CreateObjectHook(int* params)
{
	const short objectIndex = (short)params[10];
	const short A8 = (short)params[23];

	XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;
	short tieFlightgroupIndex = *(short*)0x09E9708;
	short wingmanIndex = *(short*)0x09E96EC;

	if (A8 == -1)
	{
		for (const auto& line : g_tieLines)
		{
			const auto& group = line[0];

			if (_stricmp(group.c_str(), "fg") == 0)
			{
				if (line.size() < 6)
				{
					continue;
				}

				int fg = std::stoi(line[1]);

				if (fg < 0 || fg > 192)
				{
					continue;
				}

				if (fg != tieFlightgroupIndex)
				{
					continue;
				}

				const auto& element = line[2];

				if (_stricmp(element.c_str(), "index") == 0)
				{
					int wingman = std::stoi(line[3]);

					if (wingman == wingmanIndex)
					{
						if (_stricmp(line[4].c_str(), "markings") == 0)
						{
							int value = std::stoi(line[5]);
							xwaObjects[objectIndex].pMobileObject->Markings = value;
						}
					}
				}
			}
		}

	}

	return objectIndex;
}