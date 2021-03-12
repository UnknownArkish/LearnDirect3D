
#include "DeferredPassCommon.hlsli"
#include "GBufferCommon.hlsli"
#include "ConstantBufferCommon0.hlsli"

CONSTANT_BUFFER_VIEW(0)

cbuffer LightingConstantBuffer : register(b1)
{
    LightingConstantBufferData gLightingData;
}

Texture2D GBufferA : register(t0);
Texture2D GBufferB : register(t1);
Texture2D GBufferC : register(t2);
SamplerState GBufferSampler : register(s0);

float4 main(DeferredPassVS2PS Input) : SV_TARGET
{
    float4 GBufferAData = GBufferA.Sample(GBufferSampler, Input.uvs);
    float4 GBufferBData = GBufferB.Sample(GBufferSampler, Input.uvs);
    float4 GBufferCData = GBufferC.Sample(GBufferSampler, Input.uvs);
	
    GBufferDataEncode Encode;
    InitGBufferDataEncode(Encode);
    Encode.GBufferA = GBufferAData;
    Encode.GBufferB = GBufferBData;
    Encode.GBufferC = GBufferCData;
    
    GBufferData Data;
    InitGBufferData(Data);
    DecodeGBuffer(Encode, Data);
    
    float3 FinalColor = float3(0.0f, 0.0f, 0.0f);
    
    // Direction Light
    [unroll(16)]
    for (int i = 0; i < gLightingData.NumDirectionLight; i++)
    {
        DirectionLight Lighting = CookDirectionLight(gLightingData.DirectionLights[i]);
        
    }
    [unroll(16)]
    for (int i = 0; i < gLightingData.NumPointLight; i++)
    {
        PointLight LightingData = CookPointLight(gLightingData.PointLights[i]);
    }
    
    //FinalColor = Data.NormalWS * 0.5 + 0.5;
    
    return float4(FinalColor.xyz, 1.0f);
}