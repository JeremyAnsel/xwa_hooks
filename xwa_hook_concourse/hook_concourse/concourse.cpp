#include "targetver.h"
#include "concourse.h"
#include "config.h"
#include <fstream>
#include <queue>
#include <algorithm>
#include <tuple>

#define NOMINMAX
#include <Windows.h>

#pragma comment(lib, "Winmm.lib")

// Stack contents for function params
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

std::vector<std::string> TokennizeSpaces(const std::string& str)
{
	const char* delimiters = " \t\n\r\f\v";
	std::vector<std::string> tokens;

	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	std::string::size_type pos = str.find_first_of(delimiters, lastPos);

	while (std::string::npos != pos || std::string::npos != lastPos)
	{
		std::string value = Trim(str.substr(lastPos, pos - lastPos));

		if (!value.empty())
		{
			tokens.push_back(value);
		}

		lastPos = str.find_first_not_of(delimiters, pos);
		pos = str.find_first_of(delimiters, lastPos);
	}

	return tokens;
}

class Config
{
public:
	Config()
	{
		auto lines = GetFileLines("hook_concourse.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_concourse");
		}

		this->SkipFirst7Missions = GetFileKeyValueInt(lines, "SkipFirst7Missions", 0) != 0;
		this->KalidorCrescentPositionX = GetFileKeyValueInt(lines, "KalidorCrescentPositionX", 224);
		this->KalidorCrescentPositionY = GetFileKeyValueInt(lines, "KalidorCrescentPositionY", 168);
	}

	bool SkipFirst7Missions;
	int KalidorCrescentPositionX;
	int KalidorCrescentPositionY;
};

Config g_config;

class RandomGenerator
{
public:
	RandomGenerator()
	{
		this->seed = (unsigned short)timeGetTime();
	}

	unsigned short Generate()
	{
		this->seed = this->seed * 9421 + 1;
		return this->seed;
	}

	unsigned short Generate(int rangeMin, int rangeMax)
	{
		int range = rangeMax - rangeMin;

		if (range <= 0)
		{
			return 0;
		}

		return rangeMin + (this->Generate() % range);
	}

	unsigned short seed;
};

RandomGenerator g_randomGenerator;

std::vector<std::string> GetMissionConcourseLines(const std::string& mission)
{
	static std::vector<std::string> _lines;
	static std::string _mission;

	if (mission != _mission)
	{
		const std::string path = GetStringWithoutExtension(mission);

		auto lines = GetFileLines(path + "_concourse.txt");

		if (!lines.size())
		{
			lines = GetFileLines(path + ".ini", "concourse");
		}

		_lines = lines;
		_mission = mission;
	}


	return _lines;
}

#pragma pack(push, 1)

struct XwaMissionDescription
{
	char MissionFileName[64];
	char MissionDesciption[128];
	char BattleDescription[128];
	int Id;
	int m144;
};

static_assert(sizeof(XwaMissionDescription) == 328, "size of XwaMissionDescription must be 328");

#pragma pack(pop)

int GetMissionFileNameById(int missionId)
{
	const int s_XwaMissionDescriptionsCount = *(int*)0x009F5EC0;
	const XwaMissionDescription* s_XwaMissionDescriptions = *(XwaMissionDescription**)0x009F4B98;

	int index = 0;

	for (; index < s_XwaMissionDescriptionsCount; index++)
	{
		if (s_XwaMissionDescriptions[index].Id == missionId)
		{
			break;
		}
	}

	if (index >= s_XwaMissionDescriptionsCount)
	{
		index = -1;
	}

	return index;
}

std::vector<std::string> g_MedalDetails;
std::vector<std::string> g_FamilyMedalsDescriptions;

void LoadMedalDetails()
{
	static bool _loaded = false;

	if (_loaded)
	{
		return;
	}

	if (std::ifstream("MedalDetailList.txt"))
	{
		const auto lines = GetFileLines("MedalDetailList.txt");

		for (const std::string& line : lines)
		{
			if (line == "null")
			{
				g_MedalDetails.push_back("");
			}
			else
			{
				g_MedalDetails.push_back(line);
			}
		}
	}
	else
	{
		for (int index = 0; index < 53; index++)
		{
			const char* filename = ((const char**)0x006042C8)[index];
			g_MedalDetails.push_back(filename);
		}
	}

	_loaded = true;
}

void LoadFamilyMedalsDescriptions()
{
	static bool _loaded = false;

	if (_loaded)
	{
		return;
	}

	std::ifstream file("FamilyMedalsDescriptions.txt");

	if (file)
	{
		std::string line;

		while (std::getline(file, line))
		{
			if (line.empty())
			{
				g_FamilyMedalsDescriptions.push_back("Unknown");
			}
			else
			{
				g_FamilyMedalsDescriptions.push_back(line);
			}
		}
	}

	_loaded = true;
}

int MedalDetailExistHook(int* params)
{
	LoadMedalDetails();

	const int index = params[0];

	params[0] = index < (int)g_MedalDetails.size() && !g_MedalDetails[index].empty() ? 1 : 0;
	return 0;
}

int MedalDetailFilenameHook(int* params)
{
	static const char* s_MedalDetailFilenameEmpty = "";

	LoadMedalDetails();

	const int index = params[0];

	params[0] = index < (int)g_MedalDetails.size() ? (int)g_MedalDetails[index].c_str() : (int)s_MedalDetailFilenameEmpty;
	return 0;
}

int FamilyMedalDescriptionHook(int* params)
{
	LoadFamilyMedalsDescriptions();

	const int index = params[0] - 0x402;

	const auto XwaGetFrontTxtString = (const char* (*)(int))0x0055CB50;

	const char* description;

	if (index < (int)g_FamilyMedalsDescriptions.size())
	{
		description = g_FamilyMedalsDescriptions[index].c_str();
	}
	else if (index < 50)
	{
		description = XwaGetFrontTxtString(0x402 + index);
	}
	else
	{
		description = "Unknown";
	}

	return (int)description;
}

std::queue<std::string> g_movieNames;
std::vector<std::pair<int, std::string>> g_CustomMoviesOnPoints;
std::vector<int> g_KalidorCrescentPoints;
std::vector<std::string> g_KalidorCrescentDescriptions;

void LoadCustomPoints()
{
	static bool _loaded = false;

	if (_loaded)
	{
		return;
	}

	if (std::ifstream("CustomMoviesOnPointsList.txt"))
	{
		const auto lines = GetFileLines("CustomMoviesOnPointsList.txt");

		for (const std::string& line : lines)
		{
			if (line.empty())
			{
				continue;
			}

			const auto tokens = Tokennize(line);

			if (tokens.size() < 2)
			{
				continue;
			}

			int points = std::stoi(tokens[0], 0, 0);
			std::string name = tokens[1];

			g_CustomMoviesOnPoints.push_back(std::make_pair(points, name));
		}
	}

	if (std::ifstream("KalidorCrescentPoints.txt"))
	{
		const auto lines = GetFileLines("KalidorCrescentPoints.txt");

		g_KalidorCrescentPoints = GetFileListIntValues(lines);
	}
	else
	{
		const int* s_XwaKalidorCrescentPoints = (int*)0x005B30A8;

		for (int i = 0; i < 6; i++)
		{
			int points = s_XwaKalidorCrescentPoints[i] / 10;
			g_KalidorCrescentPoints.push_back(points);
		}
	}

	std::ifstream file("KalidorCrescentDescriptions.txt");

	if (file)
	{
		std::string line;

		while (std::getline(file, line))
		{
			if (line.empty())
			{
				g_KalidorCrescentDescriptions.push_back("Unknown");
			}
			else
			{
				g_KalidorCrescentDescriptions.push_back(line);
			}
		}
	}

	_loaded = true;
}

int CustomMoviesOnPoints1Hook(int* params)
{
	const int a4 = params[0];
	const int a8 = params[1];

	const auto L0042E750 = (int(*)(int, int))0x0042E750;

	L0042E750(a4, a8);

	//g_movieNames.push("TECHROOM");

	return 0;
}

int CustomMoviesOnPointsPlayHook(int* params)
{
	const int A4 = params[0];

	const auto L00558100 = (int(*)(int))0x00558100;
	const auto XwaPlayMovie = (int(*)(const char*, int))0x0055BC20;

	const int s_V0x0ABD1E4 = *(int*)0x0ABD1E4;

	L00558100(s_V0x0ABD1E4);

	for (; !g_movieNames.empty(); g_movieNames.pop())
	{
		const std::string& name = g_movieNames.front();
		XwaPlayMovie(name.c_str(), 0);
	}

	return 0;
}

int CustomPointsCheckHook(int* params)
{
	LoadCustomPoints();

	const int points = params[0] / 10;

	int& XwaPilotData_m10EA6 = *(int*)0x00AF3906;
	int& XwaPilotData_m10F3A = *(int*)0x00AF399A;
	int& XwaPilotData_m00012 = *(int*)0x00AE2A72;

	if (XwaPilotData_m10EA6 < (int)g_KalidorCrescentPoints.size())
	{
		if (points >= g_KalidorCrescentPoints[XwaPilotData_m10EA6])
		{
			XwaPilotData_m10EA6++;
			XwaPilotData_m10F3A = XwaPilotData_m10EA6;
		}
	}

	while (XwaPilotData_m00012 < (int)g_CustomMoviesOnPoints.size() && points >= g_CustomMoviesOnPoints[XwaPilotData_m00012].first)
	{
		g_movieNames.push(g_CustomMoviesOnPoints[XwaPilotData_m00012].second);
		XwaPilotData_m00012++;
	}

	return 0;
}

class ConcourseDoors
{
public:
	ConcourseDoors()
	{
		auto lines = GetFileLines("ConcourseDoors.txt");

		this->planet_position_left = GetFileKeyValueInt(lines, "planet_position_left", 0);
		this->planet_position_right = GetFileKeyValueInt(lines, "planet_position_right", 640);
		this->planet_position_top = GetFileKeyValueInt(lines, "planet_position_top", 55);
		this->planet_position_bottom = GetFileKeyValueInt(lines, "planet_position_bottom", 125);
		this->tourdoor_posX = GetFileKeyValueInt(lines, "tourdoor_posX", 0xE7);
		this->tourdoor_posY = GetFileKeyValueInt(lines, "tourdoor_posY", 0xAE);
		this->mkanim_posX = GetFileKeyValueInt(lines, "mkanim_posX", 0x6E);
		this->mkanim_posY = GetFileKeyValueInt(lines, "mkanim_posY", 0x166);
		this->traindoor_posX = GetFileKeyValueInt(lines, "traindoor_posX", 0x218);
		this->traindoor_posY = GetFileKeyValueInt(lines, "traindoor_posY", 0xAE);
		this->combatdoor_poxX = GetFileKeyValueInt(lines, "combatdoor_poxX", 0x23);
		this->combatdoor_poxY = GetFileKeyValueInt(lines, "combatdoor_poxY", 0xAE);
		this->filmdoor_posX = GetFileKeyValueInt(lines, "filmdoor_posX", 0x218);
		this->filmdoor_posY = GetFileKeyValueInt(lines, "filmdoor_posY", 0x11E);
		this->techdoor_posX = GetFileKeyValueInt(lines, "techdoor_posX", 0x1AA);
		this->techdoor_posY = GetFileKeyValueInt(lines, "techdoor_posY", 0xDB);
	}

	int planet_position_left;
	int planet_position_right;
	int planet_position_top;
	int planet_position_bottom;
	int tourdoor_posX;
	int tourdoor_posY;
	int mkanim_posX;
	int mkanim_posY;
	int traindoor_posX;
	int traindoor_posY;
	int combatdoor_poxX;
	int combatdoor_poxY;
	int filmdoor_posX;
	int filmdoor_posY;
	int techdoor_posX;
	int techdoor_posY;
};

ConcourseDoors g_concourseDoors;

struct ConcourseAnimation
{
	int id;
	std::string room;
	int posX;
	int posY;
	short nameGroupId;
	std::string name;
	int delayMin;
	int delayMax;
	std::string group;
	std::string missions;
	int frame;

	bool IsRoom(const char* currentRoom) const
	{
		return _stricmp(this->room.c_str(), currentRoom) == 0;
	}

	bool IsGroup(const char* currentGroup) const
	{
		return _stricmp(this->group.c_str(), currentGroup) == 0;
	}

	bool IsInCurrentMission() const
	{
		const int currentMission = *(int*)0x00ABC970;

		std::vector<std::string> missionList = TokennizeSpaces(this->missions);

		if (missionList.empty())
		{
			return true;
		}

		for (const auto& missionEntry : missionList)
		{
			if (missionEntry == "-1")
			{
				return true;
			}
		}

		bool includeMission = false;

		for (const auto& missionEntry : missionList)
		{
			if (missionEntry[0] == '~')
			{
				continue;
			}

			includeMission = true;

			int missionIndex = std::stoi(missionEntry, 0, 0);

			if (missionIndex == currentMission)
			{
				return true;
			}
		}

		for (const auto& missionEntry : missionList)
		{
			if (missionEntry[0] != '~')
			{
				continue;
			}

			int missionIndex = std::stoi(missionEntry.substr(1), 0, 0);

			if (missionIndex == currentMission)
			{
				return false;
			}
		}

		return !includeMission;
	}
};

class ConcourseAnimations
{
public:
	ConcourseAnimations()
	{
		auto lines = GetFileLines("ConcourseAnimations.txt");
		int index = 0;

		for (const std::string& line : lines)
		{
			std::vector<std::string> values = Tokennize(line);

			if (values.size() < 8)
			{
				continue;
			}

			ConcourseAnimation animation;

			animation.id = index;
			index++;

			animation.room = values[0];
			animation.posX = std::stoi(values[1], 0, 0);
			animation.posY = std::stoi(values[2], 0, 0);
			animation.nameGroupId = (short)std::stoi(values[3], 0, 0);
			animation.name = "concourse_anim_" + std::to_string(animation.nameGroupId);
			animation.delayMin = std::stoi(values[4], 0, 0);
			animation.delayMax = std::stoi(values[5], 0, 0);
			animation.group = values[6];
			animation.missions = values[7];
			animation.frame = 0;

			this->animations.push_back(animation);
		}
	}

	std::vector<std::string> GetGroups(const char* currentRoom) const
	{
		std::vector<std::string> groups;

		for (const auto& animation : this->animations)
		{
			if (!animation.IsRoom(currentRoom))
			{
				continue;
			}

			if (!animation.IsInCurrentMission())
			{
				continue;
			}

			if (std::find(groups.begin(), groups.end(), animation.group) != groups.end())
			{
				continue;
			}

			groups.push_back(animation.group);
		}

		return groups;
	}

	int GetGroupCount(const char* currentRoom, const char* currentGroup) const
	{
		int count = 0;

		for (const auto& animation : this->animations)
		{
			if (!animation.IsRoom(currentRoom))
			{
				continue;
			}

			if (!animation.IsInCurrentMission())
			{
				continue;
			}

			if (!animation.IsGroup(currentGroup))
			{
				continue;
			}

			count++;
		}

		return count;
	}

	ConcourseAnimation* GetAnimationByIndex(const char* currentRoom, const char* currentGroup, int index)
	{
		int count = 0;

		for (auto& animation : this->animations)
		{
			if (!animation.IsRoom(currentRoom))
			{
				continue;
			}

			if (!animation.IsInCurrentMission())
			{
				continue;
			}

			if (!animation.IsGroup(currentGroup))
			{
				continue;
			}

			if (count == index)
			{
				return &animation;
			}

			count++;
		}

		return nullptr;
	}

	ConcourseAnimation* GetRandomAnimation(const char* currentRoom, const char* currentGroup)
	{
		int count = this->GetGroupCount(currentRoom, currentGroup);
		int index = g_randomGenerator.Generate(0, count);
		return this->GetAnimationByIndex(currentRoom, currentGroup, index);
	}

	void SetGroupFrame(const char* currentRoom, const char* currentGroup, int currentId, int frameIndex)
	{
		for (auto& animation : this->animations)
		{
			if (!animation.IsRoom(currentRoom))
			{
				continue;
			}

			if (!animation.IsInCurrentMission())
			{
				continue;
			}

			if (!animation.IsGroup(currentGroup))
			{
				continue;
			}

			if (animation.id == currentId)
			{
				animation.frame = frameIndex + g_randomGenerator.Generate(animation.delayMin, animation.delayMax);
			}
			else
			{
				animation.frame = 0x7fffffff;
			}
		}
	}

	std::vector<ConcourseAnimation> animations;
};

ConcourseAnimations g_concourseAnimations;

int LoadGameStateHook(int* params)
{
	const auto XwaGetResDataItemImagesCount = (short(*)(short))0x004CF920;
	const auto XwaLoadDatImage = (int(*)(const char*, short, short, short))0x0055E800;

	const int frameCount = *(int*)0x00A21441;
	params[Params_EDX] = frameCount;

	if (frameCount != 0)
	{
		return 0;
	}

	for (const auto& animation : g_concourseAnimations.animations)
	{
		short imageCount = XwaGetResDataItemImagesCount(animation.nameGroupId);
		XwaLoadDatImage(animation.name.c_str(), animation.nameGroupId, 1, imageCount);
	}

	return 0;
}

int FreeGameStateHook(int* params)
{
	*(int*)0x00A1C08D = 0;

	const auto StopSound = (int(*)(const char*))0x00538C50;
	const auto XwaFrontResFreeItem = (void(*)(const char*))0x00532080;
	const auto XwaFreeResDataItem = (short(*)(short))0x004CDE40;

	for (const auto& animation : g_concourseAnimations.animations)
	{
		StopSound(animation.name.c_str());
		XwaFrontResFreeItem(animation.name.c_str());
		XwaFreeResDataItem(animation.nameGroupId);
	}

	return 0;
}

void DrawConcourseAnimations(const char* currentRoom, int frameIndex)
{
	const auto XwaFrontResDraw = (int(*)(const char*, int, int))0x00534A60;
	const auto XwaFrontResMoveToNextImage = (bool(*)(const char*, bool))0x00532230;
	const auto XwaFrontResSetCurrentImage = (bool(*)(const char*, int))0x005322F0;
	const auto XwaFrontResGetCurrentImage = (int(*)(const char*))0x005321F0;
	const auto XwaPlaySound = (int(*)(const char*, int, int, int, int, int))0x00538850;

	if (frameIndex == 0)
	{
		for (const auto& animation : g_concourseAnimations.animations)
		{
			XwaFrontResSetCurrentImage(animation.name.c_str(), 0);
		}

		for (const auto& group : g_concourseAnimations.GetGroups(currentRoom))
		{
			ConcourseAnimation* randomAnimation = g_concourseAnimations.GetRandomAnimation(currentRoom, group.c_str());

			if (randomAnimation != nullptr)
			{
				g_concourseAnimations.SetGroupFrame(currentRoom, randomAnimation->group.c_str(), randomAnimation->id, frameIndex);
			}
		}
	}

	for (auto& animation : g_concourseAnimations.animations)
	{
		if (!animation.IsRoom(currentRoom))
		{
			continue;
		}

		if (!animation.IsInCurrentMission())
		{
			continue;
		}

		if (frameIndex < animation.frame)
		{
			continue;
		}

		if (animation.frame == frameIndex)
		{
			int volume = *(unsigned char*)0x00B0C887 * 12;
			XwaPlaySound(animation.name.c_str(), 1, 0, 0xff, volume, 0x3f);
		}

		XwaFrontResDraw(animation.name.c_str(), animation.posX, animation.posY);
		XwaFrontResMoveToNextImage(animation.name.c_str(), true);

		if (XwaFrontResGetCurrentImage(animation.name.c_str()) == 0)
		{
			ConcourseAnimation* randomAnimation = g_concourseAnimations.GetRandomAnimation(currentRoom, animation.group.c_str());

			if (randomAnimation != nullptr)
			{
				g_concourseAnimations.SetGroupFrame(currentRoom, randomAnimation->group.c_str(), randomAnimation->id, frameIndex + 1);
			}
		}
	}
}

int DrawConcoursePlanetHook(int* params)
{
	const auto Xwa_rand = (int(*)())0x0059BEB0;

	int planetWidthOver2 = params[Params_ESI];
	int planetHeightOver2 = params[Params_EDI];

	int positionX = g_concourseDoors.planet_position_left;
	int positionY = g_concourseDoors.planet_position_top;

	int randX = Xwa_rand();

	if (g_concourseDoors.planet_position_right > g_concourseDoors.planet_position_left)
	{
		positionX += randX % (g_concourseDoors.planet_position_right - g_concourseDoors.planet_position_left);
	}

	int randY = Xwa_rand();

	if (g_concourseDoors.planet_position_bottom > g_concourseDoors.planet_position_top)
	{
		positionY += randY % (g_concourseDoors.planet_position_bottom - g_concourseDoors.planet_position_top);
	}

	static int _mission = -1;
	static int _planetPositionX;
	static int _planetPositionY;

	const int currentMission = *(int*)0x00ABC970;
	const XwaMissionDescription* s_XwaMissionDescriptions = *(XwaMissionDescription**)0x009F4B98;
	const int missionDirectoryId = *(int*)(0x00AE2A60 + 0x002A);

	if (currentMission != _mission)
	{
		_mission = currentMission;
		_planetPositionX = -1;
		_planetPositionY = -1;

		int currentMissionIndex = GetMissionFileNameById(currentMission);

		if (currentMissionIndex != -1)
		{
			const char* missionDirectory = ((const char**)0x00603168)[missionDirectoryId];
			const char* missionFileName = s_XwaMissionDescriptions[currentMissionIndex].MissionFileName;

			char path[256];
			sprintf_s(path, "%s\\%s", missionDirectory, missionDirectoryId == 3 ? "temp.tie" : missionFileName);

			const auto lines = GetMissionConcourseLines(path);

			_planetPositionX = GetFileKeyValueInt(lines, "FrontPlanetPositionX", -1);
			_planetPositionY = GetFileKeyValueInt(lines, "FrontPlanetPositionY", -1);
		}
	}

	params[Params_EBX] = (_planetPositionX != -1 ? _planetPositionX : positionX) - planetWidthOver2;
	params[Params_EDX] = (_planetPositionY != -1 ? _planetPositionY : positionY) - planetHeightOver2;

	return 0;
}

int DrawConcourseDoorsHook(int* params)
{
	const int frameIndex = params[Params_EBP];

	const auto XwaFrontResDraw = (int(*)(const char*, int, int))0x00534A60;
	const auto XwaFrontResDrawCurrentImage = (int(*)(const char*, int, int))0x00535470;
	const auto XwaFrontResMoveToNextImage = (bool(*)(const char*, bool))0x00532230;

	XwaFrontResDrawCurrentImage("tourdoor", g_concourseDoors.tourdoor_posX, g_concourseDoors.tourdoor_posY);
	XwaFrontResDraw("mkanim", g_concourseDoors.mkanim_posX, g_concourseDoors.mkanim_posY);
	XwaFrontResDrawCurrentImage("traindoor", g_concourseDoors.traindoor_posX, g_concourseDoors.traindoor_posY);
	XwaFrontResDrawCurrentImage("combatdoor", g_concourseDoors.combatdoor_poxX, g_concourseDoors.combatdoor_poxY);
	XwaFrontResDrawCurrentImage("filmdoor", g_concourseDoors.filmdoor_posX, g_concourseDoors.filmdoor_posY);
	XwaFrontResDrawCurrentImage("techdoor", g_concourseDoors.techdoor_posX, g_concourseDoors.techdoor_posY);

	DrawConcourseAnimations("concourse", frameIndex);

	return 0;
}

int TrainDoorRectHook(int* params)
{
	int A4 = params[0];

	const auto XwaRectMove = (void(*)(int, int, int))0x00558CD0;

	XwaRectMove(A4, g_concourseDoors.traindoor_posX, g_concourseDoors.traindoor_posY);

	return 0;
}

int CombatDoorRectHook(int* params)
{
	int A4 = params[0];

	const auto XwaRectMove = (void(*)(int, int, int))0x00558CD0;

	XwaRectMove(A4, g_concourseDoors.combatdoor_poxX, g_concourseDoors.combatdoor_poxY);

	return 0;
}

int TourDoorRectHook(int* params)
{
	int A4 = params[0];

	const auto XwaRectMove = (void(*)(int, int, int))0x00558CD0;

	XwaRectMove(A4, g_concourseDoors.tourdoor_posX, g_concourseDoors.tourdoor_posY);

	return 0;
}

int MkanimRectHook(int* params)
{
	int A4 = params[0];
	int x1 = params[1];
	int y1 = params[2];
	int x2 = params[3];
	int y2 = params[4];
	int posX = g_concourseDoors.mkanim_posX;
	int posY = g_concourseDoors.mkanim_posY;

	const auto XwaRectSet = (void(*)(int, int, int, int, int))0x00558C90;

	XwaRectSet(A4, posX, posY, posX + x2 - x1, posY + y2 - y1);

	return 0;
}

int FilmDoorRectHook(int* params)
{
	int A4 = params[0];

	const auto XwaRectMove = (void(*)(int, int, int))0x00558CD0;

	XwaRectMove(A4, g_concourseDoors.filmdoor_posX, g_concourseDoors.filmdoor_posY);

	return 0;
}

int TechDoorRectHook(int* params)
{
	int A4 = params[0];

	const auto XwaRectMove = (void(*)(int, int, int))0x00558CD0;

	XwaRectMove(A4, g_concourseDoors.techdoor_posX, g_concourseDoors.techdoor_posY);

	return 0;
}

std::vector<std::pair<int, int>> g_MedalsPositions;
std::vector<std::string> g_MedalsDescriptions;

void LoadMedalsPositions()
{
	static bool _loaded = false;

	if (_loaded)
	{
		return;
	}

	if (std::ifstream("MedalsPositions.txt"))
	{
		const auto lines = GetFileLines("MedalsPositions.txt");

		for (const std::string& line : lines)
		{
			if (line.empty())
			{
				continue;
			}

			const auto tokens = Tokennize(line);

			if (tokens.size() < 2)
			{
				continue;
			}

			int positionX = std::stoi(tokens[0], 0, 0);
			int positionY = std::stoi(tokens[1], 0, 0);

			g_MedalsPositions.push_back(std::make_pair(positionX, positionY));
		}
	}
	else
	{
		g_MedalsPositions.push_back(std::make_pair(190, 70));
		g_MedalsPositions.push_back(std::make_pair(260, 90));
		g_MedalsPositions.push_back(std::make_pair(330, 70));
		g_MedalsPositions.push_back(std::make_pair(158, 170));
		g_MedalsPositions.push_back(std::make_pair(378, 168));
		g_MedalsPositions.push_back(std::make_pair(196, 300));
		g_MedalsPositions.push_back(std::make_pair(328, 296));
	}

	_loaded = true;
}

void LoadMedalsDescriptions()
{
	static bool _loaded = false;

	if (_loaded)
	{
		return;
	}

	std::ifstream file("MedalsDescriptions.txt");

	if (file)
	{
		std::string line;

		while (std::getline(file, line))
		{
			if (line.empty())
			{
				g_MedalsDescriptions.push_back("Unknown");
			}
			else
			{
				g_MedalsDescriptions.push_back(line);
			}
		}
	}

	_loaded = true;
}

int MedalPositionHook(int* params)
{
	LoadMedalsPositions();

	const int index = params[0];

	if (index < (int)g_MedalsPositions.size())
	{
		params[0] = g_MedalsPositions[index].first;
		params[1] = g_MedalsPositions[index].second;
	}
	else
	{
		params[0] = 0;
		params[1] = 0;
	}

	return 0;
}

int MedalDescriptionHook(int* params)
{
	LoadMedalsDescriptions();

	const int index = params[0];

	const auto XwaGetFrontTxtString = (const char* (*)(int))0x0055CB50;

	const char* description;

	if (index < (int)g_MedalsDescriptions.size())
	{
		description = g_MedalsDescriptions[index].c_str();
	}
	else if (index < 7)
	{
		description = XwaGetFrontTxtString(0x3AD + index);
	}
	else
	{
		description = "Unknown";
	}

	return (int)description;
}

int KalidorCrescentPosition1Hook(int* params)
{
	const int A4 = params[0];

	const auto L00558CD0 = (void(*)(int, int, int))0x00558CD0;

	L00558CD0(A4, g_config.KalidorCrescentPositionX, g_config.KalidorCrescentPositionY);

	return 0;
}

int KalidorCrescentPosition2Hook(int* params)
{
	const int A4 = params[0];

	const auto L00534A60 = (void(*)(int, int, int))0x00534A60;

	L00534A60(A4, g_config.KalidorCrescentPositionX, g_config.KalidorCrescentPositionY);

	return 0;
}

std::vector<int> g_RankPoints;
std::vector<int> g_PilotRatingPoints;

void LoadRankPoints()
{
	static bool _loaded = false;

	if (_loaded)
	{
		return;
	}

	if (std::ifstream("RankPoints.txt"))
	{
		const auto lines = GetFileLines("RankPoints.txt");

		g_RankPoints = GetFileListIntValues(lines);
	}
	else
	{
		const int* s_RankPoints = (int*)0x005B30C0;

		for (int i = 0; i < 8; i++)
		{
			int points = s_RankPoints[i];
			g_RankPoints.push_back(points);
		}
	}

	if (std::ifstream("PilotRatingPoints.txt"))
	{
		const auto lines = GetFileLines("PilotRatingPoints.txt");

		g_PilotRatingPoints = GetFileListIntValues(lines);
	}
	else
	{
		const int* s_PilotRatingPoints = (int*)0x005B2FD4;

		for (int i = 0; i < 30; i++)
		{
			int points = s_PilotRatingPoints[i];
			g_PilotRatingPoints.push_back(points);
		}
	}

	_loaded = true;
}

int GetRankPointsArrayPtr()
{
	LoadRankPoints();

	return (int)g_RankPoints.data();
}

int GetPilotRatingPointsArrayPtr()
{
	LoadRankPoints();

	return (int)g_PilotRatingPoints.data();
}

int MedalAndKalidorCrescentDescriptionHook(int* params)
{
	int index = params[0];

	const auto XwaGetFrontTxtString = (const char* (*)(int))0x0055CB50;

	const char* description;

	if (index > 100)
	{
		// Medal Description
		LoadMedalsDescriptions();

		index = index - 0x3AD;

		if (index < (int)g_MedalsDescriptions.size())
		{
			description = g_MedalsDescriptions[index].c_str();
		}
		else if (index < 7)
		{
			description = XwaGetFrontTxtString(0x3AD + index);
		}
		else
		{
			description = "Unknown";
		}
	}
	else
	{
		// Kalidor Crescent Description
		LoadCustomPoints();

		index = index - 1;

		if (index < (int)g_KalidorCrescentDescriptions.size())
		{
			description = g_KalidorCrescentDescriptions[index].c_str();
		}
		else if (index - 1 < 6)
		{
			description = XwaGetFrontTxtString(0x3A7 + index);
		}
		else
		{
			description = "Unknown";
		}
	}

	return (int)description;
}

bool DoesMovieExist(const std::string& name)
{
	const char* extensions[] =
	{
		".avi",
		".wmv",
		".mp4",
		".znm",
		".snm"
	};

	for (const char* ext : extensions)
	{
		std::string filename = std::string("movies\\") + name + ext;

		if (std::ifstream(filename))
		{
			return true;
		}
	}

	return false;
}

int PlayPerMissionMovieHook(int* params)
{
	const char* A4 = (const char*)params[0];
	const int A8 = params[1];

	const auto XwaPlayMovie = (int(*)(const char*, int))0x0055BC20;

	const XwaMissionDescription* s_XwaMissionDescriptions = *(XwaMissionDescription**)0x009F4B98;
	const int s_V0x09F5E74 = *(int*)0x009F5E74;

	std::string missionFilename = GetStringWithoutExtension(s_XwaMissionDescriptions[s_V0x09F5E74].MissionFileName);
	std::string name = missionFilename + "_" + A4;

	if (DoesMovieExist(name))
	{
		XwaPlayMovie(name.c_str(), A8);
	}
	else
	{
		XwaPlayMovie(A4, A8);
	}

	return 0;
}

std::vector<unsigned char> g_emailsStatus;

void UpdateEmailsStatusPointers()
{
	*(int*)(0x00539F3B + 0x02) = (int)g_emailsStatus.data();
	*(int*)(0x00539F59 + 0x02) = (int)g_emailsStatus.data();
	*(int*)(0x00560048 + 0x02) = (int)g_emailsStatus.data();
	*(int*)(0x0056005D + 0x02) = (int)g_emailsStatus.data();
	*(int*)(0x00560C06 + 0x02) = (int)g_emailsStatus.data();
	*(int*)(0x00560C9A + 0x02) = (int)g_emailsStatus.data();
	*(int*)(0x00560E45 + 0x02) = (int)g_emailsStatus.data();
	*(int*)(0x00563937 + 0x02) = (int)g_emailsStatus.data();
	*(int*)(0x00563949 + 0x02) = (int)g_emailsStatus.data();
	*(int*)(0x0056395E + 0x02) = (int)g_emailsStatus.data();
}

void ResizeEmailsStatusBuffer(int newSize)
{
	std::vector<unsigned char> buffer;
	buffer.reserve(g_emailsStatus.capacity());
	memcpy_s(buffer.data(), g_emailsStatus.capacity(), g_emailsStatus.data(), g_emailsStatus.capacity());

	g_emailsStatus.clear();
	g_emailsStatus.reserve(newSize);
	memset(g_emailsStatus.data(), 0, g_emailsStatus.capacity());

	memcpy_s(g_emailsStatus.data(), std::min(g_emailsStatus.capacity(), buffer.capacity()), buffer.data(), std::min(g_emailsStatus.capacity(), buffer.capacity()));

	UpdateEmailsStatusPointers();
}

int WritePilotHook(int* params)
{
	const char* pilotName = (const char*)0x00ABD680;

	if (std::ofstream file{ std::string(pilotName) + "_emails.bin", std::ofstream::trunc | std::ofstream::binary })
	{
		file.write((const char*)g_emailsStatus.data(), g_emailsStatus.capacity());
	}

	return 1;
}

int ReadPilotHook(int* params)
{
	const char* pilotName = (const char*)0x00ABD680;

	g_emailsStatus.clear();

	if (std::ifstream file{ std::string(pilotName) + "_emails.bin", std::ifstream::binary })
	{
		file.seekg(0, std::ios::end);
		int count = (int)file.tellg();
		file.seekg(0, std::ios::beg);
		ResizeEmailsStatusBuffer(count);
		file.read((char*)g_emailsStatus.data(), count);
	}
	else
	{
		int offset = 0x00AE2A60 + 0x2508E;
		int count = 100;
		ResizeEmailsStatusBuffer(count);
		memcpy_s(g_emailsStatus.data(), g_emailsStatus.capacity(), (const char*)offset, count);
	}

	return 1;
}

int CreatePilotHook(int* params)
{
	const char* A4 = (const char*)params[0];
	const char* A8 = (const char*)params[1];

	const auto Xwa_fopen = (int(*)(const char*, const char*))0x0059AE10;

	int result = Xwa_fopen(A4, A8);

	if (result != 0)
	{
		return result;
	}

	const char* pilotName = (const char*)params[14];

	g_emailsStatus.clear();
	ResizeEmailsStatusBuffer(100);

	if (std::ofstream file{ std::string(pilotName) + "_emails.bin", std::ofstream::trunc | std::ofstream::binary })
	{
		file.write((const char*)g_emailsStatus.data(), g_emailsStatus.capacity());
	}

	return 0;
}

int ReadEmailTxtHook(int* params)
{
	int& s_XwaEmailsCount = *(int*)0x009F4B10;

	s_XwaEmailsCount = params[Params_EDI];
	ResizeEmailsStatusBuffer(s_XwaEmailsCount);

	return 0;
}

int RandInteger(int min, int max)
{
	const auto Xwa_rand = (int(*)())0x0059BEB0;

	int value = ((Xwa_rand() & 0x7fff) << 15) | (Xwa_rand() & 0x7fff);
	return min + (value % (max - min));
}

static unsigned short g_concourseStarfieldColors[31];
static std::vector<int> g_concourseStarfieldPositionsX;
static std::vector<int> g_concourseStarfieldPositionsY;
static std::vector<int> g_concourseStarfieldFrames;
static std::vector<int> g_concourseStarfieldTypes;

int SetupConcourseStarfieldHook(int* params)
{
	g_concourseStarfieldPositionsX.clear();
	g_concourseStarfieldPositionsY.clear();
	g_concourseStarfieldFrames.clear();
	g_concourseStarfieldTypes.clear();

	const auto XwaGetCurrentSurfacePitch = (int(*)())0x0053FA60;
	const auto XwaGetColor = (unsigned int(*)(unsigned char, unsigned char, unsigned char))0x0055D270;
	const auto Xwa_rand = (int(*)())0x0059BEB0;

	for (int i = 0; i < 16; i++)
	{
		unsigned char al = (unsigned char)(0x2F + i * 0x0B);
		unsigned char bl = (unsigned char)(0x40 + i * 0x0C);

		g_concourseStarfieldColors[i] = (unsigned short)XwaGetColor(al, al, bl);
	}

	for (int i = 0; i < 15; i++)
	{
		g_concourseStarfieldColors[16 + i] = g_concourseStarfieldColors[14 - i];
	}

	unsigned short* surfaceData = *(unsigned short**)0x009F60D4;
	int surfacePitch = XwaGetCurrentSurfacePitch() / 2;
	int width = 640;
	int height = 480;

	std::vector<std::tuple<int, int>> pixels;
	pixels.reserve(width * height);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (surfaceData[surfacePitch * y + x] != 0)
			{
				continue;
			}

			pixels.push_back(std::make_tuple(x, y));
		}
	}

	int count = pixels.size() * 256 / 50000;

	g_concourseStarfieldPositionsX.resize(count);
	g_concourseStarfieldPositionsY.resize(count);
	g_concourseStarfieldFrames.resize(count);
	g_concourseStarfieldTypes.resize(count);

	for (int i = 0; i < count; i++)
	{
		int index = RandInteger(0, pixels.size());
		const auto& pixel = pixels[index];

		int x = std::get<0>(pixel);
		int y = std::get<1>(pixel);

		g_concourseStarfieldPositionsX[i] = x;
		g_concourseStarfieldPositionsY[i] = y;
		g_concourseStarfieldFrames[i] = Xwa_rand() % 31;
		g_concourseStarfieldTypes[i] = 2 + (Xwa_rand() % 8);
	}

	return 1;
}

int DrawConcourseStarfieldHook(int* params)
{
	const int frameIndex = params[0];

	const auto XwaGetCurrentSurfacePitch = (int(*)())0x0053FA60;

	unsigned short* surfaceData = *(unsigned short**)0x009F60D4;
	int surfacePitch = XwaGetCurrentSurfacePitch() / 2;

	for (int i = 0; i < (int)g_concourseStarfieldTypes.size(); i++)
	{
		int x = g_concourseStarfieldPositionsX[i];
		int y = g_concourseStarfieldPositionsY[i];
		int frame = g_concourseStarfieldFrames[i];
		int type = g_concourseStarfieldTypes[i];

		surfaceData[surfacePitch * y + x] = g_concourseStarfieldColors[frame];

		if (type == 9)
			continue;

		if (2 + (frameIndex % 7) != type)
			continue;

		frame++;

		if (frame >= 31)
		{
			frame = 0;
		}

		g_concourseStarfieldFrames[i] = frame;
	}

	return 1;
}

static bool g_isPlanetImageLoaded = false;

int GetMissionFrontPlanetImageIndex(const std::string& mission)
{
	const auto lines = GetMissionConcourseLines(mission);

	return GetFileKeyValueInt(lines, "FrontPlanetIndex", -1);
}

int SwitchFrontPlanetImageHook(int* params)
{
	const auto XwaLoadPlanetImage = (int(*)(int))0x0055DCA0;

	const XwaMissionDescription* s_XwaMissionDescriptions = *(XwaMissionDescription**)0x009F4B98;
	const int missionDirectoryId = *(int*)(0x00AE2A60 + 0x002A);
	const int currentMission = *(int*)0x00ABC970;

	int currentMissionIndex = GetMissionFileNameById(currentMission);

	if (currentMissionIndex != -1)
	{
		const char* missionDirectory = ((const char**)0x00603168)[missionDirectoryId];
		const char* missionFileName = s_XwaMissionDescriptions[currentMissionIndex].MissionFileName;

		char path[256];
		sprintf_s(path, "%s\\%s", missionDirectory, missionDirectoryId == 3 ? "temp.tie" : missionFileName);

		int missionPlanetImageIndex = GetMissionFrontPlanetImageIndex(path);

		if (missionPlanetImageIndex != -1)
		{
			g_isPlanetImageLoaded = true;
			XwaLoadPlanetImage(missionPlanetImageIndex - 0x3AA1);
		}
	}

	int edi = params[Params_EDI];

	if (edi == 0)
	{
		params[Params_ReturnAddress] = 0x0053A005;
	}

	return 0;
}

int LoadFrontPlanetImageHook(int* params)
{
	int planetImageIndex = params[0];

	const auto XwaLoadPlanetImage = (int(*)(int))0x0055DCA0;

	if (!g_isPlanetImageLoaded)
	{
		XwaLoadPlanetImage(planetImageIndex);
	}

	return 0;
}
