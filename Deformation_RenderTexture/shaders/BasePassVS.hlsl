#include "VertexLayout.hlsli"

BasePassVS2HS main(UniversalIA2VS input)
{
    BasePassVS2HS result;

    result.posLS = input.pos.xyz;
    result.normalLS = input.normal;
    result.tangentLS = input.tangent;
    result.uvs = input.uvs;

    return result;
}