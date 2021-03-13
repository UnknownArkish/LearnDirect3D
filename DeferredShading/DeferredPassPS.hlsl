
#include "DeferredPassCommon.hlsli"
#include "GBufferCommon.hlsli"
#include "ConstantBufferCommon0.hlsli"

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

float4 main(DeferredPassVS2PS Input) : SV_TARGET
{
    GBufferData Data = SampleGBuffer(
        Input.uvs, 
        GBuffer0, GBufferA, GBufferB, GBufferC, 
        GBufferSampler
    );
    
    float3 FinalColor = float3(0.0f, 0.0f, 0.0f);
    
    // Direction Light
    [unroll(16)]
    for (int i = 0; i < gLightingData.NumDirectionLight; i++)
    {
        DirectionLight LightingData = CookDirectionLight(gLightingData.DirectionLights[i]);
        float NoL = max(dot(Data.NormalWS, -LightingData.DirectionWS), 0);
        
        FinalColor += Data.BaseColor * LightingData.Color * NoL;
    }
    [unroll(16)]
    for (int i = 0; i < gLightingData.NumPointLight; i++)
    {
        PointLight LightingData = CookPointLight(gLightingData.PointLights[i]);
        
        float3 LightDirection = normalize(LightingData.PosWS - Data.PositionWS);
        float NoL = max(dot(Data.NormalWS, LightDirection), 0);
        
        float Distance = distance(LightingData.PosWS, Data.PositionWS);
        float attenator = pow(
            max((LightingData.Radius - Distance) / LightingData.Radius, 0),
            4
        );
        
        FinalColor += Data.BaseColor * LightingData.Color * NoL * attenator * LightingData.Intensity;
    }
    
    //FinalColor = Data.NormalWS * 0.5 + 0.5;
    
    return float4(FinalColor.xyz, 1.0f);
}