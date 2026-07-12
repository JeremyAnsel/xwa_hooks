
cbuffer ConstantBuffer : register(b0)
{
    float4 dstRect; // x0,y0,x1,y1 destination in target pixels (pre-transform)
    float4 uvRect; // u0,v0,u1,v1
    float4 xformRow0; // D2D matrix m11, m12, m21, m22
    float4 xformRow1; // D2D matrix dx, dy | target width, height (px)
    float4 blendColor; // used by the pixel shader
};

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
    float4 color = texture0.Sample(sampler0, input.tex);

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

    float4 finalColor = float4(r * a, g * a, b * a, a);
    return finalColor;
}
