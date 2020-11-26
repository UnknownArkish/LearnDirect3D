#include "VertexLayout.hlsli"
#include "ConstantBufferCommon0.hlsli"
#include "Lighting.hlsli"

CONSTANT_BUFFER_VIEW(0)
CONSTANT_BUFFER_OBJECT(1)

cbuffer LightConstantBuffer : register(b2)
{
    PointLight gPointLight;
};

Texture2D _MainTex : register(t0);
SamplerState _MainTexSampler : register(s0);

Texture2D normalTex : register(t1);
SamplerState normalTexSampler : register(s1);

Texture2D parallelMap : register(t2);
SamplerState parallelMapSampler : register(s2);

Texture2D debugTex : register(t3);

float4 main(BasePassVS2PS input) : SV_TARGET
{
    float3 viewWS = gViewPosWS;
    float3 viewLS = mul(float4(viewWS, 1.0f), gWorld2Local).xyz;



    float3 posLS = input.posLS;

    float3 normalLS = normalize(input.normalLS);
    float4 tangentLS = normalize(input.tangentLS);
    float3 binormalLS = normalize(cross(normalLS, tangentLS.xyz) * tangentLS.w);
    matrix local2Tanget = transpose(
        matrix(
            tangentLS.xyz, 0.0f,
            binormalLS, 0.0f,
            normalLS, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    ));

    
    //float3 

    return debugTex.Sample(_MainTexSampler, input.uvs);
}