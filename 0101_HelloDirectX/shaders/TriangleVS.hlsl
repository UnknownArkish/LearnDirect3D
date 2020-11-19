#include "VertexLayoutCommon.hlsli"
#include "ConstantBufferCommon.hlsli"

VertexOut main(UniversalIA2VS vIn)
{
    VertexOut vOut;
    vOut.posH = mul(float4(vIn.pos, 1.0f), gWorld);
    vOut.posH = mul(vOut.posH, gView);
    vOut.posH = mul(vOut.posH, gProjection);
    vOut.color = vIn.color;
    return vOut;
}