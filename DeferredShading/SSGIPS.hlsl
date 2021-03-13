
#include "DeferredPassCommon.hlsli"
#include "GBufferCommon.hlsli"
#include "ConstantBufferCommon0.hlsli"

Texture2D GBuffer0 : register(t0);
SamplerState GBufferSampler : register(s0);

float4 main(DeferredPassVS2PS Input) : SV_TARGET
{
    float4 GBuffer0Data = GBuffer0.Sample(GBufferSampler, Input.uvs);
    return GBuffer0Data;
}