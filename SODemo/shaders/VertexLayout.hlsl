#include "VertexLayoutCommon.hlsli"

float4 main(UniversalIA2VS input) : SV_POSITION
{
    return float4(input.pos, 1.0f);
}