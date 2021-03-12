
struct DirectionLightRaw
{
    float4 param1;
    float4 param2;
};

struct PointLightRaw
{
    float4 param1;
    float4 param2;
};

struct DirectionLight
{
    float3 color;

    float3 directionWS;
};

struct PointLight
{
    float3 color;

    float3 posWS;
    float radius;
};

DirectionLight CookDirectionLight(DirectionLightRaw rawData)
{
    DirectionLight result;
    
    result.color = rawData.param1.xyz;
    result.directionWS = rawData.param2.xyz;
    
    return result;
}

PointLight CookPointLight(PointLightRaw rawData)
{
    PointLight result;
    
    result.color = rawData.param1.xyz;
    result.posWS = rawData.param2.xyz;
    result.radius = rawData.param2.w;
    
    return result;
}