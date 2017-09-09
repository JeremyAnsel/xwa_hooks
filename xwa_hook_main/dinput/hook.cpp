#include "common.h"
#include "hook.h"
#include "hook_function.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <utility>

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
		LoadHooksLst();
		LoadHooksLibraries();
	}

	void LoadHooksLst()
	{
		std::ifstream file("dinput_hooks.lst");

		if (file)
		{
			std::string line;

			while (std::getline(file, line))
			{
				if (!line.length())
				{
					continue;
				}

				if (!std::ifstream(line))
				{
					continue;
				}

				this->_wrappers.push_back(HookWrapper{ line });
			}
		}
	}

	void LoadHooksLibraries()
	{
		for (auto& wrapper : this->_wrappers)
		{
			wrapper.Load();

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

			auto getPatch = (const HookPatch *(*)(int))GetProcAddress(wrapper._module, "GetHookPatch");
			if (getPatch != nullptr)
			{
				const HookPatch *patch = nullptr;
				for (int i = 0; patch = getPatch(i); i++)
				{
					if (memcmp((const void *)patch->offset, patch->original, patch->size) == 0 ||
						(patch->original_alternative && memcmp((const void *)patch->offset, patch->original_alternative, patch->size) == 0))
					{
						DWORD old, dummy;
						VirtualProtect((void *)patch->offset, patch->size, PAGE_READWRITE, &old);
						memcpy((void *)patch->offset, patch->patched, patch->size);
						if (patch->calladdr_pos >= 0)
						{
							*(unsigned *)(patch->offset + patch->calladdr_pos) = patch->calladdr;
						}
						VirtualProtect((void *)patch->offset, patch->size, old, &dummy);
					}
					else
						MessageBoxA(nullptr, "Unexpected code found when patching hook", "DInput.dll", MB_OK | MB_ICONERROR);
				}
			}
		}
	}

	std::vector<HookWrapper> _wrappers;
	std::map<int, int(*)(int*)> _functions;
};

HookList g_hookList;

int HookError(int* params)
{
	MessageBoxA(nullptr, "The called hook was not correctly installed or initialized.", "DInput.dll", MB_OK | MB_ICONERROR);

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
