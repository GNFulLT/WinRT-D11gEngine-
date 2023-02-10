struct VS_OUTPUT
{
    float3 dir : DIRECTION;
    float4 pos : SV_Position;
};

cbuffer cbPerObject : register(b0)
{
    float4x4 mvp;
};

VS_OUTPUT main(float3 pos:POSITION)
{
    VS_OUTPUT outV;
    outV.pos = mul(float4(pos, 1.0), mvp).xyww;
    outV.dir = pos;
    return outV;
}