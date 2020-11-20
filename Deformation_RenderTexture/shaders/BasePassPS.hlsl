#include "VertexLayoutCommon.hlsli"

float4 main(UniversalVS2PS input) : SV_TARGET
{
    return input.color;
}