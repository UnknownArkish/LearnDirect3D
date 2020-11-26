#include "VertexLayoutCommon.hlsli"

Texture2D _MainTex : register(t0);
SamplerState _MainTexSampler : register(s0);

Texture2D normalTex : register(t1);
SamplerState normalTexSampler : register(s1);

Texture2D parallelMap : register(t2);
SamplerState parallelMapSampler : register(s2);

Texture2D debugTex : register(t3);

float4 main(UniversalVS2PS input) : SV_TARGET
{
    return debugTex.Sample(_MainTexSampler, input.uvs);
}