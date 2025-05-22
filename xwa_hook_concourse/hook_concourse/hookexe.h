#pragma once
#include <string>

extern HWND g_hookexe;

void ExeShowMessage(const std::string& message);

void ExeWebmLoadVideo(const std::string& name);
void ExeWebmFreeVideo(const std::string& name);
void ExeWebmFreeAllVideos();
void ExeWebmResetAllTimecode();
int64_t ExeWebmGetTimecode(const std::string& name);
uint64_t ExeWebmGetDuration(const std::string& name);
int ExeWebmReadVideoFrame(const std::string& name, unsigned int* image, unsigned int* width, unsigned int* height);
