#include "includes\RenderTexture.h"

RenderTexture::RenderTexture() : Texture(),
	_IsDeclared(false), _Desc()
{
	ZeroMemory(&_Desc, sizeof(D3D11_TEXTURE2D_DESC));
}

HRESULT RenderTexture::Declare(ID3D11Device* device, UINT width, UINT height, DXGI_FORMAT format, bool useMipmap)
{
	if (_IsDeclared) return E_NOTIMPL;

	HRESULT result = E_NOTIMPL;

	_Desc.Width = width;
	_Desc.Height = height;
	_Desc.MipLevels = useMipmap ? 0 : 1;
	_Desc.ArraySize = 1;
	_Desc.Format = format;
	_Desc.SampleDesc.Count = 1;
	_Desc.SampleDesc.Quality = 0;
	_Desc.Usage = D3D11_USAGE_DEFAULT;			// GPU read/ write
	_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	_Desc.CPUAccessFlags = 0;					// CPU not read/ write
	if(useMipmap) _Desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	ID3D11Texture2D* pTexture2D;
	result = device->CreateTexture2D(&_Desc, nullptr, &pTexture2D);
	if (SUCCEEDED(result))
	{
		_pResource = pTexture2D;
		_IsDeclared = true;
	}
	return result;
}

UINT RenderTexture::GetWidth() const
{
	return _Desc.Width;
}

UINT RenderTexture::GetHeight() const
{
	return _Desc.Height;
}

DXGI_FORMAT RenderTexture::GetFormat() const
{
	return _Desc.Format;
}
