#include "targetver.h"
#include "lightmaps.h"
#include <vector>

std::vector<char> g_buffer;

int LightmapsHook(int* params)
{
	const unsigned char* esi = (unsigned char*)params[0];
	const int ebx = params[1];
	const int eax = params[2];

	const unsigned short* a1C = (unsigned short*)params[711];
	const unsigned char esp28 = (unsigned char)params[13];
	int& esp34 = params[16];

	if ((int)g_buffer.capacity() < eax * 2)
	{
		g_buffer.reserve(eax * 2);
	}

	unsigned char* ecx = (unsigned char*)g_buffer.data() + ebx;

	esp34 = 0;

	for (int i = 0; i < eax; i++)
	{
		unsigned char esp40 = *esi;
		esi++;

		if (a1C[esp40] == 0)
		{
			*ecx = 0;
		}
		else
		{
			esp34 = 1;

			if (esp40 == 0)
			{
				*ecx = esp28;
			}
			else
			{
				*ecx = esp40;
			}
		}

		ecx++;
	}

	return (int)g_buffer.data() + ebx;
}
