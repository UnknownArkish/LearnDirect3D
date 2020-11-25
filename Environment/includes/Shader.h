#pragma once

#include <d3d11.h>
#include "D3DUtil.h"
#include "Common.h"

struct ShaderDeclareDesc
{
	const WCHAR* CsoName;
	const WCHAR* FileName;
	LPCSTR EntryPoint;
	LPCSTR ShaderModel;
};

class Shader
{
public:
	Shader();

	void Use(ID3D11DeviceContext* deviceContext);

	HRESULT VSDeclare(ID3D11Device* device, const ShaderDeclareDesc& desc);

	HRESULT HSDeclare(ID3D11Device* device, const ShaderDeclareDesc& desc);
	HRESULT DSDeclare(ID3D11Device* device, const ShaderDeclareDesc& desc);

	HRESULT GSDeclare(ID3D11Device* device, const ShaderDeclareDesc& desc);
	HRESULT GSDeclareWithStreamOut(ID3D11Device* device, const ShaderDeclareDesc& desc, const D3D11_SO_DECLARATION_ENTRY* pSODeclaration, UINT numEntries);
	HRESULT PSDeclare(ID3D11Device* device, const ShaderDeclareDesc& desc);

	template<UINT TNameLength>
	void VSSetDebugName(_In_ const char(&name)[TNameLength]) { SetDebugNameInternal(_pVertexShader.Get(), name); }
	template<UINT TNameLength>
	void GSSetDebugName(_In_ const char(&name)[TNameLength]) { SetDebugNameInternal(_pGeometryShader.Get(), name); }
	template<UINT TNameLength>
	void PSSetDebugName(_In_ const char(&name)[TNameLength]) { SetDebugNameInternal(_pPixelShader.Get(), name); }
protected:
private:
	template<UINT TNameLength>
	void SetDebugNameInternal(_In_ ID3D11DeviceChild* resource, _In_ const char(&name)[TNameLength])
	{
		assert(resource);
		D3D11SetDebugObjectName(resource, name);
	}
private:
	ComPtr<ID3D11VertexShader> _pVertexShader;
	ComPtr<ID3D11HullShader> _pHullShader;
	ComPtr<ID3D11DomainShader> _pDomainShader;
	ComPtr<ID3D11GeometryShader> _pGeometryShader;
	ComPtr<ID3D11PixelShader> _pPixelShader;
};