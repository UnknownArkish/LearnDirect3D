
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
    float3 Color;

    float3 DirectionWS;
};

struct PointLight
{
    float3 Color;

    float3 posWS;
    float radius;
};

DirectionLight CookDirectionLight(DirectionLightRaw rawData)
{
    DirectionLight result;
    
    result.Color = rawData.param1.xyz;
    result.DirectionWS = normalize(rawData.param2.xyz);
    
    return result;
}

PointLight CookPointLight(PointLightRaw rawData)
{
    PointLight result;
    
    result.Color = rawData.param1.xyz;
    result.posWS = rawData.param2.xyz;
    result.radius = rawData.param2.w;
    
    return result;
}