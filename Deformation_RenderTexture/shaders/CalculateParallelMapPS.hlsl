
#include "VertexLayout.hlsli"

cbuffer DeformationConstantBuffer : register(b0)
{
    float3 ForwardWS;   float _pad_b0_0;
    float3 OriginWS;    float _pad_b0_1;
    float4 Params;
};

Texture2D deformationMap : register(t0);
SamplerState deformationMapSampler : register(s0);

float4 main(CalculateParallelMapVS2PS input) : SV_TARGET
{
    // 计算 DeformationMap的变换坐标系
    float3 forwardWS = ForwardWS;
    float3 upWS = float3(0.0, 1.0f, 0.0f);
    float3 rightWS = cross(upWS, forwardWS);
    upWS = cross(forwardWS, rightWS);
    
    matrix world2DeformationMap = transpose(
        matrix(
        rightWS.xyz, 0.0f,
        upWS.xyz, 0.0f,
        forwardWS.xyz, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    ));
    
    // 将input.posWS变换到DeformationMap坐标系
    float4 posDS = float4(input.posWS - OriginWS, 1.0f);
    posDS = mul(posDS, world2DeformationMap);
    
    float2 size = float2(Params.x, Params.y);
    float2 halfSize = size / 2;
    
    // 归一化posDS
    float2 posDSNormalized = (posDS.xy + halfSize) / size;
    
    [branch]
    if(posDSNormalized.x >= 0.0f && posDSNormalized.x <= 1.0f &&
        posDSNormalized.y >= 0.0f && posDSNormalized.y <= 1.0f)
    {
        float height = deformationMap.Sample(deformationMapSampler, posDSNormalized).r;
        return float4(height, height, height, 1.0f);
    }
    else
    {
        return float4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}