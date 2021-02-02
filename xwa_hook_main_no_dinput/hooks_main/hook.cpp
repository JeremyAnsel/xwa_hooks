#include "targetver.h"
#include "hook.h"
#include "hook_function.h"
#include "config.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <utility>
#include <filesystem>
#include <chrono>
#include <thread>

class Config
{
public:
	Config()
	{
		auto lines = GetFileLines("dinput.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "dinput");
		}

		this->OutputStats = GetFileKeyValueInt(lines, "OutputStats", 0) != 0;
	}

	bool OutputStats;
};

Config g_config;

std::string int_to_hex(int i)
{
	std::stringstream stream;
	stream << std::setfill('0') << std::setw(8) << std::hex << i;
	return stream.str();
}

class VirtualProtectMemory
{
public:
	VirtualProtectMemory()
	{
		address = (void*)0x401000;
		size = 0x1A7BA0;
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

void VirtualProtectHookMemoryWrite()
{
	DWORD oldProtection;
	VirtualProtect((void*)0x401000, 0x1A7BA0, PAGE_READWRITE, &oldProtection);
}

void VirtualProtectHookMemory()
{
	DWORD oldProtection;
	VirtualProtect((void*)0x401000, 0x1A7BA0, PAGE_EXECUTE_READWRITE, &oldProtection);
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
		this->_functions.reserve(0x1A8000);
		memset(this->_functions.data(), 0, 0x1A8000 * 4);
		this->_names.resize(0x1A8000);
		int functionsCount = 0;

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

					if (this->_functions.data()[function.from - 0x401000] != nullptr)
					{
						std::string message;
						message.append("\"");
						message.append(this->_names[function.from - 0x401000]);
						message.append("\" is duplicated.");
						MessageBoxA(nullptr, message.c_str(), "xwa_hook_main (DInput.dll)", MB_OK | MB_ICONERROR);
						continue;
					}

					this->_functions.data()[function.from - 0x401000] = function.function;
					this->_names[function.from - 0x401000] = wrapper._name + " 0x" + int_to_hex(function.from);

					functionsCount++;
				}
			}
		}

		if (g_config.OutputStats)
		{
			OutputDebugString((
				"Hooks: "
				+ std::to_string(functionsCount)
				+ " functions in "
				+ std::to_string(this->_wrappers.size())
				+ " modules").c_str());
		}
	}

	std::vector<HookWrapper> _wrappers;
	std::vector<int(*)(int*)> _functions;
	std::vector<std::string> _names;
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

class BenchmarkFunctions
{
public:
	BenchmarkFunctions()
	{
		this->_benchmarks.resize(0x1A8000);
		this->_count.resize(0x1A8000);
	}

	~BenchmarkFunctions()
	{
		if (g_config.OutputStats)
		{
			std::vector<std::pair<int, double>> functions;

			for (int i = 0; i < 0x1A8000; i++)
			{
				std::chrono::duration<double> ellapsed = this->_benchmarks[i];

				if (ellapsed.count() != 0)
				{
					functions.push_back(std::make_pair(i, ellapsed.count()));
				}
			}

			std::sort(functions.begin(), functions.end(), [](const auto& a, const auto& b) {return a.second < b.second;});

			for (const auto& f : functions)
			{
				OutputDebugString((
					g_hookList._names[f.first]
					+ ": "
					+ std::to_string(this->_count[f.first])
					+ " calls in "
					+ std::to_string(f.second)
					+ " s").c_str());
			}
		}
	}

	std::vector<std::chrono::duration<long long, std::nano>> _benchmarks;
	std::vector<int> _count;
};

BenchmarkFunctions g_benchmarks;

int HookError(int call, int* params)
{
	std::string text;
	text.append("The called hook (");
	text.append(int_to_hex(call));
	text.append(") was not correctly installed or initialized.");

	MessageBoxA(nullptr, text.c_str(), "xwa_hook_main (DInput.dll)", MB_OK | MB_ICONERROR);

	return 0;
}

int HookWithStats(int call, int* params)
{
	auto start = std::chrono::high_resolution_clock::now();

	auto it = g_hookList._functions.data()[call - 0x401000];
	int ret;

	if (it != nullptr)
	{
		ret = it(params);
	}
	else
	{
		return HookError(call, params);
	}

	auto end = std::chrono::high_resolution_clock::now();

	g_benchmarks._benchmarks[call - 0x401000] += end - start;
	g_benchmarks._count[call - 0x401000]++;

	return ret;
}

int Hook(int call, int* params)
{
	if (g_config.OutputStats)
	{
		return HookWithStats(call, params);
	}

	auto it = g_hookList._functions.data()[call - 0x401000];

	if (it != nullptr)
	{
		return it(params);
	}
	else
	{
		return HookError(call, params);
	}
}
