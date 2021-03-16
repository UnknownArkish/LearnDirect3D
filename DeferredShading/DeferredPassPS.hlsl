
#include "DeferredPassCommon.hlsli"
#include "GBufferCommon.hlsli"
#include "ConstantBufferCommon0.hlsli"
#include "BRDF.hlsli"

CONSTANT_BUFFER_VIEW(0)

cbuffer LightingConstantBuffer : register(b1)
{
    LightingConstantBufferData gLightingData;
}

Texture2D GBuffer0 : register(t0);
Texture2D GBufferA : register(t1);
Texture2D GBufferB : register(t2);
Texture2D GBufferC : register(t3);
SamplerState GBufferSampler : register(s0);

//const float PI = 3.14159265359;

float3 CalculateRadiance(GBufferData Data, float3 L, float3 V)
{
    float3 F0 = float3(0.04f, 0.04f, 0.04f);
    F0 = lerp(F0, Data.BaseColor, Data.Metallic);
    
    float3 H = normalize(L + V);
        
    float NDF = DistributionGGX(Data.NormalWS, H, Data.Roughness);
    float G = GeometrySmith(Data.NormalWS, V, L, Data.Roughness);
    float3 F = FresnelSchlick(clamp(dot(H, V), 0.0f, 1.0f), F0);
        
    float3 nominator = NDF * G * F;
    float denominator = 4 * max(dot(Data.NormalWS, V), 0.0f) * max(dot(Data.NormalWS, L), 0.0f);
    float3 specular = nominator / max(denominator, 0.001f);
        
    float3 kS = F;
    float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
        
    kD *= (1.0f - Data.Metallic);
        
    float NdotL = max(dot(Data.NormalWS, L), 0.0f);
        
    return (kD * Data.BaseColor / 3.14159265359 + specular) * NdotL;
}

float4 main(DeferredPassVS2PS Input) : SV_TARGET
{
    GBufferData Data = SampleGBuffer(
        Input.uvs, 
        GBuffer0, GBufferA, GBufferB, GBufferC, 
        GBufferSampler
    );
    
    float3 FinalColor = float3(0.0f, 0.0f, 0.0f);
    
    float3 V = normalize(gViewPosWS - Data.PositionWS);
    // Direction Light
    [unroll(16)]
    for (int i = 0; i < gLightingData.NumDirectionLight; i++)
    {
        DirectionLight LightingData = CookDirectionLight(gLightingData.DirectionLights[i]);
        
        float3 L = normalize(-LightingData.DirectionWS);
        
        FinalColor += CalculateRadiance(Data, L, V) * LightingData.Color * LightingData.Intensity;
        
        //float3 H = normalize(L + Data.NormalWS);
        
        //float3 radiance = LightingData.Color;
        
        //float NDF = DistributionGGX(NDF, H, Data.Roughness);
        //float G = GeometrySmith(Data.NormalWS, V, L, Data.Roughness);
        //float3 F = FresnelSchlick(clamp(dot(H, V), 0.0f, 1.0f), F0);
        
        //flaot3 nominator = NDF * G * F;
        //float denominator = 4 * max(dot(Data.NormalWS, V), 0.0f) * max(dot(Data.NormalWS, L), 0.0f);
        //float3 specular = nominator / max(denominator, 0.001f);
        
        //float3 kS = F;
        //float3 kD = float3(1.0f) - kS;
        
        //kD *= (1.0f - Data.Metallic);
        
        //float NdotL = max(dot(Data.NormalWS, L), 0.0f);
        
        //FinalColor += (kD * albedo / PI + specular) * radiance * NdotL;
        
        //float NoL = max(dot(Data.NormalWS, -LightingData.DirectionWS), 0);
        //FinalColor += Data.BaseColor * LightingData.Color * NoL;
    }
    [unroll(16)]
    for (int i = 0; i < gLightingData.NumPointLight; i++)
    {
        PointLight LightingData = CookPointLight(gLightingData.PointLights[i]);
        
        float3 L = normalize(LightingData.PosWS - Data.PositionWS);
        float distance = length(LightingData.PosWS - Data.PositionWS);
        float attenuation = 1.0f / (distance * distance);
        
        FinalColor += CalculateRadiance(Data, L, V) * LightingData.Color * attenuation * LightingData.Intensity;
        
        //float3 LightDirection = normalize(LightingData.PosWS - Data.PositionWS);
        //float NoL = max(dot(Data.NormalWS, LightDirection), 0);
        
        //float Distance = distance(LightingData.PosWS, Data.PositionWS);
        //float attenator = pow(
        //    max((LightingData.Radius - Distance) / LightingData.Radius, 0),
        //    4
        //);
        
        //FinalColor += Data.BaseColor * LightingData.Color * NoL * attenator * LightingData.Intensity;
    }
    
    //FinalColor = Data.NormalWS * 0.5 + 0.5;
    
    FinalColor += (0.03f * Data.BaseColor);
    
    return float4(FinalColor.xyz, 1.0f);
}