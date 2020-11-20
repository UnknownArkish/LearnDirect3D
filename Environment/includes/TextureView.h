#pragma once

#include <d3d11.h>
#include "Common.h"
#include <assert.h>

#include "Texture.h"
#include "TextureLoader.h"

class ITextureView
{
public:
	virtual void VSBind(ID3D11DeviceContext* deviceContext) = 0;
	virtual void GSBind(ID3D11DeviceContext* deviceContext) = 0;
	virtual void PSBind(ID3D11DeviceContext* deviceContext) = 0;
	virtual void Declare(ID3D11Device* device, D3D11_SHADER_RESOURCE_VIEW_DESC& desc, Texture* texture) = 0;
protected:
	ID3D11Resource* GetTextureResource(Texture* texture)
	{
		return texture != nullptr ?
			texture->_pResource.Get() :
			nullptr;
	}
protected:
	friend class TextureLoader;
	ComPtr<ID3D11ShaderResourceView> _pResourceView;
};


template<UINT slot>
class TextureView : public ITextureView
{
public:
	virtual void VSBind(ID3D11DeviceContext* deviceContext) override;
	virtual void GSBind(ID3D11DeviceContext* deviceContext) override;
	virtual void PSBind(ID3D11DeviceContext* deviceContext) override;
	virtual void Declare(ID3D11Device* device, D3D11_SHADER_RESOURCE_VIEW_DESC& desc, Texture* texture) override;
};

template<UINT slot>
inline void TextureView<slot>::VSBind(ID3D11DeviceContext* deviceContext)
{
	assert(deviceContext);
	deviceContext->VSSetShaderResources(slot, 1, _pResourceView.GetAddressOf());
}
template<UINT slot>
inline void TextureView<slot>::GSBind(ID3D11DeviceContext* deviceContext)
{
	assert(deviceContext);
	deviceContext->GSSetShaderResources(slot, 1, _pResourceView.GetAddressOf());
}

template<UINT slot>
inline void TextureView<slot>::PSBind(ID3D11DeviceContext* deviceContext)
{
	assert(deviceContext);
	deviceContext->PSSetShaderResources(slot, 1, _pResourceView.GetAddressOf());
}

template<UINT slot>
inline void TextureView<slot>::Declare(ID3D11Device* device, D3D11_SHADER_RESOURCE_VIEW_DESC& desc, Texture* texture)
{
	assert(device);
	device->CreateShaderResourceView(GetTextureResource(texture), &desc, _pResourceView.ReleaseAndGetAddressOf());
}