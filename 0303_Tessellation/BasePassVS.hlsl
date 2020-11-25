
#include "Tessellation.hlsli"

BasePassVS2HS main(float3 pos : POSITION)
{
	BasePassVS2HS result;

	result.posWS = pos;
	//result.debug = float4(pos, 1.0f);

	return result;
}