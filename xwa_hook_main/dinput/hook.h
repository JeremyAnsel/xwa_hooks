#pragma once
#include "hook_function.h"

bool PatchMemory(const HookPatch& patch);
bool LoadAndPatchHooks();

int Hook(int call, int* params);
