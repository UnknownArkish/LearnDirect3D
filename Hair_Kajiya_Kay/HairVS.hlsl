#include "VertexLayout.hlsli"
#include "ConstantBufferCommon0.hlsli"

CONSTANT_BUFFER_VIEW(0)
CONSTANT_BUFFER_OBJECT(1)

HairPassVS2PS main(UniversalIA2VS input)
{
	HairPassVS2PS result;

	result.posHS = float4(input.pos, 1.0f);
	result.posHS = mul(result.posHS, gLocal2World);
	result.posHS = mul(result.posHS, gWorld2View);
	result.posHS = mul(result.posHS, gView2Proj);
	
	result.posLS = input.pos;
	result.color = input.color;
	result.tangentLS = input.tangent;
	result.normalLS = input.normal;
	
	return result;
}