
#include "VertexLayout.hlsli"

float4 main(
    uint PrimitiveID : SV_PrimitiveID,
    BasePassVS2PS input
) : SV_TARGET
{
    return float4(input.color, 1.0);
}