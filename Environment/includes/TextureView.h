#pragma once

#include <d3d11.h>
#include "Common.h"
#include <assert.h>

#include "Texture.h"

class ITextureView
{
public:
	virtual void VSBind(ID3D11DeviceContext* deviceContext, UINT slot) = 0;
	virtual void HSBind(ID3D11DeviceContext* deviceContext, UINT slot) = 0;
	virtual void DSBind(ID3D11DeviceContext* deviceContext, UINT slot) = 0;
	virtual void GSBind(ID3D11DeviceContext* deviceContext, UINT slot) = 0;
	virtual void PSBind(ID3D11DeviceContext* deviceContext, UINT slot) = 0;
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

class TextureView : public ITextureView
{
public:
	virtual void VSBind(ID3D11DeviceContext* deviceContext, UINT slot) override;
	virtual void HSBind(ID3D11DeviceContext* deviceContext, UINT slot) override;
	virtual void DSBind(ID3D11DeviceContext* deviceContext, UINT slot) override;
	virtual void GSBind(ID3D11DeviceContext* deviceContext, UINT slot) override;
	virtual void PSBind(ID3D11DeviceContext* deviceContext, UINT slot) override;
	virtual void Declare(Texture* texture) override;
};


inline void TextureView::VSBind(ID3D11DeviceContext* deviceContext, UINT slot)
{
	assert(deviceContext);
	if (_Texture)
	{
		deviceContext->VSSetShaderResources(slot, 1, GetShaderResourceViews());
	}
}
inline void TextureView::HSBind(ID3D11DeviceContext* deviceContext, UINT slot)
{
	assert(deviceContext);
	if (_Texture)
	{
		deviceContext->HSSetShaderResources(slot, 1, GetShaderResourceViews());
	}
}
inline void TextureView::DSBind(ID3D11DeviceContext* deviceContext, UINT slot)
{
	assert(deviceContext);
	if (_Texture)
	{
		deviceContext->DSSetShaderResources(slot, 1, GetShaderResourceViews());
	}
}


inline void TextureView::GSBind(ID3D11DeviceContext* deviceContext, UINT slot)
{
	assert(deviceContext);
	if (_Texture)
	{
		deviceContext->GSSetShaderResources(slot, 1, GetShaderResourceViews());
	}
}


inline void TextureView::PSBind(ID3D11DeviceContext* deviceContext, UINT slot)
{
	assert(deviceContext);
	if (_Texture)
	{
		deviceContext->PSSetShaderResources(slot, 1, GetShaderResourceViews());
	}
}


inline void TextureView::Declare(Texture* texture)
{
	_Texture = texture;
}


class UnorderedAccessView
{
public:
	void Declare(Texture* texture);
	void Bind(ID3D11DeviceContext* deviceContext, UINT slot);
private:
	ComPtr<ID3D11UnorderedAccessView> _UAV;
};