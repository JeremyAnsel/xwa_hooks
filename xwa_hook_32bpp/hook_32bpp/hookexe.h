#pragma once
#include <string>

extern HWND g_hookexe;

void ExeShowMessage(const std::string& message);

void ExeSetSettings();
unsigned int ExeReadOpt(const std::string& filename);
unsigned int ExeGetOptVersion();
void* ExeWriteOpt();
void ExeFreeOptMemory();
