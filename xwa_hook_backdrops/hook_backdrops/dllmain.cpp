#include "targetver.h"

#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <Windows.h>

#include <string>

struct PatchItem
{
	int Offset;
	const char* From;
	const char* To;
};

struct Patch
{
	const char* Name;
	int Count;
	PatchItem* Items;
};

PatchItem g_addStarfieldBackdrops[] =
{
	{ 0x17A3D9, "33DB85C08944241C0F8E4D0300008B74241469F63E0E0000", "8D5C24145350E83CDB020083C408E9480300009090909090" },
};

Patch g_patches[] =
{
	{ "To call the hook that adds starfield backdrops", ARRAYSIZE(g_addStarfieldBackdrops), g_addStarfieldBackdrops }
};

class VirtualProtectMemory
{
public:
	VirtualProtectMemory()
	{
		address = (void*)0x401000;
		size = 0x1A7B40;
		VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &oldProtection);
	}

	~VirtualProtectMemory()
	{
		VirtualProtect(address, size, oldProtection, nullptr);
	}

private:
	DWORD oldProtection;
	LPVOID address;
	SIZE_T size;
};

bool IsPatchMemoryEqual(const char* offset, const char* item)
{
	offset += 0x400C00;

	char byte[3];

	bool ret = true;

	for (int i = 0; item[i] != 0; i += 2, offset++)
	{
		byte[0] = item[i];
		byte[1] = item[i + 1];
		byte[2] = 0;

		char v = std::stoi(byte, 0, 16);

		if (*offset != v)
		{
			return false;
		}
	}

	return true;
}

void WritePatchMemory(char* offset, const char* item)
{
	offset += 0x400C00;

	char byte[3];

	for (int i = 0; item[i] != 0; i += 2, offset++)
	{
		byte[0] = item[i];
		byte[1] = item[i + 1];
		byte[2] = 0;

		char v = std::stoi(byte, 0, 16);
		*offset = v;
	}
}

void ExePatch()
{
	bool error = false;
	const char* error_patch = nullptr;

	for (const auto& patch : g_patches)
	{
		for (int itemIndex = 0; itemIndex < patch.Count; itemIndex++)
		{
			const auto& item = patch.Items[itemIndex];

			if (!IsPatchMemoryEqual((char*)item.Offset, item.From) && !IsPatchMemoryEqual((char*)item.Offset, item.To))
			{
				error = true;
				break;
			}
		}

		if (error)
		{
			error_patch = patch.Name;
			break;
		}
	}

	if (error)
	{
		std::string message;
		message.append("\"");
		message.append(error_patch);
		message.append("\" is not correctly initialized.");
		MessageBoxA(nullptr, message.c_str(), "xwa_hook_backdrops", MB_OK | MB_ICONERROR);
		return;
	}

	VirtualProtectMemory memoryProtect;

	for (const auto& patch : g_patches)
	{
		for (int itemIndex = 0; itemIndex < patch.Count; itemIndex++)
		{
			const auto& item = patch.Items[itemIndex];

			WritePatchMemory((char*)item.Offset, item.To);
		}
	}
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		ExePatch();
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}
