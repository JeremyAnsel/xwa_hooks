#include "targetver.h"
#include "hull_icon.h"
#include "config.h"
#include <fstream>

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
			const auto lines = GetFileLines("HullIconList.txt");
			this->_hullIcons = GetFileListUnsignedShortValues(lines);
		}
		else
		{
			this->_hullIcons = GetDefaultHullIconValues();
		}
	}

	std::vector<unsigned short> _hullIcons;
};

HullIconValues g_hullIconValues;

int HullIconHook(int* params)
{
	const unsigned short modelIndex = (unsigned short)params[0];

	if (modelIndex < g_hullIconValues._hullIcons.size())
	{
		return g_hullIconValues._hullIcons[modelIndex];
	}

	return 0;
}
