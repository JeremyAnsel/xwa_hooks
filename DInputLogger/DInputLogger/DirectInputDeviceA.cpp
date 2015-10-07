#include "common.h"
#include "DirectInputDeviceA.h"

DirectInputDeviceA::DirectInputDeviceA(IDirectInputDeviceA* original)
{
	this->_original = original;
}

DirectInputDeviceA::~DirectInputDeviceA()
{

}

HRESULT DirectInputDeviceA::QueryInterface(
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

ULONG DirectInputDeviceA::AddRef()
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

ULONG DirectInputDeviceA::Release()
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

HRESULT DirectInputDeviceA::GetCapabilities(
	LPDIDEVCAPS lpDIDevCaps
	)
{
	std::ostringstream str;
	str << this << " " << __FUNCTION__;

	LogText(str.str());

	HRESULT hr = this->_original->GetCapabilities(lpDIDevCaps);

	str.str("");
	str << "\t" << tostr_HR(hr);

	LogText(str.str());

	return hr;
}

HRESULT DirectInputDeviceA::EnumObjects(
	LPDIENUMDEVICEOBJECTSCALLBACKA lpCallback,
	LPVOID pvRef,
	DWORD dwFlags
	)
{
	std::ostringstream str;
	str << this << " " << __FUNCTION__;

	LogText(str.str());

	HRESULT hr = this->_original->EnumObjects(lpCallback, pvRef, dwFlags);

	str.str("");
	str << "\t" << tostr_HR(hr);

	LogText(str.str());

	return hr;
}

HRESULT DirectInputDeviceA::GetProperty(
	REFGUID rguidProp,
	LPDIPROPHEADER pdiph
	)
{
	std::ostringstream str;
	str << this << " " << __FUNCTION__;

	LogText(str.str());

	HRESULT hr = this->_original->GetProperty(rguidProp, pdiph);

	str.str("");
	str << "\t" << tostr_HR(hr);

	LogText(str.str());

	return hr;
}

HRESULT DirectInputDeviceA::SetProperty(
	REFGUID rguidProp,
	LPCDIPROPHEADER pdiph
	)
{
	std::ostringstream str;
	str << this << " " << __FUNCTION__;

	LogText(str.str());

	HRESULT hr = this->_original->SetProperty(rguidProp, pdiph);

	str.str("");
	str << "\t" << tostr_HR(hr);

	LogText(str.str());

	return hr;
}

HRESULT DirectInputDeviceA::Acquire()
{
	std::ostringstream str;
	str << this << " " << __FUNCTION__;

	LogText(str.str());

	HRESULT hr = this->_original->Acquire();

	str.str("");
	str << "\t" << tostr_HR(hr);

	LogText(str.str());

	return hr;
}

HRESULT DirectInputDeviceA::Unacquire()
{
	std::ostringstream str;
	str << this << " " << __FUNCTION__;

	LogText(str.str());

	HRESULT hr = this->_original->Unacquire();

	str.str("");
	str << "\t" << tostr_HR(hr);

	LogText(str.str());

	return hr;
}

HRESULT DirectInputDeviceA::GetDeviceState(
	DWORD cbData,
	LPVOID lpvData
	)
{
	std::ostringstream str;
	str << this << " " << __FUNCTION__;

	LogText(str.str());

	HRESULT hr = this->_original->GetDeviceState(cbData, lpvData);

	str.str("");
	str << "\t" << tostr_HR(hr);

	LogText(str.str());

	return hr;
}

HRESULT DirectInputDeviceA::GetDeviceData(
	DWORD cbObjectData,
	LPDIDEVICEOBJECTDATA rgdod,
	LPDWORD pdwInOut,
	DWORD dwFlags
	)
{
	std::ostringstream str;
	str << this << " " << __FUNCTION__;

	LogText(str.str());

	HRESULT hr = this->_original->GetDeviceData(cbObjectData, rgdod, pdwInOut, dwFlags);

	str.str("");
	str << "\t" << tostr_HR(hr);

	LogText(str.str());

	return hr;
}

HRESULT DirectInputDeviceA::SetDataFormat(
	LPCDIDATAFORMAT lpdf
	)
{
	std::ostringstream str;
	str << this << " " << __FUNCTION__;

	LogText(str.str());

	HRESULT hr = this->_original->SetDataFormat(lpdf);

	str.str("");
	str << "\t" << tostr_HR(hr);

	LogText(str.str());

	return hr;
}

HRESULT DirectInputDeviceA::SetEventNotification(
	HANDLE hEvent
	)
{
	std::ostringstream str;
	str << this << " " << __FUNCTION__;

	LogText(str.str());

	HRESULT hr = this->_original->SetEventNotification(hEvent);

	str.str("");
	str << "\t" << tostr_HR(hr);

	LogText(str.str());

	return hr;
}

HRESULT DirectInputDeviceA::SetCooperativeLevel(
	HWND hwnd,
	DWORD dwFlags
	)
{
	std::ostringstream str;
	str << this << " " << __FUNCTION__;

	LogText(str.str());

	HRESULT hr = this->_original->SetCooperativeLevel(hwnd, dwFlags);

	str.str("");
	str << "\t" << tostr_HR(hr);

	LogText(str.str());

	return hr;
}

HRESULT DirectInputDeviceA::GetObjectInfo(
	LPDIDEVICEOBJECTINSTANCE pdidoi,
	DWORD dwObj,
	DWORD dwHow
	)
{
	std::ostringstream str;
	str << this << " " << __FUNCTION__;

	LogText(str.str());

	HRESULT hr = this->_original->GetObjectInfo(pdidoi, dwObj, dwHow);

	str.str("");
	str << "\t" << tostr_HR(hr);

	LogText(str.str());

	return hr;
}

HRESULT DirectInputDeviceA::GetDeviceInfo(
	LPDIDEVICEINSTANCE pdidi
	)
{
	std::ostringstream str;
	str << this << " " << __FUNCTION__;

	LogText(str.str());

	HRESULT hr = this->_original->GetDeviceInfo(pdidi);

	str.str("");
	str << "\t" << tostr_HR(hr);

	LogText(str.str());

	return hr;
}

HRESULT DirectInputDeviceA::RunControlPanel(
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

HRESULT DirectInputDeviceA::Initialize(
	HINSTANCE hinst,
	DWORD dwVersion,
	REFGUID rguid
	)
{
	std::ostringstream str;
	str << this << " " << __FUNCTION__;

	LogText(str.str());

	HRESULT hr = this->_original->Initialize(hinst, dwVersion, rguid);

	str.str("");
	str << "\t" << tostr_HR(hr);

	LogText(str.str());

	return hr;
}
