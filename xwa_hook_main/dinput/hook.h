#pragma once
#include "hook_function.h"

void VirtualProtectMemoryReadWrite();
void VirtualProtectMemoryExecuteReadWrite();

bool PatchMemory(const HookPatch& patch);
bool LoadAndPatchHooks();

int Hook(int call, int* params);
