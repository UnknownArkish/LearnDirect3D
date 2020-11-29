
#include "VertexLayout.hlsli"

#define NUM_CONTROL_POINTS 3

// Patch Constant Function
BasePassHS2DS_ConstantData CalcHSPatchConstants(
	InputPatch<BasePassVS2HS, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
    BasePassHS2DS_ConstantData Output;

    Output.EdgeTessFactor[0] =
		Output.EdgeTessFactor[1] =
		Output.EdgeTessFactor[2] = 5;
	Output.InsideTessFactor = 5;

	return Output;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
BasePassHS2DS main(
	InputPatch<BasePassVS2HS, NUM_CONTROL_POINTS> input,
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
    BasePassHS2DS Output;

    Output.posLS = input[i].posLS;
    Output.tangentLS = input[i].tangentLS;
    Output.normalLS = input[i].normalLS;
    Output.uvs = input[i].uvs;

	return Output;
}
