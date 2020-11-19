#include "VertexLayout.hlsli"
#include "ConstantBufferCommon.hlsli"

BasePassVS2PS main(BasePassIA2VS input)
{
    BasePassVS2PS result;

    result.posH = float4((input.pos + input.offset), 1.0f);
    result.posH = mul(result.posH, gWorld);
    result.posH = mul(result.posH, gView);
    result.posH = mul(result.posH, gProjection);
    result.color = input.color;
    
    return result;
}