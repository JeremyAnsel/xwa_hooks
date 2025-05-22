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

PixelShaderInput main(uint id : SV_VertexID)
{
    PixelShaderInput output;
    output.tex = float2(((id << 1) & 2) * 0.5f, (id & 2) * 0.5f);
    output.pos = float4(((id << 1) & 2) - 1.0f, (id & 2) - 1.0f, 0.5f, 1.0f);

    output.pos.x = (output.pos.x + 1.0f) * sizeX - 1.0f + positionX * 2.0f;
    output.pos.y = (output.pos.y + 1.0f) * -sizeY + 1.0f - positionY * 2.0f;

    return output;
}
