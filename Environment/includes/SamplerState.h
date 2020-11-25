#pragma once

#include <assert.h>
#include <d3d11.h>
#include "Common.h"

class SamplerState
{
public:
	SamplerState() :_Desc() 
	{ 
		ZeroMemory(&_Desc, sizeof(D3D11_SAMPLER_DESC)); 
		_Desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		_Desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		_Desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		_Desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		_Desc.MinLOD = 0;
		_Desc.MaxAnisotropy = 1;
		_Desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		_Desc.MinLOD = 0;
		_Desc.MaxLOD = D3D11_FLOAT32_MAX;
	}

	HRESULT Delcare(ID3D11Device* device, const D3D11_SAMPLER_DESC& desc);
	void GetDesc(D3D11_SAMPLER_DESC& output) const { output = _Desc; }

	void VSBind(ID3D11DeviceContext* deviceContext, UINT slot);
	void HSBind(ID3D11DeviceContext* deviceContext, UINT slot);
	void DSBind(ID3D11DeviceContext* deviceContext, UINT slot);
	void GSBind(ID3D11DeviceContext* deviceContext, UINT slot);
	void PSBind(ID3D11DeviceContext* deviceContext, UINT slot);

	template<UINT TNameLength>
	void SetDebugName(_In_ const char(&name)[TNameLength])
	{
		assert(_pSamplerState);
		D3D11SetDebugObjectName(_pSamplerState.Get(), name);
	}
private:
	D3D11_SAMPLER_DESC _Desc;
	ComPtr<ID3D11SamplerState> _pSamplerState;
};

inline HRESULT SamplerState::Delcare(ID3D11Device* device, const D3D11_SAMPLER_DESC& desc)
{
	assert(device);
	_Desc = desc;
	return device->CreateSamplerState(&desc, _pSamplerState.ReleaseAndGetAddressOf());
}

inline void SamplerState::VSBind(ID3D11DeviceContext* deviceContext, UINT slot)
{
	assert(deviceContext);
	deviceContext->VSSetSamplers(slot, 1, _pSamplerState.GetAddressOf());
}

inline void SamplerState::HSBind(ID3D11DeviceContext* deviceContext, UINT slot)
{
	assert(deviceContext);
	deviceContext->HSSetSamplers(slot, 1, _pSamplerState.GetAddressOf());
}

inline void SamplerState::DSBind(ID3D11DeviceContext* deviceContext, UINT slot)
{
	assert(deviceContext);
	deviceContext->DSSetSamplers(slot, 1, _pSamplerState.GetAddressOf());
}

inline void SamplerState::GSBind(ID3D11DeviceContext* deviceContext, UINT slot)
{
	assert(deviceContext);
	deviceContext->GSSetSamplers(slot, 1, _pSamplerState.GetAddressOf());
}

inline void SamplerState::PSBind(ID3D11DeviceContext* deviceContext, UINT slot)
{
	assert(deviceContext);
	deviceContext->PSSetSamplers(slot, 1, _pSamplerState.GetAddressOf());
}
