
struct GBufferData
{
    float4 ScreenColor;
    
    float3 PositionWS;
    float DepthCS;
    
    float3 NormalWS;
    float Roughness;
    
    float3 BaseColor;
    float Metallic;
};

struct GBufferDataEncode
{
    float4 GBuffer0;        // ScreenColor
    float4 GBufferA;        // PositionWS + DepthCS
    float4 GBufferB;        // NormalWS + Roughness
    float4 GBufferC;        // BaseColor + Metallic
};

void InitGBufferData(inout GBufferData Data)
{
    Data.ScreenColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    Data.PositionWS = float3(0.0f, 0.0f, 0.0f);
    Data.DepthCS = 1.0f;
    Data.NormalWS = float3(1.0f, 0.0f, 0.0f);
    Data.Roughness = 0.0f;
    Data.BaseColor = float3(0.0f, 0.0f, 0.0f);
    Data.Metallic = 0.0f;
}
void InitGBufferDataEncode(inout GBufferDataEncode Encode)
{
    Encode.GBuffer0 = float4(0.0f, 0.0f, 0.0f, 0.0f);
    Encode.GBufferA = float4(0.0f, 0.0f, 0.0f, 0.0f);
    Encode.GBufferB = float4(0.0f, 0.0f, 0.0f, 0.0f);
    Encode.GBufferC = float4(0.0f, 0.0f, 0.0f, 0.0f);
}

void EncodeGBuffer(
    GBufferData Data,
    out GBufferDataEncode OutEncode
)
{
    OutEncode.GBuffer0 = Data.ScreenColor;
    OutEncode.GBufferA = float4(Data.PositionWS.xyz, Data.DepthCS);
    OutEncode.GBufferB = float4(Data.NormalWS.xyz, Data.Roughness);
    OutEncode.GBufferC = float4(Data.BaseColor.xyz, Data.Metallic);
}

void DecodeGBuffer(
    GBufferDataEncode Encode,
    out GBufferData OutData
)
{
    OutData.ScreenColor = Encode.GBuffer0.xyzw;
    OutData.PositionWS = Encode.GBufferA.xyz;
    OutData.DepthCS = Encode.GBufferA.w;
    OutData.NormalWS = Encode.GBufferB.xyz;
    OutData.Roughness = Encode.GBufferB.w;
    OutData.BaseColor = Encode.GBufferC.xyz;
    OutData.Metallic = Encode.GBufferC.w;
}

GBufferData SampleGBuffer( float2 uvs, 
    Texture2D GBuffer0, Texture2D GBufferA, Texture2D GBufferB, Texture2D GBufferC,
    SamplerState Sampler
)
{
    float4 GBuffer0Data = GBuffer0.Sample(Sampler, uvs);
    float4 GBufferAData = GBufferA.Sample(Sampler, uvs);
    float4 GBufferBData = GBufferB.Sample(Sampler, uvs);
    float4 GBufferCData = GBufferC.Sample(Sampler, uvs);
	
    GBufferDataEncode Encode;
    InitGBufferDataEncode(Encode);
    Encode.GBuffer0 = GBuffer0Data;
    Encode.GBufferA = GBufferAData;
    Encode.GBufferB = GBufferBData;
    Encode.GBufferC = GBufferCData;
    
    GBufferData Data;
    InitGBufferData(Data);
    DecodeGBuffer(Encode, Data);
    
    return Data;
}
