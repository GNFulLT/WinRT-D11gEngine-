struct VS_OUTPUT
{
    float4 pos2 : POS;
    float4 col : COLOR;
    float isSecond : SecondCheck;
    float4 pos : SV_Position;
    
};

struct VS_INPUT
{
    float3 in_pos : POSITION;
    float3 in_pos2 : POSITIONEND;
};

cbuffer cbPerObject
{
    float4x4 matrx;
};


float windowHeight = 480;
float windowWidth = 640;

float2 ViewportToScreen(float2 position)
{
    float2 screenPosition;
    screenPosition.x = (1.0 - position.x) * 0.5 * windowWidth;
    screenPosition.y = (position.y + 1.0) * 0.5 * windowHeight;
    return screenPosition;
}

float3 slerp(float3 start, float3 end, float percent)
{
     // Dot product - the cosine of the angle between 2 vectors.
    float slerpDot = dot(start, end);
     // Clamp it to be in the range of Acos()
     // This may be unnecessary, but floating point
     // precision can be a fickle mistress.
    slerpDot = clamp(slerpDot, -1.0, 1.0);
     // Acos(dot) returns the angle between start and end,
     // And multiplying that by percent returns the angle between
     // start and the final result.
    float theta = acos(slerpDot) * percent;
    float3 RelativeVec = normalize(end - start * slerpDot); // Orthonormal basis
     // The final result.
    return ((start * cos(theta)) + (RelativeVec * sin(theta)));
}

float3 Slerp(float3 start, float3 end, float t)
{
    float dotProduct = dot(start, end);
    dotProduct = min(dotProduct, 1.0f);
    dotProduct = max(dotProduct, -1.0f);
    float theta = acos(dotProduct) * t;
    float3 relative = end - dotProduct * start;
    relative = normalize(relative);
    return start * cos(theta) + relative * sin(theta);
}

VS_OUTPUT main(VS_INPUT in_pos)
{
    VS_OUTPUT output;
    
    
    
    output.pos = mul(float4(in_pos.in_pos, 1.0f), matrx);
    output.pos2 = float4(in_pos.in_pos, 1.0f);
    output.col = float4(0.f, 0.f, 0.f, 1.f);
    output.isSecond = 0.0f;
  
    if (in_pos.in_pos2.x == -1.f && in_pos.in_pos2.y == -1.f && in_pos.in_pos2.z == -1.f)
    {
        return output;
    }
    else
    {
        float3 lerped1;
        float3 lerped2;
        
        output.pos = float4(slerp(in_pos.in_pos, in_pos.in_pos2, (sin(10000 / 1000.f) + 1) / 2), 1.0f);
        output.pos.y = in_pos.in_pos.y;
        output.pos2 = output.pos;
        output.isSecond = 1.f;
        return output;
    }
   
};