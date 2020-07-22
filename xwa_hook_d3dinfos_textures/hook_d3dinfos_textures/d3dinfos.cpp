#include "targetver.h"
#include "d3dinfos.h"
#include "config.h"

#pragma pack(push, 1)

struct XwaD3DInfo
{
	char m00[0x53];
	XwaD3DInfo* pNext;
	XwaD3DInfo* pPrevious;
};

static_assert(sizeof(XwaD3DInfo) == 91, "size of XwaD3DInfo must be 91");

#pragma pack(pop)

class D3DInfoArray
{
public:
	D3DInfoArray()
	{
		auto lines = GetFileLines("hooks.ini", "hook_d3dinfos_textures");

		if (lines.empty())
		{
			lines = GetFileLines("hook_d3dinfos_textures.cfg");
		}

		int count = abs(GetFileKeyValueInt(lines, "Size"));

		if (count == 0)
		{
			count = 10000;
		}

		this->_d3dinfos.reserve(count);
	}

	int Size()
	{
		return this->_d3dinfos.capacity();
	}

	XwaD3DInfo* Data()
	{
		return this->_d3dinfos.data();
	}

private:
	std::vector<XwaD3DInfo> _d3dinfos;
};

D3DInfoArray g_XwaD3DInfos;

int InitD3DInfosHook(int* params)
{
	auto& XwaD3DInfosFirstEmpty = *(XwaD3DInfo**)0x00686B24;
	auto& XwaD3DInfosFirstUsed = *(XwaD3DInfo**)0x00686B28;
	auto& XwaD3DInfosCount = *(int*)0x00686B2C;

	int size = g_XwaD3DInfos.Size();
	XwaD3DInfo* data = g_XwaD3DInfos.Data();

	for (int i = 0; i < size - 1; i++)
	{
		data[i] = {};
		data[i].pNext = &data[i + 1];
	}

	data[size - 1] = {};

	XwaD3DInfosFirstEmpty = &data[0];
	XwaD3DInfosFirstUsed = nullptr;
	XwaD3DInfosCount = 0;

	return 0;
}
