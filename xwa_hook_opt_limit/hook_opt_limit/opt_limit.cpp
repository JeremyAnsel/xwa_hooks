#include "targetver.h"
#include "opt_limit.h"
#include "config.h"
#include <vector>

#pragma pack(push, 1)

struct HitData
{
	float m00;
	float m04;
	char m08;
};

#pragma pack(pop)

class HitDataArray
{
public:
	HitDataArray()
	{
		int count = abs(GetFileKeyValueInt("hook_opt_limit.cfg", "Size"));

		if (count == 0)
		{
			count = 4096;
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
