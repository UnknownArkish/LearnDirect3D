#include "includes\Texture2D.h"
#include "includes\Common.h"

Texture2D::Texture2D() : 
	_IsDeclared(false), _Desc(),
	_CanWrite(false), _Datas(nullptr), _Dirty(true)
{
	ZeroMemory(&_Desc, sizeof(D3D11_TEXTURE2D_DESC));
}

Texture2D::~Texture2D()
{
	if (_Datas)
	{
		delete _Datas;
		_Datas = nullptr;
	}
}

HRESULT Texture2D::Declare(ID3D11Device* device, UINT width, UINT height, DXGI_FORMAT format, bool useMipmap)
{
	assert(!_IsDeclared);

	assert(device);

	HRESULT result = E_NOTIMPL;

	_Desc.Width = width;
	_Desc.Height = height;
	_Desc.MipLevels = useMipmap ? 0 : 1;
	_Desc.ArraySize = 1;
	_Desc.Format = format;
	_Desc.SampleDesc.Count = 1;
	_Desc.SampleDesc.Quality = 0;
	_Desc.Usage = D3D11_USAGE_DEFAULT;			// GPU read/ write
	_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS | (useMipmap ? D3D11_BIND_RENDER_TARGET : 0);
	_Desc.CPUAccessFlags = 0;					// CPU not read/ write
	if (useMipmap) _Desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	_Datas = new DirectX::XMFLOAT4[(size_t)width * height];
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
	initData.pSysMem = _Datas;
	ID3D11Texture2D* pTexture2D;
	result = device->CreateTexture2D(&_Desc, nullptr, &pTexture2D);
	if (SUCCEEDED(result))
	{
		_pResource = pTexture2D;
		_IsDeclared = true;
		_CanWrite = true;
	}
	else
	{
		delete[] _Datas;
	}

	return result;
}

HRESULT Texture2D::DeclareWithDDS(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const wchar_t* fileName, DirectX::DDS_ALPHA_MODE* alphaMode)
{
	assert(!_IsDeclared);
	assert(device);

	HRESULT result = DirectX::CreateDDSTextureFromFile(device, fileName, _pResource.GetAddressOf(), _pResourceView.GetAddressOf(), 0, alphaMode);
	if (SUCCEEDED(result))
	{
		ComPtr<ID3D11Texture2D> pTexture2D;
		_pResource.As(&pTexture2D);
		pTexture2D->GetDesc(&_Desc);

		_CanWrite = false;
		_Datas = nullptr;
		_IsDeclared = true;
	}
	return result;
}

HRESULT Texture2D::DeclareWithWIC(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const wchar_t* fileName)
{
	assert(!_IsDeclared);
	assert(device);

	HRESULT result = DirectX::CreateWICTextureFromFile(device, deviceContext, fileName, _pResource.ReleaseAndGetAddressOf(), _pResourceView.GetAddressOf(), 0);
	if (SUCCEEDED(result))
	{
		ComPtr<ID3D11Texture2D> pTexture2D;
		_pResource.As(&pTexture2D);
		pTexture2D->GetDesc(&_Desc);

		_CanWrite = false;
		_Datas = nullptr;
		_IsDeclared = true;
	}
	return result;
}

UINT Texture2D::GetWidth() const
{
	assert(_IsDeclared);
	return _Desc.Width;
}

UINT Texture2D::GetHeight() const
{
	assert(_IsDeclared);
	return _Desc.Height;
}

DXGI_FORMAT Texture2D::GetFormat() const
{
	assert(_IsDeclared);
	return _Desc.Format;
}

void Texture2D::SetColors(const DirectX::XMFLOAT4* colors, UINT numColors)
{
	assert(_IsDeclared);

	assert(colors);
	assert(_CanWrite && _Datas != nullptr);

	UINT size = GetWidth() * GetHeight();
	numColors = DirectX::XMMin(size, numColors);
	memcpy_s(_Datas, size, colors, size);
	_Dirty = true;
}

void Texture2D::Apply(ID3D11DeviceContext* deviceContext)
{
	assert(_IsDeclared);

	if (!_CanWrite) return;
	if (_Dirty)
	{
		assert(deviceContext);
		assert(_Datas != nullptr);

		deviceContext->UpdateSubresource(
			_pResource.Get(), 0, nullptr,
			_Datas,
			GetWidth() * sizeof(DirectX::XMFLOAT4),
			GetWidth() * GetHeight() * sizeof(DirectX::XMFLOAT4)
		);

		_Dirty = false;
	}
}

void Texture2D::GetColors(DirectX::XMFLOAT4** output) const
{
	assert(_IsDeclared);

	assert(*output);
	if (_CanWrite)
	{
		assert(_Datas != nullptr);

		UINT size = GetWidth() * GetHeight();
		memcpy_s(*output, size, _Datas, size);
	}
}