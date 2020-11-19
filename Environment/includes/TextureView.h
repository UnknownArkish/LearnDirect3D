#pragma once

#include <d3d11.h>

class Texture;
#include "Texture.h"

template<UINT slot>
class TextureView
{
public:
	friend class Texture;
public:
	virtual HRESULT Declare(ID3D11Device* device, const D3D11_SHADER_RESOURCE_VIEW_DESC& desc, const Texture* texture) = 0;
};