#include "targetver.h"
#include "engine_sound.h"
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <vector>
#include <map>
#include <utility>

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

std::vector<std::string> GetFileListValues(const std::string& path)
{
	std::vector<std::string> values;

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

			values.push_back(line);
		}
	}

	return values;
}

class FlightModelsList
{
public:
	FlightModelsList()
	{
		for (auto& line : GetFileListValues("FlightModels\\Spacecraft0.LST"))
		{
			this->_spacecraftList.push_back(GetStringWithoutExtension(line));
		}

		for (auto& line : GetFileListValues("FlightModels\\Equipment0.LST"))
		{
			this->_equipmentList.push_back(GetStringWithoutExtension(line));
		}
	}

	std::string GetLstLine(int modelIndex)
	{
		const int xwaObjectStats = 0x05FB240;
		const int dataIndex1 = *(short*)(xwaObjectStats + modelIndex * 0x18 + 0x14);
		const int dataIndex2 = *(short*)(xwaObjectStats + modelIndex * 0x18 + 0x16);

		switch (dataIndex1)
		{
		case 0:
			if ((unsigned int)dataIndex2 < this->_spacecraftList.size())
			{
				return this->_spacecraftList[dataIndex2];
			}

			break;

		case 1:
			if ((unsigned int)dataIndex2 < this->_equipmentList.size())
			{
				return this->_equipmentList[dataIndex2];
			}

			break;
		}

		return std::string();
	}

private:
	std::vector<std::string> _spacecraftList;
	std::vector<std::string> _equipmentList;
};

FlightModelsList g_flightModelsList;

int GetEngineSoundTypeInterior(int modelIndex)
{
	std::string pilotPath = g_flightModelsList.GetLstLine(modelIndex);

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
	std::string pilotPath = g_flightModelsList.GetLstLine(modelIndex);

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

std::string GetWeaponSoundBehavior(int modelIndex)
{
	std::string path = g_flightModelsList.GetLstLine(modelIndex);

	if (!path.empty())
	{
		path.append("Sound.txt");
	}

	std::string behavior;

	if (!path.empty() && std::ifstream(path))
	{
		behavior = GetFileKeyValue(path, "WeaponSoundBehavior");
	}
	else
	{
		switch (modelIndex)
		{
		case 58: // ModelIndex_058_0_45_CorellianTransport2
		case 59: // ModelIndex_059_0_46_MilleniumFalcon2
		case 65: // ModelIndex_065_0_52_FamilyTransport
			behavior = "CorellianTransport";
			break;

		case 5: // ModelIndex_005_0_4_TieFighter
		case 6: // ModelIndex_006_0_5_TieInterceptor
		case 7: // ModelIndex_007_0_6_TieBomber
		case 8: // ModelIndex_008_0_7_TieAdvanced
		case 9: // ModelIndex_009_0_8_TieDefender
		case 18: // ModelIndex_018_0_17_TieBizarro
		case 19: // ModelIndex_019_0_18_TieBigGun
		case 20: // ModelIndex_020_0_19_TieWarheads
		case 21: // ModelIndex_021_0_20_TieBomb
		case 22: //ModelIndex_022_0_21_TieBooster
			behavior = "TieFighter";
			break;

		default:
			behavior = std::string();
		}
	}

	return behavior;
}

class ModelIndexSound
{
public:
	int GetEngineSoundTypeInterior(int modelIndex)
	{
		auto it = this->_typeInterior.find(modelIndex);

		if (it != this->_typeInterior.end())
		{
			return it->second;
		}
		else
		{
			int value = GetEngineSoundTypeInterior(modelIndex);
			this->_typeInterior.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

	int GetEngineSoundTypeFlyBy(int modelIndex)
	{
		auto it = this->_typeFlyBy.find(modelIndex);

		if (it != this->_typeFlyBy.end())
		{
			return it->second;
		}
		else
		{
			int value = GetEngineSoundTypeFlyBy(modelIndex);
			this->_typeFlyBy.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

	std::string GetWeaponSoundBehavior(int modelIndex)
	{
		auto it = this->_weaponSoundBehavior.find(modelIndex);

		if (it != this->_weaponSoundBehavior.end())
		{
			return it->second;
		}
		else
		{
			std::string value = GetWeaponSoundBehavior(modelIndex);
			this->_weaponSoundBehavior.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

private:
	std::map<int, int> _typeInterior;
	std::map<int, int> _typeFlyBy;
	std::map<int, std::string> _weaponSoundBehavior;
};

ModelIndexSound g_modelIndexSound;

int InteriorSoundHook(int* params)
{
	const int modelIndex = params[0];
	int& esi = params[1];
	int& ebp = params[2];
	int& ebx = params[3];

	const char sfx_quality = *(char*)0xB0C890;

	const int type = g_modelIndexSound.GetEngineSoundTypeInterior(modelIndex);

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

	const int type = g_modelIndexSound.GetEngineSoundTypeInterior(modelIndex);

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

	const int type = g_modelIndexSound.GetEngineSoundTypeFlyBy(modelIndex);

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

	const int type = g_modelIndexSound.GetEngineSoundTypeFlyBy(modelIndex);

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

int WeaponSoundHook(int* params)
{
	const int A4 = params[0];
	const int A8 = params[1];
	const int modelIndex = params[2];
	const int weaponIndex = params[3];

	const auto playSound = (int(*)(int, int, int))0x0043BF90;

	switch (weaponIndex)
	{
	case 280: // ModelIndex_280_1_17_LaserRebel
	case 281: // ModelIndex_281_1_18_LaserRebelTurbo
	case 282: // ModelIndex_282_1_19_LaserImp
	case 283: // ModelIndex_283_1_20_LaserImpTurbo
	case 284: // ModelIndex_284_1_22_LaserIon
	case 285: // ModelIndex_285_1_23_LaserIonTurbo
	{
		std::string behavior = g_modelIndexSound.GetWeaponSoundBehavior(modelIndex);

		if (!behavior.empty())
		{
			if (behavior == "CorellianTransport")
			{
				switch (weaponIndex)
				{
				case 280: // ModelIndex_280_1_17_LaserRebel
					return playSound(15, A4, A8); // FalconLaser

				case 281: // ModelIndex_281_1_18_LaserRebelTurbo
					return playSound(16, A4, A8); // FalconLaserTurbo
				}
			}
			else if (behavior == "TieFighter")
			{
				return playSound(20, A4, A8); // EmpireLaserChChChhh
			}
		}

		return playSound(4 + weaponIndex - 280, A4, A8);
	}

	case 286: // ModelIndex_286_1_24_ProtonTorpedo
	case 291: // ModelIndex_291_1_24_ProtonTorpedo
		return playSound(10, A4, A8); // TorpedoFire

	case 287: // ModelIndex_287_1_25_ConcussionMissile
	case 292: // ModelIndex_292_1_25_ConcussionMissile
		return playSound(11, A4, A8); // MissileFire

	case 288: // ModelIndex_288_1_18_LaserRebelTurbo
	case 297: // ModelIndex_297_1_18_LaserRebelTurbo
	case 301: //ModelIndex_301_1_18_LaserRebelTurbo
	case 302: // ModelIndex_302_1_18_LaserRebelTurbo
		return playSound(12, A4, A8); // RebelLaserStarship

	case 289: // ModelIndex_289_1_20_LaserImpTurbo
	case 303: // ModelIndex_303_1_20_LaserImpTurbo
	case 304: // ModelIndex_304_1_20_LaserImpTurbo
	case 305: // ModelIndex_305_1_20_LaserImpTurbo
		return playSound(13, A4, A8); // EmpireLaserStarship

	case 290: // ModelIndex_290_1_23_LaserIonTurbo
		return playSound(14, A4, A8); // IonCannonStarship

	case 293: // ModelIndex_293_1_26_SpaceBomb
		return playSound(17, A4, A8); // BombFire

	case 294: // ModelIndex_294_1_27_HeavyRocket
		return playSound(18, A4, A8); // RocketFire

	case 295: // ModelIndex_295_1_28_MagPulse
	case 296: // ModelIndex_296_1_28_MagPulse
	case 298: // ModelIndex_298_1_29_Flare
		return playSound(19, A4, A8); // MagPulseFire

	case 307: // ModelIndex_307__1_0, open weapon
	{
		std::string behavior = g_modelIndexSound.GetWeaponSoundBehavior(modelIndex);

		if (!behavior.empty())
		{
			if (behavior == "Rebel")
			{
				return playSound(4, A4, A8); // RebelLaser
			}
			else if (behavior == "RebelTurbo")
			{
				return playSound(5, A4, A8); // RebelLaserTurbo
			}
			else if (behavior == "Empire")
			{
				return playSound(6, A4, A8); // EmpireLaser
			}
			else if (behavior == "EmpireTurbo")
			{
				return playSound(7, A4, A8); // EmpireLaserTurbo
			}
		}

		break;
	}
	}

	return 0;
}
