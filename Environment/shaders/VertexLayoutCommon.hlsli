
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
    float4 posHS : SV_POSITION;
    float3 posWS : POSITION;
    float4 color : COLOR;
    float2 uvs : TEXCOORD;
    
    float3 normalWS : NORMAL;
};

void InitialUniversalVS2PS(inout UniversalVS2PS Input)
{
    Input.posHS = float4(0.0f, 0.0f, 0.0f, 1.0f);
    Input.posWS = float3(0.0f, 0.0f, 0.0f);
    Input.color = float4(0.0f, 0.0f, 0.0f, 1.0f);
    Input.uvs = float2(0.0f, 0.0f);
    
    Input.normalWS = float3(1.0f, 0.0f, 0.0f);
}

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