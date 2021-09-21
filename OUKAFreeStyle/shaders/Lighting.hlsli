
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
    float Intensity;

    float3 DirectionWS;
};

struct PointLight
{
    float3 Color;
    float Intensity;

    float3 PosWS;
    float Radius;
};

DirectionLight CookDirectionLight(DirectionLightRaw rawData)
{
    DirectionLight result;
    
    result.Color = rawData.param1.xyz;
    result.Intensity = rawData.param1.w;
    result.DirectionWS = normalize(rawData.param2.xyz);
    
    return result;
}

PointLight CookPointLight(PointLightRaw rawData)
{
    PointLight result;
    
    result.Color = rawData.param1.xyz;
    result.Intensity = rawData.param1.w;
    result.PosWS = rawData.param2.xyz;
    result.Radius = rawData.param2.w;
    
    return result;
}