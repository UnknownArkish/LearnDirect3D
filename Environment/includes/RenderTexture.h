#pragma once

#include "Texture.h"

class RenderTexture : public Texture
{
public:
	RenderTexture();
	HRESULT Declare(UINT width, UINT height, DXGI_FORMAT format);

	// Í¨¹ý Texture ¼Ì³Ð
	virtual UINT GetWidth() override;
	virtual UINT GetHeight() override;
	virtual DXGI_FORMAT GetFormat() override;
	void GetDesc(D3D11_TEXTURE2D_DESC& output)const { output = _Desc; }
private:
	DXGI_FORMAT _Format;
	bool _IsDeclared;
	D3D11_TEXTURE2D_DESC _Desc;

};