#pragma once

#include <d3d11.h>
#include "Common.h"
#include <assert.h>
#include <string>

class ITextureView;
class TextureLoader;

class Texture
{
public:

	virtual UINT GetWidth() const = 0;
	virtual UINT GetHeight() const = 0;
	virtual DXGI_FORMAT GetFormat() const = 0;

	template<UINT TNameLength>
	void SetDebugName(_In_ const char(&name)[TNameLength])
	{
		assert(_pResource);
		D3D11SetDebugObjectName(_pResource.Get(), name);
	}
protected:
	Texture() : _pResource(nullptr){}
protected:
	friend class ITextureView;
	friend class TextureLoader;
	ComPtr<ID3D11Resource> _pResource;
};

#include "TextureView.h"
#include "TextureLoader.h"