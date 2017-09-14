#pragma once

struct HookFunction
{
	int from;
	int(*function)(int* params);
};

struct HookPatchItem
{
	int Offset;
	const char* From;
	const char* To;
};

struct HookPatch
{
	const char* Name;
	int Count;
	const HookPatchItem* Items;
};

#define MAKE_HOOK_PATCH(name, patch) { name, sizeof(patch) / sizeof(HookPatchItem), patch }
