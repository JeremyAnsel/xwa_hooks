#include "targetver.h"
#include "backdrops.h"
#include "config.h"
#include <iostream>
#include <fstream>
#include <map>
#include <utility>

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

#pragma pack(push, 1)

struct TieFlightGroupWaypoint
{
	short X;
	short Y;
	short Z;
	short m06;
};

static_assert(sizeof(TieFlightGroupWaypoint) == 8, "size of TieFlightGroupWaypoint must be 8");

struct TieFlightGroup
{
	char Name[20];
	char unk014[5];
	unsigned char GlobalCargoIndex;
	char unk01A[14];
	char Cargo[20];
	char SpecialCargo[20];
	char unk050[27];
	unsigned char CraftId;
	unsigned char CraftsCount;
	char unk06D[3];
	unsigned char Iff;
	unsigned char Team;
	char unk072[6];
	unsigned char GlobalGroupId;
	char unk079[1];
	unsigned char WavesCount;
	char unk07B[47];
	unsigned char ArrivalDelayMinutes;
	unsigned char ArrivalDelaySeconds;
	char unk0AC[3294];
	TieFlightGroupWaypoint StartPoints[4];
	char StartPointRegions[4];
	char unkDAE[100];
	int PlanetId;
	char unkE16[40];
};

static_assert(sizeof(TieFlightGroup) == 3646, "size of TieFlightGroup must be 3646");

struct TieFlightGroupEx
{
	TieFlightGroup FlightGroup;
	int PlayerIndex;
};

static_assert(sizeof(TieFlightGroupEx) == 3650, "size of TieFlightGroupEx must be 3650");

struct TieHeader
{
	char unk0000[9128];
	unsigned char TimeLimit;
	char unk23A9[65];
};

static_assert(sizeof(TieHeader) == 9194, "size of TieHeader must be 9194");

struct XwaMission
{
	TieFlightGroup FlightGroups[192];
	char unkAAE80[18780];
	TieHeader Header;
	char unkB1BC6[562426];
};

static_assert(sizeof(XwaMission) == 1290432, "size of XwaMission must be 1290432");

struct XwaPlayer
{
	char unk0000[16];
	unsigned char Region;
	char unk0011[3006];
};

static_assert(sizeof(XwaPlayer) == 3023, "size of XwaPlayer must be 3023");

struct XwaObject
{
	char unk00[2];
	unsigned short ModelIndex;
	unsigned char ShipCategory;
	unsigned char TieFlightGroupIndex;
	unsigned char Region;
	int PositionX;
	int PositionY;
	int PositionZ;
	short HeadingXY;
	short HeadingZ;
	char unk17[16];
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

struct ExeEnableEntry
{
	unsigned char EnableOptions; // flags
	unsigned char RessourceOptions; // flags
	unsigned char ObjectCategory;
	unsigned char ShipCategory;
	unsigned int ObjectSize;
	void* pData1;
	void* pData2;
	unsigned short GameOptions; // flags
	unsigned short CraftIndex;
	short DataIndex1;
	short DataIndex2;
};

static_assert(sizeof(ExeEnableEntry) == 24, "size of ExeEnableEntry must be 24");

#pragma pack(pop)

int g_planetsImageCount[] =
{
	0,
	7, // 6010
	7, // 6011
	7, // 6012
	7, // 6013
	7, // 6014
	7, // 6020
	7, // 6021
	7, // 6022
	7, // 6023
	7, // 6024
	7, // 6025
	7, // 6026
	7, // 6027
	7, // 6028
	7, // 6029
	7, // 6030
	7, // 6031
	7, // 6032
	7, // 6033
	3, // 6034
	7, // 6040
	7, // 6041
	1, // 6042
	7, // 6043
	0, // 6050
	7, // 6060
	7, // 6061
	7, // 6062
	7, // 6063
	7, // 6064
	7, // 6070
	7, // 6071
	7, // 6072
	7, // 6073
	7, // 6074
	7, // 6075
	7, // 6076
	7, // 6077
	7, // 6078
	3, // 6079
	7, // 6080
	7, // 6081
	7, // 6082
	7, // 6083
	7, // 6084
	7, // 6090
	7, // 6091
	7, // 6092
	7, // 6093
	7, // 6094
	7, // 6100
	7, // 6101
	7, // 6102
	7, // 6103
	7, // 6104
	7, // 6110
	7, // 6111
	7, // 6112
	7, // 6113
	7, // 6114
};

int g_lastBuoyRegion;
int g_previousBuoyRegion;
int g_currentRegion;

int BackdropsHook(int* params)
{
	const auto XwaRand = (int(*)())0x0059BEB0;

	const int regionsCount = params[0];
	int& flightgroupsCount = *(int*)params[1];

	XwaMission* mission = *(XwaMission**)0x09EB8E0;

	for (int region = 0; region < regionsCount; region++)
	{
		if (((int*)0x07849F0)[region] == 0 && region != 1)
		{
			continue;
		}

		// starfield

		// zenith
		mission->FlightGroups[flightgroupsCount].CraftId = 0xB7;
		mission->FlightGroups[flightgroupsCount].PlanetId = 0x37;
		mission->FlightGroups[flightgroupsCount].GlobalCargoIndex = 5;
		mission->FlightGroups[flightgroupsCount].WavesCount = 0;
		mission->FlightGroups[flightgroupsCount].CraftsCount = 0x01;
		mission->FlightGroups[flightgroupsCount].GlobalGroupId = 0x0A;
		mission->FlightGroups[flightgroupsCount].Iff = 0x02;
		mission->FlightGroups[flightgroupsCount].Team = 0x09;
		mission->FlightGroups[flightgroupsCount].StartPoints[0].Z = 1;
		mission->FlightGroups[flightgroupsCount].StartPointRegions[0] = region;
		strcpy_s(mission->FlightGroups[flightgroupsCount].Name, "0.0 0.0 0.0");
		strcpy_s(mission->FlightGroups[flightgroupsCount].Cargo, "0.0");
		strcpy_s(mission->FlightGroups[flightgroupsCount].SpecialCargo, "0.94");
		flightgroupsCount++;

		// nadir
		mission->FlightGroups[flightgroupsCount].CraftId = 0xB7;
		mission->FlightGroups[flightgroupsCount].PlanetId = 0x37;
		mission->FlightGroups[flightgroupsCount].GlobalCargoIndex = 5;
		mission->FlightGroups[flightgroupsCount].WavesCount = 0;
		mission->FlightGroups[flightgroupsCount].CraftsCount = 0x01;
		mission->FlightGroups[flightgroupsCount].GlobalGroupId = 0x0A;
		mission->FlightGroups[flightgroupsCount].Iff = 0x02;
		mission->FlightGroups[flightgroupsCount].Team = 0x09;
		mission->FlightGroups[flightgroupsCount].StartPoints[0].Z = -1;
		mission->FlightGroups[flightgroupsCount].StartPointRegions[0] = region;
		strcpy_s(mission->FlightGroups[flightgroupsCount].Name, "0.0 0.0 0.0");
		strcpy_s(mission->FlightGroups[flightgroupsCount].Cargo, "0.0");
		strcpy_s(mission->FlightGroups[flightgroupsCount].SpecialCargo, "0.94");
		flightgroupsCount++;

		// north
		mission->FlightGroups[flightgroupsCount].CraftId = 0xB7;
		mission->FlightGroups[flightgroupsCount].PlanetId = 0x37;
		mission->FlightGroups[flightgroupsCount].GlobalCargoIndex = 0;
		mission->FlightGroups[flightgroupsCount].WavesCount = 0;
		mission->FlightGroups[flightgroupsCount].CraftsCount = 0x01;
		mission->FlightGroups[flightgroupsCount].GlobalGroupId = 0x0A;
		mission->FlightGroups[flightgroupsCount].Iff = 0x02;
		mission->FlightGroups[flightgroupsCount].Team = 0x09;
		mission->FlightGroups[flightgroupsCount].StartPoints[0].Y = -1;
		mission->FlightGroups[flightgroupsCount].StartPointRegions[0] = region;
		strcpy_s(mission->FlightGroups[flightgroupsCount].Name, "0.0 0.0 0.0");
		strcpy_s(mission->FlightGroups[flightgroupsCount].Cargo, "0.0");
		strcpy_s(mission->FlightGroups[flightgroupsCount].SpecialCargo, "1.053");
		flightgroupsCount++;

		// south
		mission->FlightGroups[flightgroupsCount].CraftId = 0xB7;
		mission->FlightGroups[flightgroupsCount].PlanetId = 0x37;
		mission->FlightGroups[flightgroupsCount].GlobalCargoIndex = 0;
		mission->FlightGroups[flightgroupsCount].WavesCount = 0;
		mission->FlightGroups[flightgroupsCount].CraftsCount = 0x01;
		mission->FlightGroups[flightgroupsCount].GlobalGroupId = 0x0A;
		mission->FlightGroups[flightgroupsCount].Iff = 0x02;
		mission->FlightGroups[flightgroupsCount].Team = 0x09;
		mission->FlightGroups[flightgroupsCount].StartPoints[0].Y = 1;
		mission->FlightGroups[flightgroupsCount].StartPointRegions[0] = region;
		strcpy_s(mission->FlightGroups[flightgroupsCount].Name, "0.0 0.0 0.0");
		strcpy_s(mission->FlightGroups[flightgroupsCount].Cargo, "0.0");
		strcpy_s(mission->FlightGroups[flightgroupsCount].SpecialCargo, "1.053");
		flightgroupsCount++;

		// west
		mission->FlightGroups[flightgroupsCount].CraftId = 0xB7;
		mission->FlightGroups[flightgroupsCount].PlanetId = 0x37;
		mission->FlightGroups[flightgroupsCount].GlobalCargoIndex = 0;
		mission->FlightGroups[flightgroupsCount].WavesCount = 0;
		mission->FlightGroups[flightgroupsCount].CraftsCount = 0x01;
		mission->FlightGroups[flightgroupsCount].GlobalGroupId = 0x0A;
		mission->FlightGroups[flightgroupsCount].Iff = 0x02;
		mission->FlightGroups[flightgroupsCount].Team = 0x09;
		mission->FlightGroups[flightgroupsCount].StartPoints[0].X = -1;
		mission->FlightGroups[flightgroupsCount].StartPointRegions[0] = region;
		strcpy_s(mission->FlightGroups[flightgroupsCount].Name, "0.0 0.0 0.0");
		strcpy_s(mission->FlightGroups[flightgroupsCount].Cargo, "0.0");
		strcpy_s(mission->FlightGroups[flightgroupsCount].SpecialCargo, "1.053");
		flightgroupsCount++;

		// est
		mission->FlightGroups[flightgroupsCount].CraftId = 0xB7;
		mission->FlightGroups[flightgroupsCount].PlanetId = 0x37;
		mission->FlightGroups[flightgroupsCount].GlobalCargoIndex = 0;
		mission->FlightGroups[flightgroupsCount].WavesCount = 0;
		mission->FlightGroups[flightgroupsCount].CraftsCount = 0x01;
		mission->FlightGroups[flightgroupsCount].GlobalGroupId = 0x0A;
		mission->FlightGroups[flightgroupsCount].Iff = 0x02;
		mission->FlightGroups[flightgroupsCount].Team = 0x09;
		mission->FlightGroups[flightgroupsCount].StartPoints[0].X = 1;
		mission->FlightGroups[flightgroupsCount].StartPointRegions[0] = region;
		strcpy_s(mission->FlightGroups[flightgroupsCount].Name, "0.0 0.0 0.0");
		strcpy_s(mission->FlightGroups[flightgroupsCount].Cargo, "0.0");
		strcpy_s(mission->FlightGroups[flightgroupsCount].SpecialCargo, "1.053");
		flightgroupsCount++;

		// default backdrops
		mission->FlightGroups[flightgroupsCount].CraftId = 0xB7;

		int planetId0 = (XwaRand() % 0x3B) + 0x01;

		if (planetId0 >= 0x37)
		{
			planetId0++;
		}

		mission->FlightGroups[flightgroupsCount].PlanetId = planetId0;

		int imageCount0 = g_planetsImageCount[planetId0];
		unsigned char globalCargoIndex0 = 0;

		if (imageCount0 != 0)
		{
			globalCargoIndex0 = XwaRand() % imageCount0;
		}

		mission->FlightGroups[flightgroupsCount].GlobalCargoIndex = globalCargoIndex0;
		mission->FlightGroups[flightgroupsCount].WavesCount = 0;
		mission->FlightGroups[flightgroupsCount].CraftsCount = 0x01;
		mission->FlightGroups[flightgroupsCount].GlobalGroupId = 0x0A;
		mission->FlightGroups[flightgroupsCount].Iff = 0x02;
		mission->FlightGroups[flightgroupsCount].Team = 0x09;

		mission->FlightGroups[flightgroupsCount + 1].CraftId = 0xB7;
		mission->FlightGroups[flightgroupsCount + 1].PlanetId = (XwaRand() % 0x0A) + 0x54;
		mission->FlightGroups[flightgroupsCount + 1].GlobalCargoIndex = 0;
		mission->FlightGroups[flightgroupsCount + 1].WavesCount = 0;
		mission->FlightGroups[flightgroupsCount + 1].CraftsCount = 0x01;
		mission->FlightGroups[flightgroupsCount + 1].GlobalGroupId = 0x0A;
		mission->FlightGroups[flightgroupsCount + 1].Iff = 0x02;
		mission->FlightGroups[flightgroupsCount + 1].Team = 0x09;

		int edx = XwaRand() % 0x06;

		if (edx < 4)
		{
			switch (edx)
			{
			case 0:
				mission->FlightGroups[flightgroupsCount].StartPoints[0].X = 1;
				break;

			case 1:
				mission->FlightGroups[flightgroupsCount].StartPoints[0].X = -1;
				break;

			case 2:
				mission->FlightGroups[flightgroupsCount].StartPoints[0].Y = 1;
				break;

			case 3:
				mission->FlightGroups[flightgroupsCount].StartPoints[0].Y = -1;
				break;
			}

			switch (XwaRand() % 0x02)
			{
			case 0:
				mission->FlightGroups[flightgroupsCount + 1].StartPoints[0].Z = 1;
				break;

			case 1:
				mission->FlightGroups[flightgroupsCount + 1].StartPoints[0].Z = -1;
				break;
			}
		}
		else
		{
			switch (edx)
			{
			case 4:
				mission->FlightGroups[flightgroupsCount].StartPoints[0].Z = 1;
				mission->FlightGroups[flightgroupsCount + 1].StartPoints[0].Z = -1;
				break;

			case 5:
				mission->FlightGroups[flightgroupsCount].StartPoints[0].Z = -1;
				mission->FlightGroups[flightgroupsCount + 1].StartPoints[0].Z = 1;
				break;
			}
		}

		mission->FlightGroups[flightgroupsCount].StartPointRegions[0] = region;
		strcpy_s(mission->FlightGroups[flightgroupsCount].Name, "1.0 1.0 1.0");
		sprintf_s(mission->FlightGroups[flightgroupsCount].Cargo, "%.1f", ((XwaRand() % 0x06) + 0x05) * 0.1);
		sprintf_s(mission->FlightGroups[flightgroupsCount].SpecialCargo, "%.1f", ((XwaRand() % 0x10) + 0x0F) * 0.1);

		mission->FlightGroups[flightgroupsCount + 1].StartPointRegions[0] = region;
		strcpy_s(mission->FlightGroups[flightgroupsCount + 1].Name, "1.0 1.0 1.0");
		sprintf_s(mission->FlightGroups[flightgroupsCount + 1].Cargo, "%.1f", ((XwaRand() % 0x06) + 0x05) * 0.1);
		sprintf_s(mission->FlightGroups[flightgroupsCount + 1].SpecialCargo, "1.0");

		flightgroupsCount += 2;
	}

	return 0;
}

void CopyFileContent(const std::string& out, const std::vector<std::string>& lines, bool append)
{
	auto ofile = std::ofstream(out, append ? std::ofstream::app : std::ofstream::trunc);

	if (!ofile)
	{
		return;
	}

	for (const std::string& line : lines)
	{
		ofile << line << std::endl;
	}
}

int LoadMissionHook(int* params)
{
	g_lastBuoyRegion = -1;
	g_previousBuoyRegion = -1;
	g_currentRegion = -1;

	const char* fileName = (const char*)params[0];
	const auto LoadMission = (int(*)(const char*))0x00415760;

	const auto FreeResDataItems = (void(*)())0x004CD680;
	const auto ReadResdataDat = (short(*)(const char*))0x004CD390;

	const std::string fileNameBase = GetStringWithoutExtension(fileName);

	auto lines = GetFileLines(fileNameBase + "_Resdata.txt");

	if (!lines.size())
	{
		lines = GetFileLines(fileNameBase + ".ini", "Resdata");
	}

	//if (lines.size())
	{
		CopyFileContent("Resdata_Temp.txt", lines, false);
		CopyFileContent("Resdata_Temp.txt", GetFileLines("Resdata.txt"), true);

		FreeResDataItems();
		ReadResdataDat("Resdata_Temp.txt");
	}
	//else
	//{
	//	FreeResDataItems();
	//	ReadResdataDat("Resdata.txt");
	//}

	return LoadMission(fileName);
}

int SwitchHyperBuoyHook(int* params)
{
	const auto XwaShowMessage = (void(*)(int, int))0x00497D40;

	const int nextRegion = params[Params_ESI];
	const int currentRegion = *(int*)0x008C1CD8;

	g_lastBuoyRegion = currentRegion;

	XwaShowMessage(params[0], params[1]);

	return 0;
}

int GetBackdropIndexFromFG(int fg)
{
	const XwaMission* mission = *(XwaMission**)0x09EB8E0;

	if (fg < 0 || fg >= 192)
	{
		return -1;
	}

	if (mission->FlightGroups[fg].CraftId != 0xB7)
	{
		return -1;
	}

	int region = mission->FlightGroups[fg].StartPointRegions[0];
	int backdrop = region * 32;

	for (int fgIndex = 0; fgIndex < fg; fgIndex++)
	{
		if (mission->FlightGroups[fgIndex].PlanetId == 0)
		{
			continue;
		}

		if (mission->FlightGroups[fgIndex].CraftId != 0xB7)
		{
			continue;
		}

		if (mission->FlightGroups[fgIndex].StartPointRegions[0] != region)
		{
			continue;
		}

		backdrop++;
	}

	return backdrop;
}

int GetBackdropFGFromIndex(int backdropIndex)
{
	const XwaMission* mission = *(XwaMission**)0x09EB8E0;

	if (backdropIndex < 0 || backdropIndex >= 160)
	{
		return -1;
	}

	int backdrops[5]
	{
		0 * 32,
		1 * 32,
		2 * 32,
		3 * 32,
		4 * 32
	};

	for (int fg = 0; fg < 192; fg++)
	{
		if (mission->FlightGroups[fg].CraftId != 0xB7)
		{
			continue;
		}

		if (mission->FlightGroups[fg].PlanetId == 0)
		{
			continue;
		}

		if (mission->FlightGroups[fg].CraftId != 0xB7)
		{
			continue;
		}

		int region = mission->FlightGroups[fg].StartPointRegions[0];

		if (backdrops[region] == backdropIndex)
		{
			return fg;
		}

		backdrops[region]++;
	}

	return -1;
}

int BackdropRenderFilterHook(int* params)
{
	const auto XwaGetTotalSecondsFromSecondsMinutesHours = (int(*)(unsigned char, unsigned char, unsigned char))0x004DA310;

	static int _backdropsFromRegion[160];
	static std::string _mission;
	const char* xwaMissionFileName = (const char*)0x06002E8;

	if (_mission != xwaMissionFileName)
	{
		_mission = xwaMissionFileName;

		for (int i = 0; i < 160; i++)
		{
			_backdropsFromRegion[i] = -1;
		}

		const std::string path = GetStringWithoutExtension(_mission);

		auto file = GetFileLines(path + ".txt");

		if (!file.size())
		{
			file = GetFileLines(path + ".ini", "mission_tie");
		}

		const auto lines = GetFileListValues(file);

		for (const auto& line : lines)
		{
			const auto& group = line[0];

			if (_stricmp(group.c_str(), "backdrop") == 0)
			{
				if (line.size() < 4)
				{
					continue;
				}

				int backdropFG = std::stoi(line[1]);
				int backdrop = GetBackdropIndexFromFG(backdropFG);

				if (backdrop < 0 || backdrop >= 160)
				{
					continue;
				}

				const auto& element = line[2];

				if (_stricmp(element.c_str(), "from_region") == 0)
				{
					int value = std::stoi(line[3]);
					_backdropsFromRegion[backdrop] = value;
				}
			}
		}
	}

	const XwaMission* mission = *(XwaMission**)0x09EB8E0;
	const int backdropIndex = params[Params_ESI];
	const int backdropFromRegion = _backdropsFromRegion[backdropIndex];
	const XwaPlayer* players = (XwaPlayer*)0x008B94E0;
	const int currentPlayerId = *(int*)0x008C1CC8;
	const int currentRegion = players[currentPlayerId].Region;

	if (currentRegion != g_currentRegion)
	{
		g_currentRegion = currentRegion;
		g_previousBuoyRegion = g_lastBuoyRegion;
	}

	bool skip = false;

	if (!skip)
	{
		if (g_previousBuoyRegion != -1 && backdropFromRegion != -1 && backdropFromRegion != g_previousBuoyRegion)
		{
			skip = true;
		}
	}

	if (!skip)
	{
		int fg = GetBackdropFGFromIndex(backdropIndex);

		if (fg != -1)
		{
			int delaySeconds = mission->FlightGroups[fg].ArrivalDelayMinutes * 60 + mission->FlightGroups[fg].ArrivalDelaySeconds;
			unsigned char MissionTimeHours = *(unsigned char*)(0x008053E0 + 0x0017);
			unsigned char MissionTimeMinutes = *(unsigned char*)(0x008053E0 + 0x0018);
			unsigned char MissionTimeSeconds = *(unsigned char*)(0x008053E0 + 0x0019);
			int missionTime = XwaGetTotalSecondsFromSecondsMinutesHours(MissionTimeHours, MissionTimeMinutes, MissionTimeSeconds);

			if (delaySeconds > missionTime)
			{
				skip = true;
			}
		}
	}

	if (skip)
	{
		params[Params_ReturnAddress] = 0x004072E4;
	}

	params[Params_ECX] = *(unsigned char*)0x00910DF4;

	return 0;
}

int ShowBuoyRegionNameHook(int* params)
{
	static std::string _buoyRegionNames[192];
	static std::string _mission;
	const char* xwaMissionFileName = (const char*)0x06002E8;

	if (_mission != xwaMissionFileName)
	{
		_mission = xwaMissionFileName;

		for (int i = 0; i < 192; i++)
		{
			_buoyRegionNames[i].clear();
		}

		const std::string path = GetStringWithoutExtension(_mission);

		auto file = GetFileLines(path + ".txt");

		if (!file.size())
		{
			file = GetFileLines(path + ".ini", "mission_tie");
		}

		const auto lines = GetFileListValues(file);

		for (const auto& line : lines)
		{
			const auto& group = line[0];

			if (_stricmp(group.c_str(), "buoy") == 0)
			{
				if (line.size() < 4)
				{
					continue;
				}

				int fg = std::stoi(line[1]);

				if (fg < 0 || fg >= 192)
				{
					continue;
				}

				const auto& element = line[2];

				if (_stricmp(element.c_str(), "region_name") == 0)
				{
					_buoyRegionNames[fg] = line[3];
				}
			}
		}
	}

	const unsigned short A4 = (unsigned short)params[0];
	const char* A8 = (const char*)params[1];

	const auto L00498CE0 = (void(*)(unsigned short, const char*))0x00498CE0;

	const XwaObject* xwaObjects = *(XwaObject**)0x07B33C4;

	int buoyObjectIndex = params[Params_EBX];
	int buoyFG = xwaObjects[buoyObjectIndex].TieFlightGroupIndex;

	if (_buoyRegionNames[buoyFG].empty())
	{
		L00498CE0(A4, A8);
	}
	else
	{
		L00498CE0(A4, _buoyRegionNames[buoyFG].c_str());
	}

	return 0;
}

class BackdropsSettings
{
public:
	float GetScale(int backdropIndex, int imageNumber)
	{
		this->Update();

		auto it = this->_scales.find(std::make_tuple(backdropIndex, imageNumber));

		if (it != this->_scales.end())
		{
			return (float)it->second;
		}

		it = this->_scales.find(std::make_tuple(backdropIndex, -1));

		if (it != this->_scales.end())
		{
			return (float)it->second;
		}

		return 	*(float*)0x005A93AC;
	}

private:
	void Update()
	{
		static std::string _mission;

		const char* xwaMissionFileName = (const char*)0x06002E8;

		if (_mission == xwaMissionFileName)
		{
			return;
		}

		_mission = xwaMissionFileName;
		this->_scales.clear();

		const std::string mission = GetStringWithoutExtension(xwaMissionFileName);
		std::vector<std::string> file = GetFileLines(mission + "_BackdropScales.txt");

		if (!file.size())
		{
			file = GetFileLines(mission + ".ini", "BackdropScales");
		}

		std::string scalesFileName = GetFileKeyValue(file, "ScalesFileName");

		if (scalesFileName.size())
		{
			std::vector<std::string> scales = GetFileLines("Resdata\\" + scalesFileName);
			file.insert(file.end(), scales.begin(), scales.end());
		}

		std::vector<std::string> defaultScales = GetFileLines("Resdata\\BackdropScales.txt");

		if (defaultScales.size())
		{
			file.insert(file.end(), defaultScales.begin(), defaultScales.end());
		}

		const auto lines = GetFileListValues(file);

		for (const auto& line : lines)
		{
			if (line.size() < 2)
			{
				continue;
			}

			int backdropIndex;
			int imageNumber;
			int scale;

			if (line.size() == 2)
			{
				backdropIndex = std::stoi(line[0]);
				imageNumber = -1;
				scale = std::stoi(line[1]);
			}
			else
			{
				backdropIndex = std::stoi(line[0]);
				imageNumber = std::stoi(line[1]);
				scale = std::stoi(line[2]);
			}

			auto it = this->_scales.find(std::make_tuple(backdropIndex, imageNumber));

			if (it != this->_scales.end())
			{
				continue;
			}

			this->_scales.insert(std::make_pair(std::make_tuple(backdropIndex, imageNumber), scale));
		}
	}

	std::map<std::tuple<int, int>, int> _scales;
};

BackdropsSettings g_backdropsSettings;

int SetBackdropScaleHook(int* params)
{
	const ExeEnableEntry* ExeEnableTable = (ExeEnableEntry*)0x005FB240;

	unsigned short modelIndex = (unsigned short)params[4];
	float esp18 = *(float*)(params + 6);
	float scale = *(float*)0x005A93AC;

	short backdropIndex = ExeEnableTable[modelIndex].DataIndex1;
	int imageNumber = *(unsigned char*)(0x00808342 + params[Params_ESI]) - 1;

	scale = g_backdropsSettings.GetScale(backdropIndex, imageNumber);

	return (int)(esp18 * scale + 0.5f);
}

std::vector<std::tuple<unsigned short, unsigned char>> ReadXwaPlanets()
{
	std::vector<std::tuple<unsigned short, unsigned char>> planets;

	for (int i = 0; i < 104; i++)
	{
		unsigned short modelIndex = *(unsigned short*)(0x005B1140 + i * 3 + 0);
		unsigned char flags = *(unsigned char*)(0x005B1140 + i * 3 + 2);

		planets.push_back(std::make_tuple(modelIndex, flags));
	}

	return planets;
}

void SetXwaPlanet(int index, unsigned short modelIndex, unsigned char flags)
{
	*(unsigned short*)(0x005B1140 + index * 3 + 0) = modelIndex;
	*(unsigned char*)(0x005B1140 + index * 3 + 2) = flags;
}

std::vector<short> ReadXwaPlanetsObjectEntry()
{
	ExeEnableEntry* ExeEnableTable = (ExeEnableEntry*)0x005FB240;

	std::vector<short> entries;

	for (int i = 0; i < 60; i++)
	{
		short dataIndex1 = ExeEnableTable[427 + i].DataIndex1;

		entries.push_back(dataIndex1);
	}

	return entries;
}

int LoadMissionBackdropsHook(int* params)
{
	const auto FreeModelIndex = (void(*)(unsigned short))0x00432400;
	const auto LoadModelIndex = (void(*)(unsigned short))0x00432120;

	*(int*)0x0080ACE0 = 0;

	unsigned char TimeHours = *(unsigned char*)(0x08053E0 + 0x0017);
	unsigned char TimeMinutes = *(unsigned char*)(0x08053E0 + 0x0018);
	unsigned char TimeSeconds = *(unsigned char*)(0x08053E0 + 0x0019);
	unsigned int totalSeconds = TimeHours * 3600 + TimeMinutes * 60 + TimeSeconds;

	static std::vector<std::tuple<unsigned short, unsigned char>> s_XwaPlanets = ReadXwaPlanets();
	static std::vector<short> s_XwaPlanetsEntry = ReadXwaPlanetsObjectEntry();

	unsigned short* s_ExeSpecies = (unsigned short*)0x005B0F70;
	ExeEnableEntry* ExeEnableTable = (ExeEnableEntry*)0x005FB240;
	TieFlightGroupEx* s_XwaTieFlightGroups = (TieFlightGroupEx*)0x0080DC80;
	int FlightGroupsCount = *(short*)0x007B4C00;

	static int flightGroupsPlanetId[192];
	if (totalSeconds == 0)
	{
		for (int fg = 0; fg < FlightGroupsCount; fg++)
		{
			TieFlightGroup& flightGroup = s_XwaTieFlightGroups[fg].FlightGroup;
			flightGroupsPlanetId[fg] = flightGroup.PlanetId;
		}
	}

	int planetsCount = 0;
	int planetsCountPerRegion[5];
	for (int i = 0; i < 5; i++)
	{
		planetsCountPerRegion[i] = 0;
	}

	for (int i = 0; i < 60; i++)
	{
		ExeEnableTable[427 + i].EnableOptions = 0;

		FreeModelIndex(427 + i);
	}

	for (int fg = 0; fg < FlightGroupsCount; fg++)
	{
		TieFlightGroup& flightGroup = s_XwaTieFlightGroups[fg].FlightGroup;

		// ModelIndex_263_9001_1100_ResData_Backdrop
		if (s_ExeSpecies[flightGroup.CraftId] != 263)
		{
			continue;
		}

		if (flightGroup.PlanetId == 0)
		{
			continue;
		}

		int planetId = flightGroupsPlanetId[fg];

		bool isDefaultPlanet = planetId >= 1 && planetId <= 60;
		bool isExtraPlanet = planetId >= 104 && planetId <= 255;
		bool isPlanet = isDefaultPlanet || isExtraPlanet;

		if (!isPlanet)
		{
			continue;
		}

		int region = flightGroup.StartPointRegions[0];

		if (planetsCount >= 60)
		{
			flightGroup.PlanetId = 0;
			continue;
		}

		if (isDefaultPlanet)
		{
			unsigned short origModelIndex = std::get<0>(s_XwaPlanets[planetId]);
			unsigned char origFlags = std::get<1>(s_XwaPlanets[planetId]);

			SetXwaPlanet(1 + planetsCount, 427 + planetsCount, origFlags);

			ExeEnableTable[427 + planetsCount].EnableOptions = 2;
			ExeEnableTable[427 + planetsCount].DataIndex1 = s_XwaPlanetsEntry[origModelIndex - 427];
			ExeEnableTable[427 + planetsCount].DataIndex2 = 0;

			flightGroup.PlanetId = 1 + planetsCount;
		}
		else if (isExtraPlanet)
		{
			SetXwaPlanet(1 + planetsCount, 427 + planetsCount, 5);

			ExeEnableTable[427 + planetsCount].EnableOptions = 2;
			ExeEnableTable[427 + planetsCount].DataIndex1 = 6304 + planetId - 104;
			ExeEnableTable[427 + planetsCount].DataIndex2 = 0;

			flightGroup.PlanetId = 1 + planetsCount;
		}
		else
		{
			flightGroup.PlanetId = 0;
			continue;
		}

		if (flightGroup.PlanetId != 0)
		{
			unsigned short modelIndex = 427 + planetsCount;

			if (ExeEnableTable[modelIndex].pData1 == 0)
			{
				LoadModelIndex(modelIndex);
			}
		}

		planetsCountPerRegion[region]++;
		planetsCount++;
	}

	return 0;
}
