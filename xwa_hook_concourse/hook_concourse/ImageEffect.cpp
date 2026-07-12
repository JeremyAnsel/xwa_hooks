#include "targetver.h"
//#define NOMINMAX
#include <Windows.h>
#include <d3d11.h>
#include <d2d1.h>
#include <comdef.h>
#include "ComPtr.h"

#include "SurfaceDC.h"
#include "ImageEffect.h"

#ifdef _DEBUG
#include "../Debug/ImageVertexShader.h"
#include "../Debug/ImagePixelShader.h"
#else
#include "../Release/ImageVertexShader.h"
#include "../Release/ImagePixelShader.h"
#endif

struct ImageEffectConstantBuffer
{
	float dstRect[4];
	float uvRect[4];
	float xformRow0[4];
	float xformRow1[4];
	float blendColor[4];
};

ImageEffect::ImageEffect()
{
	_isInitialized = false;
	_blendColor = 0;
	_clipRect = D2D1::RectF();
	_transform = D2D1::Matrix3x2F::Identity();
}

bool ImageEffect::IsInitialized()
{
	return _isInitialized;
}

HRESULT ImageEffect::Initialize(SurfaceDC* dc)
{
	if (IsInitialized())
	{
		return S_OK;
	}

	HRESULT hr = S_OK;

	if (FAILED(hr = dc->d3d11Device->CreateVertexShader(g_ImageVertexShader, sizeof(g_ImageVertexShader), nullptr, &_vertexShader)))
		return hr;

	if (FAILED(hr = dc->d3d11Device->CreatePixelShader(g_ImagePixelShader, sizeof(g_ImagePixelShader), nullptr, &_pixelShader)))
		return hr;

	D3D11_BUFFER_DESC constantBufferDesc{};
	constantBufferDesc.ByteWidth = sizeof(ImageEffectConstantBuffer);
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.MiscFlags = 0;
	constantBufferDesc.StructureByteStride = 0;

	if (FAILED(hr = dc->d3d11Device->CreateBuffer(&constantBufferDesc, nullptr, &_constantBuffer)))
		return hr;

	D3D11_SAMPLER_DESC sampDesc{};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	if (FAILED(hr = dc->d3d11Device->CreateSamplerState(&sampDesc, &_sampler)))
		return hr;

	D3D11_RASTERIZER_DESC rsDesc{};
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_NONE;
	rsDesc.FrontCounterClockwise = FALSE;
	rsDesc.DepthBias = 0;
	rsDesc.DepthBiasClamp = 0.0f;
	rsDesc.SlopeScaledDepthBias = 0.0f;
	//rsDesc.DepthClipEnable = TRUE;
	//rsDesc.ScissorEnable = TRUE;
	rsDesc.DepthClipEnable = FALSE;
	rsDesc.ScissorEnable = FALSE;
	rsDesc.MultisampleEnable = TRUE;
	rsDesc.AntialiasedLineEnable = FALSE;

	if (FAILED(hr = dc->d3d11Device->CreateRasterizerState(&rsDesc, &_rasterizerState)))
		return hr;

	D3D11_BLEND_DESC blendDesc{};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (FAILED(hr = dc->d3d11Device->CreateBlendState(&blendDesc, &_blendState)))
		return hr;

	D3D11_DEPTH_STENCIL_DESC depthDesc{};
	depthDesc.DepthEnable = FALSE;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	depthDesc.StencilEnable = FALSE;

	if (FAILED(hr = dc->d3d11Device->CreateDepthStencilState(&depthDesc, &_depthStencilState)))
		return hr;

	_isInitialized = true;

	return hr;
}

void ImageEffect::SetBlendColor(unsigned int color)
{
	_blendColor = color;
}

void ImageEffect::SetClipRect(const D2D1_RECT_F& rect)
{
	_clipRect = rect;
}

void ImageEffect::SetTransform(const D2D1_MATRIX_3X2_F& transform)
{
	_transform = transform;
}

HRESULT ImageEffect::DrawImage(SurfaceDC* dc, ID3D11Texture2D* bitmap, const D2D1_POINT_2F& targetOffset, const D2D1_RECT_F& imageRectangle)
{
	HRESULT hr = S_OK;

	D3D11_SHADER_RESOURCE_VIEW_DESC textureViewDesc{};
	textureViewDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	textureViewDesc.Texture2D.MipLevels = 1;
	textureViewDesc.Texture2D.MostDetailedMip = 0;

	ComPtr<ID3D11ShaderResourceView> textureView;
	//dc->d3d11Device->CreateShaderResourceView(bitmap, &textureViewDesc, &textureView);
	dc->d3d11Device->CreateShaderResourceView(bitmap, nullptr, &textureView);

	D2D1_SIZE_U targetPx = { dc->width, dc->height };
	if (targetPx.width == 0 || targetPx.height == 0) return hr;

	D3D11_TEXTURE2D_DESC bitmapDesc{};
	bitmap->GetDesc(&bitmapDesc);

	D2D1_RECT_F src = imageRectangle;
	D2D1_POINT_2F off = targetOffset;
	D2D1_MATRIX_3X2_F m = _transform;

	ImageEffectConstantBuffer constantBuffer{};
	constantBuffer.dstRect[0] = off.x;
	constantBuffer.dstRect[1] = off.y;
	constantBuffer.dstRect[2] = off.x + (src.right - src.left);
	constantBuffer.dstRect[3] = off.y + (src.bottom - src.top);
	constantBuffer.uvRect[0] = src.left / (FLOAT)bitmapDesc.Width;
	constantBuffer.uvRect[1] = src.top / (FLOAT)bitmapDesc.Height;
	constantBuffer.uvRect[2] = src.right / (FLOAT)bitmapDesc.Width;
	constantBuffer.uvRect[3] = src.bottom / (FLOAT)bitmapDesc.Height;
	constantBuffer.xformRow0[0] = m._11; constantBuffer.xformRow0[1] = m._12;
	constantBuffer.xformRow0[2] = m._21; constantBuffer.xformRow0[3] = m._22;
	constantBuffer.xformRow1[0] = m._31; constantBuffer.xformRow1[1] = m._32;
	constantBuffer.xformRow1[2] = (FLOAT)targetPx.width;
	constantBuffer.xformRow1[3] = (FLOAT)targetPx.height;
	// BitmapEffect::SetBlendColor packing: LSB-first bytes / 255
	constantBuffer.blendColor[0] = (float)((_blendColor) & 0xFF) / 255.0f;
	constantBuffer.blendColor[1] = (float)((_blendColor >> 8) & 0xFF) / 255.0f;
	constantBuffer.blendColor[2] = (float)((_blendColor >> 16) & 0xFF) / 255.0f;
	constantBuffer.blendColor[3] = (float)((_blendColor >> 24) & 0xFF) / 255.0f;

	dc->d3d11DeviceContext->UpdateSubresource(_constantBuffer, 0, nullptr, &constantBuffer, 0, 0);

	// scissor = intersection of the tracked clip stack (transformed AABBs)
	LONG sx0 = 0, sy0 = 0, sx1 = (LONG)targetPx.width, sy1 = (LONG)targetPx.height;
	{
		const D2D1_RECT_F& r = _clipRect;
		const D2D1_MATRIX_3X2_F& cm = _transform;
		D2D1_POINT_2F pts[4] = { { r.left, r.top }, { r.right, r.top }, { r.left, r.bottom }, { r.right, r.bottom } };
		FLOAT minX = 1e9f, minY = 1e9f, maxX = -1e9f, maxY = -1e9f;
		for (int k = 0; k < 4; k++)
		{
			FLOAT tx = pts[k].x * cm._11 + pts[k].y * cm._21 + cm._31;
			FLOAT ty = pts[k].x * cm._12 + pts[k].y * cm._22 + cm._32;
			if (tx < minX) minX = tx; if (tx > maxX) maxX = tx;
			if (ty < minY) minY = ty; if (ty > maxY) maxY = ty;
		}
		if ((LONG)minX > sx0) sx0 = (LONG)minX;
		if ((LONG)minY > sy0) sy0 = (LONG)minY;
		if ((LONG)maxX < sx1) sx1 = (LONG)maxX;
		if ((LONG)maxY < sy1) sy1 = (LONG)maxY;
	}
	if (sx0 >= sx1 || sy0 >= sy1) return hr; // fully clipped

	dc->d2d1RenderTarget->Flush(nullptr, nullptr);

	ID3D11RenderTargetView* oldRenderTargetView;
	ID3D11DepthStencilView* ol1DepthStencilView;
	ID3D11VertexShader* oldVertexShader;
	ID3D11PixelShader* oldPixelShader;
	ID3D11GeometryShader* oldGeometryShader;
	ID3D11InputLayout* oldInputLayout;
	D3D11_PRIMITIVE_TOPOLOGY oldPrimitiveTopology;
	ID3D11ShaderResourceView* oldShaderResourceView;
	ID3D11Buffer* oldVSConstantBuffer;
	ID3D11Buffer* oldPSConstantBuffer;
	ID3D11SamplerState* oldPSSamplerState;
	ID3D11RasterizerState* oldRasterizerState;
	ID3D11BlendState* oldBlendState;
	FLOAT oldBlendFactor[4];
	UINT oldSampleMask;
	ID3D11DepthStencilState* oldDepthStateState;
	UINT oldStencilReference;
	UINT oldViewportCount = 1;
	D3D11_VIEWPORT oldViewport;
	D3D11_RECT oldScissorRects;
	UINT oldScissorRectsCount = 1;

	dc->d3d11DeviceContext->OMGetRenderTargets(1, &oldRenderTargetView, &ol1DepthStencilView);
	dc->d3d11DeviceContext->VSGetShader(&oldVertexShader, nullptr, nullptr);
	dc->d3d11DeviceContext->PSGetShader(&oldPixelShader, nullptr, nullptr);
	dc->d3d11DeviceContext->GSGetShader(&oldGeometryShader, nullptr, nullptr);
	dc->d3d11DeviceContext->IAGetInputLayout(&oldInputLayout);
	dc->d3d11DeviceContext->IAGetPrimitiveTopology(&oldPrimitiveTopology);
	dc->d3d11DeviceContext->PSGetShaderResources(0, 1, &oldShaderResourceView);
	dc->d3d11DeviceContext->VSGetConstantBuffers(0, 1, &oldVSConstantBuffer);
	dc->d3d11DeviceContext->PSGetConstantBuffers(0, 1, &oldPSConstantBuffer);
	dc->d3d11DeviceContext->PSGetSamplers(0, 1, &oldPSSamplerState);
	dc->d3d11DeviceContext->RSGetState(&oldRasterizerState);
	dc->d3d11DeviceContext->OMGetBlendState(&oldBlendState, oldBlendFactor, &oldSampleMask);
	dc->d3d11DeviceContext->OMGetDepthStencilState(&oldDepthStateState, &oldStencilReference);
	dc->d3d11DeviceContext->RSGetViewports(&oldViewportCount, &oldViewport);
	dc->d3d11DeviceContext->RSGetScissorRects(&oldScissorRectsCount, &oldScissorRects);

	dc->d3d11DeviceContext->OMSetRenderTargets(1, &dc->d3d11RenderTargetView, nullptr);
	dc->d3d11DeviceContext->VSSetShader(_vertexShader, nullptr, 0);
	dc->d3d11DeviceContext->PSSetShader(_pixelShader, nullptr, 0);
	dc->d3d11DeviceContext->GSSetShader(nullptr, nullptr, 0);
	dc->d3d11DeviceContext->IASetInputLayout(nullptr);
	dc->d3d11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	dc->d3d11DeviceContext->PSSetShaderResources(0, 1, textureView.GetAddressOf());
	dc->d3d11DeviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
	dc->d3d11DeviceContext->PSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
	dc->d3d11DeviceContext->PSSetSamplers(0, 1, _sampler.GetAddressOf());
	dc->d3d11DeviceContext->RSSetState(_rasterizerState);

	D3D11_VIEWPORT vp = { 0.0f, 0.0f, (FLOAT)targetPx.width, (FLOAT)targetPx.height, 0.0f, 1.0f };
	dc->d3d11DeviceContext->RSSetViewports(1, &vp);
	D3D11_RECT sc = { sx0, sy0, sx1, sy1 };
	dc->d3d11DeviceContext->RSSetScissorRects(1, &sc);
	FLOAT bf[4] = { 1, 1, 1, 1 };
	dc->d3d11DeviceContext->OMSetBlendState(_blendState, bf, 0xFFFFFFFF);
	dc->d3d11DeviceContext->OMSetDepthStencilState(_depthStencilState, 0);

	dc->d3d11DeviceContext->Draw(4, 0);

	dc->d3d11DeviceContext->OMSetRenderTargets(1, &oldRenderTargetView, ol1DepthStencilView);
	dc->d3d11DeviceContext->VSSetShader(oldVertexShader, nullptr, 0);
	dc->d3d11DeviceContext->PSSetShader(oldPixelShader, nullptr, 0);
	dc->d3d11DeviceContext->GSSetShader(oldGeometryShader, nullptr, 0);
	dc->d3d11DeviceContext->IASetInputLayout(oldInputLayout);
	dc->d3d11DeviceContext->IASetPrimitiveTopology(oldPrimitiveTopology);
	dc->d3d11DeviceContext->PSSetShaderResources(0, 1, &oldShaderResourceView);
	dc->d3d11DeviceContext->VSSetConstantBuffers(0, 1, &oldVSConstantBuffer);
	dc->d3d11DeviceContext->PSSetConstantBuffers(0, 1, &oldPSConstantBuffer);
	dc->d3d11DeviceContext->PSSetSamplers(0, 1, &oldPSSamplerState);
	dc->d3d11DeviceContext->RSSetState(oldRasterizerState);
	dc->d3d11DeviceContext->OMSetBlendState(oldBlendState, oldBlendFactor, oldSampleMask);
	dc->d3d11DeviceContext->OMSetDepthStencilState(oldDepthStateState, oldStencilReference);
	dc->d3d11DeviceContext->RSSetViewports(1, &oldViewport);
	dc->d3d11DeviceContext->RSSetScissorRects(oldScissorRectsCount, &oldScissorRects);

	return hr;
}
