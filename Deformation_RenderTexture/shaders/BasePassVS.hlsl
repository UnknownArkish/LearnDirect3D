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

    result.posLS = input.pos.xyz + offset.xyz * data.z;
    result.posHS = mul(float4(result.posLS.xyz, 1.0f), gLocal2World);
    result.posHS = mul(result.posHS, gWorld2View);
    result.posHS = mul(result.posHS, gView2Proj);
    result.uvs = input.uvs;

    result.normalLS = input.normal;
    result.tangentLS = input.tangent;
    
    float3 normalLS = normalize(input.normal);
    float4 tangentLS = normalize(input.tangent);
    float3 binormalLS = normalize(cross(normalLS, tangentLS.xyz) * tangentLS.w);
    matrix local2Tangent = transpose(
        matrix(
            tangentLS.xyz, 0.0f,
            binormalLS.xyz, 0.0f,
            normalLS.xyz, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        )
    );
    
    return result;
}