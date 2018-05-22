#include "targetver.h"
#include "tie.h"
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

#pragma pack(push, 1)

struct TieFlightGroupEx
{
	char unk000[115];
	char Markings;
	char unk074[3534];
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

	std::string path = GetStringWithoutExtension(fileName);
	path.append(".txt");

	auto lines = GetFileListValues(path);

	if (lines.empty())
	{
		return 1;
	}

	for (const auto& line : lines)
	{
		if (line.size() < 4)
		{
			continue;
		}

		const auto& group = line[0];

		if (_stricmp(group.c_str(), "fg") == 0)
		{
			int fg = std::stoi(line[1]);

			if (fg < 0 || fg > 192)
			{
				continue;
			}

			const auto& element = line[2];
			int value = std::stoi(line[3]);

			if (_stricmp(element.c_str(), "markings") == 0)
			{
				s_XwaTieFlightGroups[fg].Markings = value;
			}
		}
	}

	return 1;
}
