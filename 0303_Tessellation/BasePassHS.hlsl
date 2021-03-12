

#include "Tessellation.hlsli"

// Output control point
struct HS_CONTROL_POINT_OUTPUT
{
	float3 vPosition : WORLDPOS; 
};

// Output patch constant data.
struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3]			: SV_TessFactor; // e.g. would be [4] for a quad domain
	float InsideTessFactor			: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
	// TODO: change/add other stuff
};

#define NUM_CONTROL_POINTS 3

// Patch Constant Function
HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<BasePassVS2HS, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT Output;

	// Insert code to compute Output here
	Output.EdgeTessFactor[0] =
		Output.EdgeTessFactor[1] =
		Output.EdgeTessFactor[2] = 2;
	Output.InsideTessFactor = 15; // e.g. could calculate dynamic tessellation factors instead

	return Output;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(6)]
[patchconstantfunc("CalcHSPatchConstants")]
HS_CONTROL_POINT_OUTPUT main( 
	InputPatch<BasePassVS2HS, NUM_CONTROL_POINTS> ip ,	// 对应vs的输出
	uint i : SV_OutputControlPointID,			// 对应 controlpoint 的个数
	uint PatchID : SV_PrimitiveID )
{
	HS_CONTROL_POINT_OUTPUT Output;

	float factor = 1.0f;
	// Insert code to compute Output here
	[branch]
	if (i >= 3)
	{
		i -= 3;
		factor = 0.5f;
	}
	Output.vPosition = ip[i].posWS * factor;

	return Output;
}
