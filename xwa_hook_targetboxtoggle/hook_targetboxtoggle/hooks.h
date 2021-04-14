#pragma once
#include "hook_function.h"

#include "targetboxtoggle.h"

#include "config.h"

class HooksConfig
{
public:
	HooksConfig()
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

static HooksConfig g_hooksConfig;

static const HookFunction g_hookFunctions[] =
{
	{ 0x4FBAA1, TargetboxToggleHook },
};

static const HookPatchItem g_targetboxTogglePatch[] =
{
	{ 0x0FAE9C, "8B86E0948B00", "E87FD00A0090" },
	{ 0x102E30, "83EC08", g_hooksConfig.TargetBox ? "83EC08" : "C39090" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that toggles targetbox", g_targetboxTogglePatch),
};
