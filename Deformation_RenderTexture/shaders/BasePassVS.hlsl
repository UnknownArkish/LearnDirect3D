#include "VertexLayoutCommon.hlsli"
#include "ConstantBufferCommon0.hlsli"

CONSTANT_BUFFER_VIEW(0)
CONSTANT_BUFFER_OBJECT(1)

cbuffer OffsetConstantBuffer : register(b2)
{
    float4 data;
}

Texture2D offsetMap : register(t0);
SamplerState offsetMapSampler : register(s0);

UniversalVS2PS main(UniversalIA2VS input, uint vertexID : SV_VertexID)
{
    UniversalVS2PS result;
    
    float col = vertexID % data.x + 0.5f;
    float row = floor(vertexID / data.x) + 0.5f;
    
    float2 offsetUVs = float2(col / data.x, 1.0f - row / data.y);
    float4 offset = offsetMap.SampleLevel(offsetMapSampler, offsetUVs, 0);

    result.posH = float4(input.pos, 1.0f);
    result.posH = mul(result.posH, gLocal2World) + float4(offset.xyz, 0.0f);
    result.posH = mul(result.posH, gWorld2View);
    result.posH = mul(result.posH, gView2Proj);
    result.color = input.color;
    result.uvs = input.uvs;
    
    return result;
}