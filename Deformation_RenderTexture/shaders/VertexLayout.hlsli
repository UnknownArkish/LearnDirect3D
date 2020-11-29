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

struct BasePassVS2HS
{
    float3 posLS : POSITION;
    float4 tangentLS : TANGENT;
    float3 normalLS : NORMAL;
    float2 uvs : TEXCOORD;
};

struct BasePassHS2DS
{
    float3 posLS : POSITION;
    float4 tangentLS : TANGENT;
    float3 normalLS : NORMAL;
    float2 uvs : TEXCOORD;
};
struct BasePassHS2DS_ConstantData
{
    float EdgeTessFactor[3] : SV_TessFactor; // e.g. would be [4] for a quad domain
    float InsideTessFactor : SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
};

struct BasePassDS2PS
{
    float4 posHS : SV_POSITION;
    
    float3 posLS : POSITION;
    float4 tangentLS : TANGENT;
    float3 normalLS : NORMAL;
    float2 uvs : TEXCOORD;
};

struct CalculateDeformationMapVS2PS
{
    float4 posHS : SV_POSITION;
    float3 posWS : POSITION;            // 世界位置坐标
    float3 normalLS : NORMAL;
    float4 tangentLS : TANGENT;
};