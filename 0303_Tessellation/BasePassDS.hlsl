struct DS_OUTPUT
{
	float4 vPosition  : SV_POSITION;
	// TODO: change/add other stuff
};

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

#define NUM_CONTROL_POINTS 6

[domain("tri")]
DS_OUTPUT main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
{
	DS_OUTPUT Output;

	Output.vPosition = float4(
		patch[3].vPosition*domain.x+patch[4].vPosition*domain.y+patch[5].vPosition*domain.z,1);

	return Output;
}
