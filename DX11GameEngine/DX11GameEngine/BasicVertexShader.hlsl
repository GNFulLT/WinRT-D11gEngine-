float4 main( float3 pos : POSITION) : SV_POSITION
{
	/*VSOutput op;
	op.pos = float4(pos, 1.0f);
	op.color = color;*/
	return float4(pos,1.0f);
}