
struct UniversalIA2VS
{
    float3 pos : POSITION;
    float4 color : COLOR;
    float2 uvs : TEXCOORD;
    float3 normal : NORMAL;
    float4 tangent : TANGENT;
};
struct UniversalVS2PS
{
    float4 posH : SV_POSITION;
    float3 posWS : POSITION;
    float4 color : COLOR;
    float2 uvs : TEXCOORD;
};

//struct VertexIn
//{
//    float3 pos : POSITION;
//    float4 color : COLOR;
//    float2 uvs : TEXCOORD;
//};

//struct VertexOut
//{
//    float4 posH : SV_POSITION;
//    float4 color : COLOR;
//};