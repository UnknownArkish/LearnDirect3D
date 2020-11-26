#include "VertexLayoutCommon.hlsli"

struct CalculateParallelMapVS2PS
{
    float4 posHS : SV_POSITION;
    float3 posWS : POSITION;
};

struct BasePassVS2PS
{
    float4 posHS : SV_POSITION;

    float2 uvs : TEXCOORD;
    float3 posLS : POSITION;
    float4 tangentLS : TANGENT;
    float3 normalLS : NORMAL;
};