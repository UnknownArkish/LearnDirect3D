#pragma once

#include "Texture.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"

#include <DirectXMath.h>

class Texture2D : public Texture
{
public:
	Texture2D();
	~Texture2D();

	HRESULT Declare(ID3D11Device* device, UINT width, UINT height, DXGI_FORMAT format, bool useMipmap);
	HRESULT DeclareWithDDS(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const wchar_t* fileName, DirectX::DDS_ALPHA_MODE* alphaMode = nullptr);
	HRESULT DeclareWithWIC(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const wchar_t* fileName);

	void GetDesc(D3D11_TEXTURE2D_DESC& desc) { desc = _Desc; }

	virtual UINT GetWidth() const override;
	virtual UINT GetHeight() const override;
	virtual DXGI_FORMAT GetFormat() const override;

	void SetColors(const DirectX::XMFLOAT4* colors, UINT numColors);
	void Apply(ID3D11DeviceContext* deviceContext);
	void GetColors(DirectX::XMFLOAT4** output) const;
private:
	bool _IsDeclared;
	D3D11_TEXTURE2D_DESC _Desc;

	bool _CanWrite;
	DirectX::XMFLOAT4* _Datas;
	bool _Dirty;
};