
#define D2D_INPUT_COUNT 1
#define D2D_INPUT0_SIMPLE

#include "d2d1effecthelpers.hlsli"

cbuffer constants : register(b0)
{
    float4 blendColor : COLOR;
};

D2D_PS_ENTRY(main)
{
    float4 color = D2DGetInput(0);
    
    float blendB = blendColor.r;
    float blendG = blendColor.g;
    float blendR = blendColor.b;
    bool hasBlendColor = blendB != 0 || blendG != 0 || blendR != 0;
    bool isBlackTransparent = blendColor.a >= 0.5f;
    
    float b = color.b;
    float g = color.g;
    float r = color.r;
    float a = color.a;

    if (isBlackTransparent)
    {
        if (b == 0 && g == 0 && r == 0)
        {
            a = 0;
        }
    }

    if (a != 0 && hasBlendColor)
    {
        float s = max(b, max(g, r));

        b = s * blendB;
        g = s * blendG;
        r = s * blendR;
    }
    
    if (a == 0)
    {
        b = 0;
        g = 0;
        r = 0;
        a = 0;
    }

    color = float4(r * a, g * a, b * a, a);
    
    return color;
}
