#pragma once

#include <d3d11.h>
#include "Common.h"
#include <assert.h>

#include "Texture.h"

class ITextureView
{
public:
	virtual void VSBind(ID3D11DeviceContext* deviceContext) = 0;
	virtual void GSBind(ID3D11DeviceContext* deviceContext) = 0;
	virtual void PSBind(ID3D11DeviceContext* deviceContext) = 0;
	virtual void Declare(Texture* texture) = 0;
protected:
	ID3D11ShaderResourceView** GetShaderResourceViews()
	{
		return _Texture != NULL ?
			_Texture->_pResourceView.GetAddressOf() :
			nullptr;
	}
protected:
	Texture* _Texture;
};

template<UINT slot>
class TextureView : public ITextureView
{
public:
	virtual void VSBind(ID3D11DeviceContext* deviceContext) override;
	virtual void GSBind(ID3D11DeviceContext* deviceContext) override;
	virtual void PSBind(ID3D11DeviceContext* deviceContext) override;
	virtual void Declare(Texture* texture) override;
};

template<UINT slot>
inline void TextureView<slot>::VSBind(ID3D11DeviceContext* deviceContext)
{
	assert(deviceContext);
	if (_Texture)
	{
		deviceContext->VSSetShaderResources(slot, 1, GetShaderResourceViews());
	}
}

template<UINT slot>
inline void TextureView<slot>::GSBind(ID3D11DeviceContext* deviceContext)
{
	assert(deviceContext);
	if (_Texture)
	{
		deviceContext->GSSetShaderResources(slot, 1, GetShaderResourceViews());
	}
}

template<UINT slot>
inline void TextureView<slot>::PSBind(ID3D11DeviceContext* deviceContext)
{
	assert(deviceContext);
	if (_Texture)
	{
		deviceContext->PSSetShaderResources(slot, 1, GetShaderResourceViews());
	}
}

template<UINT slot>
inline void TextureView<slot>::Declare(Texture* texture)
{
	_Texture = texture;
}
