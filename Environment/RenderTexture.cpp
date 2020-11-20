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
	if (useMipmap) _Desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	_UseMipmap = useMipmap;

	ComPtr<ID3D11Texture2D> pTexture2D;
	result = device->CreateTexture2D(&_Desc, nullptr, pTexture2D.GetAddressOf());
	if (FAILED(result)) return result;

	_pResource = pTexture2D;

	// RTV
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = _Desc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	result = device->CreateRenderTargetView(_pResource.Get(), &rtvDesc, _pRenderTargetView.GetAddressOf());
	if (FAILED(result)) return result;

	// SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = _Desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = -1;    // 使用所有的mip等级
	result = device->CreateShaderResourceView(_pResource.Get(), &srvDesc, _pResourceView.GetAddressOf());
	if (FAILED(result)) return result;

	// Depth Texture
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 0;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	ComPtr<ID3D11Texture2D> depthTex;
	result = device->CreateTexture2D(&texDesc, nullptr, depthTex.GetAddressOf());
	if (FAILED(result)) return result;

	_pDepthResource = depthTex;

	// dsv
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Format = texDesc.Format;
	dsvDesc.Flags = 0;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	result = device->CreateDepthStencilView(_pDepthResource.Get(), &dsvDesc, _pDepthStencilView.GetAddressOf());
	if (FAILED(result)) return result;


	// VP
	_ViewPort.TopLeftX = 0.0f;
	_ViewPort.TopLeftY = 0.0f;
	_ViewPort.Width = static_cast<float>(width);
	_ViewPort.Height = static_cast<float>(height);
	_ViewPort.MinDepth = 0.0f;
	_ViewPort.MaxDepth = 1.0f;

	_IsDeclared = true;
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

void RenderTexture::BeginRender(ID3D11DeviceContext* deviceContext)
{
	assert(deviceContext);

	deviceContext->OMGetRenderTargets(1, _pCacheRTV.GetAddressOf(), _pCacheDSV.GetAddressOf());
	UINT num_Viewports = 1;
	deviceContext->RSGetViewports(&num_Viewports, &_CacheVP);


	float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	deviceContext->ClearRenderTargetView(_pRenderTargetView.Get(), black);
	deviceContext->ClearDepthStencilView(_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	deviceContext->OMSetRenderTargets(1, _pRenderTargetView.GetAddressOf(), _pDepthStencilView.Get());
	deviceContext->RSSetViewports(1, &_ViewPort);
}

void RenderTexture::EndRender(ID3D11DeviceContext* deviceContext)
{
	// 恢复默认设定
	deviceContext->RSSetViewports(1, &_CacheVP);
	deviceContext->OMSetRenderTargets(1, _pCacheRTV.GetAddressOf(), _pCacheDSV.Get());

	// 若之前有指定需要mipmap链，则生成
	if (_UseMipmap)
	{
		deviceContext->GenerateMips(_pResourceView.Get());
	}

	// 清空临时缓存的渲染目标视图和深度模板视图
	_pCacheRTV.Reset();
	_pCacheDSV.Reset();
}
