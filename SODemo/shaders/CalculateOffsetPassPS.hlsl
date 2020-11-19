#include "VertexLayout.hlsli"

float4 main(CalculateOffsetPassGS2PS input) : SV_TARGET
{
    return input.color;
}