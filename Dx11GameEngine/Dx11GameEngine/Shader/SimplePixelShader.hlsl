struct PS_INPUT
{
    float2 uv : UV;

};

Texture2D shaderTexture;
SamplerState SampleType;

float4 main(PS_INPUT input) : SV_TARGET
{
    
    float4 col =  shaderTexture.Sample(SampleType, input.uv);
    return col;
}