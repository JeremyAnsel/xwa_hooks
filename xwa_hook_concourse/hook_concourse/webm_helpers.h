#pragma once
#include <string>

void WebmLoadVideo(const std::string& name);
void WebmFreeVideo(const std::string& name);
void WebmFreeAllVideos();
void WebmResetAllTimecode();
int64_t WebmGetTimecode(const std::string& name);
int WebmReadVideoFrame(const std::string& name, uint8_t** image, unsigned int* width, unsigned int* height);
