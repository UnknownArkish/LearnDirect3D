#pragma once

#include <d3d11.h>
#include "Common.h"

class Texture
{
public:
	virtual UINT GetWidth() = 0;
	virtual UINT GetHeight() = 0;
	virtual DXGI_FORMAT GetFormat() = 0;
protected:
	Texture() : _pResource(nullptr) {}

protected:
	ComPtr<ID3D11Resource> _pResource;
};