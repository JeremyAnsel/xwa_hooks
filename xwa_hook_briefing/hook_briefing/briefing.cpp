#include "targetver.h"
#include "briefing.h"
#include "config.h"
#include <unordered_map>
#include <array>
#define NOMINMAX
#include <Windows.h>

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

class Config
{
public:
	Config()
	{
		auto lines = GetFileLines("hook_briefing.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_briefing");
		}

		this->ShowDebugMessages = GetFileKeyValueInt(lines, "ShowDebugMessages", 0) != 0;
	}

	bool ShowDebugMessages;
};

Config g_config;

std::string GetTranslateString(const std::string& section, const std::vector<std::string>& lines, const std::string& key)
{
	static std::string _section;
	static std::vector<std::string> _lines;

	if (section != _section)
	{
		_section = section;
		_lines = GetFileLines("xwa.ini", section);
	}

	std::string value = GetFileKeyValue(lines, key);
	std::string translated = GetFileKeyValue(_lines, key);

	return translated.size() ? translated : value;
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

std::string to_string(int n, int width)
{
	std::string s(width, '0');
	for (int i = width; i--; n /= 10)
	{
		s[i] = char('0' + n % 10);
	}
	return s;
}

typedef int(*gameUpdateFunction)(int frameCount);

const auto XwaSwitchGameState = (void(*)(gameUpdateFunction, gameUpdateFunction))0x00541810;
const auto XwaRunGameState = (void(*)(gameUpdateFunction, LPRECT))0x00541890;
const auto XwaPopGameState = (void(*)())0x00541CB0;
const auto XwaResetEntryText = (int(*)())0x0055B590;
const auto XwaRectSet = (void(*)(LPRECT, LONG, LONG, LONG, LONG))0x00558C90;
const auto XwaRectCopy = (void(*)(LPRECT, LPRECT))0x00558CB0;
const auto XwaRectMove = (void(*)(LPRECT, LONG, LONG))0x00558CD0;
const auto XwaFrontResLoad = (int(*)(const char*, const char*))0x00531D70;
const auto XwaFrontResFreeItem = (void(*)(const char*))0x00532080;
const auto XwaFrontResGetArea = (bool(*)(const char*, RECT*))0x00532180;
const auto XwaFrontResDraw = (int(*)(const char*, int, int))0x00534A60;
const auto XwaFrontResDrawCurrentImage = (int(*)(const char*, int, int))0x00535470;
const auto XwaFrontResSetCurrentImage = (bool(*)(const char*, int))0x005322F0;
const auto XwaFrontResMoveToNextImage = (bool(*)(const char*, bool))0x00532230;
const auto XwaOffScreenSurfaceLock = (int(*)())0x0053F830;
const auto XwaOffScreenSurfaceUnlock = (int(*)(int))0x0053F8D0;
const auto XwaFontGetStringWidth = (int(*)(const char*, int))0x00557F10;
const auto L00556100 = (int(*)(int, int, const char*, int, int, int, int, const char*))0x00556100;
const auto L00557AB0 = (int(*)(int, const char*, LPRECT, short, int, int))0x00557AB0;
const auto L00569BE0 = (void(*)(LPRECT, const char*, short))0x00569BE0;
const auto L00557910 = (int(*)(int, const char*, LPRECT, int, int, unsigned short))0x00557910;
const auto XwaUILabel = (int(*)(int, const char*, LPRECT, unsigned int))0x005575A0;
const auto XwaUIScrollBar = (int(*)(LPRECT, int, int, int, int, unsigned int, int))0x005555D0;
const auto XwaPlayWaveSound = (int(*)(const char*, unsigned char, unsigned char))0x005582E0;
const auto XwaStopWaveSound = (void(*)())0x00558BE0;

std::string GetMissionFileName()
{
	const int missionDirectoryId = *(int*)(0x00AE2A60 + 0x002A);
	const char* missionDirectory = ((const char**)0x00603168)[missionDirectoryId];
	const int s_V0x09F5E74 = *(int*)0x009F5E74;
	const XwaMissionDescription* s_XwaMissionDescriptions = *(XwaMissionDescription**)0x009F4B98;

	if (s_XwaMissionDescriptions == nullptr)
	{
		return "default.tie";
	}

	std::string missionFilename = std::string(missionDirectory) + std::string("\\") + std::string(s_XwaMissionDescriptions[s_V0x09F5E74].MissionFileName);
	return missionFilename;
}

enum GoalMethodEnum
{
	GoalMethod_All,
	GoalMethod_AtLeastOne
};

struct MissionQuestion
{
	std::string question;
	std::string answer;
	std::string answerWave;
	std::string officer;
	int officerPositionX;
	int officerPositionY;
	std::string goals;
	GoalMethodEnum goalsMethod;
};

GoalMethodEnum ParseGoalsMethod(const std::string& method)
{
	if (method == "All")
	{
		return GoalMethod_All;
	}

	if (method == "AtLeastOne")
	{
		return GoalMethod_AtLeastOne;
	}

	return GoalMethod_All;
}

bool CheckGoals(const std::string& questionGoalsStr, GoalMethodEnum method);

class MissionConfig
{
public:
	bool HasBriefingQuestions()
	{
		this->UpdateIfChanged();
		return this->_briefingQuestions.size() != 0;
	}

	std::string GetBriefingOfficerImage()
	{
		this->UpdateIfChanged();
		return this->_briefingOfficerImage;
	}

	int GetBriefingOfficerImagePositionX()
	{
		this->UpdateIfChanged();
		return this->_briefingOfficerImagePositionX;
	}

	int GetBriefingOfficerImagePositionY()
	{
		this->UpdateIfChanged();
		return this->_briefingOfficerImagePositionY;
	}

	std::string GetBriefingBackQuestion()
	{
		this->UpdateIfChanged();
		return this->_briefingBackQuestion;
	}

	std::string GetBriefingDoneQuestion()
	{
		this->UpdateIfChanged();
		return this->_briefingDoneQuestion;
	}

	const std::vector<MissionQuestion>& GetBriefingQuestions()
	{
		this->UpdateIfChanged();
		return this->_briefingQuestions;
	}

	bool HasDebriefingQuestions()
	{
		this->UpdateIfChanged();
		return this->_debriefingVictoryQuestions.size() != 0 || this->_debriefingDefeatQuestions.size() != 0;
	}

	bool HasDebriefingVictoryQuestions()
	{
		this->UpdateIfChanged();
		return this->_debriefingVictoryQuestions.size() != 0;
	}

	std::string GetDebriefingVictoryOfficerImage()
	{
		this->UpdateIfChanged();
		return this->_debriefingVictoryOfficerImage;
	}

	int GetDebriefingVictoryOfficerImagePositionX()
	{
		this->UpdateIfChanged();
		return this->_debriefingVictoryOfficerImagePositionX;
	}

	int GetDebriefingVictoryOfficerImagePositionY()
	{
		this->UpdateIfChanged();
		return this->_debriefingVictoryOfficerImagePositionY;
	}

	std::string GetDebriefingVictoryBackQuestion()
	{
		this->UpdateIfChanged();
		return this->_debriefingVictoryBackQuestion;
	}

	std::string GetDebriefingVictoryDoneQuestion()
	{
		this->UpdateIfChanged();
		return this->_debriefingVictoryDoneQuestion;
	}

	const std::vector<MissionQuestion>& GetDebriefingVictoryQuestions()
	{
		this->UpdateIfChanged();
		return this->_debriefingVictoryQuestions;
	}

	bool HasDebriefingDefeatQuestions()
	{
		this->UpdateIfChanged();
		return this->_debriefingDefeatQuestions.size() != 0;
	}

	std::string GetDebriefingDefeatOfficerImage()
	{
		this->UpdateIfChanged();
		return this->_debriefingDefeatOfficerImage;
	}

	int GetDebriefingDefeatOfficerImagePositionX()
	{
		this->UpdateIfChanged();
		return this->_debriefingDefeatOfficerImagePositionX;
	}

	int GetDebriefingDefeatOfficerImagePositionY()
	{
		this->UpdateIfChanged();
		return this->_debriefingDefeatOfficerImagePositionY;
	}

	std::string GetDebriefingDefeatBackQuestion()
	{
		this->UpdateIfChanged();
		return this->_debriefingDefeatBackQuestion;
	}

	std::string GetDebriefingDefeatDoneQuestion()
	{
		this->UpdateIfChanged();
		return this->_debriefingDefeatDoneQuestion;
	}

	const std::vector<MissionQuestion>& GetDebriefingDefeatQuestions()
	{
		this->UpdateIfChanged();
		return this->_debriefingDefeatQuestions;
	}

private:
	void UpdateIfChanged()
	{
		static std::string _mission;
		static int _missionIndex = 0;

		std::string xwaMissionFileName = GetMissionFileName();
		const int missionFileNameIndex = *(int*)0x06002E4;

		if (missionFileNameIndex == 0 ? (_mission == xwaMissionFileName) : (_missionIndex == missionFileNameIndex))
		{
			return;
		}

		_mission = xwaMissionFileName;
		_missionIndex = missionFileNameIndex;

		const std::string path = GetStringWithoutExtension(_mission);
		const int officerPositionXDefault = 100;
		const int officerPositionYDefault = 150;

		auto lines = GetFileLines(path + "BriefingQuestions.txt");

		if (!lines.size())
		{
			lines = GetFileLines(path + ".ini", "BriefingQuestions");
		}

		this->_briefingOfficerImage = GetFileKeyValue(lines, "OfficerImage");
		if (this->_briefingOfficerImage.empty())
		{
			this->_briefingOfficerImage = "frontres\\tour\\squad5.cbm";
		}

		this->_briefingOfficerImagePositionX = GetFileKeyValueInt(lines, "OfficerImagePositionX", officerPositionXDefault);
		this->_briefingOfficerImagePositionY = GetFileKeyValueInt(lines, "OfficerImagePositionY", officerPositionYDefault);

		this->_briefingBackQuestion = GetTranslateString(path + "_BriefingQuestions", lines, "BackQuestion");
		if (this->_briefingBackQuestion.empty())
		{
			this->_briefingBackQuestion = "Back";
		}

		this->_briefingDoneQuestion = GetTranslateString(path + "_BriefingQuestions", lines, "DoneQuestion");
		if (this->_briefingDoneQuestion.empty())
		{
			this->_briefingDoneQuestion = "Done";
		}

		this->_briefingQuestions.clear();
		for (int i = 0; i < 100; i++)
		{
			MissionQuestion question;
			question.question = GetTranslateString(path + "_BriefingQuestions", lines, "Question" + to_string(i, 2));
			question.answer = GetTranslateString(path + "_BriefingQuestions", lines, "Answer" + to_string(i, 2));
			question.answerWave = GetTranslateString(path + "_BriefingQuestions", lines, "AnswerWave" + to_string(i, 2));
			question.officer = GetFileKeyValue(lines, "Officer" + to_string(i, 2));
			question.officerPositionX = GetFileKeyValueInt(lines, "OfficerPositionX" + to_string(i, 2), officerPositionXDefault);
			question.officerPositionY = GetFileKeyValueInt(lines, "OfficerPositionY" + to_string(i, 2), officerPositionYDefault);

			if (question.question.empty() || question.answer.empty())
			{
				continue;
			}

			this->_briefingQuestions.push_back(question);
		}

		lines = GetFileLines(path + "DebriefingVictoryQuestions.txt");

		if (!lines.size())
		{
			lines = GetFileLines(path + ".ini", "DebriefingVictoryQuestions");
		}

		this->_debriefingVictoryOfficerImage = GetFileKeyValue(lines, "OfficerImage");
		if (this->_debriefingVictoryOfficerImage.empty())
		{
			this->_debriefingVictoryOfficerImage = "frontres\\tour\\squad5.cbm";
		}

		this->_debriefingVictoryOfficerImagePositionX = GetFileKeyValueInt(lines, "OfficerImagePositionX", officerPositionXDefault);
		this->_debriefingVictoryOfficerImagePositionY = GetFileKeyValueInt(lines, "OfficerImagePositionY", officerPositionYDefault);

		this->_debriefingVictoryBackQuestion = GetTranslateString(path + "_DebriefingVictoryQuestions", lines, "BackQuestion");
		if (this->_debriefingVictoryBackQuestion.empty())
		{
			this->_debriefingVictoryBackQuestion = "Back";
		}

		this->_debriefingVictoryDoneQuestion = GetTranslateString(path + "_DebriefingVictoryQuestions", lines, "DoneQuestion");
		if (this->_debriefingVictoryDoneQuestion.empty())
		{
			this->_debriefingVictoryDoneQuestion = "Done";
		}

		this->_debriefingVictoryQuestions.clear();
		for (int i = 0; i < 100; i++)
		{
			MissionQuestion question;
			question.question = GetTranslateString(path + "_DebriefingVictoryQuestions", lines, "Question" + to_string(i, 2));
			question.answer = GetTranslateString(path + "_DebriefingVictoryQuestions", lines, "Answer" + to_string(i, 2));
			question.answerWave = GetTranslateString(path + "_DebriefingVictoryQuestions", lines, "AnswerWave" + to_string(i, 2));
			question.officer = GetFileKeyValue(lines, "Officer" + to_string(i, 2));
			question.officerPositionX = GetFileKeyValueInt(lines, "OfficerPositionX" + to_string(i, 2), officerPositionXDefault);
			question.officerPositionY = GetFileKeyValueInt(lines, "OfficerPositionY" + to_string(i, 2), officerPositionYDefault);
			question.goals = GetFileKeyValue(lines, "Goals" + to_string(i, 2));
			question.goalsMethod = ParseGoalsMethod(GetFileKeyValue(lines, "GoalsMethod" + to_string(i, 2)));

			if (question.question.empty() || question.answer.empty() || !CheckGoals(question.goals, question.goalsMethod))
			{
				continue;
			}

			this->_debriefingVictoryQuestions.push_back(question);
		}

		lines = GetFileLines(path + "DebriefingDefeatQuestions.txt");

		if (!lines.size())
		{
			lines = GetFileLines(path + ".ini", "DebriefingDefeatQuestions");
		}

		this->_debriefingDefeatOfficerImage = GetFileKeyValue(lines, "OfficerImage");
		if (this->_debriefingDefeatOfficerImage.empty())
		{
			this->_debriefingDefeatOfficerImage = "frontres\\tour\\squad5.cbm";
		}

		this->_debriefingDefeatOfficerImagePositionX = GetFileKeyValueInt(lines, "OfficerImagePositionX", officerPositionXDefault);
		this->_debriefingDefeatOfficerImagePositionY = GetFileKeyValueInt(lines, "OfficerImagePositionY", officerPositionYDefault);

		this->_debriefingDefeatBackQuestion = GetTranslateString(path + "_DebriefingDefeatQuestions", lines, "BackQuestion");
		if (this->_debriefingDefeatBackQuestion.empty())
		{
			this->_debriefingDefeatBackQuestion = "Back";
		}

		this->_debriefingDefeatDoneQuestion = GetTranslateString(path + "_DebriefingDefeatQuestions", lines, "DoneQuestion");
		if (this->_debriefingDefeatDoneQuestion.empty())
		{
			this->_debriefingDefeatDoneQuestion = "Done";
		}

		this->_debriefingDefeatQuestions.clear();
		for (int i = 0; i < 100; i++)
		{
			MissionQuestion question;
			question.question = GetTranslateString(path + "_DebriefingDefeatQuestions", lines, "Question" + to_string(i, 2));
			question.answer = GetTranslateString(path + "_DebriefingDefeatQuestions", lines, "Answer" + to_string(i, 2));
			question.answerWave = GetTranslateString(path + "_DebriefingDefeatQuestions", lines, "AnswerWave" + to_string(i, 2));
			question.officer = GetFileKeyValue(lines, "Officer" + to_string(i, 2));
			question.officerPositionX = GetFileKeyValueInt(lines, "OfficerPositionX" + to_string(i, 2), officerPositionXDefault);
			question.officerPositionY = GetFileKeyValueInt(lines, "OfficerPositionY" + to_string(i, 2), officerPositionYDefault);
			question.goals = GetFileKeyValue(lines, "Goals" + to_string(i, 2));
			question.goalsMethod = ParseGoalsMethod(GetFileKeyValue(lines, "GoalsMethod" + to_string(i, 2)));

			if (question.question.empty() || question.answer.empty() || !CheckGoals(question.goals, question.goalsMethod))
			{
				continue;
			}

			this->_debriefingDefeatQuestions.push_back(question);
		}
	}

	std::string _briefingOfficerImage;
	int _briefingOfficerImagePositionX;
	int _briefingOfficerImagePositionY;
	std::string _briefingBackQuestion;
	std::string _briefingDoneQuestion;
	std::vector<MissionQuestion> _briefingQuestions;

	std::string _debriefingVictoryOfficerImage;
	int _debriefingVictoryOfficerImagePositionX;
	int _debriefingVictoryOfficerImagePositionY;
	std::string _debriefingVictoryBackQuestion;
	std::string _debriefingVictoryDoneQuestion;
	std::vector<MissionQuestion> _debriefingVictoryQuestions;

	std::string _debriefingDefeatOfficerImage;
	int _debriefingDefeatOfficerImagePositionX;
	int _debriefingDefeatOfficerImagePositionY;
	std::string _debriefingDefeatBackQuestion;
	std::string _debriefingDefeatDoneQuestion;
	std::vector<MissionQuestion> _debriefingDefeatQuestions;
};

MissionConfig g_missionConfig;

static std::unordered_map<std::string, int> g_stringWidth;

int CustomL00569BE0(LPRECT A4, const char* A8, short AC, int startLineIndex, int maxLines)
{
	unsigned int defaultColor = *(unsigned int*)0x009F7EFB;

	RECT esp24;
	XwaRectCopy(&esp24, A4);

	int esp34 = esp24.right - esp24.left;
	int esp30 = esp24.top + 0x0D;

	int edi = 0;

	char esp38[8192]{};
	esp38[0] = 0;

	short esp14 = -1;
	short esp10 = -1;
	int esp18 = 0;
	int esp1C = 0;

	short esp20 = 0;
	int linesCount = 0;

	while (esp20 == 0)
	{
		if (startLineIndex != -1 && (linesCount - startLineIndex >= maxLines))
		{
			break;
		}

		unsigned char al = (unsigned char)A8[edi];

		if (al != '$' && al != 0)
		{
			if (al == ' ')
			{
				esp1C = edi;
			}

			if (isspace(al) == 0)
			{
				while (true)
				{
					unsigned char al = (unsigned char)A8[edi];

					if (isspace(al) != 0 || al == '$' || al == 0)
						break;

					if (al == '[')
					{
						esp38[edi - esp18] = 0x05;
						edi++;
					}
					else if (al == ']')
					{
						esp38[edi - esp18] = 0x02;
						edi++;
					}
					else
					{
						esp38[edi - esp18] = al;
						edi++;
					}
				}
			}
			else
			{
				esp38[edi - esp18] = al;
				edi++;
			}

			esp38[edi - esp18] = 0;

			int width;
			std::string key = esp38;
			auto it = g_stringWidth.find(key);

			if (it != g_stringWidth.end())
			{
				width = it->second;
			}
			else
			{
				width = XwaFontGetStringWidth(esp38, 0x0A);
				g_stringWidth.insert({ key, width });
			}

			if (width >= esp34)
			{
				esp10 = esp18;
				esp18 = esp1C + 1;

				esp14 = esp1C;
				edi = esp18;
			}
		}
		else
		{
			esp10 = esp18;

			if (AC == 0 && al == '$')
			{
				esp14 = edi - 1;
			}
			else
			{
				esp14 = edi;
			}

			esp18 = edi + 1;

			edi = esp18;

			if (A8[esp18] == 0)
			{
				esp20 = 0x01;
			}
		}

		int edx = 0;

		for (int esi = 0; esi < esp10; esi++)
		{
			unsigned char al = (unsigned char)A8[esi];

			if (al == '[')
			{
				edx = 0x01;
			}

			if (al == ']')
			{
				edx = 0;
			}
		}

		if (esp10 == -1)
			continue;

		int eax = 0;

		if (edx != 0)
		{
			esp38[eax] = 0x02;
			eax++;
		}

		esp38[eax] = 0;

		for (int esi = esp10; esi <= esp14; esi++)
		{
			unsigned char cl = (unsigned char)A8[esi];

			if (cl == '[')
			{
				esp38[eax] = 0x05;
				eax++;
			}
			else if (cl == ']')
			{
				esp38[eax] = 0x02;
				eax++;
			}
			else
			{
				esp38[eax] = cl;
				eax++;
			}
		}

		esp38[eax] = 0;

		if (startLineIndex != -1 && linesCount >= startLineIndex)
		{
			if (AC == 0 && esp38[0] == '>')
			{
				XwaRectMove(&esp24, 0, 0x01);
				XwaUILabel(0x0A, esp38 + 0x01, &esp24, *(unsigned int*)0x0AE2A48);
				XwaRectMove(&esp24, 0, -1);
			}
			else
			{
				L00557910(0x0A, esp38, &esp24, 0, 0, defaultColor);
			}
		}

		esp14 = -1;
		esp10 = -1;

		if (startLineIndex != -1 && linesCount >= startLineIndex)
		{
			XwaRectMove(&esp24, 0, 0x0E);
		}

		linesCount++;
	}

	return linesCount;
}

static int g_RenderFormattedText_ItemsIndex = 0;

void RenderFormattedText(LPRECT textrc, const std::string& text)
{
	std::string str = text;
	str.resize(8190);
	//CustomL00569BE0(rc, str.c_str(), 0);

	int linesCount = CustomL00569BE0(textrc, str.c_str(), 0, -1, 0);
	int maxLines = (480 - textrc->bottom - textrc->top) / 0x0A;

	RECT scrollBarRect;
	XwaRectSet(&scrollBarRect, textrc->right + 1, textrc->top, textrc->right + 20, textrc->bottom + 20);
	if (linesCount > maxLines)
	{
		g_RenderFormattedText_ItemsIndex = XwaUIScrollBar(&scrollBarRect, g_RenderFormattedText_ItemsIndex, linesCount, 0, 0x05, *(unsigned int*)0x0B07C6C, maxLines);
	}

	int currentCount = std::min(linesCount - g_RenderFormattedText_ItemsIndex, maxLines);
	CustomL00569BE0(textrc, str.c_str(), 0, g_RenderFormattedText_ItemsIndex, currentCount);

	//RECT rc;
	//XwaRectSet(&rc, 100, 385, 610, 480);
	//std::string msg = "linesCount=" + std::to_string(linesCount) + " maxLines=" + std::to_string(maxLines);
	//L00557AB0(12, msg.c_str(), &rc, 0xffffffff, 0x03, 0);
}

static std::string g_custom_briefing_answer;

int BriefingGameStateExit(int A4)
{
	g_custom_briefing_answer.clear();
	g_stringWidth.clear();

	// Free mission descriptions
	if (*(int*)0x009F4B98)
	{
		((void(*)(int))0x0055D480)(*(int*)0x009F4B98);
		*(int*)0x009F4B98 = 0;
	}

	XwaFrontResFreeItem("background");
	XwaFrontResFreeItem("officer");

	return 0;
}

int BriefingGameStateUpdate(int A4)
{
	int gotoState = 0;

	static int itemsIndex;
	static int officerImageX;
	static int officerImageY;

	if (A4 == 0)
	{
		itemsIndex = 0;
		g_RenderFormattedText_ItemsIndex = 0;

		// Load mission descriptions
		((void(*)(int))0x00547800)(*(int*)0x00AE2A8A);

		XwaFrontResLoad("frontres\\combat\\multiplayer.bmp", "background");
		XwaFrontResLoad(g_missionConfig.GetBriefingOfficerImage().c_str(), "officer");
		officerImageX = g_missionConfig.GetBriefingOfficerImagePositionX();
		officerImageY = g_missionConfig.GetBriefingOfficerImagePositionY();
		XwaFrontResSetCurrentImage("officer", 0);

		XwaFrontResDrawCurrentImage("background", 0, 0);
		XwaOffScreenSurfaceLock();
		XwaFrontResDrawCurrentImage("background", 0, 0);
		XwaOffScreenSurfaceUnlock(0x01);

		g_custom_briefing_answer.clear();
	}

	XwaFrontResDrawCurrentImage("background", 0, 0);

	RECT officerRect;
	XwaFrontResGetArea("officer", &officerRect);
	XwaFrontResDraw("officer", officerImageX, officerImageY);
	XwaFrontResMoveToNextImage("officer", 1);

	if (g_custom_briefing_answer.size())
	{
		RECT rc;
		XwaRectSet(&rc, 320, 60, 560, 260);

		RenderFormattedText(&rc, g_custom_briefing_answer);
	}

	auto& questions = g_missionConfig.GetBriefingQuestions();
	int count = questions.size() + 2;

	RECT scrollBarRect;
	XwaRectSet(&scrollBarRect, 561, 290, 580, 390);
	int itemsCount = count - 2;
	int stepCount = 5;
	if (itemsCount > stepCount)
	{
		itemsIndex = XwaUIScrollBar(&scrollBarRect, itemsIndex, itemsCount, 0, 0x05, *(unsigned int*)0x0B07C6C, stepCount);
	}
	int currentCount = std::min(itemsCount - itemsIndex, stepCount);
	int x = 320;
	int y = 370 - currentCount * 16;

	for (int ebp = 0; ebp < currentCount; ebp++)
	{
		int index = itemsIndex + ebp;
		std::string text = questions[index].question;
		int result = L00556100(x, y, text.c_str(), 12, *(unsigned int*)0x0ABD228, 20, 0, "settingsound");

		if (result != 0)
		{
			g_RenderFormattedText_ItemsIndex = 0;
			g_custom_briefing_answer = questions[index].answer;

			std::string officer = questions[index].officer;
			officerImageX = questions[index].officerPositionX;
			officerImageY = questions[index].officerPositionY;
			if (officer.empty())
			{
				officer = g_missionConfig.GetBriefingOfficerImage();
				officerImageX = g_missionConfig.GetBriefingOfficerImagePositionX();
				officerImageY = g_missionConfig.GetBriefingOfficerImagePositionY();
			}
			XwaFrontResFreeItem("officer");
			XwaFrontResLoad(officer.c_str(), "officer");
			XwaFrontResSetCurrentImage("officer", 0);

			XwaPlayWaveSound(questions[index].answerWave.c_str(), 0, 0);
		}

		y += 16;
	}

	{
		std::string text = g_missionConfig.GetBriefingBackQuestion();
		int result = L00556100(x, 385, text.c_str(), 12, *(unsigned int*)0x0ABD228, 20, 0, "settingsound");

		if (result != 0)
		{
			gotoState = 1;
		}
	}

	{
		std::string text = g_missionConfig.GetBriefingDoneQuestion();
		int result = L00556100(x, 370, text.c_str(), 12, *(unsigned int*)0x0ABD228, 20, 0, "settingsound");

		if (result != 0)
		{
			gotoState = 2;
		}
	}

	if (g_config.ShowDebugMessages)
	{
		std::string xwaMissionFileName = GetMissionFileName();

		RECT rc;
		XwaRectSet(&rc, 100, 400, 610, 480);
		L00557AB0(12, xwaMissionFileName.c_str(), &rc, 0xffffffff, 0x03, 0);
	}

	gameUpdateFunction XwaBriefingGameStateUpdate = (gameUpdateFunction)0x00564E90;
	gameUpdateFunction XwaBriefingGameStateExit = (gameUpdateFunction)0x00564DC0;
	gameUpdateFunction XwaTrainingRoomGameStateUpdate = (gameUpdateFunction)0x005316B0;

	switch (gotoState)
	{
	case 1:
		XwaStopWaveSound();
		XwaSwitchGameState(XwaBriefingGameStateUpdate, XwaBriefingGameStateExit);
		break;

	case 2:
		XwaStopWaveSound();
		((void(*)())0x0057E8D0);
		XwaSwitchGameState(XwaTrainingRoomGameStateUpdate, 0);
		break;
	}

	// config menu
	return ((int(*)(int))0x00529330)(0) == 1 ? 1 : 0;
}

std::array<unsigned char, 192 * 8> g_fgGoals{};

bool CheckGoals(const std::string& questionGoalsStr, GoalMethodEnum method)
{
	if (questionGoalsStr.empty())
	{
		return true;
	}

	std::string goalsText;
	for (int fg = 0; fg < 192; fg++)
	{
		for (int g = 0; g < 8; g++)
		{
			unsigned char goal = g_fgGoals[fg * 8 + g];

			//if (goal != 0 && goal != 0x04)
			if (goal != 0)
			{
				goalsText += " " + std::to_string(fg) + "_" + std::to_string(g) + "_" + std::to_string(goal);
			}
		}
	}

	if (goalsText.empty())
	{
		return false;
	}

	std::vector<std::string> questionGoals = Tokennize(questionGoalsStr);

	if (method == GoalMethod_All)
	{
		bool check = true;

		for (const std::string& questionGoal : questionGoals)
		{
			if (goalsText.find(questionGoal) == std::string::npos)
			{
				check = false;
				break;
			}
		}

		return check;
	}

	if (method == GoalMethod_AtLeastOne)
	{
		bool check = false;

		for (const std::string& questionGoal : questionGoals)
		{
			if (goalsText.find(questionGoal) != std::string::npos)
			{
				check = true;
				break;
			}
		}

		return check;
	}

	return false;
}

int CustomBriefingHook(int* params)
{
	//int state = *(int*)0x0783BE4;

	//if (state == 2 && g_missionConfig.HasBriefingQuestions())
	//{
	//	RECT rc;
	//	XwaRectSet(&rc, 0, 0, 0x280, 0x1E0);
	//	XwaRunGameState(BriefingGameStateUpdate, &rc);
	//}

	memset(g_fgGoals.data(), 0, 192 * 8);

	((void(*)())0x00566530)();
	return 0;
}

int CustomToHangarTextHook(int* params)
{
	if (g_missionConfig.HasBriefingQuestions())
	{
		return (int)"To Briefing Officer";
	}

	int index = params[0];
	const char* text = ((const char* (*)(int))0x0055CB50)(index);
	return (int)text;
}

int SwitchToCustomBriefingHook(int* params)
{
	if (g_missionConfig.HasBriefingQuestions())
	{
		XwaStopWaveSound();
		XwaSwitchGameState(BriefingGameStateUpdate, BriefingGameStateExit);
	}
	else
	{
		XwaStopWaveSound();
		gameUpdateFunction XwaTrainingRoomGameStateUpdate = (gameUpdateFunction)0x005316B0;
		XwaSwitchGameState(XwaTrainingRoomGameStateUpdate, 0);
	}

	return 0;
}

int GetNetworkPlayerFlightGroupId()
{
	int index = 0;

	if (*(int*)0x0ABD7B4 != 0x02)
	{
		for (; index < 8; index++)
		{
			// s_XwaPilot.NetworkPlayers[index].DirectPlayId
			int DirectPlayId = *(int*)(0x00AE2A60 + 0x10F42 + index * 0x0064 + 0x0024);

			if (DirectPlayId == ((int(*)())0x0052EFF0)())
			{
				break;
			}
		}
	}

	if (index == 8)
	{
		return 0;
	}

	return *(int*)(0x00AE2A60 + 0x10F42 + index * 0x0064 + 0x001C);
}

int GetPlayerTeam()
{
	int fg = GetNetworkPlayerFlightGroupId();
	return *(unsigned char*)(*(int*)0x009EB8E0 + 0x0000 + fg * 0xE3E + 0x0071);
}

std::array<unsigned char, 192 * 8> GetDebriefingFGGoals()
{
	//int team = GetPlayerTeam();

	//std::array<unsigned char, 192 * 8> state{};

	//if (team < 0 || team >= 10)
	//{
	//	return state;
	//}

	//for (int fg = 0; fg < 192; fg++)
	//{
	//	for (int goal = 0; goal < 8; goal++)
	//	{
	//		unsigned char* v = (unsigned char*)0x007B7020 + fg * 0x172 + 0x10D + team * 8 + goal;
	//		state[fg * 8 + goal] = *v;
	//	}
	//}

	//return state;

	return g_fgGoals;
}

int DebriefingGameStateExit(int A4)
{
	g_custom_briefing_answer.clear();
	g_stringWidth.clear();

	// Free mission descriptions
	if (*(int*)0x009F4B98)
	{
		((void(*)(int))0x0055D480)(*(int*)0x009F4B98);
		*(int*)0x009F4B98 = 0;
	}

	XwaFrontResFreeItem("background");
	XwaFrontResFreeItem("officer");

	return 0;
}

int DebriefingGameStateUpdate(int A4)
{
	int gotoState = 0;

	static int itemsIndex;
	static int s_missionState;
	static int officerImageX;
	static int officerImageY;

	if (A4 == 0)
	{
		itemsIndex = 0;
		g_RenderFormattedText_ItemsIndex = 0;

		// Set mission states
		((int(*)())0x00582ED0)();
		// if 1 then victory else defeat
		s_missionState = *(int*)0x09EAA04;
		// Load mission descriptions
		((void(*)(int))0x00547800)(*(int*)0x00AE2A8A);

		XwaFrontResLoad("frontres\\combat\\multiplayer.bmp", "background");
		if (s_missionState == 1)
		{
			XwaFrontResLoad(g_missionConfig.GetDebriefingVictoryOfficerImage().c_str(), "officer");
			officerImageX = g_missionConfig.GetDebriefingVictoryOfficerImagePositionX();
			officerImageY = g_missionConfig.GetDebriefingVictoryOfficerImagePositionY();
		}
		else
		{
			XwaFrontResLoad(g_missionConfig.GetDebriefingDefeatOfficerImage().c_str(), "officer");
			officerImageX = g_missionConfig.GetDebriefingDefeatOfficerImagePositionX();
			officerImageY = g_missionConfig.GetDebriefingDefeatOfficerImagePositionY();
		}
		XwaFrontResSetCurrentImage("officer", 0);

		XwaFrontResDrawCurrentImage("background", 0, 0);
		XwaOffScreenSurfaceLock();
		XwaFrontResDrawCurrentImage("background", 0, 0);
		XwaOffScreenSurfaceUnlock(0x01);

		g_custom_briefing_answer.clear();
	}

	XwaFrontResDrawCurrentImage("background", 0, 0);

	RECT officerRect;
	XwaFrontResGetArea("officer", &officerRect);
	XwaFrontResDraw("officer", officerImageX, officerImageY);
	XwaFrontResMoveToNextImage("officer", 1);

	if (g_custom_briefing_answer.size())
	{
		RECT rc;
		XwaRectSet(&rc, 320, 60, 560, 260);

		RenderFormattedText(&rc, g_custom_briefing_answer);
	}

	std::array<unsigned char, 192 * 8> fgGoals = GetDebriefingFGGoals();

	auto& questions = s_missionState == 1 ? g_missionConfig.GetDebriefingVictoryQuestions() : g_missionConfig.GetDebriefingDefeatQuestions();
	int count = questions.size() + 2;

	RECT scrollBarRect;
	XwaRectSet(&scrollBarRect, 561, 290, 580, 390);
	int itemsCount = count - 2;
	int stepCount = 5;
	if (itemsCount > stepCount)
	{
		itemsIndex = XwaUIScrollBar(&scrollBarRect, itemsIndex, itemsCount, 0, 0x05, *(unsigned int*)0x0B07C6C, stepCount);
	}
	int currentCount = std::min(itemsCount - itemsIndex, stepCount);
	int x = 320;
	int y = 370 - currentCount * 16;

	for (int ebp = 0; ebp < currentCount; ebp++)
	{
		int index = itemsIndex + ebp;
		std::string text = questions[index].question;
		int result = L00556100(x, y, text.c_str(), 12, *(unsigned int*)0x0ABD228, 20, 0, "settingsound");

		if (result != 0)
		{
			g_RenderFormattedText_ItemsIndex = 0;
			g_custom_briefing_answer = questions[index].answer;

			std::string officer = questions[index].officer;
			officerImageX = questions[index].officerPositionX;
			officerImageY = questions[index].officerPositionY;
			if (officer.empty())
			{
				officer = s_missionState == 1 ? g_missionConfig.GetDebriefingVictoryOfficerImage() : g_missionConfig.GetDebriefingDefeatOfficerImage();
				officerImageX = s_missionState == 1 ? g_missionConfig.GetDebriefingVictoryOfficerImagePositionX() : g_missionConfig.GetDebriefingDefeatOfficerImagePositionX();
				officerImageY = s_missionState == 1 ? g_missionConfig.GetDebriefingVictoryOfficerImagePositionY() : g_missionConfig.GetDebriefingDefeatOfficerImagePositionY();
			}
			XwaFrontResFreeItem("officer");
			XwaFrontResLoad(officer.c_str(), "officer");
			XwaFrontResSetCurrentImage("officer", 0);

			XwaPlayWaveSound(questions[index].answerWave.c_str(), 0, 0);
		}

		y += 16;
	}

	//{
	//	std::string text = s_missionState == 1 ? g_missionConfig.GetDebriefingVictoryBackQuestion() : g_missionConfig.GetDebriefingDefeatBackQuestion();
	//	int result = L00556100(x, 385, text.c_str(), 12, *(unsigned int*)0x0ABD228, 20, 0, "settingsound");

	//	if (result != 0)
	//	{
	//		gotoState = 1;
	//	}
	//}

	{
		std::string text = s_missionState == 1 ? g_missionConfig.GetDebriefingVictoryDoneQuestion() : g_missionConfig.GetDebriefingDefeatDoneQuestion();
		int result = L00556100(x, 370, text.c_str(), 12, *(unsigned int*)0x0ABD228, 20, 0, "settingsound");

		if (result != 0)
		{
			gotoState = 2;
		}
	}

	if (g_config.ShowDebugMessages)
	{
		std::string xwaMissionFileName = GetMissionFileName();

		// if 1 then victory else defeat
		xwaMissionFileName += " state=" + std::to_string(s_missionState);

		std::string goalsText;
		for (int fg = 0; fg < 192; fg++)
		{
			for (int g = 0; g < 8; g++)
			{
				unsigned char goal = fgGoals[fg * 8 + g];

				//if (goal != 0 && goal != 0x04)
				if (goal != 0)
				{
					goalsText += " " + std::to_string(fg) + "_" + std::to_string(g) + "_" + std::to_string(goal);
				}
			}
		}

		RECT rc;
		XwaRectSet(&rc, 100, 400, 610, 480);
		L00557AB0(12, xwaMissionFileName.c_str(), &rc, 0xffffffff, 0x03, 0);

		XwaRectSet(&rc, 100, 100, 610, 480);
		L00557AB0(12, goalsText.c_str(), &rc, 0xffffffff, 0x03, 0);
	}

	gameUpdateFunction XwaDebriefingGameStateUpdate = (gameUpdateFunction)0x0057ECE0;
	gameUpdateFunction XwaDebriefingGameStateExit = (gameUpdateFunction)0x0057EC50;

	switch (gotoState)
	{
	case 2:
		XwaStopWaveSound();
		((void(*)())0x0057E8D0);
		XwaSwitchGameState(XwaDebriefingGameStateUpdate, XwaDebriefingGameStateExit);
		break;
	}

	// config menu
	return ((int(*)(int))0x00529330)(0) == 1 ? 1 : 0;
}

int SwitchToCustomDebriefingHook(int* params)
{
	params[Params_ReturnAddress] = 0x005718DF;

	if (g_missionConfig.HasDebriefingQuestions())
	{
		XwaStopWaveSound();
		XwaSwitchGameState(DebriefingGameStateUpdate, DebriefingGameStateExit);
	}
	else
	{
		XwaStopWaveSound();
		gameUpdateFunction XwaDebriefingGameStateUpdate = (gameUpdateFunction)0x0057ECE0;
		gameUpdateFunction XwaDebriefingGameStateExit = (gameUpdateFunction)0x0057EC50;
		XwaSwitchGameState(XwaDebriefingGameStateUpdate, XwaDebriefingGameStateExit);
	}

	return 0;
}

int CheckFGGoalStateHook(int* params)
{
	int A4 = params[0];
	int A8 = params[1];
	int AC = params[2];

	const auto L004D5AE0 = (int(*)(int, int, int))0x004D5AE0;
	unsigned char goalType = *(unsigned char*)(0x0080E78A + params[Params_ESI]);
	unsigned char condition = *(unsigned char*)A4;

	int state0 = L004D5AE0(A4, A8, AC);
	int state = state0;

	if (goalType == 1)
	{
		if (state == 1)
		{
			state = 2;
		}
		else if (state == 2)
		{
			state = 1;
		}
	}

	int team = GetPlayerTeam();
	int offset = params[Params_ESI];
	int fg = offset / 0xE42;
	int goal = (offset % 0xE42) / 0x50;

	if (AC == team)
	{
		g_fgGoals[fg * 8 + goal] = (unsigned char)state;
	}

	if (g_config.ShowDebugMessages)
	{
		short s_V0x074D5C8 = *(short*)0x074D5C8;
		short s_V0x074D5C4 = *(short*)0x074D5C4;
		OutputDebugString((__FUNCTION__ " " + std::to_string(fg) + "-" + std::to_string(goal) + " team=" + std::to_string(AC) + " state=" + std::to_string(state) + " " + std::to_string(s_V0x074D5C8) + "/" + std::to_string(s_V0x074D5C4) + " type=" + std::to_string(goalType) + " cond=" + std::to_string(condition)).c_str());
	}

	return state0;
}

int MissionSetupLoadMissionDescTextHook(int* params)
{
	((void(*)(int))0x0052ADD0)(params[Params_EBX]);

	char* A4 = (char*)params[Params_EBP];
	const char* missionFilename = (const char*)0x00ABD680;

	static std::string _filename;
	static std::string _text;

	if (_filename != missionFilename)
	{
		_filename = missionFilename;

		std::string path = GetStringWithoutExtension(missionFilename);

		auto lines = GetFileLines(path + "BriefingText.txt");

		if (!lines.size())
		{
			lines = GetFileLines(path + ".ini", "BriefingText");
		}

		std::string text = GetFileKeyValue(lines, "Description");
		if (text.size() > 0xffff)
		{
			text = text.substr(0, 0xffff);
		}

		if (!text.empty())
		{
			int length = text.size();
			char* txt = text.data();

			for (int ecx = 0; ecx < length; ecx++)
			{
				if (txt[ecx] == '#')
				{
					txt[ecx] = 0;
					break;
				}

				if (txt[ecx] == '[')
				{
					txt[ecx] = 0x06;

					for (int eax = ecx + 1; eax < length; eax++)
					{
						if (txt[eax] == ']')
						{
							txt[eax] = txt[eax + 1];
							txt[eax + 1] = 0x01;
							break;
						}
					}
				}
			}
		}

		_text = text;
	}

	if (!_text.empty())
	{
		strcpy_s(A4, 0x10000, _text.c_str());
	}

	return 0;
}

int MissionSetupBuildBriefingTextHook(int* params)
{
	((void(*)(int))0x0052ADD0)(params[Params_EBP]);

	char* A4 = (char*)params[Params_EBX];
	const char* missionFilename = (const char*)0x00ABD680;

	static std::string _filename;
	static std::string _text;

	if (_filename != missionFilename)
	{
		_filename = missionFilename;

		std::string path = GetStringWithoutExtension(missionFilename);

		auto lines = GetFileLines(path + "BriefingText.txt");

		if (!lines.size())
		{
			lines = GetFileLines(path + ".ini", "BriefingText");
		}

		std::string text = GetFileKeyValue(lines, "Description");
		if (text.size() > 0xffff)
		{
			text = text.substr(0, 0xffff);
		}

		if (text.find('#') != std::string::npos)
		{
			size_t i = text.find('#');
			text = text.substr(i + 1);
		}

		if (!text.empty())
		{
			int length = text.size();
			char* txt = text.data();

			for (int ecx = 0; ecx < length; ecx++)
			{
				if (txt[ecx] == '[')
				{
					txt[ecx] = 0x06;

					for (int eax = ecx + 1; eax < length; eax++)
					{
						if (txt[eax] == ']')
						{
							txt[eax] = txt[eax + 1];
							txt[eax + 1] = 0x01;
							break;
						}
					}
				}
			}
		}

		_text = text;
	}

	if (!_text.empty())
	{
		strcpy_s(A4, 0x10000, _text.c_str());
	}

	return 0;
}

int MissionDebriefBuildTextHook(int* params)
{
	((void(*)(int))0x0052ADD0)(params[0]);

	int paramsIndex = 0xC2A;
	char* A4 = (char*)params[paramsIndex + 1];
	char* A8 = (char*)params[paramsIndex + 2];
	bool AC = params[paramsIndex + 3] != 0;
	const char* missionFilename = (const char*)0x00ABD680;

	static std::string _filename;
	static bool _AC = false;
	static std::string _textA4;
	static std::string _textA8;

	if (_filename != missionFilename || _AC != AC)
	{
		_filename = missionFilename;
		_AC = AC;
		_textA4.clear();
		_textA8.clear();

		std::string path = GetStringWithoutExtension(missionFilename);

		auto lines = GetFileLines(path + "BriefingText.txt");

		if (!lines.size())
		{
			lines = GetFileLines(path + ".ini", "BriefingText");
		}

		if (AC)
		{
			std::string text = GetFileKeyValue(lines, "WinDebriefing");
			if (text.size() > 0xffff)
			{
				text = text.substr(0, 0xffff);
			}

			if (!text.empty())
			{
				int length = text.size();
				char* txt = text.data();

				for (int ecx = 0; ecx < length; ecx++)
				{
					if (txt[ecx] == '[')
					{
						txt[ecx] = 0x06;

						for (int eax = ecx + 1; eax < length; eax++)
						{
							if (txt[eax] == ']')
							{
								txt[eax] = txt[eax + 1];
								txt[eax + 1] = 0x01;
								break;
							}
						}
					}
				}
			}

			_textA4 = text;
			_textA8.clear();
		}
		else
		{
			std::string text = GetFileKeyValue(lines, "LostDebriefing");
			if (text.size() > 0xffff)
			{
				text = text.substr(0, 0xffff);
			}

			if (!text.empty())
			{
				{
					std::string temp = text;
					int length = temp.size();
					char* txt = temp.data();

					for (int ecx = 0; ecx < length; ecx++)
					{
						if (txt[ecx] == '#')
						{
							txt[ecx] = 0;
							break;
						}

						if (txt[ecx] == '[')
						{
							txt[ecx] = 0x06;

							for (int eax = ecx + 1; eax < length; eax++)
							{
								if (txt[eax] == ']')
								{
									txt[eax] = txt[eax + 1];
									txt[eax + 1] = 0x01;
									break;
								}
							}
						}
					}

					_textA4 = temp;
				}

				{
					std::string temp = text;

					if (temp.find('#') != std::string::npos)
					{
						size_t i = temp.find('#');
						temp = temp.substr(i + 1);
					}

					int length = temp.size();
					char* txt = temp.data();

					for (int ecx = 0; ecx < length; ecx++)
					{
						if (txt[ecx] == '[')
						{
							txt[ecx] = 0x06;

							for (int eax = ecx + 1; eax < length; eax++)
							{
								if (txt[eax] == ']')
								{
									txt[eax] = txt[eax + 1];
									txt[eax + 1] = 0x01;
									break;
								}
							}
						}
					}

					_textA8 = temp;
				}
			}
		}
	}

	if (AC)
	{
		if (!_textA4.empty())
		{
			strcpy_s(A4, 0x10000, _textA4.c_str());
			A8[0] = 0;
		}
	}
	else
	{
		if (!_textA4.empty() || !_textA8.empty())
		{
			strcpy_s(A4, 0x10000, _textA4.c_str());
			strcpy_s(A8, 0x10000, _textA8.c_str());
		}
	}

	return 0;
}
