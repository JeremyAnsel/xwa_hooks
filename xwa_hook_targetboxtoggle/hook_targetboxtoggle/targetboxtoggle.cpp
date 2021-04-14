#include "targetver.h"
#include "targetboxtoggle.h"
#include "config.h"

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
		auto lines = GetFileLines("hook_targetboxtoggle.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_targetboxtoggle");
		}

		this->TargetBox = GetFileKeyValueInt(lines, "TargetBox", 1) != 0;
	}

	bool TargetBox;
};

Config g_config;

#pragma pack(push, 1)

struct XwaPlayer
{
	int ObjectIndex;
	char Unk0004[6];
	short Iff;
	char Unk000C[23];
	char ShowTargetBox;
	char Unk0024[2987];
};

static_assert(sizeof(XwaPlayer) == 3023, "size of XwaPlayer must be 3023");

#pragma pack(pop)

int TargetboxToggleHook(int* params)
{
	if (!g_config.TargetBox)
	{
		return 0;
	}

	int playerIndex = params[Params_EDI];

	const auto PlaySound = (int(*)(int, int, int))0x0043BF90;

	XwaPlayer* XwaPlayers = (XwaPlayer*)0x008B94E0;
	short keyPressed = *(short*)0x8053C0;
	const short Key_CONTROL_T = 0x122;
	const int Sound_Click1 = 0x44;

	params[Params_EAX] = XwaPlayers[playerIndex].ObjectIndex;

	if (keyPressed == Key_CONTROL_T)
	{
		XwaPlayers[playerIndex].ShowTargetBox = XwaPlayers[playerIndex].ShowTargetBox == 0 ? 1 : 0;
		PlaySound(Sound_Click1, 0xFFFF, playerIndex);
	}

	return 0;
}
