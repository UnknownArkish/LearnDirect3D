#include "VertexLayoutCommon.hlsli"
#include "ConstantBufferCommon.hlsli"

cbuffer OffsetConstantBuffer : register(b1)
{
    float4 data;
}

Texture2D offsetTex : register(t0);
SamplerState offsetTexSampler : register(s0);

UniversalVS2PS main(UniversalIA2VS input, uint vertexID : SV_VertexID)
{
    UniversalVS2PS result;
    
    float col = vertexID % data.x + 0.5f;
    float row = floor(vertexID / data.x) + 0.5f;
    
    float2 offsetUVs = float2(col / data.x, 1.0f - row / data.y);
    float4 offset = offsetTex.SampleLevel(offsetTexSampler, offsetUVs, 0);

    result.posH = float4(input.pos, 1.0f);
    result.posH = mul(result.posH, gWorld) + float4(offset.xyz, 0.0f);
    result.posH = mul(result.posH, gView);
    result.posH = mul(result.posH, gProjection);
    result.color = input.color;
    result.uvs = input.uvs;
    
    return result;
}