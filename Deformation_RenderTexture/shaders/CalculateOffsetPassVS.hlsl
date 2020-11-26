#include "VertexLayoutCommon.hlsli"

cbuffer OffsetConstantBuffer : register(b0)
{
    float4 data;
}

UniversalVS2PS main(UniversalIA2VS input, uint vertexID : SV_VertexID)
{
    UniversalVS2PS result;

    float col = vertexID % data.x;
    float row = floor(vertexID / data.x);
    
    float x = ((col + 0.5f) * 2 - data.x) / data.x;
    float y = ((row + 0.5f) * 2 - data.y) / data.y;
    result.posH = float4(x, y, 0.0f, 1.0f);
    
    result.color = input.color;
    result.uvs = input.uvs;
    
    return result;
}