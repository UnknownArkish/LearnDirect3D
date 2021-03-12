
#include "DeferredPassCommon.hlsli"
#include "GBufferCommon.hlsli"

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
    
    return float4(Data.PositionWS.xyz, 1.0f);
}