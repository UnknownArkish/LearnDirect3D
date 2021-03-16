
#include "DeferredPassCommon.hlsli"
#include "GBufferCommon.hlsli"
#include "ConstantBufferCommon0.hlsli"

CONSTANT_BUFFER_VIEW(0)

Texture2D GBuffer0 : register(t0);
Texture2D GBufferA : register(t1);
Texture2D GBufferB : register(t2);
Texture2D GBufferC : register(t3);
SamplerState GBufferSampler : register(s0);

float4 main(DeferredPassVS2PS Input) : SV_TARGET
{
    GBufferData Data = SampleGBuffer(
        Input.uvs, 
        GBuffer0, GBufferA, GBufferB, GBufferC, 
        GBufferSampler
    );
    
    float2 ScreenPosition = Input.uvs * gScreenSize;
    
    float3 FinalColor = float3(0.0f, 0.0f, 0.0f);
    float Min = -0.2f, Max = 0.2f;
    float Step = 0.015f;
    //[unroll(21)]
    for (float x = Min; x < Max; x+= Step)
    {
        //[unroll(21)]
        for (float y = Min; y < Max; y+= Step)
        {
            //float2 NewScreenPosition = ScreenPosition + float2(x, y);
            //float2 NewUVs = NewScreenPosition / gScreenSize;
            float2 NewUVs = Input.uvs + float2(x, y);
            
            if (NewUVs.x >= 0.0f && NewUVs.x <= 1.0f &&
                NewUVs.y >= 0.0f && NewUVs.y <= 1.0f)
            {
                GBufferData NewData = SampleGBuffer(
                    NewUVs,
                    GBuffer0, GBufferA, GBufferB, GBufferC,
                    GBufferSampler
                );
                
                float E = max(
                    0,
                    dot(NewData.NormalWS, normalize(Data.PositionWS - NewData.PositionWS)) *
                    max(dot(Data.NormalWS, normalize(NewData.PositionWS - Data.PositionWS)), 0)
                );
                E /= (1.0f + pow(length(Data.PositionWS - NewData.PositionWS), 2));
                
                FinalColor += E * NewData.ScreenColor;
            }
        }
    }
    FinalColor *= 0.15f;
    
    return float4(Data.ScreenColor.xyz + FinalColor.xyz, Data.ScreenColor.w);
}