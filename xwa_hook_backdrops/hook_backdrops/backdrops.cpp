#include "targetver.h"
#include "backdrops.h"
#include "config.h"
#include <iostream>
#include <fstream>

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
	char unk07B[3343];
	TieFlightGroupWaypoint StartPoints[4];
	char StartPointRegions[4];
	char unkDAE[100];
	int PlanetId;
	char unkE16[40];
};

static_assert(sizeof(TieFlightGroup) == 3646, "size of TieFlightGroup must be 3646");

struct XwaMission
{
	TieFlightGroup FlightGroups[192];
	char unkAAE80[590400];
};

static_assert(sizeof(XwaMission) == 1290432, "size of XwaMission must be 1290432");

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

	if (lines.size())
	{
		CopyFileContent("Resdata_Temp.txt", lines, false);
		CopyFileContent("Resdata_Temp.txt", GetFileLines("Resdata.txt"), true);

		FreeResDataItems();
		ReadResdataDat("Resdata_Temp.txt");
	}
	else
	{
		FreeResDataItems();
		ReadResdataDat("Resdata.txt");
	}

	return LoadMission(fileName);
}
