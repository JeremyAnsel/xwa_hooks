#include "logger.h"

#include <fstream>
#include <ctime>

clock_t g_startTime = clock();

std::ofstream g_file;

void InitLogFile()
{
	if (g_file.is_open())
	{
		return;
	}

	std::ofstream file;
	file.open("logger.txt");

	if (file.is_open())
	{
		file.close();
	}

	g_file.open("logger.txt", std::ios::app);
}

void LogText(std::string text)
{
	InitLogFile();

	int currentTime = clock() - g_startTime;

	g_file << currentTime << '\t' << text.c_str() << std::endl;
}
