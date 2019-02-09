#include "targetver.h"
#include "countermeasures.h"

#pragma pack(push, 1)

struct ExeCraftEntry
{
	char Unk0000[563];
	unsigned char CounterMeasuresCount;
	char Unk0234[423];
};

static_assert(sizeof(ExeCraftEntry) == 987, "size of ExeCraftEntry must be 987");

struct XwaCraft
{
	char Unk0000[487];
	unsigned char CounterMeasuresType;
	unsigned char CounterMeasuresCount;
	char Unk01E9[528];
};

static_assert(sizeof(XwaCraft) == 1017, "size of XwaCraft must be 1017");

#pragma pack(pop)

int CountermeasuresCountHook(int* params)
{
	XwaCraft* craft = (XwaCraft*)params[0];
	int craftIndex = params[1];

	const auto XwaMulWordPercent = (unsigned short(*)(unsigned short, unsigned short))0x00494C90;

	const ExeCraftEntry* ExeCraftTable = (ExeCraftEntry*)0x005BB480;

	unsigned char countermeasuresCount = ExeCraftTable[craftIndex].CounterMeasuresCount;
	unsigned char countermeasuresType = craft->CounterMeasuresType;

	if (countermeasuresType == 0x02)
	{
		countermeasuresCount = (unsigned char)XwaMulWordPercent(countermeasuresCount, 0xAAAC);
	}

	return countermeasuresCount;
}
