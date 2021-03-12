
struct GBufferData
{
    float3 PositionWS;
    float DepthCS;
    
    float3 NormalWS;
    float3 BaseColor;
};

struct GBufferDataEncode
{
    float4 GBufferA;        // PositionWS + DepthCS
    float4 GBufferB;        // NormalWS
    float4 GBufferC;        // BaseColor
};

void InitGBufferData(inout GBufferData Data)
{
    Data.PositionWS = float3(0.0f, 0.0f, 0.0f);
    Data.DepthCS = 1.0f;
    Data.NormalWS = float3(1.0f, 0.0f, 0.0f);
    Data.BaseColor = float3(0.0f, 0.0f, 0.0f);
}
void InitGBufferDataEncode(inout GBufferDataEncode Encode)
{
    Encode.GBufferA = float4(0.0f, 0.0f, 0.0f, 0.0f);
    Encode.GBufferB = float4(0.0f, 0.0f, 0.0f, 0.0f);
    Encode.GBufferC = float4(0.0f, 0.0f, 0.0f, 0.0f);
}

void EncodeGBuffer(
    GBufferData Data,
    out GBufferDataEncode OutEncode
)
{
    OutEncode.GBufferA = float4(Data.PositionWS.xyz, Data.DepthCS);
    OutEncode.GBufferB = float4(Data.NormalWS, 0.0f);
    OutEncode.GBufferC = float4(Data.BaseColor.xyz, 0.0f);
}

void DecodeGBuffer(
    GBufferDataEncode Encode,
    out GBufferData OutData
)
{
    OutData.PositionWS = Encode.GBufferA.xyz;
    OutData.DepthCS = Encode.GBufferA.w;
    OutData.NormalWS = Encode.GBufferB.xyz;
    OutData.BaseColor = Encode.GBufferC.xyz;
}