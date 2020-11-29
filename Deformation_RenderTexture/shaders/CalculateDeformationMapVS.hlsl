
/*
	
	计算DeformationMap，DeformationMap的xyz是切线坐标系下的偏移，z是ParallelMap的深度

*/

#include "VertexLayout.hlsli"
#include "ConstantBufferCommon0.hlsli"


CONSTANT_BUFFER_VIEW(0)
CONSTANT_BUFFER_OBJECT(1)

cbuffer DeformationConstantBuffer : register(b2)
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
    float4 posWS = mul(float4(input.pos, 1.0f), gLocal2World);
    float3 offsetTS = float3(0.0f, 0.0f, 0.0f);
	
    // 计算TS下的Offset
    float2 deformationSize = float2(Params.x, Params.y);
    float2 deformationHalfSize = deformationSize / 2;
    
    // 计算世界坐标系->贴图坐标系的矩阵
    float3 forwardWS = ForwardWS;
    float3 upWS = float3(0.0f, 1.0f, 0.0f);
    float3 rightWS = normalize(cross(upWS, forwardWS));
    upWS = normalize(cross(forwardWS, rightWS));
    matrix deformation2World = matrix(
            rightWS, 0.0f,
            upWS, 0.0f,
            forwardWS, 0.0f,
            OriginWS, 1.0f
    );
    matrix world2Deformation = transpose(
        matrix(
                rightWS, 0.0f,
                upWS, 0.0f,
                forwardWS, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
        )
    );
    
    float4 posDS = posWS - float4(OriginWS.xyz, 0.0f);
    posDS = mul(posDS, world2Deformation);
    // 归一化PosDS
    float2 posDSNormalized = (posDS.xy + deformationHalfSize) / deformationSize;
    [branch]
    if (posDSNormalized.x >= 0.0f && posDSNormalized.x <= 1.0f &&
        posDSNormalized.y >= 0.0f && posDSNormalized.y <= 1.0f)
    {
        // 计算本地坐标系->切线坐标西的矩阵
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
        
        //posDSNormalized.y = 1.0f - posDSNormalized.y;
        float height = deformationMap.SampleLevel(deformationMapSampler, posDSNormalized, 0).r * Params.z;
        
        float4 offsetDS = float4(0.0f, 0.0f, height, 0.0f);
        float4 offsetWS = mul(offsetDS, deformation2World);
        float4 offsetLS = mul(offsetWS, gWorld2Local);
        
        offsetTS = mul(offsetLS, local2Tangent).xyz;
        //offsetTS = offsetLS.xyz;
    }
    else
    {
        offsetTS = float3(0.0f, 0.0f, 0.0f);
    }
    
    result.posHS = posHS;
    result.posWS = posWS.xyz;
    result.offsetTS = offsetTS;
    
    return result;
}