
#include "VertexLayout.hlsli"
#include "ConstantBufferCommon.hlsli"

[maxvertexcount(1)]
void main(
	point CalculateOffsetPassVS2GS input[1] : SV_POSITION, 
	inout PointStream<CalculateOffsetPassGS2PS> output
)
{
	[unroll]
	for (uint i = 0; i < 1; i++)
	{
        CalculateOffsetPassGS2PS result;
		
        result.posH = float4(input[i].pos, 1.0f);
        result.posH = mul(result.posH, gWorld);
        result.posH = mul(result.posH, gView);
        result.posH = mul(result.posH, gProjection);
		
        result.color = input[i].color;
        result.offset = float3(0.0f, 1.0f, 0.0f);
		
		output.Append(result);
	}
}