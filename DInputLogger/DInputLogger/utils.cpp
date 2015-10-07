#include "common.h"
#include "utils.h"

#include <unordered_map>
#include <comdef.h>

std::unordered_map<void*, void*> g_map;

void* GetWrapper(void* original)
{
	return g_map[original];
}

void AddWrapper(void* wrapper, void* original)
{
	g_map[original] = wrapper;
}

void RemoveWrapper(void* original)
{
	g_map.erase(original);
}

std::string tostr_IID(REFIID iid)
{
	LPOLESTR lpsz;
	StringFromIID(iid, &lpsz);

	std::wstring wstr(lpsz);
	std::string str(wstr.begin(), wstr.end());

	CoTaskMemFree(lpsz);

	return str;
}

std::string tostr_HR(HRESULT hr)
{
	return _com_error(hr).ErrorMessage();
}
