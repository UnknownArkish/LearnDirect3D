
/*
	
	计算DeformationMap，DeformationMap的xyz是切线坐标系下的偏移，z是ParallelMap的深度

*/

#include "VertexLayout.hlsli"
#include "ConstantBufferCommon0.hlsli"

CONSTANT_BUFFER_OBJECT(0)

cbuffer DeformationConstantBuffer : register(b1)
{
    float3 ForwardWS;   float _pad_b2_0;
    float3 OriginWS;    float _pad_b2_1;
    // x : width, y : height, z : Intensity
    float4 Params;
};
Texture2D deformationMap : register(t0);
SamplerState deformationMapSampler : register(s0);

CalculateDeformationMapVS2PS main(UniversalIA2VS input)
{
    CalculateDeformationMapVS2PS result;
	
    float4 posHS = float4(input.uvs * 2 - 1, 0.0f, 1.0f);
    result.posHS = posHS;
    
    float4 posWS = mul(float4(input.pos, 1.0f), gLocal2World);
    result.posWS = posWS.xyz;
    result.normalLS = input.normal;
    result.tangentLS = input.tangent;
    
    return result;
}