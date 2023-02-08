struct VS_OUTPUT
{
    float2 uv : UV;
    float4 pos : SV_POSITION;

};
cbuffer cbPerObject
{
    float4x4 mvp;
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float2 uv : UV;
};

VS_OUTPUT main(VS_INPUT in_pos)
{
    VS_OUTPUT outp;
    outp.pos = mul(float4(in_pos.pos, 1.0f),mvp);
    outp.uv = in_pos.uv;
    return outp;
}