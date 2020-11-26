

cbuffer ViewConstantBuffer : register(b0)
{
    matrix gWorld2View;
    matrix gView2Proj;
    
    float3 gViewPosWS;
    float _pad_b0_0;
}

cbuffer ObjectConstantBuffer : register(b1)
{
    matrix gLocal2World;
    
    float3 gObjectPosWS;
    float _pad_b1_0;
}

