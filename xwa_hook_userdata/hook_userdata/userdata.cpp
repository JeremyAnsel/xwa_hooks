#include "targetver.h"
#include "userdata.h"
#include <string>
#include <fstream>
#include <direct.h>

enum ParamsEnum
{
	Params_ReturnAddress = -1,
	Params_EAX = -3,
	Params_ECX = -4,
	Params_EDX = -5,
	Params_EBX = -6,
	Params_EBP = -8,
	Params_ESI = -9,
	Params_EDI = -10,
};

std::string Trim(const std::string& str)
{
	const char* ws = " \t\n\r\f\v";
	std::string s = str;

	s.erase(str.find_last_not_of(ws) + 1);
	s.erase(0, s.find_first_not_of(ws));

	return s;
}

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

int OpenConfigHook(int* params)
{
	const char* A4 = (const char*)params[0];
	const char* A8 = (const char*)params[1];

	const auto XwaSetConfigPerformanceHeight = (int(*)(int))0x00528470;
	const auto XwaFileOpen = (int(*)(const char*, const char*))0x0052AD30;

	XwaSetConfigPerformanceHeight(-1);

	// use_3d_hardware
	((unsigned char*)(0x00B0C7A0 + 0x001C))[0] = 1;
	((unsigned char*)(0x00B0C7A0 + 0x001C))[1] = 1;

	// hardware_mipmap
	((unsigned char*)(0x00B0C7A0 + 0x002A))[0] = 2;
	((unsigned char*)(0x00B0C7A0 + 0x002A))[1] = 2;

	// number_of_sfx
	*(unsigned char*)(0x00B0C7A0 + 0x013D) = 32;

	// sfx_quality
	*(unsigned char*)(0x00B0C7A0 + 0x00F0) = 1;

	int result = XwaFileOpen(A4, A8);

	return result;
}

int SplitConfigLineHook(int* params)
{
	static char s_key[256];
	static char s_value[256];

	char* s_XwaTempString = (char*)0x00ABD680;

	std::string line{ s_XwaTempString };
	line = Trim(line);

	int pos = line.find("=");

	if (pos == -1)
	{
		pos = line.find(" ");
	}

	if (pos == -1)
	{
		strcpy_s(s_key, line.c_str());
		s_value[0] = 0;
	}
	else
	{
		std::string key = Trim(line.substr(0, pos));

		if (key.length() == 0)
		{
			s_key[0] = 0;
			s_value[0] = 0;
		}
		else
		{
			std::string value = Trim(line.substr(pos + 1));

			strcpy_s(s_key, key.c_str());
			strcpy_s(s_value, value.c_str());
		}
	}

	strcpy_s(s_XwaTempString, 256, s_key);
	params[Params_ESI] = (int)s_value;

	params[Params_ReturnAddress] = 0x005247BD;
	return 0;
}

int ConfigPresetsHook(int* params)
{
	int presetIndex = *(unsigned short*)0x008053C0 == 205 ? 0 : 1;
	params[Params_EAX] = presetIndex;

	int currentPlayerId = *(int*)0x008C1CC8;
	int pCraft = params[Params_EBX];
	int pPlayer = params[Params_ESI];
	int playerIndex = pPlayer / 0xBCF;

	if (playerIndex == currentPlayerId)
	{
		unsigned short craft_m183 = *(unsigned short*)(pCraft + 0x0183);

		unsigned char preset_throttle = *(unsigned char*)(0x0B0C7A0 + 0x0261 + presetIndex);
		*(unsigned short*)(pCraft + 0x00F0) = (int)preset_throttle * 65535 / 100;

		unsigned char preset_laser = *(unsigned char*)(0x0B0C7A0 + 0x0263 + presetIndex);
		*(unsigned char*)(pCraft + 0x01AD) = preset_laser;

		if ((craft_m183 & 0x01) != 0)
		{
			unsigned char preset_shield = *(unsigned char*)(0x0B0C7A0 + 0x0265 + presetIndex);
			*(unsigned char*)(pCraft + 0x01AA) = preset_shield;
		}

		if ((craft_m183 & 0x100) != 0)
		{
			unsigned char preset_beam = *(unsigned char*)(0x0B0C7A0 + 0x0267 + presetIndex);
			*(unsigned char*)(pCraft + 0x01DF) = preset_beam;
		}

		params[Params_ReturnAddress] = 0x004FD720;
	}

	return 0;
}
