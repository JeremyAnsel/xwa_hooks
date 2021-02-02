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

struct XwaCraft
{
	char Unk0000[389];
	unsigned short m185; // flags
	char Unk0187[626];
};

static_assert(sizeof(XwaCraft) == 1017, "size of XwaCraft must be 1017");

struct XwaMobileObject
{
	char Unk0000[221];
	XwaCraft* pCraft;
	char Unk00E1[4];
};

static_assert(sizeof(XwaMobileObject) == 229, "size of XwaMobileObject must be 229");

struct XwaObject
{
	char unk00[35];
	XwaMobileObject* pMobileObject;
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

struct SceneCompData
{
	XwaObject* pObject;
	char unk04[280];
};

static_assert(sizeof(SceneCompData) == 284, "size of SceneCompData must be 284");

#pragma pack(pop)

class D3DInfoArray
{
public:
	D3DInfoArray()
	{
		auto lines = GetFileLines("hook_d3dinfos_textures.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_d3dinfos_textures");
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

int TestTextureIlluminationHook(int* params)
{
	const int esp14 = params[5];
	const SceneCompData* A4 = (SceneCompData*)params[18];

	bool jump = false;

	if (esp14 == 0)
	{
		jump = true;
	}
	else if (A4->pObject != nullptr && A4->pObject->pMobileObject != nullptr && A4->pObject->pMobileObject->pCraft != nullptr)
	{
		const XwaCraft* craft = A4->pObject->pMobileObject->pCraft;

		if (craft->m185 == 0)
		{
			jump = true;
		}
	}

	if (jump)
	{
		params[-1] = 0x443B9A;
	}

	return 0;
}
