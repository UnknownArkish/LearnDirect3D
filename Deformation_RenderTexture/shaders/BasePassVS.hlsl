#include "VertexLayout.hlsli"
#include "ConstantBufferCommon0.hlsli"

CONSTANT_BUFFER_VIEW(0)
CONSTANT_BUFFER_OBJECT(1)

cbuffer OffsetConstantBuffer : register(b2)
{
    float4 data;
}

Texture2D offsetMap : register(t0);
SamplerState offsetMapSampler : register(s0);

BasePassVS2PS main(UniversalIA2VS input, uint vertexID : SV_VertexID)
{
    BasePassVS2PS result;
    
    float col = vertexID % data.x + 0.5f;
    float row = floor(vertexID / data.x) + 0.5f;
    
    float2 offsetUVs = float2(col / data.x, 1.0f - row / data.y);
    float4 offset = offsetMap.SampleLevel(offsetMapSampler, offsetUVs, 0);

    result.posHS = float4(input.pos.xyz, 1.0f);
    result.posHS = mul(result.posHS, gLocal2World) + float4(offset.xyz, 0.0f);
    result.posLS = mul(result.posHS, gWorld2Local).xyz;

    result.posHS = mul(result.posHS, gWorld2View);
    result.posHS = mul(result.posHS, gView2Proj);
    result.uvs = input.uvs;

    result.normalLS = input.normal;
    result.tangentLS = input.tangent;
    
    return result;
}