#include "VertexLayout.hlsli"
#include "ConstantBufferCommon0.hlsli"
#include "Lighting.hlsli"

CONSTANT_BUFFER_VIEW(0)
CONSTANT_BUFFER_OBJECT(1)

cbuffer LightConstantBuffer : register(b2)
{
    PointLightRaw gPointLight;
};
cbuffer ParallelConstantBuffer : register(b3)
{
    float gParallelStepScale;
    float gParallelIntensity;
    float2 _pad_b3_0;
}

Texture2D _MainTex : register(t0);
SamplerState _MainTexSampler : register(s0);

Texture2D normalTex : register(t1);
SamplerState normalTexSampler : register(s1);

Texture2D parallelMap : register(t2);
SamplerState parallelMapSampler : register(s2);

float2 ParallelMapping(float2 uvs, float3 viewDir)
{
    const float layers = 64;
    int interator = 0;

    float deltaDepth = 1.0f / layers;
    float2 P = viewDir.xy / viewDir.z * gParallelStepScale;
    float2 deltaUVs = P / layers;

    float2 currentUVs = uvs;

    float currentDepth = 0.0f;
    float currentParallel = parallelMap.Sample(parallelMapSampler, currentUVs).r * gParallelIntensity;
    [unroll]
    while (currentDepth < currentParallel && interator < layers)
    {
        currentUVs -= deltaUVs;
        currentParallel = parallelMap.Sample(parallelMapSampler, currentUVs).r * gParallelIntensity;
        currentDepth += deltaDepth;
        
        interator++;
    }
    return currentUVs;

    float2 prevUVs = currentUVs + deltaUVs;
    float prevDepth = currentDepth - deltaDepth;

    float currentDepthDiff = currentParallel - currentDepth;
    float prevDepthDiff = parallelMap.Sample(parallelMapSampler, prevUVs).r * gParallelIntensity - prevDepth;
    float factor = currentDepthDiff / (currentDepthDiff - prevDepthDiff);

    float2 result = prevUVs * factor + currentUVs * (1 - factor);
    return result;
}

float4 main(BasePassVS2PS input) : SV_TARGET
{
    float3 normalLS = normalize(input.normalLS);
    float4 tangentLS = normalize(input.tangentLS);
    float3 binormalLS = normalize(cross(normalLS, tangentLS.xyz) * tangentLS.w);
    matrix local2Tangent = transpose(
        matrix(
            tangentLS.xyz, 0.0f,
            binormalLS, 0.0f,
            normalLS, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    ));
    // Light
    PointLight pointLight = CookPointLight(gPointLight);
    float3 lightWS = pointLight.posWS;
    float3 lightLS = mul(float4(lightWS, 1.0f), gWorld2Local).xyz;
    // View
    float3 viewWS = gViewPosWS;
    float3 viewLS = mul(float4(viewWS, 1.0f), gWorld2Local).xyz;
    // Frag
    float3 posLS = input.posLS;

    float3 lightDirLS = lightLS - posLS;
    float3 viewDirLS = normalize(viewLS - posLS);

    float3 lightDirTS = mul(float4(lightDirLS, 0.0f), local2Tangent).xyz;
    float3 viewDirTS = mul(float4(viewDirLS, 0.0f), local2Tangent).xyz;

    float2 parallelUVs = ParallelMapping(input.uvs, viewDirTS);
    //parallelUVs = input.uvs;
    
    float3 normalTS = normalize(normalTex.Sample(normalTexSampler, parallelUVs));
    float NdotL = max(dot(normalTS, lightDirTS), 0.0f);
    //NdotL = 1.0f;
    
    float3 ambientColor = float3(0.1f, 0.1f, 0.1f) * 0;
    float3 baseColor = _MainTex.Sample(_MainTexSampler, parallelUVs).xyz;
    
    float3 finalColor = ambientColor + baseColor * NdotL * pointLight.color;

    return float4(finalColor.xyz, 1.0f);
    //return parallelMap.Sample(parallelMapSampler, input.uvs);
}