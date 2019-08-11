#include "targetver.h"
#include "res1200.h"
#include <vector>

class ResBuffer
{
public:
	ResBuffer()
	{
		unsigned int size = 1200;

		this->_buffer.reserve(size);
	}

	void Reserve(int size, int pitch)
	{
		if (this->_buffer.capacity() < size)
		{
			this->_buffer.reserve(size);
		}

		for (unsigned int i = 0; i < size; i++)
		{
			this->_buffer.data()[i] = i * pitch;
		}
	}

	std::vector<int> _buffer;
};

ResBuffer g_resBuffer;
ResBuffer g_resBuffer2;

int Res1200Hook1(int* params)
{
	const unsigned int s_XwaTextBoundY = *(unsigned int*)0x007D4B6C;
	const int& s_XwaFlightScreenPitch = *(int*)0x0080DC58;
	const auto L0050E470 = (char*(*)())0x0050E470;
	char*& s_V0x06002BC = *(char**)0x006002BC;
	const int s_XwaFlightScreenRes = *(int*)0x007827D4;
	const int*& s_V0x074D5A4 = *(const int**)0x0074D5A4;
	const int*& s_V0x074D5A0 = *(const int**)0x0074D5A0;

	g_resBuffer.Reserve(s_XwaTextBoundY, s_XwaFlightScreenPitch);

	s_V0x06002BC = L0050E470();

	if (s_XwaFlightScreenRes >= 0 && s_XwaFlightScreenRes <= 5)
	{
		memset(s_V0x06002BC, 0, s_XwaFlightScreenPitch * s_XwaTextBoundY);

		*(int*)0x005BA870 = 0x005BA858;
		*(int*)0x005BA874 = 0x0C;
	}

	s_V0x074D5A4 = &s_XwaFlightScreenPitch;
	s_V0x074D5A0 = g_resBuffer._buffer.data();

	return 0;
}

int Res1200Hook2(int* params)
{
	unsigned int width = (unsigned int)params[6];
	unsigned int height = (unsigned int)params[7];

	const int s_XwaFlightScreenBppDiv8 = *(int*)0x005AA0AC;
	const int*& s_V0x074D5A0 = *(const int**)0x0074D5A0;

	g_resBuffer.Reserve(height, width * s_XwaFlightScreenBppDiv8);

	s_V0x074D5A0 = g_resBuffer._buffer.data();

	return 0;
}

int Res1200Hook3(int* params)
{
	unsigned int height = (unsigned int)params[7];

	const int& s_XwaFlightScreenPitch = *(int*)0x0080DC58;
	const int*& s_V0x074D5A4 = *(const int**)0x0074D5A4;
	const int*& s_V0x074D5A0 = *(const int**)0x0074D5A0;

	g_resBuffer.Reserve(height, s_XwaFlightScreenPitch);

	s_V0x074D5A4 = &s_XwaFlightScreenPitch;
	s_V0x074D5A0 = g_resBuffer._buffer.data();

	return 0;
}

int Res1200Hook800(int* params)
{
	const int A8 = params[3];
	const int AC = params[4];

	const int*& s_V0x074D5A0 = *(const int**)0x0074D5A0;
	const int s_XwaFlightScreenBppDiv8 = *(int*)0x005AA0AC;

	g_resBuffer2.Reserve(AC, A8 * s_XwaFlightScreenBppDiv8);

	s_V0x074D5A0 = g_resBuffer2._buffer.data();

	return 0;
}
