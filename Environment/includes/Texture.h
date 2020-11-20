#pragma once

#include <d3d11.h>
#include "Common.h"
#include <assert.h>
#include <string>

class ITextureView;

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
		if (_pResourceView) D3D11SetDebugObjectName(_pResourceView.Get(), std::strcat(name, "_View"));
	}
protected:
	Texture() : _pResource(nullptr), _pResourceView(nullptr) {}
protected:
	friend class ITextureView;
	ComPtr<ID3D11Resource> _pResource;
	ComPtr<ID3D11ShaderResourceView> _pResourceView;
};