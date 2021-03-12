
#include "VertexLayoutCommon.hlsli"
#include "Lighting.hlsli"

struct DeferredPassVS2PS
{
    float4 posHS : SV_POSITION;
    float2 uvs : TEXCOORD0;
};

struct LightingConstantBufferData
{
    DirectionLightRaw DirectionLights[16];
    PointLightRaw PointLights[16];
    int NumDirectionLight;
    int NumPointLight;
    
    float _pad_0;
    float _pad_1;
};