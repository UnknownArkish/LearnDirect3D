
#include "DeferredPassCommon.hlsli"

DeferredPassVS2PS main(UniversalIA2VS Input)
{
    DeferredPassVS2PS Output;

    Output.posHS = float4(Input.pos.xy, 0, 1);
    Output.uvs = Input.uvs;
    
    return Output;
}