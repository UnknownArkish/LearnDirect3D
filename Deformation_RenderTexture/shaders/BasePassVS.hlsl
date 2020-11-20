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
    float4 offset = offsetTex.SampleLevel(offsetTexSampler, offsetUVs, 0);
    //float4 offset = float4(0.0f, 0.0f, 0.0f, 1.0f);

    result.posH = float4((input.pos + offset.xyz), 1.0f);
    result.posH = mul(result.posH, gWorld);
    result.posH = mul(result.posH, gView);
    result.posH = mul(result.posH, gProjection);
    result.color = input.color;
    result.uvs = input.uvs;
    
    return result;
}