#include "includes/TextureLoader.h"


HRESULT TextureLoader::LoadFromDDS(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const wchar_t* fileName, Texture2D* outputTex, ITextureView* outputView, DirectX::DDS_ALPHA_MODE* alphaMode)
{
	assert(device);
	assert(deviceContext);

	assert(outputTex);
	assert(!outputTex->_IsDeclared);

	HRESULT result = DirectX::CreateDDSTextureFromFile(
		device, deviceContext, fileName,
		outputTex->_pResource.ReleaseAndGetAddressOf(),
		outputView != nullptr ? outputView->_pResourceView.ReleaseAndGetAddressOf() : nullptr,
		0, alphaMode);
	if (SUCCEEDED(result))
	{
		ComPtr<ID3D11Texture2D> pTexture2D;
		outputTex->_pResource.As(&pTexture2D);
		pTexture2D->GetDesc(&outputTex->_Desc);

		outputTex->_CanWrite = false;
		outputTex->_Datas = nullptr;
		outputTex->_IsDeclared = true;
	}
	return result;
}

HRESULT TextureLoader::LoadFromWIC(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const wchar_t* fileName, Texture2D* outputTex, ITextureView* outputView)
{
	assert(device);
	assert(deviceContext);

	assert(outputTex);
	assert(!outputTex->_IsDeclared);

	HRESULT result = DirectX::CreateWICTextureFromFile(
		device, deviceContext, fileName,
		outputTex->_pResource.ReleaseAndGetAddressOf(),
		outputView != nullptr ? outputView->_pResourceView.ReleaseAndGetAddressOf() : nullptr,
		0);
	if (SUCCEEDED(result))
	{
		ComPtr<ID3D11Texture2D> pTexture2D;
		outputTex->_pResource.As(&pTexture2D);
		pTexture2D->GetDesc(&outputTex->_Desc);

		outputTex->_CanWrite = false;
		outputTex->_Datas = nullptr;
		outputTex->_IsDeclared = true;
	}
	return result;
}
