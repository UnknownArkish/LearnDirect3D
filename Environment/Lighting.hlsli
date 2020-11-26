
struct DirectionalLightRaw
{
    float4 param1;
};

struct PointLightRaw
{
    float4 param1;
};

struct DirectionalLight
{
    float3 directionWS;
};

struct PointLight
{
    float3 posWS;
    float radius;
};

DirectionalLight CookDirectionalLight(DirectionalLightRaw rawData)
{
    DirectionalLight result;
    
    result.directionWS = rawData.param1.xyz;
    
    return result;
}

PointLight CookPointLight(PointLightRaw rawData)
{
    PointLight result;
    
    result.posWS = rawData.param1.xyz;
    result.radius = rawData.param1.w;
    
    return result;
}