
struct CalculateOffsetPassIA2VS
{
    float3 pos : POSITION;
    float4 color : COLOR;
};
struct CalculateOffsetPassVS2GS
{
    float3 pos : POSITION;
    float4 color : COLOR;
};
struct CalculateOffsetPassGS2PS
{
    float4 posH : SV_POSITION;
    float4 color : COLOR;
    float3 offset : COLOR1;
};

struct BasePassIA2VS
{
    float3 pos : POSITION;
    float4 color : COLOR0;
    //float2 uvs : TEXCOORD;
    
    float3 offset : COLOR1;
};
struct BasePassVS2PS
{
    float4 posH : SV_POSITION;
    
    float4 color : COLOR0;
};
