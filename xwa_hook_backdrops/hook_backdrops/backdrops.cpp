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
		mission->FlightGroups[flightgroupsCount + 1].CraftId = 0xB7;

		do
		{
			mission->FlightGroups[flightgroupsCount].PlanetId = (XwaRand() % 0x3C) + 0x01;
		} while (mission->FlightGroups[flightgroupsCount].PlanetId == 0x37);

		mission->FlightGroups[flightgroupsCount + 1].PlanetId = (XwaRand() % 0x0A) + 0x54;

		mission->FlightGroups[flightgroupsCount].GlobalCargoIndex = XwaRand() % 0x07;
		mission->FlightGroups[flightgroupsCount].WavesCount = 0;
		mission->FlightGroups[flightgroupsCount].CraftsCount = 0x01;
		mission->FlightGroups[flightgroupsCount].GlobalGroupId = 0x0A;
		mission->FlightGroups[flightgroupsCount].Iff = 0x02;
		mission->FlightGroups[flightgroupsCount].Team = 0x09;

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
