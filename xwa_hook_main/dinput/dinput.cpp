#include "common.h"
#include "hook.h"

class LibraryWrapper
{
public:
	LibraryWrapper(LPCSTR lpLibFileName)
	{
		char path[MAX_PATH];
		GetSystemDirectoryA(path, MAX_PATH);
		strcat_s(path, "\\");
		strcat_s(path, lpLibFileName);

		this->_module = LoadLibraryA(path);
	}

	~LibraryWrapper()
	{
		if (this->_module)
		{
			FreeLibrary(this->_module);
		}
	}

	HMODULE _module;
};

static LibraryWrapper dinput("dinput.dll");

extern "C" HRESULT WINAPI DirectInputCreateA(
	HINSTANCE hinst,
	DWORD dwVersion,
	LPDIRECTINPUTA* lplpDirectInput,
	LPUNKNOWN punkOuter
	)
{
	if (dwVersion != 0)
	{
		static auto dinput_proc = (decltype(DirectInputCreateA)*)GetProcAddress(dinput._module, "DirectInputCreateA");

		return dinput_proc(hinst, dwVersion, lplpDirectInput, punkOuter);
	}
	else
	{
		int* esp = (int*)punkOuter;

		int result = Hook(esp[9], esp + 10);

		if (esp[-5] == 0x5A8B3D)
		{
			esp[7] = result;
		}

		return 0;
	}
}

extern "C" HRESULT WINAPI DirectInputCreateEx(
	HINSTANCE hinst,
	DWORD dwVersion,
	REFIID riidltf,
	LPVOID * ppvOut,
	LPUNKNOWN punkOuter
	)
{
	static auto dinput_proc = (decltype(DirectInputCreateEx)*)GetProcAddress(dinput._module, "DirectInputCreateEx");

	return dinput_proc(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}

extern "C" HRESULT WINAPI DirectInputCreateW(
	HINSTANCE hinst,
	DWORD dwVersion,
	LPDIRECTINPUTW* lplpDirectInput,
	LPUNKNOWN punkOuter
	)
{
	static auto dinput_proc = (decltype(DirectInputCreateW)*)GetProcAddress(dinput._module, "DirectInputCreateW");

	return dinput_proc(hinst, dwVersion, lplpDirectInput, punkOuter);
}
