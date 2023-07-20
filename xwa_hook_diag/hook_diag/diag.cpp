#include "targetver.h"
#include "diag.h"
#include <Windows.h>
#include <Shlwapi.h>
#include <Psapi.h>
#include <chrono>
#include <vector>
#include <array>
#include <numeric>
#include <string>

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Shlwapi.lib")

enum ParamsEnum
{
	Params_ReturnAddress = -1,
	Params_EAX = -3,
	Params_ECX = -4,
	Params_EDX = -5,
	Params_EBX = -6,
	Params_EBP = -8,
	Params_ESI = -9,
	Params_EDI = -10,
};

class StepValueDouble
{
public:
	StepValueDouble()
		:_index(0)
	{
		_values.fill(0.0);
	}

	double GetValue(double step)
	{
		_values[_index] = step;
		_index++;

		if (_index >= (int)_values.size())
		{
			_index = 0;
		}

		double mean = (int)(std::accumulate(_values.begin(), _values.end(), 0.0) / _values.size());
		return mean;
	}

private:
	std::array<double, 60> _values;
	int _index;
};

std::chrono::steady_clock::time_point g_chrono[9]{};

double GetProcessCpuUsage()
{
	static bool initialized = false;
	static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
	static int numProcessors;
	static HANDLE self;

	FILETIME ftime, fsys, fuser;

	if (!initialized)
	{
		initialized = true;

		SYSTEM_INFO sysInfo;
		GetSystemInfo(&sysInfo);
		numProcessors = sysInfo.dwNumberOfProcessors;

		GetSystemTimeAsFileTime(&ftime);
		memcpy(&lastCPU, &ftime, sizeof(FILETIME));

		self = GetCurrentProcess();
		GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
		memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
		memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
	}

	ULARGE_INTEGER now, sys, user;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&now, &ftime, sizeof(FILETIME));

	GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	memcpy(&sys, &fsys, sizeof(FILETIME));
	memcpy(&user, &fuser, sizeof(FILETIME));

	double percent = ((sys.QuadPart - lastSysCPU.QuadPart) + (user.QuadPart - lastUserCPU.QuadPart)) * 100.0 / (now.QuadPart - lastCPU.QuadPart);
	//double percent = (user.QuadPart - lastUserCPU.QuadPart) * 100.0 / (now.QuadPart - lastCPU.QuadPart);
	percent /= numProcessors;
	lastCPU = now;
	lastUserCPU = user;
	lastSysCPU = sys;

	return max(percent, 0.0);
}

double GetThreadCpuUsage()
{
	static bool initialized = false;
	static ULARGE_INTEGER lastCPU, lastUserCPU;
	static HANDLE self;

	FILETIME ftime, fuser;

	if (!initialized)
	{
		initialized = true;

		GetSystemTimeAsFileTime(&ftime);
		memcpy(&lastCPU, &ftime, sizeof(FILETIME));

		self = GetCurrentThread();
		GetThreadTimes(self, &ftime, &ftime, &ftime, &fuser);
		memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
	}

	ULARGE_INTEGER now, user;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&now, &ftime, sizeof(FILETIME));

	GetThreadTimes(self, &ftime, &ftime, &ftime, &fuser);
	memcpy(&user, &fuser, sizeof(FILETIME));

	double percent = (user.QuadPart - lastUserCPU.QuadPart) * 100.0 / (now.QuadPart - lastCPU.QuadPart);
	lastCPU = now;
	lastUserCPU = user;

	return max(percent, 0.0);
}

unsigned int GetResDataItemsMemSize()
{
	unsigned int ResDataItemsMemSize = 0;

	for (int i = 0; i < 200; i++)
	{
		int ptr = 0x00968CE0 + i * 0x1A;

		if (*(int*)(ptr + 0x00) == -1)
		{
			continue;
		}

		ResDataItemsMemSize += *(unsigned int*)(ptr + 0x08);
	}

	return ResDataItemsMemSize;
}

int LoadingScreenStepHook(int* params)
{
	const int step = params[0];

	const auto L00531840 = (int(*)(int))0x00531840;

	g_chrono[step] = std::chrono::high_resolution_clock::now();

	if (step > 0)
	{
		std::chrono::duration<double> elapsed = g_chrono[step] - g_chrono[step - 1];
		std::chrono::duration<double> duration = g_chrono[step] - g_chrono[0];

		OutputDebugString((
			__FUNCTION__ " step=" + std::to_string(step)
			+ " elapsed=" + std::to_string(elapsed.count()) + "s"
			+ " total=" + std::to_string(duration.count()) + "s"
			).c_str());
	}

	return L00531840(step);
}

void ConcourseDrawBegin()
{
	const auto XwaFrontSurfaceLock = (int(*)())0x0053EF80;

	*(int*)0x009F60D4 = XwaFrontSurfaceLock();
}

void ConcourseDrawEnd()
{
	const auto XwaFrontSurfaceUnlock = (void(*)())0x0053F010;

	XwaFrontSurfaceUnlock();
}

void ConcourseDrawText(short x, short y, const char* text)
{
	const auto XwaDrawText = (void(*)(int, const char*, int, int, unsigned int))0x00557310;

	XwaDrawText(10, text, x, y, 0xffff);
}

void InFlightDrawBegin()
{
}

void InFlightDrawEnd()
{
}

void InFlightDrawText(short x, short y, const char* text)
{
	const auto XwaDrawText = (void(*)(const char*))0x00435130;
	const auto XwaSetTextOffset = (void(*)(short, short))0x004349F0;
	const auto XwaGetTextWidth = (short(*)(const char*))0x004E6F20;
	const auto L00434A60 = (void(*)(short, short, short, short))0x00434A60;
	const auto L004349D0 = (void(*)(short, short))0x004349D0;
	const auto L00434C70 = (void(*)(unsigned char))0x00434C70;

	*(unsigned int*)0x0091AB68 = 0xffffffff;

	L00434C70(0);

	L00434A60(0, 0, XwaGetTextWidth(text), 15);
	L004349D0(0, 0);

	XwaSetTextOffset(x, y);
	XwaDrawText(text);
}

void DiagDrawBegin(bool isConcourse)
{
	if (isConcourse)
	{
		ConcourseDrawBegin();
	}
	else
	{
		InFlightDrawBegin();
	}
}

void DiagDrawEnd(bool isConcourse)
{
	if (isConcourse)
	{
		ConcourseDrawEnd();
	}
	else
	{
		InFlightDrawEnd();
	}
}

void DiagDrawText(bool isConcourse, short x, short y, const char* text)
{
	if (isConcourse)
	{
		ConcourseDrawText(x, y, text);
	}
	else
	{
		InFlightDrawText(x, y, text);
	}
}

void DiagDrawTextValue(bool isConcourse, short x, short y, const char* text, int value)
{
	char buffer[160]{};
	sprintf_s(buffer, "%s%d", text, value);
	DiagDrawText(isConcourse, x, y, buffer);
}

void DiagDrawTextMemory(bool isConcourse, short x, short y, const char* text, unsigned long long value)
{
	char bufferValue[160]{};
	StrFormatByteSize64A(value, bufferValue, sizeof(bufferValue));

	char buffer[160]{};
	sprintf_s(buffer, "%s%s", text, bufferValue);
	DiagDrawText(isConcourse, x, y, buffer);
}

void DiagDrawMessages(bool isConcourse)
{
	//static std::vector<char> g_vector1;
	//static bool init = false;

	//if (!init)
	//{
	//	init = true;
	//	g_vector1.reserve(1024U * 1024U * 1024U);

	//	memset(g_vector1.data(), 0, g_vector1.capacity());
	//}

	char text[160]{};
	const int text_size = 16;
	int text_y = -text_size;

	DiagDrawBegin(isConcourse);

	//DiagDrawTextMemory(isConcourse, 0, text_y += text_size, "g_vector1.capacity: ", g_vector1.capacity());

	PROCESS_MEMORY_COUNTERS processMemory{};
	processMemory.cb = sizeof(processMemory);
	GetProcessMemoryInfo(GetCurrentProcess(), &processMemory, sizeof(processMemory));

	DiagDrawTextMemory(isConcourse, 0, text_y += text_size, "Memory usage: ", processMemory.PeakPagefileUsage);

	if (!isConcourse)
	{
		DiagDrawTextValue(isConcourse, 0, text_y += text_size, "XwaAllocHandleCount: ", *(int*)0x00781E50);
		DiagDrawTextMemory(isConcourse, 0, text_y += text_size, "XwaAllocHandleTotalSize: ", *(int*)0x00781E58);
		DiagDrawTextMemory(isConcourse, 0, text_y += text_size, "XwaLoadOptBufMemSize: ", *(int*)0x0074C270);
		DiagDrawTextMemory(isConcourse, 0, text_y += text_size, "ResDataItemsMemSize: ", GetResDataItemsMemSize());
	}

	double cpuUsage = GetThreadCpuUsage();
	static StepValueDouble _cpuUsage;
	int cpuUsageMean = (int)_cpuUsage.GetValue(cpuUsage);

	DiagDrawTextValue(isConcourse, 0, text_y += text_size, "Cpu usage (%): ", cpuUsageMean);

	if (!isConcourse)
	{
		text_y += text_size;

		DiagDrawText(isConcourse, 0, text_y += text_size, "Loading Screen Steps:");

		for (int step = 1; step < 9; step++)
		{
			std::chrono::duration<double> elapsed = g_chrono[step] - g_chrono[step - 1];
			std::chrono::duration<double> duration = g_chrono[step] - g_chrono[0];

			std::string str =
				"step=" + std::to_string(step)
				+ " elapsed=" + std::to_string(elapsed.count()) + "s"
				+ " total=" + std::to_string(duration.count()) + "s";

			DiagDrawText(isConcourse, 0, text_y += text_size, str.c_str());
		}
	}

	DiagDrawEnd(isConcourse);
}

int ConcoursePresentHook(int* params)
{
	const int IDirectDraw = *(int*)0x009F702A;

	params[Params_EAX] = IDirectDraw;

	if (IDirectDraw == 0)
	{
		return 0;
	}

	DiagDrawMessages(true);

	return 0;
}

int InFlightPresentHook(int* params)
{
	params[Params_EAX] = *(int*)0x007712B0;

	DiagDrawMessages(false);

	return 0;
}
