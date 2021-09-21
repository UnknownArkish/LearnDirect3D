

#define CONSTANT_BUFFER_VIEW(slot) \
cbuffer ViewConstantBuffer : register(b##slot)  \
{                                               \
    matrix gWorld2View;                         \
    matrix gView2Proj;                          \
                                                \
    float3 gViewPosWS;                          \
    float _pad_0_b##slot;                       \
                                                    \
    float2 gScreenSize;                             \
    float2 _pad_1_b##slot;                          \
}

#define CONSTANT_BUFFER_OBJECT(slot)                \
cbuffer ObjectConstantBuffer : register(b##slot)    \
{                                                   \
    matrix gLocal2World;                            \
    matrix gWorld2Local;                            \
                                                    \
    float3 gObjectPosWS;                            \
    float _pad_0_b##slot;                           \
}