#include "VertexLayout.hlsli"

float4 main(CalculateOffsetMapVS2PS input) : SV_TARGET
{
    return float4(input.offset, 1.0f);
}