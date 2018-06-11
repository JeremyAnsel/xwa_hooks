#pragma once
#include "hook_function.h"

void VirtualProtectHookMemory();

bool PatchMemory(const HookPatch& patch);
bool LoadAndPatchHooks();

int Hook(int call, int* params);
