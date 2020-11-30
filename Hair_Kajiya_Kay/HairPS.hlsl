
#include "VertexLayout.hlsli"
#include "ConstantBufferCommon0.hlsli"
#include "Lighting.hlsli"

CONSTANT_BUFFER_VIEW(0)
CONSTANT_BUFFER_OBJECT(1)

cbuffer LightConstantBuffer : register(b2)
{
	PointLightRaw gPointLight;
};

float StrandShading(float3 T, float3 V, float3 L, float exponent)
{
	float3 H = normalize(L + V);
	float TdotH = dot(T, H);
	float sinTH = sqrt(1.0f - TdotH * TdotH);
	float dirAtten = smoothstep(-1.0f, 0.0f, dot(T, H));
	return dirAtten * pow(sinTH, exponent);
}


float4 main(HairPassVS2PS input) : SV_TARGET
{
	PointLight pointLight = CookPointLight(gPointLight);
	
	float3 normalLS = normalize(input.normalLS);
	float4 tangentLS = normalize(input.tangentLS);
	
	float3 viewPosLS = mul(float4(gViewPosWS, 1.0f), gWorld2Local).xyz;
	float3 viewDirLS = normalize(viewPosLS - input.posLS);
	
	float3 lightPosLS = mul(float4(pointLight.posWS, 1.0f), gWorld2Local).xyz;
	float3 lightDirLS = normalize(lightPosLS - input.posLS);
	
	float3 specularColor = float3(1.0f, 1.0f, 1.0f);
	float3 specular = specularColor * StrandShading(tangentLS.xyz, viewDirLS, lightDirLS, 2.0f);
	
	return float4(specularColor, 1.0f);

}