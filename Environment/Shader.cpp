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
	deviceContext->GSSetShader(_pGeometryShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(_pPixelShader.Get(), nullptr, 0);
}

HRESULT Shader::VSDeclare(ID3D11Device* device, const ShaderDeclareDesc& desc)
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

HRESULT Shader::GSDeclare(ID3D11Device* device, const ShaderDeclareDesc& desc)
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
	ID3D11Device* device, const ShaderDeclareDesc& desc, 
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

HRESULT Shader::PSDeclare(ID3D11Device* device, const ShaderDeclareDesc& desc)
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
