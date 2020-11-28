#include "VertexLayoutCommon.hlsli"

struct CalculateOffsetMapVS2PS
{
    float4 posHS : SV_POSITION;
    float3 offset : POSITION;
};

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

struct CalculateDeformationMapVS2PS
{
    float4 posHS : SV_POSITION;
    float3 posWS : POSITION;            // 世界位置坐标
    float3 offsetTS : COLOR;            // 切线空间的偏移 
};