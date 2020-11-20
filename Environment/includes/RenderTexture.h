#pragma once

#include "Texture.h"

class RenderTexture : public Texture
{
public:
	RenderTexture();
	HRESULT Declare(ID3D11Device* device, UINT width, UINT height, DXGI_FORMAT format, bool useMipmap);

	// 通过 Texture 继承
	virtual UINT GetWidth() const override;
	virtual UINT GetHeight() const override;
	virtual DXGI_FORMAT GetFormat() const override;
	void GetDesc(D3D11_TEXTURE2D_DESC& output)const { output = _Desc; }

	void BeginRender(ID3D11DeviceContext* deviceContext);
	void EndRender(ID3D11DeviceContext* deviceContext);
private:
	bool _IsDeclared;
	D3D11_TEXTURE2D_DESC _Desc;
	bool _UseMipmap;

	ComPtr<ID3D11Resource> _pDepthResource;

	ComPtr<ID3D11RenderTargetView> _pRenderTargetView;
	ComPtr<ID3D11DepthStencilView> _pDepthStencilView;
	D3D11_VIEWPORT _ViewPort;

	ComPtr<ID3D11RenderTargetView> _pCacheRTV;
	ComPtr<ID3D11DepthStencilView> _pCacheDSV;
	D3D11_VIEWPORT _CacheVP;
};