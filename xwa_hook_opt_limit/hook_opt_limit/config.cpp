#include "targetver.h"
#include "config.h"
#include <fstream>
#include <algorithm>
#include <cctype>

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
