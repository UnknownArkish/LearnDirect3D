
#include "VertexLayoutCommon.hlsli"
#include "ConstantBufferCommon0.hlsli"
#include "GBufferCommon.hlsli"

CONSTANT_BUFFER_VIEW(0)

struct BasePassPSOutput
{
    float4 GBufferA : SV_Target0;
    float4 GBufferB : SV_Target1;
    float4 GBufferC : SV_Target2;
};

BasePassPSOutput main( UniversalVS2PS Input)
{
    GBufferData Data;
    InitGBufferData(Data);
    
    float4 posHS = mul(float4(Input.posWS, 1.0f), gWorld2View);
    posHS = mul(posHS, gView2Proj);
    
    Data.PositionWS = Input.posWS;
    Data.DepthCS = (posHS.xyz / posHS.w).z;
    Data.NormalWS = normalize(Input.normalWS);
    Data.BaseColor = float3(1.0f, 1.0f, 1.0f);
    
    GBufferDataEncode Encode;
    EncodeGBuffer(Data, Encode);
    
    BasePassPSOutput Output;
    Output.GBufferA = Encode.GBufferA;
    Output.GBufferB = Encode.GBufferB;
    Output.GBufferC = Encode.GBufferC;
    
    return Output;
}