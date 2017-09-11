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

void LoadHooks()
{
	g_hookList;
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
