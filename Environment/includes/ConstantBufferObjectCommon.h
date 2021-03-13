#include <DirectXMath.h>

struct BasePassConstantBuffer
{
	DirectX::XMMATRIX World;
	DirectX::XMMATRIX View;
	DirectX::XMMATRIX Projection;
};

struct ViewConstantBufferData
{
	ViewConstantBufferData()
		: World2View(), View2Proj(), 
		ViewPosWS(0.0f, 0.0f, 0.0f), 
		ScreenSize(0.0f, 0.0f),

		_pad_0(), _pad_1()
	{}

	DirectX::XMMATRIX World2View;
	DirectX::XMMATRIX View2Proj;

	DirectX::XMFLOAT3 ViewPosWS;
	float _pad_0;

	DirectX::XMFLOAT2 ScreenSize;
	DirectX::XMFLOAT2 _pad_1;
};

struct ObjectConstantBufferData
{
	DirectX::XMMATRIX Local2World;
	DirectX::XMMATRIX World2Local;

	DirectX::XMFLOAT3 ObjectPosWS;
	float _pad_0;
};