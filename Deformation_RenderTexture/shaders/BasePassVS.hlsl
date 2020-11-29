#include "VertexLayout.hlsli"
#include "ConstantBufferCommon0.hlsli"

CONSTANT_BUFFER_VIEW(0)
CONSTANT_BUFFER_OBJECT(1)

Texture2D deformationMap : register(t0);
SamplerState deformationMapSampler : register(s0);

BasePassVS2PS main(UniversalIA2VS input)
{
    BasePassVS2PS result;

    // 计算切线坐标系->本地坐标系的矩阵
    float3 normalLS = normalize(input.normal);
    float4 tangentLS = normalize(input.tangent);
    float3 binormalLS = normalize(cross(normalLS, tangentLS.xyz) * tangentLS.w);
    matrix tangent2Local = matrix(
        tangentLS.xyz, 0.0f,
        binormalLS.xyz, 0.0f,
        normalLS.xyz, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
    // 从DeformationMap采样xyz，即TS下的offset
    float3 offsetTS = deformationMap.SampleLevel(deformationMapSampler, input.uvs, 0).xyz;
    float3 offsetLS = mul(float4(offsetTS, 0.0f), tangent2Local).xyz;
    offsetLS = float3(0.0f, 0.0f, 0.0f);

    result.posLS = input.pos.xyz + offsetLS;
    result.posHS = mul(float4(result.posLS.xyz, 1.0f), gLocal2World);
    result.posHS = mul(result.posHS, gWorld2View);
    result.posHS = mul(result.posHS, gView2Proj);
    result.uvs = input.uvs;

    result.normalLS = input.normal;
    result.tangentLS = input.tangent;
    
    return result;
}