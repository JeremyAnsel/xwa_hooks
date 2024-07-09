#pragma once

DEFINE_GUID(GUID_BitmapPixelShader, 0x6716FB29, 0x06A0, 0x460F, 0xAA, 0x9C, 0x8B, 0x8F, 0xF0, 0x33, 0x5E, 0xA6);
DEFINE_GUID(CLSID_BitmapEffect, 0xB7B36C92, 0x3498, 0x4A94, 0x9E, 0x95, 0x9F, 0x24, 0x6F, 0x92, 0x45, 0xBF);

// Our effect contains one transform, which is simply a wrapper around a pixel shader. As such,
// we can simply make the effect itself act as the transform.
class BitmapEffect : public ID2D1EffectImpl, public ID2D1DrawTransform
{
public:
	// Declare effect registration methods.
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);

	static HRESULT __stdcall CreateBitmapEffectImpl(_Outptr_ IUnknown** ppEffectImpl);

	HRESULT SetBlendColor(UINT32 color);
	UINT32 GetBlendColor() const;

	// Declare ID2D1EffectImpl implementation methods.
	IFACEMETHODIMP Initialize(
		_In_ ID2D1EffectContext* pContextInternal,
		_In_ ID2D1TransformGraph* pTransformGraph
	);

	IFACEMETHODIMP PrepareForRender(D2D1_CHANGE_TYPE changeType);

	IFACEMETHODIMP SetGraph(_In_ ID2D1TransformGraph* pGraph);

	// Declare ID2D1DrawTransform implementation methods.
	IFACEMETHODIMP SetDrawInfo(_In_ ID2D1DrawInfo* pRenderInfo);

	// Declare ID2D1Transform implementation methods.
	IFACEMETHODIMP MapOutputRectToInputRects(
		_In_ const D2D1_RECT_L* pOutputRect,
		_Out_writes_(inputRectCount) D2D1_RECT_L* pInputRects,
		UINT32 inputRectCount
	) const;

	IFACEMETHODIMP MapInputRectsToOutputRect(
		_In_reads_(inputRectCount) CONST D2D1_RECT_L* pInputRects,
		_In_reads_(inputRectCount) CONST D2D1_RECT_L* pInputOpaqueSubRects,
		UINT32 inputRectCount,
		_Out_ D2D1_RECT_L* pOutputRect,
		_Out_ D2D1_RECT_L* pOutputOpaqueSubRect
	);

	IFACEMETHODIMP MapInvalidRect(
		UINT32 inputIndex,
		D2D1_RECT_L invalidInputRect,
		_Out_ D2D1_RECT_L* pInvalidOutputRect
	) const;

	// Declare ID2D1TransformNode implementation methods.
	IFACEMETHODIMP_(UINT32) GetInputCount() const;

	// Declare IUnknown implementation methods.
	IFACEMETHODIMP_(ULONG) AddRef();
	IFACEMETHODIMP_(ULONG) Release();
	IFACEMETHODIMP QueryInterface(_In_ REFIID riid, _Outptr_ void** ppOutput);

private:
	BitmapEffect();
	HRESULT UpdateConstants();

	// This struct defines the constant buffer of our pixel shader.
	struct
	{
		float blendColor[4];
	} m_constants;

	ComPtr<ID2D1DrawInfo> m_drawInfo;
	LONG m_refCount;
	D2D1_RECT_L m_inputRect;
};
