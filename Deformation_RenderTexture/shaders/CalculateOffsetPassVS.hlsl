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
cbuffer OffsetConstantBuffer : register(b3)
{
    float Width;
    float Height;
    float2 _pad_b3_0;
}

Texture2D deformationMap : register(t0);
SamplerState deformationMapSampler : register(s0);

CalculateOffsetMapVS2PS main(UniversalIA2VS input, uint vertexID : SV_VertexID)
{
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
    
    
    CalculateOffsetMapVS2PS result;

    // 计算 VertexID对应的裁剪坐标
    float col = vertexID % Width;
    float row = floor(vertexID / Width);
    float x = ((col + 0.5f) * 2 - Width) / Width;
    float y = ((row + 0.5f) * 2 - Height) / Height;
    result.posHS = float4(x, y, 0.0f, 1.0f);
    
    
    // 计算offset
    float4 posWS = mul(float4(input.pos, 1.0f), gLocal2World);
    float4 posDS = posWS - float4(OriginWS.xyz, 0.0f);
    posDS = mul(posDS, world2Deformation);
    float2 size = float2(Params.x, Params.y);
    float2 halfSize = size / 2;
    // 归一化posDS
    float2 posDSNormalized = (posDS.xy + halfSize) / size;
    [branch]
    if (posDSNormalized.x >= 0.0f && posDSNormalized.x <= 1.0f &&
        posDSNormalized.y >= 0.0f && posDSNormalized.y <= 1.0f)
    {
        posDSNormalized.y = 1.0f - posDSNormalized.y;
        float height = deformationMap.SampleLevel(deformationMapSampler, posDSNormalized, 0).r * Params.z;
        
        float4 newPosWS = float4(posDS.xyz + float3(0.0, 0.0, height), 1.0f);
        newPosWS = mul(newPosWS, deformation2World);
        
        float4 newPosLS = mul(newPosWS, gWorld2Local);
        
        float3 offset = newPosLS.xyz - input.pos.xyz;
        result.offset = offset;
        //result.offset = float3(0.0f, 0.0f, 1.0f);
    }
    else
    {
        result.offset = float3(0.0f, 0.0f, 0.0f);

    }
    
    return result;
}