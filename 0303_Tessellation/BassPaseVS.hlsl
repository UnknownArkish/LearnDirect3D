
cbuffer ConstantBuffer : register(b0)
{
    matrix gWorldViewProj;
}

float4 main(float3 PosL : POSITION) : SV_POSITION
{
    return mul(float4(PosL, 1.0f), gWorldViewProj);
}