
#include "VertexLayout.hlsli"

CalculateOffsetPassVS2GS main(CalculateOffsetPassIA2VS input)
{
    CalculateOffsetPassVS2GS result;
    
    result.pos = input.pos;
    result.color = input.color;
    
    return result;
}