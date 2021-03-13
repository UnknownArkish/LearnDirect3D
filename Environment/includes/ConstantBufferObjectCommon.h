#include <DirectXMath.h>

struct BasePassConstantBuffer
{
	DirectX::XMMATRIX World;
	DirectX::XMMATRIX View;
	DirectX::XMMATRIX Projection;
};

struct ViewConstantBufferData
{
	DirectX::XMMATRIX World2View;
	DirectX::XMMATRIX View2Proj;

	DirectX::XMFLOAT3 ViewPosWS;
	float _pad_0;
};

struct ObjectConstantBufferData
{
	DirectX::XMMATRIX Local2World;
	DirectX::XMMATRIX World2Local;

	DirectX::XMFLOAT3 ObjectPosWS;
	float _pad_0;
};