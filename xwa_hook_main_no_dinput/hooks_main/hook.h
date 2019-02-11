#pragma once
#include "hook_function.h"

void VirtualProtectHookMemoryWrite();
void VirtualProtectHookMemory();

bool PatchMemory(const HookPatch& patch);
bool LoadAndPatchHooks();

int Hook(int call, int* params);
