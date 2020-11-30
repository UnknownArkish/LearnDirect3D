#include "VertexLayoutCommon.hlsli"

struct HairPassVS2PS
{
	float4 posHS : SV_POSITION;
	
	float3 posLS : POSITION;
	float4 tangentLS : TANGENT;
	float3 normalLS : NORMAL;
	float4 color : COLOR;
};