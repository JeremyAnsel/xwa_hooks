#include "targetver.h"
#include "userdata.h"
#include <string>
#include <fstream>
#include <direct.h>

std::string GetPilotDirectory()
{
	std::string line;
	std::ifstream file("modname.txt");

	if (file)
	{
		if (!std::getline(file, line))
		{
			return std::string();
		}

		_mkdir("UserData");
		_mkdir(("UserData\\" + line).c_str());
		_mkdir(("UserData\\" + line + "\\Pilot").c_str());

		return "UserData\\" + line + "\\Pilot";
	}

	return std::string();
}

char g_currentDirectory[260];

int CreatePilotHook(int* params)
{
	const char* pilot = (const char*)params[0];

	const auto XwaCreatePilot = (int(*)(const char*))0x00528CF0;

	_getcwd(g_currentDirectory, sizeof(g_currentDirectory));
	std::string pilotDirectory = GetPilotDirectory();
	_chdir(pilotDirectory.c_str());

	int ret = XwaCreatePilot(pilot);

	return ret;
}

int CreatePilot2Hook(int* params)
{
	const char* A4 = (const char*)params[0];
	const char* A8 = (const char*)params[1];

	const auto XwaFileOpen = (int(*)(const char*, const char*))0x0052AD30;

	int ret = XwaFileOpen(A4, A8);

	_chdir(g_currentDirectory);

	return ret;
}

int SavePilotHook(int* params)
{
	const int A4 = params[0];

	const auto XwaSavePilot = (int(*)(int))0x00529090;

	_getcwd(g_currentDirectory, sizeof(g_currentDirectory));
	std::string pilotDirectory = GetPilotDirectory();
	_chdir(pilotDirectory.c_str());

	int ret = XwaSavePilot(A4);

	_chdir(g_currentDirectory);

	return ret;
}

int OpenPilotHook(int* params)
{
	const char* pilot = (const char*)params[0];

	const auto XwaOpenPilot = (int(*)(const char*))0x00529200;

	_getcwd(g_currentDirectory, sizeof(g_currentDirectory));
	std::string pilotDirectory = GetPilotDirectory();
	_chdir(pilotDirectory.c_str());

	int ret = XwaOpenPilot(pilot);

	_chdir(g_currentDirectory);

	return ret;
}
