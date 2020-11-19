#include "VertexLayout.hlsli"

float4 main(BasePassVS2PS input) : SV_TARGET
{
    return input.color;
}