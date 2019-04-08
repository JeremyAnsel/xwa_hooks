#include "targetver.h"
#include "tie.h"
#include "config.h"

#pragma pack(push, 1)

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

int TieHook(int* params)
{
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
