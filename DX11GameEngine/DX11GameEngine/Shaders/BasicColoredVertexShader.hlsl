
struct VSOutput
{
	float4 color:COLOR;
	float4 pos : SV_POSITION;

};

VSOutput main(float3 pos : POSITION, float4 color : COLOR)
{
	VSOutput op;
	op.pos = float4(pos, 1.0f);
	//op.pos = float4(pos, 1.0f);
	op.color = color;
	return op;
}