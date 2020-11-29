
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

float4 main(CalculateDeformationMapVS2PS input) : SV_TARGET
{
    float3 offsetLS = float3(0.0f, 0.0f, 0.0f);
    
    // 计算 DeformationMap的变换坐标系
    float3 forwardWS = ForwardWS;
    float3 upWS = float3(0.0, 1.0f, 0.0f);
    float3 rightWS = cross(upWS, forwardWS);
    upWS = cross(forwardWS, rightWS);
    matrix deformation2World = matrix(
            rightWS, 0.0f,
            upWS, 0.0f,
            forwardWS, 0.0f,
            OriginWS, 1.0f
    );
    matrix world2DeformationMap = transpose(
        matrix(
        rightWS.xyz, 0.0f,
        upWS.xyz, 0.0f,
        forwardWS.xyz, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    ));
    
    
    float4 posDS = float4(input.posWS - OriginWS, 1.0f);
    posDS = mul(posDS, world2DeformationMap);
    
    float2 deformationSize = float2(Params.x, Params.y);
    float2 deformationHalfSize = deformationSize / 2;
    float2 posDSNormalized = (posDS.xy + deformationHalfSize) / deformationSize;
    
    [branch]
    if (posDSNormalized.x >= 0.0f && posDSNormalized.x <= 1.0f &&
        posDSNormalized.y >= 0.0f && posDSNormalized.y <= 1.0f)
    {
        float height = deformationMap.Sample(deformationMapSampler, posDSNormalized).r * Params.z;
        
        float4 offsetDS = float4(0.0f, 0.0f, height, 0.0f);
        float4 offsetWS = mul(offsetDS, deformation2World);
        offsetLS = mul(offsetWS, gWorld2Local).xyz;
    }
    
    return float4(offsetLS.xyz, 1.0f);
}