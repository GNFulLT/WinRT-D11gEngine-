cbuffer cbPerObject
{
	float4x4 WVP;
};

struct VSInput
{
	float3 pos : POSITION;
	float4 textPos:TEXCOORD;

};
struct VSOutput
{
	float4 pos:SV_POSITION;
	float4 textPos :TEXCOORD;

};
VSOutput main(VSInput input)
{
	VSOutput op;
	op.pos = mul(float4(input.pos,1.0f),WVP);
	op.textPos = input.textPos;
	return op;
}