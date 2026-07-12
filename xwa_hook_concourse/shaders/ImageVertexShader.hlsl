
cbuffer ConstantBuffer : register(b0)
{
    float4 dstRect; // x0,y0,x1,y1 destination in target pixels (pre-transform)
    float4 uvRect; // u0,v0,u1,v1
    float4 xformRow0; // D2D matrix m11, m12, m21, m22
    float4 xformRow1; // D2D matrix dx, dy | target width, height (px)
    float4 blendColor; // used by the pixel shader
};

struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
};

PixelShaderInput main(uint id : SV_VertexID)
{
    float2 c = float2((float) (id & 1), (float) (id >> 1)); // (0,0)(1,0)(0,1)(1,1) tristrip
    float2 p = float2(lerp(dstRect.x, dstRect.z, c.x), lerp(dstRect.y, dstRect.w, c.y));

    // D2D 3x2: x' = x*m11 + y*m21 + dx ; y' = x*m12 + y*m22 + dy
    float2 t = float2(
        p.x * xformRow0.x + p.y * xformRow0.z + xformRow1.x,
        p.x * xformRow0.y + p.y * xformRow0.w + xformRow1.y);

    PixelShaderInput output;
    output.pos = float4(t.x / xformRow1.z * 2.0f - 1.0f, 1.0f - t.y / xformRow1.w * 2.0f, 0.5f, 1.0f);
    output.tex = float2(lerp(uvRect.x, uvRect.z, c.x), lerp(uvRect.y, uvRect.w, c.y));

    return output;
}
