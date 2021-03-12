
#include "VertexLayoutCommon.hlsli"
#include "ConstantBufferCommon0.hlsli"

CONSTANT_BUFFER_VIEW(0)
CONSTANT_BUFFER_OBJECT(1)

UniversalVS2PS main(UniversalIA2VS Input)
{
    UniversalVS2PS output;
    InitialUniversalVS2PS(output);
    
    output.posHS = float4(Input.pos.xyz, 1.0);
    output.posHS = mul(output.posHS, gLocal2World);
    output.posWS = output.posHS.xyz;
    output.posHS = mul(output.posHS, gWorld2View);
    output.posHS = mul(output.posHS, gView2Proj);
    output.color = Input.color;
    output.uvs = Input.uvs;
    
    output.normalWS = mul(float4(Input.normal.xyz, 0.0f), gLocal2World).xyz;
    
    return output;
}