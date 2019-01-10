#include "targetver.h"
#include "res1200.h"
#include <vector>

std::vector<int> g_resBuffer;

int Res1200Hook1(int* params)
{
	const unsigned int s_XwaTextBoundY = *(unsigned int*)0x007D4B6C;
	const int& s_XwaFlightScreenPitch = *(int*)0x0080DC58;
	const auto L0050E470 = (char*(*)())0x0050E470;
	char*& s_V0x06002BC = *(char**)0x006002BC;
	const int s_XwaFlightScreenRes = *(int*)0x007827D4;
	const int*& s_V0x074D5A4 = *(const int**)0x0074D5A4;
	const int*& s_V0x074D5A0 = *(const int**)0x0074D5A0;

	if (g_resBuffer.capacity() < s_XwaTextBoundY)
	{
		g_resBuffer.reserve(s_XwaTextBoundY);
	}

	for (unsigned int edx = 0; edx < s_XwaTextBoundY; edx++)
	{
		g_resBuffer[edx] = edx * s_XwaFlightScreenPitch;
	}

	s_V0x06002BC = L0050E470();

	if (s_XwaFlightScreenRes >= 0 && s_XwaFlightScreenRes <= 5)
	{
		memset(s_V0x06002BC, 0, s_XwaFlightScreenPitch * s_XwaTextBoundY);

		*(int*)0x005BA870 = 0x005BA858;
		*(int*)0x005BA874 = 0x0C;
	}

	s_V0x074D5A4 = &s_XwaFlightScreenPitch;
	s_V0x074D5A0 = g_resBuffer.data();

	return 0;
}

int Res1200Hook2(int* params)
{
	unsigned int height = (unsigned int)params[7];

	const int& s_XwaFlightScreenPitch = *(int*)0x0080DC58;
	const int*& s_V0x074D5A0 = *(const int**)0x0074D5A0;

	if (g_resBuffer.capacity() < height)
	{
		g_resBuffer.reserve(height);
	}

	s_V0x074D5A0 = g_resBuffer.data();

	return 0;
}

int Res1200Hook3(int* params)
{
	unsigned int height = (unsigned int)params[7];

	const int& s_XwaFlightScreenPitch = *(int*)0x0080DC58;
	const int*& s_V0x074D5A4 = *(const int**)0x0074D5A4;
	const int*& s_V0x074D5A0 = *(const int**)0x0074D5A0;

	if (g_resBuffer.capacity() < height)
	{
		g_resBuffer.reserve(height);
	}

	for (unsigned int cx = 0; cx < height; cx++)
	{
		g_resBuffer[cx] = cx * s_XwaFlightScreenPitch;
	}

	s_V0x074D5A4 = &s_XwaFlightScreenPitch;
	s_V0x074D5A0 = g_resBuffer.data();

	return 0;
}
