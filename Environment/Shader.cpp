#include "includes\Shader.h"
#include "includes\D3DUtil.h"

Shader::Shader():
	_pVertexShader(nullptr), _pGeometryShader(nullptr), _pPixelShader(nullptr)
{
}

void Shader::Use(ID3D11DeviceContext* deviceContext)
{
	assert(deviceContext);

	deviceContext->VSSetShader(_pVertexShader.Get(), nullptr, 0);
	if(_pHullShader) deviceContext->HSSetShader(_pHullShader.Get(), nullptr, 0);
	if(_pDomainShader) deviceContext->DSSetShader(_pDomainShader.Get(), nullptr, 0);
	if(_pGeometryShader) deviceContext->GSSetShader(_pGeometryShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(_pPixelShader.Get(), nullptr, 0);
}

HRESULT Shader::VSDeclare(ID3D11Device* device, const SHADER_DECLARE_DESC& desc)
{
	HRESULT result = E_FAIL;
	if (device == nullptr) return result;

	ComPtr<ID3DBlob> blob;
	result = CreateShaderFromFile(desc.CsoName, desc.FileName, desc.EntryPoint, desc.ShaderModel, blob.GetAddressOf());
	if (SUCCEEDED(result))
	{
		result = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, _pVertexShader.ReleaseAndGetAddressOf());
	}
	return result;
}

HRESULT Shader::HSDeclare(ID3D11Device* device, const SHADER_DECLARE_DESC& desc)
{
	HRESULT result = E_FAIL;
	if (device == nullptr) return result;

	ComPtr<ID3DBlob> blob;
	result = CreateShaderFromFile(desc.CsoName, desc.FileName, desc.EntryPoint, desc.ShaderModel, blob.GetAddressOf());
	if (SUCCEEDED(result))
	{
		result = device->CreateHullShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, _pHullShader.ReleaseAndGetAddressOf());
	}
	return result;
}

HRESULT Shader::DSDeclare(ID3D11Device* device, const SHADER_DECLARE_DESC& desc)
{
	HRESULT result = E_FAIL;
	if (device == nullptr) return result;

	ComPtr<ID3DBlob> blob;
	result = CreateShaderFromFile(desc.CsoName, desc.FileName, desc.EntryPoint, desc.ShaderModel, blob.GetAddressOf());
	if (SUCCEEDED(result))
	{
		result = device->CreateDomainShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, _pDomainShader.ReleaseAndGetAddressOf());
	}
	return result;
}

HRESULT Shader::GSDeclare(ID3D11Device* device, const SHADER_DECLARE_DESC& desc)
{
	HRESULT result = E_FAIL;
	if (device == nullptr) return result;

	ComPtr<ID3DBlob> blob;
	result = CreateShaderFromFile(desc.CsoName, desc.FileName, desc.EntryPoint, desc.ShaderModel, blob.GetAddressOf());
	if (SUCCEEDED(result))
	{
		result = device->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, _pGeometryShader.ReleaseAndGetAddressOf());
	}
	return result;
}

HRESULT Shader::GSDeclareWithStreamOut(
	ID3D11Device* device, const SHADER_DECLARE_DESC& desc, 
	const D3D11_SO_DECLARATION_ENTRY* pSODeclaration, UINT numEntries
)
{
	HRESULT result = E_FAIL;
	if (device == nullptr) return result;

	ComPtr<ID3DBlob> blob;
	result = CreateShaderFromFile(desc.CsoName, desc.FileName, desc.EntryPoint, desc.ShaderModel, blob.GetAddressOf());
	if (SUCCEEDED(result))
	{
		result = device->CreateGeometryShaderWithStreamOutput(
			blob->GetBufferPointer(), blob->GetBufferSize(), 
			pSODeclaration, numEntries, nullptr, 0, D3D11_SO_NO_RASTERIZED_STREAM, 
			nullptr, _pGeometryShader.ReleaseAndGetAddressOf()
		);
	}

	return E_NOTIMPL;
}

HRESULT Shader::PSDeclare(ID3D11Device* device, const SHADER_DECLARE_DESC& desc)
{
	HRESULT result = E_FAIL;
	if (device == nullptr) return result;

	ComPtr<ID3DBlob> blob;
	result = CreateShaderFromFile(desc.CsoName, desc.FileName, desc.EntryPoint, desc.ShaderModel, blob.GetAddressOf());
	if (SUCCEEDED(result))
	{
		result = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, _pPixelShader.ReleaseAndGetAddressOf());
	}
	return result;
}

void ComputeShader::Use(ID3D11DeviceContext* deviceContext)
{
	assert(deviceContext);

	deviceContext->CSSetShader(_pComputeShader.Get(), nullptr, 0);
}

HRESULT ComputeShader::Declare(ID3D11Device* device, const SHADER_DECLARE_DESC& desc)
{
	HRESULT result = E_FAIL;
	if (device == nullptr) return result;

	ComPtr<ID3DBlob> blob;
	result = CreateShaderFromFile(desc.CsoName, desc.FileName, desc.EntryPoint, desc.ShaderModel, blob.GetAddressOf());
	if (SUCCEEDED(result))
	{
		result = device->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, _pComputeShader.ReleaseAndGetAddressOf());
	}
	return result;
}
