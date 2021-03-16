
#include "VertexLayoutCommon.hlsli"
#include "ConstantBufferCommon0.hlsli"
#include "GBufferCommon.hlsli"
#include "BasePassCommon.hlsli"

CONSTANT_BUFFER_VIEW(0)
cbuffer MaterialConstantBuffer : register(b1)
{
    MaterialConstantBufferData gMaterialData;
}

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
    Data.Roughness = gMaterialData.Roughness;
    Data.BaseColor = gMaterialData.BaseColor;
    Data.Metallic = gMaterialData.Metallic;
    
    GBufferDataEncode Encode;
    EncodeGBuffer(Data, Encode);
    
    BasePassPSOutput Output;
    Output.GBufferA = Encode.GBufferA;
    Output.GBufferB = Encode.GBufferB;
    Output.GBufferC = Encode.GBufferC;
    
    return Output;
}