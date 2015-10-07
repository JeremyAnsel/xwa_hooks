#include "common.h"
#include "hook.h"
#include "hook_function.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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

					if (function.from == 0 && function.function == nullptr)
					{
						continue;
					}

					this->_functions.push_back(function);
				}
			}
		}
	}

	std::vector<HookWrapper> _wrappers;
	std::vector<HookFunction> _functions;
};

HookList g_hookList;

int HookError(int* params)
{
	MessageBoxA(nullptr, "The called hook was not correctly installed or initialized.", "DInput.dll", MB_OK | MB_ICONERROR);

	return 0;
}

int Hook(int call, int* params)
{
	for (auto& function : g_hookList._functions)
	{
		if (call == function.from)
		{
			return function.function(params);
		}
	}

	return HookError(params);
}
