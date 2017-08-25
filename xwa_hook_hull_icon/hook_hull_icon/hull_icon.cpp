#include "targetver.h"
#include "hull_icon.h"
#include <string>
#include <fstream>
#include <vector>

std::vector<unsigned short> GetFileListUnsignedShortValues(const std::string& path)
{
	std::vector<unsigned short> values;

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

			unsigned short value = (unsigned short)std::stoi(line);
			values.push_back(value);
		}
	}

	return values;
}

std::vector<unsigned short> GetDefaultHullIconValues()
{
	const auto hudIcons = (const unsigned short*)0x005A9608;

	std::vector<unsigned short> values;

	for (int i = 0; i < 72; i++)
	{
		values.push_back(hudIcons[i]);
	}

	return values;
}

class HullIconValues
{
public:
	HullIconValues()
	{
		if (std::ifstream("HullIconList.txt"))
		{
			hullIcons = GetFileListUnsignedShortValues("HullIconList.txt");
		}
		else
		{
			hullIcons = GetDefaultHullIconValues();
		}
	}

	std::vector<unsigned short> hullIcons;
};

HullIconValues g_HullIconValues;

int HullIconHook(int* params)
{
	const unsigned short modelIndex = (unsigned short)params[0];

	if (modelIndex < g_HullIconValues.hullIcons.size())
	{
		return g_HullIconValues.hullIcons[modelIndex];
	}

	return 0;
}
