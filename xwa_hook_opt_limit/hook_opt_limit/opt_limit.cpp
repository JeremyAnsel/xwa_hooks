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
		const auto lines = GetFileLines("hook_opt_limit.cfg");
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
