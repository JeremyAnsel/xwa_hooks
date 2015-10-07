#include "common.h"
#include "DirectInputA.h"

#pragma comment(lib, "dxguid")

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
	static decltype(DirectInputCreateA)* dinput_proc = (decltype(DirectInputCreateA)*)GetProcAddress(dinput._module, "DirectInputCreateA");

	std::ostringstream str;
	str << __FUNCTION__;
	str << " " << (void*)dwVersion;

	LogText(str.str());

	HRESULT hr = dinput_proc(hinst, dwVersion, lplpDirectInput, punkOuter);

	if (SUCCEEDED(hr))
	{
		*lplpDirectInput = GetOrCreateWrapperT(DirectInputA, *lplpDirectInput);
	}

	str.str("");
	str << "\t" << tostr_HR(hr);

	LogText(str.str());

	return hr;
}

extern "C" HRESULT WINAPI DirectInputCreateEx(
	HINSTANCE hinst,
	DWORD dwVersion,
	REFIID riidltf,
	LPVOID * ppvOut,
	LPUNKNOWN punkOuter
	)
{
	static decltype(DirectInputCreateEx)* dinput_proc = (decltype(DirectInputCreateEx)*)GetProcAddress(dinput._module, "DirectInputCreateEx");

	//std::ostringstream str;
	//str << __FUNCTION__;
	//str << " " << (void*)dwVersion;
	//str << " " << tostr_IID(riidltf);

	//LogText(str.str());

	HRESULT hr = dinput_proc(hinst, dwVersion, riidltf, ppvOut, punkOuter);

	//str.str("");
	//str << "\t" << tostr_HR(hr);

	//LogText(str.str());

	return hr;
}

extern "C" HRESULT WINAPI DirectInputCreateW(
	HINSTANCE hinst,
	DWORD dwVersion,
	LPDIRECTINPUTW* lplpDirectInput,
	LPUNKNOWN punkOuter
	)
{
	static decltype(DirectInputCreateW)* dinput_proc = (decltype(DirectInputCreateW)*)GetProcAddress(dinput._module, "DirectInputCreateW");

	//std::ostringstream str;
	//str << __FUNCTION__;
	//str << " " << (void*)dwVersion;

	//LogText(str.str());

	HRESULT hr = dinput_proc(hinst, dwVersion, lplpDirectInput, punkOuter);

	//str.str("");
	//str << "\t" << tostr_HR(hr);

	//LogText(str.str());

	return hr;
}
