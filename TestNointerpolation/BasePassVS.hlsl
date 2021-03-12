
#include "VertexLayout.hlsli"
#include "ConstantBufferCommon0.hlsli"

CONSTANT_BUFFER_VIEW(0)
CONSTANT_BUFFER_OBJECT(1)

BasePassVS2PS main(UniversalIA2VS input)
{
    BasePassVS2PS result;
    
    result.posHS = float4(input.pos.xyz, 1.0);
    result.posHS = mul(result.posHS, gLocal2World);
    result.posHS = mul(result.posHS, gWorld2View);
    result.posHS = mul(result.posHS, gView2Proj);
    
    result.color = float3(input.uvs.xy, 1.0);
    
    return result;
}