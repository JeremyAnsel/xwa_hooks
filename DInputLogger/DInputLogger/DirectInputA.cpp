#include "common.h"
#include "DirectInputA.h"
#include "DirectInputDeviceA.h"

DirectInputA::DirectInputA(IDirectInputA* original)
{
	this->_original = original;
}

DirectInputA::~DirectInputA()
{

}

HRESULT DirectInputA::QueryInterface(
	REFIID riid,
	LPVOID* obp
	)
{
	std::ostringstream str;
	str << this << " " << __FUNCTION__;
	str << " " << tostr_IID(riid);

	LogText(str.str());

	*obp = nullptr;

	HRESULT hr = this->_original->QueryInterface(riid, obp);

	str.str("");
	str << "\t" << tostr_HR(hr);

	LogText(str.str());

	return hr;
}

ULONG DirectInputA::AddRef()
{
	std::ostringstream str;
	str << this << " " << __FUNCTION__;

	LogText(str.str());

	ULONG count = this->_original->AddRef();

	str.str("");
	str << "\t" << count;

	LogText(str.str());

	return count;
}

ULONG DirectInputA::Release()
{
	std::ostringstream str;
	str << this << " " << __FUNCTION__;

	LogText(str.str());

	ULONG count = this->_original->Release();

	if (count == 0)
	{
		RemoveWrapper(this->_original);
		delete this;
	}

	str.str("");
	str << "\t" << count;

	LogText(str.str());

	return count;
}

HRESULT DirectInputA::CreateDevice(
	REFGUID rguid,
	LPDIRECTINPUTDEVICE *lplpDirectInputDevice,
	LPUNKNOWN pUnkOuter
	)
{
	std::ostringstream str;
	str << this << " " << __FUNCTION__;

	if (rguid == GUID_SysKeyboard)
	{
		str << " SysKeyboard";
	}
	else if (rguid == GUID_SysMouse)
	{
		str << " SysMouse";
	}
	else
	{
		str << " " << tostr_IID(rguid);
	}

	LogText(str.str());

	HRESULT hr = this->_original->CreateDevice(rguid, lplpDirectInputDevice, pUnkOuter);

	if (SUCCEEDED(hr))
	{
		*lplpDirectInputDevice = GetOrCreateWrapperT(DirectInputDeviceA, *lplpDirectInputDevice);
	}

	str.str("");
	str << "\t" << tostr_HR(hr);

	LogText(str.str());

	return hr;
}

HRESULT DirectInputA::EnumDevices(
	DWORD dwDevType,
	LPDIENUMDEVICESCALLBACKA lpCallback,
	LPVOID pvRef,
	DWORD dwFlags
	)
{
	std::ostringstream str;
	str << this << " " << __FUNCTION__;

	LogText(str.str());

	HRESULT hr = this->_original->EnumDevices(dwDevType, lpCallback, pvRef, dwFlags);

	str.str("");
	str << "\t" << tostr_HR(hr);

	LogText(str.str());

	return hr;
}

HRESULT DirectInputA::GetDeviceStatus(
	REFGUID rguidInstance
	)
{
	std::ostringstream str;
	str << this << " " << __FUNCTION__;
	str << " " << tostr_IID(rguidInstance);

	LogText(str.str());

	HRESULT hr = this->_original->GetDeviceStatus(rguidInstance);

	str.str("");
	str << "\t" << tostr_HR(hr);

	LogText(str.str());

	return hr;
}

HRESULT DirectInputA::RunControlPanel(
	HWND hwndOwner,
	DWORD dwFlags
	)
{
	std::ostringstream str;
	str << this << " " << __FUNCTION__;

	LogText(str.str());

	HRESULT hr = this->_original->RunControlPanel(hwndOwner, dwFlags);

	str.str("");
	str << "\t" << tostr_HR(hr);

	LogText(str.str());

	return hr;
}

HRESULT DirectInputA::Initialize(
	HINSTANCE hinst,
	DWORD dwVersion
	)
{
	std::ostringstream str;
	str << this << " " << __FUNCTION__;

	LogText(str.str());

	HRESULT hr = this->_original->Initialize(hinst, dwVersion);

	str.str("");
	str << "\t" << tostr_HR(hr);

	LogText(str.str());

	return hr;
}
