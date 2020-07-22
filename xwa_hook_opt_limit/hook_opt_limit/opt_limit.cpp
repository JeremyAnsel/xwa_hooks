#include "targetver.h"
#include "opt_limit.h"
#include "config.h"

#pragma pack(push, 1)

struct HitData
{
	float m00;
	float m04;
	char m08;
};

static_assert(sizeof(HitData) == 9, "size of HitData must be 9");

#pragma pack(pop)

class HitDataArray
{
public:
	HitDataArray()
	{
		auto lines = GetFileLines("hooks.ini", "hook_opt_limit");

		if (lines.empty())
		{
			lines = GetFileLines("hook_opt_limit.cfg");
		}

		int count = abs(GetFileKeyValueInt(lines, "Size"));

		if (count == 0)
		{
			count = 65536;
		}

		this->_hitData.reserve(count);
	}

	int Size()
	{
		return this->_hitData.capacity();
	}

	HitData* Data()
	{
		return this->_hitData.data();
	}

private:
	std::vector<HitData> _hitData;
};

int GetHitDataArrayPtr()
{
	static HitDataArray g_XwaHitDataArray;

	return (int)g_XwaHitDataArray.Data();
}

int GenerateSpecRciHook(int* params)
{
	const auto XwaReadShiplist = (void(*)())0x00529950;
	const auto XwaGenerateSpecRci = (void(*)())0x00577490;
	const auto XwaFileOpen = (void*(*)(const char*, const char*))0x0052AD30;
	const auto XwaFileClose = (void(*)(void*))0x0052ADD0;

	XwaReadShiplist();

	void* specRciFile = XwaFileOpen("spec.rci", "rb");

	if (specRciFile)
	{
		XwaFileClose(specRciFile);
	}
	else
	{
		XwaGenerateSpecRci();
	}

	return 0;
}
