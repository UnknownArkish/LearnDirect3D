#include "VertexLayout.hlsli"
#include "ConstantBufferCommon0.hlsli"

CONSTANT_BUFFER_VIEW(0)
CONSTANT_BUFFER_OBJECT(1)

CalculateParallelMapVS2PS main(UniversalIA2VS input)
{
    CalculateParallelMapVS2PS result;

    result.posHS = float4(input.uvs * 2 - 1, 0, 1);
    result.posWS = float4(input.pos, 1.0f);
    result.posWS = mul(result.posWS, gLocal2World);
    
    return result;
}