#include "targetver.h"
#include "engine_sound.h"
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>

std::string GetStringWithoutExtension(const std::string& str)
{
	auto b = str.find_last_of('.');

	return str.substr(0, b);
}

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

std::string GetFlightModelsLstLine(int modelIndex)
{
	const int xwaObjectStats = 0x05FB240;
	const int dataIndex1 = *(short*)(xwaObjectStats + modelIndex * 0x18 + 0x14);
	const int dataIndex2 = *(short*)(xwaObjectStats + modelIndex * 0x18 + 0x16);

	if (dataIndex1 != 0 && dataIndex1 != 1)
	{
		return std::string();
	}

	std::string lstPath;
	lstPath.append("FlightModels\\");

	std::string lstFilesNames[] =
	{
		"Spacecraft0.LST",
		"Equipment0.LST",
	};

	lstPath.append(lstFilesNames[dataIndex1]);

	std::ifstream lstFile(lstPath);

	if (lstFile)
	{
		std::string line;

		for (int i = 0; i <= dataIndex2; i++)
		{
			std::getline(lstFile, line);
		}

		return GetStringWithoutExtension(line);
	}

	return std::string();
}

int GetEngineSoundTypeInterior(int modelIndex)
{
	std::string pilotPath = GetFlightModelsLstLine(modelIndex);

	if (!pilotPath.empty())
	{
		pilotPath.append("Sound.txt");
	}

	int type = 0;

	if (!pilotPath.empty() && std::ifstream(pilotPath))
	{
		type = GetFileKeyValueInt(pilotPath, "EngineSoundInterior");
	}
	else
	{
		switch (modelIndex)
		{
		case 1: // ModelIndex_001_0_0_Xwing
		case 4: // ModelIndex_004_0_3_Bwing
		case 14: // ModelIndex_014_0_13_Z_95
		case 15: // ModelIndex_015_0_14_R41
		case 31: // ModelIndex_031_0_30_SlaveOne
			type = 1;
			break;

		case 2: // ModelIndex_002_0_1_Ywing
		case 11: // ModelIndex_011_0_10_ToscanFighter
		case 23: // ModelIndex_023_0_22_CloakshapeFighter
			type = 2;
			break;

		case 3: // ModelIndex_003_0_2_Awing
		case 10: // ModelIndex_010_0_9_IrdFighter
		case 13: // ModelIndex_013_0_12_Twing
		case 27: // ModelIndex_027_0_26_Piggyback
			type = 3;
			break;

		case 5: // ModelIndex_005_0_4_TieFighter
		case 6: // ModelIndex_006_0_5_TieInterceptor
		case 7: // ModelIndex_007_0_6_TieBomber
		case 8: // ModelIndex_008_0_7_TieAdvanced
		case 9: // ModelIndex_009_0_8_TieDefender
			type = 4;
			break;

		case 12: // ModelIndex_012_0_11_MissileBoat
		case 16: // ModelIndex_016_0_15_AssaultGunboat
		case 24: // ModelIndex_024_0_23_RazorFighter
		case 25: // ModelIndex_025_0_24_PlanetaryFighter
		case 29: // ModelIndex_029_0_28_PreybirdFighter
			type = 5;
			break;

		case 17: // ModelIndex_017_0_16_SkiprayBlastBoat
		case 58: // ModelIndex_058_0_45_CorellianTransport2
		case 65: // ModelIndex_065_0_52_FamilyTransport
			type = 6;
			break;

		case 26: // ModelIndex_026_0_25_SupaFighter
		case 32: // ModelIndex_032_0_31_SlaveTwo
		case 59: // ModelIndex_059_0_46_MilleniumFalcon2
			type = 7;
			break;
		}
	}

	return type;
}

int GetEngineSoundTypeFlyBy(int modelIndex)
{
	std::string pilotPath = GetFlightModelsLstLine(modelIndex);

	if (!pilotPath.empty())
	{
		pilotPath.append("Sound.txt");
	}

	int type = 0;

	if (!pilotPath.empty() && std::ifstream(pilotPath))
	{
		type = GetFileKeyValueInt(pilotPath, "EngineSoundFlyBy");
	}
	else
	{
		switch (modelIndex)
		{
		case 1: // ModelIndex_001_0_0_Xwing
		case 4: // ModelIndex_004_0_3_Bwing
		case 14: // ModelIndex_014_0_13_Z_95
		case 15: // ModelIndex_015_0_14_R41
		case 31: // ModelIndex_031_0_30_SlaveOne
			type = 1;
			break;

		case 2: // ModelIndex_002_0_1_Ywing
		case 11: // ModelIndex_011_0_10_ToscanFighter
		case 23: // ModelIndex_023_0_22_CloakshapeFighter
			type = 2;
			break;

		case 3: // ModelIndex_003_0_2_Awing
		case 10: // ModelIndex_010_0_9_IrdFighter
		case 13: // ModelIndex_013_0_12_Twing
		case 27: // ModelIndex_027_0_26_Piggyback
			type = 3;
			break;

		case 5: // ModelIndex_005_0_4_TieFighter
		case 6: // ModelIndex_006_0_5_TieInterceptor
		case 7: // ModelIndex_007_0_6_TieBomber
		case 8: // ModelIndex_008_0_7_TieAdvanced
		case 9: // ModelIndex_009_0_8_TieDefender
			type = 4;
			break;

		case 12: // ModelIndex_012_0_11_MissileBoat
		case 16: // ModelIndex_016_0_15_AssaultGunboat
		case 24: // ModelIndex_024_0_23_RazorFighter
		case 25: // ModelIndex_025_0_24_PlanetaryFighter
		case 29: // ModelIndex_029_0_28_PreybirdFighter
		case 43: // ModelIndex_043_0_32_Tug
		case 44: // ModelIndex_044_0_33_CombatUtilityVehicle
		case 45: // ModelIndex_045_0_34_HeavyLifter
		case 50: // ModelIndex_050_0_37_Shuttle
		case 51: // ModelIndex_051_0_38_EscortShuttle
		case 52: // ModelIndex_052_0_39_StormtrooperTransport
		case 53: // ModelIndex_053_0_40_AssaultTransport
		case 54: // ModelIndex_054_0_41_EscortTransport
		case 55: // ModelIndex_055_0_42_SystemPatrolCraft
		case 56: // ModelIndex_056_0_43_ScoutCraft
			type = 5;
			break;

		case 17: // ModelIndex_017_0_16_SkiprayBlastBoat
		case 58: // ModelIndex_058_0_45_CorellianTransport2
		case 65: // ModelIndex_065_0_52_FamilyTransport
			type = 6;
			break;

		case 26: // ModelIndex_026_0_25_SupaFighter
		case 32: // ModelIndex_032_0_31_SlaveTwo
		case 59: // ModelIndex_059_0_46_MilleniumFalcon2
			// type 6
			type = 7;
			break;
		}
	}

	return type;
}

int InteriorSoundHook(int* params)
{
	const int modelIndex = params[0];
	int& esi = params[1];
	int& ebp = params[2];
	int& ebx = params[3];

	const char sfx_quality = *(char*)0xB0C890;

	const int type = GetEngineSoundTypeInterior(modelIndex);

	switch (type)
	{
	case 1:
		esi = 0x5E; // EngineIntXW
		ebp = 0x2AF8;
		break;

	case 2:
		esi = 0x5F; // EngineIntYW
		ebp = 0x2AF8;
		break;

	case 3:
		esi = 0x60; // EngineIntAW
		ebp = 0x157C;
		break;

	case 4:
		esi = 0x61; // EngineIntTI
		ebp = 0x2AF8;
		break;

	case 5:
		esi = 0x62; // EngineIntAG
		ebp = 0x2AF8;
		break;

	case 6:
		esi = 0x63; // EngineIntCort
		ebp = 0x2AF8;
		break;

	case 7:
		esi = 0x64; // EngineIntFalc
		ebp = 0x2AF8;
		break;
	}

	if (ebp == 0x2AF8)
	{
		if (sfx_quality != 0)
		{
			ebx = 0x01;
		}
	}

	return 0;
}

int StopInteriorSoundHook(int* params)
{
	const int modelIndex = params[0];
	int& esi = params[1];

	const int type = GetEngineSoundTypeInterior(modelIndex);

	switch (type)
	{
	case 1:
		esi = 0x5E; // EngineIntXW
		break;

	case 2:
		esi = 0x5F; // EngineIntYW
		break;

	case 3:
		esi = 0x60; // EngineIntAW
		break;

	case 4:
		esi = 0x61; // EngineIntTI
		break;

	case 5:
		esi = 0x62; // EngineIntAG
		break;

	case 6:
		esi = 0x63; // EngineIntCort
		break;

	case 7:
		esi = 0x64; // EngineIntFalc
		break;
	}

	return 0;
}

int FlyBySoundHook(int* params)
{
	const int modelIndex = params[0];
	int& esi = params[1];

	const char xwaSound3dEnabled = *(char*)0x5BA990;
	const int* xwaSoundEffectsBufferId = (int*)0x917E80;

	const int type = GetEngineSoundTypeFlyBy(modelIndex);

	switch (type)
	{
	case 1:
		if (xwaSound3dEnabled == 0 || xwaSoundEffectsBufferId[0x6A] == -1)
		{
			esi = 0x69; // FlyByXW
		}
		else
		{
			esi = 0x6A; // FlyByXW3D
		}

		break;

	case 2:
		if (xwaSound3dEnabled == 0 || xwaSoundEffectsBufferId[0x6C] == -1)
		{
			esi = 0x6B; // FlyByYW
		}
		else
		{
			esi = 0x6C; // FlyByYW3D
		}

		break;

	case 3:
		if (xwaSound3dEnabled == 0 || xwaSoundEffectsBufferId[0x6E] == -1)
		{
			esi = 0x6D; // FlyByAW
		}
		else
		{
			esi = 0x6E; // FlyByAW3D
		}

		break;

	case 4:
		if (xwaSound3dEnabled == 0 || xwaSoundEffectsBufferId[0x66] == -1)
		{
			esi = 0x65; // FlyByTI
		}
		else
		{
			esi = 0x66; // FlyByTI3D
		}

		break;

	case 5:
		if (xwaSound3dEnabled == 0 || xwaSoundEffectsBufferId[0x68] == -1)
		{
			esi = 0x67; // FlyByAG
		}
		else
		{
			esi = 0x68; // FlyByAG3D
		}

		break;

	case 6:
	case 7:
		if (xwaSound3dEnabled == 0 || xwaSoundEffectsBufferId[0x70] == -1)
		{
			esi = 0x6F; // FlyByCort
		}
		else
		{
			esi = 0x70; // FlyByCort3D
		}

		break;

	case 8:
		if (xwaSound3dEnabled == 0 || xwaSoundEffectsBufferId[0x82] == -1)
		{
			esi = 0x81; // unused
		}
		else
		{
			esi = 0x82; // unused3D
		}

		break;
	}

	return 0;
}

int LaunchSoundHook(int* params)
{
	const int modelIndex = params[0];
	int& eax = params[1];

	const char xwaSound3dEnabled = *(char*)0x5BA990;
	const int* xwaSoundEffectsBufferId = (int*)0x917E80;

	const int type = GetEngineSoundTypeFlyBy(modelIndex);

	switch (type)
	{
	case 1:
		if (xwaSound3dEnabled == 0 || xwaSoundEffectsBufferId[0x6A] == -1)
		{
			eax = 0x69; // FlyByXW
		}
		else
		{
			eax = 0x6A; // FlyByXW3D
		}

		break;

	case 2:
		if (xwaSound3dEnabled == 0 || xwaSoundEffectsBufferId[0x6C] == -1)
		{
			eax = 0x6B; // FlyByYW
		}
		else
		{
			eax = 0x6C; // FlyByYW3D
		}

		break;

	case 3:
		if (xwaSound3dEnabled == 0 || xwaSoundEffectsBufferId[0x6E] == -1)
		{
			eax = 0x6D; // FlyByAW
		}
		else
		{
			eax = 0x6E; // FlyByAW3D
		}

		break;

	case 4:
		if (xwaSound3dEnabled == 0 || xwaSoundEffectsBufferId[0x66] == -1)
		{
			eax = 0x65; // FlyByTI
		}
		else
		{
			eax = 0x66; // FlyByTI3D
		}

		break;

	case 5:
		if (xwaSound3dEnabled == 0 || xwaSoundEffectsBufferId[0x68] == -1)
		{
			eax = 0x67; // FlyByAG
		}
		else
		{
			eax = 0x68; // FlyByAG3D
		}

		break;

	case 6:
	case 7:
		if (xwaSound3dEnabled == 0 || xwaSoundEffectsBufferId[0x70] == -1)
		{
			eax = 0x6F; // FlyByCort
		}
		else
		{
			eax = 0x70; // FlyByCort3D
		}

		break;

	case 8:
		if (xwaSound3dEnabled == 0 || xwaSoundEffectsBufferId[0x82] == -1)
		{
			eax = 0x81; // unused
		}
		else
		{
			eax = 0x82; // unused3D
		}

		break;
	}

	return 0;
}
