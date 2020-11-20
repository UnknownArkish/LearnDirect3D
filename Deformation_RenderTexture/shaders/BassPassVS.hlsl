#include "VertexLayoutCommon.hlsli"
#include "ConstantBufferCommon.hlsli"

cbuffer OffsetConstantBuffer : register(b1)
{
    float4 data;
}

Texture2D offsetTex : register(t1);
SamplerState offsetTexSampler : register(s1);

UniversalVS2PS main(UniversalIA2VS input, uint vertexID : SV_VertexID)
{
    UniversalVS2PS result;
    
    float col = vertexID % data.x;
    float row = floor(vertexID / data.x);
    
    float2 offsetUVs = float2(col / data.x, row / data.y);
    float4 offset = offsetTex.Sample(offsetTexSampler, offsetUVs);
    
    result.posH = float4((input.pos + offset), 1.0f);
    result.posH = mul(result.posH, gWorld);
    result.posH = mul(result.posH, gView);
    result.posH = mul(result.posH, gProjection);
    result.color = input.color;
    
    return result;
}