#pragma once

class ImageEffect
{
public:
	ImageEffect();
	bool IsInitialized();
	HRESULT Initialize(SurfaceDC* dc);
	void SetBlendColor(unsigned int color);
	void SetClipRect(const D2D1_RECT_F& rect);
	void SetTransform(const D2D1_MATRIX_3X2_F& transform);
	HRESULT DrawImage(SurfaceDC* dc, ID3D11Texture2D* bitmap, const D2D1_POINT_2F& targetOffset, const D2D1_RECT_F& imageRectangle);

private:
	bool _isInitialized;
	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3D11PixelShader> _pixelShader;
	ComPtr<ID3D11Buffer> _constantBuffer;
	ComPtr<ID3D11SamplerState> _sampler;
	ComPtr<ID3D11RasterizerState> _rasterizerState;
	ComPtr<ID3D11BlendState> _blendState;
	ComPtr<ID3D11DepthStencilState> _depthStencilState;
	unsigned int _blendColor;
	D2D1_RECT_F _clipRect;
	D2D1_MATRIX_3X2_F _transform;
};
