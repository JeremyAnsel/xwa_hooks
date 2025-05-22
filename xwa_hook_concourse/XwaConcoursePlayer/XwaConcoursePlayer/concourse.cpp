#include "framework.h"
#include "concourse.h"
#include "webm_helpers.h"

#pragma comment(lib, "dxguid")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d2d1")

extern "C"
{
	// http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;

	// https://gpuopen.com/amdpowerxpressrequesthighperformance/
	__declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
}

enum ExeDataType
{
	ExeDataType_None,
	ExeDataType_ShowMessage,
	ExeDataType_WebmLoadVideo,
	ExeDataType_WebmFreeVideo,
	ExeDataType_WebmFreeAllVideos,
	ExeDataType_WebmResetAllTimecode,
	ExeDataType_WebmGetTimecode_Low,
	ExeDataType_WebmGetTimecode_High,
	ExeDataType_WebmGetDuration_Low,
	ExeDataType_WebmGetDuration_High,
	ExeDataType_WebmReadVideoFrame_Result,
	ExeDataType_WebmReadVideoFrame_Image,
	ExeDataType_WebmReadVideoFrame_Width,
	ExeDataType_WebmReadVideoFrame_Height,
};

void ExeShowMessage(DWORD cbData, PVOID lpData)
{
	std::string message = (const char*)lpData;
	MessageBoxA(NULL, message.c_str(), "hook_concourse", MB_OK);
}

static std::map<int, std::tuple<ID3D11Texture2D*, unsigned int>> _webmTexures;

void ExeWebmLoadVideo(DWORD cbData, PVOID lpData)
{
	std::string name = (const char*)lpData;
	WebmLoadVideo(name);
}

void ExeWebmFreeVideo(DWORD cbData, PVOID lpData)
{
	std::string name = (const char*)lpData;
	WebmFreeVideo(name);
}

void ExeWebmFreeAllVideos(DWORD cbData, PVOID lpData)
{
	WebmFreeAllVideos();

	for (const auto& item : _webmTexures)
	{
		if (std::get<0>(item.second))
		{
			std::get<0>(item.second)->Release();
		}
	}

	_webmTexures.clear();
}

void ExeWebmResetAllTimecode(DWORD cbData, PVOID lpData)
{
	WebmResetAllTimecode();
}

static uint64_t _webmTimeCode;

unsigned int ExeWebmGetTimecode_Low(DWORD cbData, PVOID lpData)
{
	std::string name = (const char*)lpData;
	_webmTimeCode = (uint64_t)WebmGetTimecode(name);
	return (unsigned int)_webmTimeCode;
}

unsigned int ExeWebmGetTimecode_High(DWORD cbData, PVOID lpData)
{
	return (unsigned int)(_webmTimeCode >> 32);
}

static uint64_t _webmGetDuration;

unsigned int ExeWebmGetDuration_Low(DWORD cbData, PVOID lpData)
{
	std::string name = (const char*)lpData;
	_webmGetDuration = WebmGetDuration(name);
	return (unsigned int)_webmGetDuration;
}

unsigned int ExeWebmGetDuration_High(DWORD cbData, PVOID lpData)
{
	return (unsigned int)(_webmGetDuration >> 32);
}

static unsigned int _webmReadVideoFrame_Image;
static unsigned int _webmReadVideoFrame_Width;
static unsigned int _webmReadVideoFrame_Height;

int ExeWebmReadVideoFrame_Result(DWORD cbData, PVOID lpData)
{
	std::string name = (const char*)lpData;
	_webmReadVideoFrame_Image = 0;
	_webmReadVideoFrame_Width = 0;
	_webmReadVideoFrame_Height = 0;

	uint8_t* image = nullptr;
	int r = WebmReadVideoFrame(name, &image, &_webmReadVideoFrame_Width, &_webmReadVideoFrame_Height);

	static ComPtr<ID3D11Device> _d3dDevice;
	static ComPtr<ID3D11DeviceContext> _d3dDeviceContext;

	if (image)
	{
		if (!_d3dDevice.Get())
		{
			D3D_FEATURE_LEVEL featureLevels[] =
			{
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0
			};

			UINT numFeatureLevels = ARRAYSIZE(featureLevels);

			D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &_d3dDevice, nullptr, &_d3dDeviceContext);
		}

		ID3D11Texture2D* texture = nullptr;
		unsigned int textureHandle = 0;
		//int key = _webmReadVideoFrame_Height * 32768 + _webmReadVideoFrame_Width;

		//auto it = _webmTexures.find(key);

		//if (it != _webmTexures.end())
		//{
		//	texture = std::get<0>(it->second);
		//	textureHandle = std::get<1>(it->second);

		//	_d3dDeviceContext->UpdateSubresource(texture, 0, nullptr, image, _webmReadVideoFrame_Width * 4, 0);
		//	_d3dDeviceContext->Flush();
		//}
		//else
		{
			D3D11_TEXTURE2D_DESC textureDesc{};
			textureDesc.Width = _webmReadVideoFrame_Width;
			textureDesc.Height = _webmReadVideoFrame_Height;
			textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = 1;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

			D3D11_SUBRESOURCE_DATA textureData{};
			textureData.pSysMem = image;
			textureData.SysMemPitch = _webmReadVideoFrame_Width * 4;
			textureData.SysMemSlicePitch = 0;

			_d3dDevice->CreateTexture2D(&textureDesc, &textureData, &texture);
			_d3dDeviceContext->Flush();

			//_webmTexures.insert(std::make_pair(key, std::make_tuple(texture, textureHandle)));
		}

		static ComPtr<ID3D11Texture2D> _lastTexture;
		_lastTexture.Release();
		*_lastTexture.GetAddressOf() = texture;

		ComPtr<IDXGIResource> resource;
		texture->QueryInterface(&resource);
		HANDLE handle = NULL;
		resource->GetSharedHandle(&handle);
		textureHandle = (unsigned int)handle;

		_webmReadVideoFrame_Image = textureHandle;
	}

	return r;
}

unsigned int ExeWebmReadVideoFrame_Image(DWORD cbData, PVOID lpData)
{
	return _webmReadVideoFrame_Image;
}

unsigned int ExeWebmReadVideoFrame_Width(DWORD cbData, PVOID lpData)
{
	return _webmReadVideoFrame_Width;
}

unsigned int ExeWebmReadVideoFrame_Height(DWORD cbData, PVOID lpData)
{
	return _webmReadVideoFrame_Height;
}

LRESULT HandleMessage(ULONG_PTR dwData, DWORD cbData, PVOID lpData)
{
	switch (dwData)
	{
	case ExeDataType_ShowMessage:
		ExeShowMessage(cbData, lpData);
		return 0;

	case ExeDataType_WebmLoadVideo:
		ExeWebmLoadVideo(cbData, lpData);
		return 0;

	case ExeDataType_WebmFreeVideo:
		ExeWebmFreeVideo(cbData, lpData);
		return 0;

	case ExeDataType_WebmFreeAllVideos:
		ExeWebmFreeAllVideos(cbData, lpData);
		return 0;

	case ExeDataType_WebmResetAllTimecode:
		ExeWebmResetAllTimecode(cbData, lpData);
		return 0;

	case ExeDataType_WebmGetTimecode_Low:
		return ExeWebmGetTimecode_Low(cbData, lpData);

	case ExeDataType_WebmGetTimecode_High:
		return ExeWebmGetTimecode_High(cbData, lpData);

	case ExeDataType_WebmGetDuration_Low:
		return ExeWebmGetDuration_Low(cbData, lpData);

	case ExeDataType_WebmGetDuration_High:
		return ExeWebmGetDuration_High(cbData, lpData);

	case ExeDataType_WebmReadVideoFrame_Result:
		return ExeWebmReadVideoFrame_Result(cbData, lpData);

	case ExeDataType_WebmReadVideoFrame_Image:
		return ExeWebmReadVideoFrame_Image(cbData, lpData);

	case ExeDataType_WebmReadVideoFrame_Width:
		return ExeWebmReadVideoFrame_Width(cbData, lpData);

	case ExeDataType_WebmReadVideoFrame_Height:
		return ExeWebmReadVideoFrame_Height(cbData, lpData);
	}

	return 0;
}
