#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

namespace VertexLayout 
{
	struct BasePassVertexLayout
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
	};

	extern D3D11_INPUT_ELEMENT_DESC BASE_PASS_INPUT_LAYOUT[];
}