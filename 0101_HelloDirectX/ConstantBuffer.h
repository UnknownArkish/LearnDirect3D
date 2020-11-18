#pragma once

#include <DirectXMath.h>

namespace ConstantBuffer
{
	struct BasePassBuffer
	{
		DirectX::XMMATRIX World;
		DirectX::XMMATRIX View;
		DirectX::XMMATRIX Projection;
	};
}