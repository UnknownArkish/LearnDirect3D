#include "VertexLayoutCommon.hlsli"

Texture2D _MainTex : register(t0);
SamplerState _MainTexSampler : register(s0);

float4 main(UniversalVS2PS input) : SV_TARGET
{
    return _MainTex.Sample(_MainTexSampler, input.uvs);
}