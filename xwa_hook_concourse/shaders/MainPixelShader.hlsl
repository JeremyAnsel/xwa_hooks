
Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer ConstantBuffer : register(b0)
{
    float positionX;
    float positionY;
    float sizeX;
    float sizeY;
};

struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
    float4 texelColor = texture0.Sample(sampler0, input.tex);
    return texelColor;

    //if (texelColor.a < 0.1f)
    //{
    //    discard;
    //}

    //return float4(texelColor.xyz, 1.0f);
}
