#include "VertexLayoutCommon.hlsli"

Texture2D tex : register(t0);
SamplerState samplerState : register(s0);

float4 main(UniversalVS2PS pIn) : SV_TARGET
{
    return float4(tex.Sample(samplerState, pIn.uvs).xyz, 1.0f);
    //return float4(pIn.uvs.xy, 0.0f, 1.0f);
}