#include "common.h"
#include "hook.h"
#include "hook_function.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <filesystem>

class VirtualProtectMemory
{
public:
	VirtualProtectMemory()
	{
		address = (void*)0x401000;
		size = 0x1A7B40;
		VirtualProtect(address, size, PAGE_READWRITE, &oldProtection);
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

void VirtualProtectHookMemory()
{
	DWORD oldProtection;
	VirtualProtect((void*)0x401000, 0x1A7B40, PAGE_EXECUTE_READWRITE, &oldProtection);
}

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

bool PatchMemory(const HookPatch& patch)
{
	bool error = false;
	const char* error_patch = nullptr;

	for (int itemIndex = 0; itemIndex < patch.Count; itemIndex++)
	{
		const auto& item = patch.Items[itemIndex];

		size_t fromLength = strlen(item.From);
		size_t toLength = strlen(item.To);

		if ((fromLength % 2 != 0) || (toLength % 2 != 0) || (fromLength != toLength))
		{
			error = true;
			error_patch = patch.Name;
			break;
		}

		if (!IsPatchMemoryEqual((char*)item.Offset, item.From) && !IsPatchMemoryEqual((char*)item.Offset, item.To))
		{
			error = true;
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
		MessageBoxA(nullptr, message.c_str(), "xwa_hook_main (DInput.dll)", MB_OK | MB_ICONERROR);
		return false;
	}

	VirtualProtectMemory memoryProtect;

	for (int itemIndex = 0; itemIndex < patch.Count; itemIndex++)
	{
		const auto& item = patch.Items[itemIndex];

		WritePatchMemory((char*)item.Offset, item.To);
	}

	return true;
}

class HookWrapper
{
public:
	HookWrapper(const std::string& path)
		:_name(path), _module(nullptr)
	{
	}

	HookWrapper(const HookWrapper& hook)
		:_name(hook._name), _module(nullptr)
	{
	}

	~HookWrapper()
	{
		this->Unload();
	}

	void Load()
	{
		this->Unload();
		this->_module = LoadLibraryA(this->_name.c_str());
	}

	void Unload()
	{
		if (this->_module)
		{
			FreeLibrary(this->_module);
			this->_module = nullptr;
		}
	}

	const std::string _name;
	HMODULE _module;
};

class HookList
{
public:
	HookList()
	{
		LoadHooksList();
		LoadHooksLibraries();
		InitHooksFunctionsMap();
	}

	void LoadHooksList()
	{
		for (const auto& file : std::experimental::filesystem::directory_iterator("."))
		{
			if (!std::experimental::filesystem::is_regular_file(file))
			{
				continue;
			}

			if (file.path().extension().string() != ".dll")
			{
				continue;
			}

			const auto& filename = file.path().filename().string();

			if (filename.find("hook_") != 0 && filename.find("Hook_") != 0)
			{
				continue;
			}

			this->_wrappers.push_back(HookWrapper{ filename });
		}
	}

	void LoadHooksLibraries()
	{
		for (auto& wrapper : this->_wrappers)
		{
			wrapper.Load();
		}
	}

	void InitHooksFunctionsMap()
	{
		for (const auto& wrapper : this->_wrappers)
		{
			auto getCount = (int(*)())GetProcAddress(wrapper._module, "GetHookFunctionsCount");
			auto getHook = (HookFunction(*)(int))GetProcAddress(wrapper._module, "GetHookFunction");

			if (getCount != nullptr && getHook != nullptr)
			{
				int count = getCount();

				for (int i = 0; i < count; i++)
				{
					auto function = getHook(i);

					if (function.from == 0 || function.function == nullptr)
					{
						continue;
					}

					this->_functions.insert(std::make_pair(function.from, function.function));
				}
			}
		}
	}

	std::vector<HookWrapper> _wrappers;
	std::map<int, int(*)(int*)> _functions;
};

HookList g_hookList;

bool LoadAndPatchHooks()
{
	for (const auto& wrapper : g_hookList._wrappers)
	{
		auto getPatch = (const HookPatch* (*)(int))GetProcAddress(wrapper._module, "GetHookPatch");

		if (getPatch != nullptr)
		{
			const HookPatch* patch = nullptr;

			for (int i = 0; (patch = getPatch(i)) != nullptr; i++)
			{
				if (!PatchMemory(*patch))
				{
					return false;
				}
			}
		}
	}

	return true;
}

int HookError(int* params)
{
	MessageBoxA(nullptr, "The called hook was not correctly installed or initialized.", "xwa_hook_main (DInput.dll)", MB_OK | MB_ICONERROR);

	return 0;
}

int Hook(int call, int* params)
{
	auto it = g_hookList._functions.find(call);

	if (it != g_hookList._functions.end())
	{
		return it->second(params);
	}

	return HookError(params);
}
