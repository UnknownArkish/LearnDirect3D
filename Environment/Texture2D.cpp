#include "includes\Texture2D.h"

Texture2D::Texture2D() : 
	_IsDeclared(false), _Desc(),
	_CanWrite(false), _ColorDatas(nullptr), _Dirty(true)
{
	ZeroMemory(&_Desc, sizeof(D3D11_TEXTURE2D_DESC));
}

Texture2D::~Texture2D()
{
	if (_ColorDatas)
	{
		delete _ColorDatas;
		_ColorDatas = nullptr;
	}
}

HRESULT Texture2D::Declare(ID3D11Device* device, UINT width, UINT height, DXGI_FORMAT format, bool useMipmap)
{
	if (_IsDeclared) return 0;

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
	_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | (useMipmap ? D3D11_BIND_RENDER_TARGET : 0);
	_Desc.CPUAccessFlags = 0;					// CPU not read/ write
	if (useMipmap) _Desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	ID3D11Texture2D* pTexture2D;
	result = device->CreateTexture2D(&_Desc, nullptr, &pTexture2D);
	if (SUCCEEDED(result))
	{
		_pResource = pTexture2D;
		_IsDeclared = true;

		_ColorDatas = new DirectX::XMFLOAT4[(size_t)width * height];
		_CanWrite = true;
	}

	return result;
}

HRESULT Texture2D::DeclareWithDDS(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const wchar_t* fileName, bool sRGB, DirectX::DDS_ALPHA_MODE* alphaMode)
{

	_CanWrite = false;
	_ColorDatas = nullptr;
	return 0;
}

HRESULT Texture2D::DeclareWithWIC(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const wchar_t* fileName)
{
	if (_IsDeclared) return 0;

	HRESULT result = DirectX::CreateWICTextureFromFile(device, fileName, _pResource.ReleaseAndGetAddressOf(), nullptr, 0);
	if (SUCCEEDED(result))
	{
		_CanWrite = false;
		_ColorDatas = nullptr;
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
	assert(_CanWrite && _ColorDatas != nullptr);

	UINT size = GetWidth() * GetHeight();
	numColors = DirectX::XMMin(size, numColors);
	memcpy_s(_ColorDatas, size, colors, size);
	_Dirty = true;
}

void Texture2D::Apply(ID3D11DeviceContext* deviceContext)
{
	assert(_IsDeclared);

	if (!_CanWrite) return;
	if (_Dirty)
	{
		assert(deviceContext);
		assert(_ColorDatas != nullptr);

		deviceContext->UpdateSubresource(
			_pResource.Get(), 0, nullptr,
			_ColorDatas,
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
		assert(_ColorDatas != nullptr);

		UINT size = GetWidth() * GetHeight();
		memcpy_s(*output, size, _ColorDatas, size);
	}
}