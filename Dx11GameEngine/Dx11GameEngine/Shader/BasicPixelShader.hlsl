struct PS_INPUT
{
    float4 pos2 : POS;
    float4 col : COLOR;
    float isSecond : SecondCheck;
};

float2 GetIntersectionPoint(float2 line1End,float2 line1Start,float2 line2End,float2 line2Start)
{
    float2 line1 = line1End - line1Start;
    float2 line2 = line2End - line2Start;

    float determinant = line1.x * line2.y - line1.y * line2.x;
    float2 offset = line2Start - line1Start;

    float u = (offset.x * line2.y - offset.y * line2.x) / determinant;
    float v = (offset.x * line1.y - offset.y * line1.x) / determinant;

    float2 intersectionPoint = line1Start + u * line1;

    return intersectionPoint;
}

bool IsPointInTriangle(float2 A,float2 B,float2 C,float2 Point)
{
    // Calculate the barycentric coordinates
    float2 v0 = C - A;
    float2 v1 = B - A;
    float2 v2 = Point - A;
    float d00 = dot(v0, v0);
    float d01 = dot(v0, v1);
    float d11 = dot(v1, v1);
    float d20 = dot(v2, v0);
    float d21 = dot(v2, v1);
    float denom = d00 * d11 - d01 * d01;
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0f - v - w;
    // Check if the point is inside the triangle
    if (u >= 0 && v >= 0 && w >= 0)
        return true;
    else
        return false;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    float2 solalt;
    solalt.x = -0.5f;
    solalt.y = -0.5f;
    
    float2 sagalt;
    sagalt.x = 0.5f;
    sagalt.y = -0.5f;
    
    float2 tepe;
    tepe.x = 0.f;
    tepe.y = 0.5f;
    
    
    float2 ensol;
    ensol.x = -0.8f;
    ensol.y = 0.f;
    
    float2 ensag;
    ensag.x = +0.8f;
    ensag.y = 0.f;

    float2 kes = GetIntersectionPoint(solalt,tepe,sagalt,ensol);
    float2 kes2 = GetIntersectionPoint(sagalt, tepe, solalt, ensag);


    if (input.isSecond == 1.f)
    {
        return float4(0.5f,0.5f,0.5f,1.f);
    }
    
    return float4(1.0f, 1.0f, 1.0f, 1.0f);        
    

}