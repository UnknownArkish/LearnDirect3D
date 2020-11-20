#include "VertexLayoutCommon.hlsli"
#include "ConstantBufferCommon.hlsli"

UniversalVS2PS main(UniversalIA2VS vIn)
{
    UniversalVS2PS vOut;
    vOut.posH = mul(float4(vIn.pos, 1.0f), gWorld);
    vOut.posH = mul(vOut.posH, gView);
    vOut.posH = mul(vOut.posH, gProjection);
    vOut.color = vIn.color;
    vOut.uvs = vIn.uvs;
    return vOut;
}