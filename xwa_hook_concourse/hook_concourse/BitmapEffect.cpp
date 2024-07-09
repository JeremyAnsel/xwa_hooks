#include "targetver.h"
#include <initguid.h>

//#define NOMINMAX
#include <Windows.h>
#include <ddraw.h>
#include <d3d.h>
#include <dxgi.h>
#include <d2d1.h>
#include <d2d1effects.h>
#include <d2d1effectauthor.h>
#include <d2d1effecthelpers.h>
#include <dwrite.h>
#include <gdiplus.h>
#include <comdef.h>
#include "ComPtr.h"

#include "BitmapEffect.h"

#include <memory>

#ifdef _DEBUG
#include "../Debug/BitmapPixelShader.h"
#else
#include "../Release/BitmapPixelShader.h"
#endif

BitmapEffect::BitmapEffect() :
	m_refCount(1)
{
	m_constants.blendColor[0] = 0;
	m_constants.blendColor[1] = 0;
	m_constants.blendColor[2] = 0;
	m_constants.blendColor[3] = 0;

	m_inputRect = {};
}

HRESULT __stdcall BitmapEffect::CreateBitmapEffectImpl(_Outptr_ IUnknown** ppEffectImpl)
{
	// Since the object's refcount is initialized to 1, we don't need to AddRef here.
	*ppEffectImpl = static_cast<ID2D1EffectImpl*>(new (std::nothrow) BitmapEffect());

	if (*ppEffectImpl == nullptr)
	{
		return E_OUTOFMEMORY;
	}
	else
	{
		return S_OK;
	}
}

HRESULT BitmapEffect::Register(_In_ ID2D1Factory1* pFactory)
{
	// The inspectable metadata of an effect is defined in XML. This can be passed in from an external source
	// as well, however for simplicity we just inline the XML.
	PCWSTR pszXml =
		LR""""(<?xml version='1.0'?>
            <Effect>
                <!-- System Properties -->
                <Property name='DisplayName' type='string' value='BitmapEffect'/>
                <Property name='Author' type='string' value='JeremyAnsel'/>
                <Property name='Category' type='string' value='Render'/>
                <Property name='Description' type='string' value='Bitmap effect'/>
                <Inputs>
                    <Input name='Source'/>
                </Inputs>
                <!-- Custom Properties go here -->
                <Property name='BlendColor' type='uint32'>
                    <Property name='DisplayName' type='string' value='BlendColor'/>
                    <Property name='Default' type='uint32' value='0' />
                </Property>
            </Effect>
            )"""";

	// This defines the bindings from specific properties to the callback functions
	// on the class that ID2D1Effect::SetValue() & GetValue() will call.
	const D2D1_PROPERTY_BINDING bindings[] =
	{
		D2D1_VALUE_TYPE_BINDING(L"BlendColor", &SetBlendColor, &GetBlendColor),
	};

	// This registers the effect with the factory, which will make the effect
	// instantiatable.
	return pFactory->RegisterEffectFromString(
		CLSID_BitmapEffect,
		pszXml,
		bindings,
		ARRAYSIZE(bindings),
		CreateBitmapEffectImpl
	);
}

IFACEMETHODIMP BitmapEffect::Initialize(
	_In_ ID2D1EffectContext* pEffectContext,
	_In_ ID2D1TransformGraph* pTransformGraph
)
{
	HRESULT hr = pEffectContext->LoadPixelShader(GUID_BitmapPixelShader, g_BitmapPixelShader, sizeof(g_BitmapPixelShader));

	// This loads the shader into the Direct2D image effects system and associates it with the GUID passed in.
	// If this method is called more than once (say by other instances of the effect) with the same GUID,
	// the system will simply do nothing, ensuring that only one instance of a shader is stored regardless of how
	// many time it is used.
	if (SUCCEEDED(hr))
	{
		// The graph consists of a single transform. In fact, this class is the transform,
		// reducing the complexity of implementing an effect when all we need to
		// do is use a single pixel shader.
		hr = pTransformGraph->SetSingleTransformNode(this);
	}

	return hr;
}

HRESULT BitmapEffect::SetBlendColor(UINT32 color)
{
	m_constants.blendColor[0] = (float)((color) & 0xFF) / 255.0f;
	m_constants.blendColor[1] = (float)((color >> 8) & 0xFF) / 255.0f;
	m_constants.blendColor[2] = (float)((color >> 16) & 0xFF) / 255.0f;
	m_constants.blendColor[3] = (float)((color >> 24) & 0xFF) / 255.0f;
	return S_OK;
}

UINT32 BitmapEffect::GetBlendColor() const
{
	UINT32 color = 0;
	color |= ((UINT32)(m_constants.blendColor[0] * 255.0f) & 0xFF);
	color |= ((UINT32)(m_constants.blendColor[1] * 255.0f) & 0xFF) << 8;
	color |= ((UINT32)(m_constants.blendColor[2] * 255.0f) & 0xFF) << 16;
	color |= ((UINT32)(m_constants.blendColor[3] * 255.0f) & 0xFF) << 24;
	return color;
}

HRESULT BitmapEffect::UpdateConstants()
{
	return m_drawInfo->SetPixelShaderConstantBuffer(reinterpret_cast<BYTE*>(&m_constants), sizeof(m_constants));
}

IFACEMETHODIMP BitmapEffect::PrepareForRender(D2D1_CHANGE_TYPE changeType)
{
	return UpdateConstants();
}

// SetGraph is only called when the number of inputs changes. This never happens as we publish this effect
// as a single input effect.
IFACEMETHODIMP BitmapEffect::SetGraph(_In_ ID2D1TransformGraph* pGraph)
{
	return E_NOTIMPL;
}

// Called to assign a new render info class, which is used to inform D2D on
// how to set the state of the GPU.
IFACEMETHODIMP BitmapEffect::SetDrawInfo(_In_ ID2D1DrawInfo* pDrawInfo)
{
	*(&m_drawInfo) = pDrawInfo;

	return m_drawInfo->SetPixelShader(GUID_BitmapPixelShader);
}

// Calculates the mapping between the output and input rects. In this case,
// we want to request an expanded region to account for pixels that the bitmap effect
// may need outside of the bounds of the destination.
IFACEMETHODIMP BitmapEffect::MapOutputRectToInputRects(
	_In_ const D2D1_RECT_L* pOutputRect,
	_Out_writes_(inputRectCount) D2D1_RECT_L* pInputRects,
	UINT32 inputRectCount
) const
{
	// This effect has exactly one input, so if there is more than one input rect,
	// something is wrong.
	if (inputRectCount != 1)
	{
		return E_INVALIDARG;
	}

	pInputRects[0].left = pOutputRect->left;
	pInputRects[0].top = pOutputRect->top;
	pInputRects[0].right = pOutputRect->right;
	pInputRects[0].bottom = pOutputRect->bottom;

	return S_OK;
}

IFACEMETHODIMP BitmapEffect::MapInputRectsToOutputRect(
	_In_reads_(inputRectCount) CONST D2D1_RECT_L* pInputRects,
	_In_reads_(inputRectCount) CONST D2D1_RECT_L* pInputOpaqueSubRects,
	UINT32 inputRectCount,
	_Out_ D2D1_RECT_L* pOutputRect,
	_Out_ D2D1_RECT_L* pOutputOpaqueSubRect
)
{
	// This effect has exactly one input, so if there is more than one input rect,
	// something is wrong.
	if (inputRectCount != 1)
	{
		return E_INVALIDARG;
	}

	*pOutputRect = pInputRects[0];
	m_inputRect = pInputRects[0];

	// Indicate that entire output might contain transparency.
	ZeroMemory(pOutputOpaqueSubRect, sizeof(*pOutputOpaqueSubRect));

	return S_OK;
}

IFACEMETHODIMP BitmapEffect::MapInvalidRect(
	UINT32 inputIndex,
	D2D1_RECT_L invalidInputRect,
	_Out_ D2D1_RECT_L* pInvalidOutputRect
) const
{
	HRESULT hr = S_OK;

	// Indicate that the entire output may be invalid.
	*pInvalidOutputRect = m_inputRect;

	return hr;
}

IFACEMETHODIMP_(UINT32) BitmapEffect::GetInputCount() const
{
	return 1;
}

// D2D ensures that that effects are only referenced from one thread at a time.
// To improve performance, we simply increment/decrement our reference count
// rather than use atomic InterlockedIncrement()/InterlockedDecrement() functions.
IFACEMETHODIMP_(ULONG) BitmapEffect::AddRef()
{
	m_refCount++;
	return m_refCount;
}

IFACEMETHODIMP_(ULONG) BitmapEffect::Release()
{
	m_refCount--;

	if (m_refCount == 0)
	{
		delete this;
		return 0;
	}
	else
	{
		return m_refCount;
	}
}

// This enables the stack of parent interfaces to be queried. In the instance
// of the BitmapEffect interface, this method simply enables the developer
// to cast a BitmapEffect instance to an ID2D1EffectImpl or IUnknown instance.
IFACEMETHODIMP BitmapEffect::QueryInterface(
	_In_ REFIID riid,
	_Outptr_ void** ppOutput
)
{
	*ppOutput = nullptr;
	HRESULT hr = S_OK;

	if (riid == __uuidof(ID2D1EffectImpl))
	{
		*ppOutput = reinterpret_cast<ID2D1EffectImpl*>(this);
	}
	else if (riid == __uuidof(ID2D1DrawTransform))
	{
		*ppOutput = static_cast<ID2D1DrawTransform*>(this);
	}
	else if (riid == __uuidof(ID2D1Transform))
	{
		*ppOutput = static_cast<ID2D1Transform*>(this);
	}
	else if (riid == __uuidof(ID2D1TransformNode))
	{
		*ppOutput = static_cast<ID2D1TransformNode*>(this);
	}
	else if (riid == __uuidof(IUnknown))
	{
		*ppOutput = this;
	}
	else
	{
		hr = E_NOINTERFACE;
	}

	if (*ppOutput != nullptr)
	{
		AddRef();
	}

	return hr;
}
