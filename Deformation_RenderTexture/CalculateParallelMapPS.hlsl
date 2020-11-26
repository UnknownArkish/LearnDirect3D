
#include "VertexLayout.hlsli"

float4 main(CalculateParallelMapVS2PS input) : SV_TARGET
{
    //input.posWS;
	
    float radiusMaxPow = pow(1, 2);
    float radiusPow = pow(input.posWS.x, 2) + pow(input.posWS.y, 2);
	
    [branch]
    if(radiusPow > radiusMaxPow)
    {
        return float4(0, 0, 0, 1);
    }
    else
    {
	    return float4(0.0f, 0.0f, 1.0f, 1.0f);
    }
}