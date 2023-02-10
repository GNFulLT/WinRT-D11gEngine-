struct PS_INPUT
{
    float3 dir : DIRECTION;
};

TextureCube SkyMap;
SamplerState ObjSamplerState;

float4 main(PS_INPUT input) : SV_Target
{
    return SkyMap.Sample(ObjSamplerState, input.dir);

}