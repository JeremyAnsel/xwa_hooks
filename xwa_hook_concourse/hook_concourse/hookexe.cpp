#include "targetver.h"
#include <Windows.h>
#include "hookexe.h"

HWND g_hookexe = NULL;

enum ExeDataType
{
	ExeDataType_None,
	ExeDataType_ShowMessage,
	ExeDataType_WebmLoadVideo,
	ExeDataType_WebmFreeVideo,
	ExeDataType_WebmFreeAllVideos,
	ExeDataType_WebmResetAllTimecode,
	ExeDataType_WebmGetTimecode_Low,
	ExeDataType_WebmGetTimecode_High,
	ExeDataType_WebmGetDuration_Low,
	ExeDataType_WebmGetDuration_High,
	ExeDataType_WebmReadVideoFrame_Result,
	ExeDataType_WebmReadVideoFrame_Image,
	ExeDataType_WebmReadVideoFrame_Width,
	ExeDataType_WebmReadVideoFrame_Height,
};

void ExeShowMessage(const std::string& message)
{
	if (!g_hookexe)
	{
		return;
	}

	COPYDATASTRUCT cds{};
	cds.dwData = ExeDataType_ShowMessage;
	cds.cbData = message.length() + 1;
	cds.lpData = (void*)message.data();

	SendMessage(g_hookexe, WM_COPYDATA, NULL, (LPARAM)&cds);
}

void ExeWebmLoadVideo(const std::string& name)
{
	if (!g_hookexe)
	{
		return;
	}

	COPYDATASTRUCT cds{};
	cds.dwData = ExeDataType_WebmLoadVideo;
	cds.cbData = name.length() + 1;
	cds.lpData = (void*)name.data();

	SendMessage(g_hookexe, WM_COPYDATA, NULL, (LPARAM)&cds);
}

void ExeWebmFreeVideo(const std::string& name)
{
	if (!g_hookexe)
	{
		return;
	}

	COPYDATASTRUCT cds{};
	cds.dwData = ExeDataType_WebmFreeVideo;
	cds.cbData = name.length() + 1;
	cds.lpData = (void*)name.data();

	SendMessage(g_hookexe, WM_COPYDATA, NULL, (LPARAM)&cds);
}

void ExeWebmFreeAllVideos()
{
	if (!g_hookexe)
	{
		return;
	}

	COPYDATASTRUCT cds{};
	cds.dwData = ExeDataType_WebmFreeAllVideos;
	cds.cbData = 0;
	cds.lpData = nullptr;

	SendMessage(g_hookexe, WM_COPYDATA, NULL, (LPARAM)&cds);
}

void ExeWebmResetAllTimecode()
{
	if (!g_hookexe)
	{
		return;
	}

	COPYDATASTRUCT cds{};
	cds.dwData = ExeDataType_WebmResetAllTimecode;
	cds.cbData = 0;
	cds.lpData = nullptr;

	SendMessage(g_hookexe, WM_COPYDATA, NULL, (LPARAM)&cds);
}

int64_t ExeWebmGetTimecode(const std::string& name)
{
	if (!g_hookexe)
	{
		return 0;
	}

	COPYDATASTRUCT cds{};
	cds.dwData = ExeDataType_WebmGetTimecode_Low;
	cds.cbData = name.length() + 1;
	cds.lpData = (void*)name.data();
	uint64_t low = (unsigned int)SendMessage(g_hookexe, WM_COPYDATA, NULL, (LPARAM)&cds);

	cds.dwData = ExeDataType_WebmGetTimecode_High;
	cds.cbData = 0;
	cds.lpData = nullptr;
	uint64_t high = (unsigned int)SendMessage(g_hookexe, WM_COPYDATA, NULL, (LPARAM)&cds);

	int64_t result = (high << 32) | low;
	return result;
}

uint64_t ExeWebmGetDuration(const std::string& name)
{
	if (!g_hookexe)
	{
		return 0;
	}

	COPYDATASTRUCT cds{};
	cds.dwData = ExeDataType_WebmGetDuration_Low;
	cds.cbData = name.length() + 1;
	cds.lpData = (void*)name.data();
	uint64_t low = (unsigned int)SendMessage(g_hookexe, WM_COPYDATA, NULL, (LPARAM)&cds);

	cds.dwData = ExeDataType_WebmGetDuration_High;
	cds.cbData = 0;
	cds.lpData = nullptr;
	uint64_t high = (unsigned int)SendMessage(g_hookexe, WM_COPYDATA, NULL, (LPARAM)&cds);

	int64_t result = (high << 32) | low;
	return result;
}

std::string address_to_hex(int i);

int ExeWebmReadVideoFrame(const std::string& name, unsigned int* image, unsigned int* width, unsigned int* height)
{
	//OutputDebugString(__FUNCTION__);

	if (!g_hookexe)
	{
		*image = 0;
		*width = 0;
		*height = 0;
		return 0;
	}

	COPYDATASTRUCT cds{};
	cds.dwData = ExeDataType_WebmReadVideoFrame_Result;
	cds.cbData = name.length() + 1;
	cds.lpData = (void*)name.data();
	int r = (int)SendMessage(g_hookexe, WM_COPYDATA, NULL, (LPARAM)&cds);

	cds.dwData = ExeDataType_WebmReadVideoFrame_Image;
	cds.cbData = 0;
	cds.lpData = nullptr;
	*image = (unsigned int)SendMessage(g_hookexe, WM_COPYDATA, NULL, (LPARAM)&cds);

	cds.dwData = ExeDataType_WebmReadVideoFrame_Width;
	cds.cbData = 0;
	cds.lpData = nullptr;
	*width = (unsigned int)SendMessage(g_hookexe, WM_COPYDATA, NULL, (LPARAM)&cds);

	cds.dwData = ExeDataType_WebmReadVideoFrame_Height;
	cds.cbData = 0;
	cds.lpData = nullptr;
	*height = (unsigned int)SendMessage(g_hookexe, WM_COPYDATA, NULL, (LPARAM)&cds);

	//OutputDebugString((__FUNCTION__ " " + name + " r=" + std::to_string(r) + " image=" + address_to_hex(*image) + " " + std::to_string(*width) + "x" + std::to_string(*height)).c_str());

	return r;
}
