
#include "DeferredPassCommon.hlsli"
#include "GBufferCommon.hlsli"
#include "ConstantBufferCommon0.hlsli"

Texture2D GBuffer0 : register(t0);
SamplerState GBufferSampler : register(s0);

float4 main(DeferredPassVS2PS Input) : SV_TARGET
{
    float4 GBuffer0Data = GBuffer0.Sample(GBufferSampler, Input.uvs);
    GBuffer0Data = GBuffer0Data / (GBuffer0Data + 1.0f);
    GBuffer0Data = pow(GBuffer0Data, 1.0f / 2.2f);  
    return GBuffer0Data;
}