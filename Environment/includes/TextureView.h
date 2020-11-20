#pragma once

#include <d3d11.h>
#include "Common.h"

class Texture;
#include "Texture.h"

template<UINT slot>
class TextureView
{
public:
	friend class Texture;

	void VSBind(ID3D11DeviceContext* deviceContext);
	void GSBind(ID3D11DeviceContext* deviceContext);
	void PSBind(ID3D11DeviceContext* deviceContext);
public:
	HRESULT Declare(ID3D11Device* device, const D3D11_SHADER_RESOURCE_VIEW_DESC& desc, const Texture* texture);
private:
	ComPtr<ID3D11ShaderResourceView> _ResourceView;
};

template<UINT slot>
inline void TextureView<slot>::VSBind(ID3D11DeviceContext* deviceContext)
{
	assert(deviceContext);
	deviceContext->VSSetShaderResources(slot, 1, _ResourceView.Get());
}

template<UINT slot>
inline void TextureView<slot>::GSBind(ID3D11DeviceContext* deviceContext)
{
	assert(deviceContext);
	deviceContext->GSSetShaderResources(slot, 1, _ResourceView.Get());
}

template<UINT slot>
inline void TextureView<slot>::PSBind(ID3D11DeviceContext* deviceContext)
{
	assert(deviceContext);
	deviceContext->PSSetShaderResources(slot, 1, _ResourceView.Get());
}

template<UINT slot>
inline HRESULT TextureView<slot>::Declare(ID3D11Device* device, const D3D11_SHADER_RESOURCE_VIEW_DESC& desc, const Texture* texture)
{
	assert(device);
	device->CreateShaderResourceView(texture->_pResource.Get(), desc, _ResourceView.ReleaseAndGetAddressOf());
}
