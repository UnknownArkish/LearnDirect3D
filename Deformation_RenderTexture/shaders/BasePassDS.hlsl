
#include "VertexLayout.hlsli"
#include "ConstantBufferCommon0.hlsli"

#define NUM_CONTROL_POINTS 3

CONSTANT_BUFFER_VIEW(0)
CONSTANT_BUFFER_OBJECT(1)

Texture2D deformationMap : register(t0);
SamplerState deformationMapSampler : register(s0);

[domain("tri")]
BasePassDS2PS main(
	BasePassHS2DS_ConstantData input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<BasePassHS2DS, NUM_CONTROL_POINTS> patch)
{
    BasePassDS2PS Output;

    Output.posLS = patch[0].posLS * domain.x + patch[1].posLS * domain.y + patch[2].posLS * domain.z;
    Output.tangentLS = patch[0].tangentLS * domain.x + patch[1].tangentLS * domain.y + patch[2].tangentLS * domain.z;
    Output.normalLS = patch[0].normalLS * domain.x + patch[1].normalLS * domain.y + patch[2].normalLS * domain.z;
    Output.uvs = patch[0].uvs * domain.x + patch[1].uvs * domain.y + patch[2].uvs * domain.z;
    
    float3 offsetLS = deformationMap.SampleLevel(deformationMapSampler, Output.uvs, 0).xyz;
    Output.posLS += offsetLS;
    
    Output.posHS = float4(Output.posLS.xyz, 1.0f);
    Output.posHS = mul(Output.posHS, gLocal2World);
    Output.posHS = mul(Output.posHS, gWorld2View);
    Output.posHS = mul(Output.posHS, gView2Proj);

	return Output;
}
