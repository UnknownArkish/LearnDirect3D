#include "includes\RenderTexture.h"

RenderTexture::RenderTexture() : Texture(),
	_IsDeclared(false), _Desc()
{
	ZeroMemory(&_Desc, sizeof(D3D11_TEXTURE2D_DESC));
}

HRESULT RenderTexture::Declare(UINT width, UINT height, DXGI_FORMAT format)
{
	if (_IsDeclared) return;

	_Desc.Width = width;
	_Desc.Height = height;
	_Desc.Format = format;
	_Desc.ArraySize = 1;

	_IsDeclared = true;
	return E_NOTIMPL;
}

UINT RenderTexture::GetWidth()
{
	return _Desc.Width;
}

UINT RenderTexture::GetHeight()
{
	return _Desc.Height;
}

DXGI_FORMAT RenderTexture::GetFormat()
{
	return _Desc.Format;
}
