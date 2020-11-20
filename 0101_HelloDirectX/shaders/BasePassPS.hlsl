#include "VertexLayoutCommon.hlsli"

float4 main(UniversalVS2PS pIn) : SV_TARGET
{
    return float4(pIn.uvs.xy, 0.0f, 1.0f);
}