#pragma once

#include <assert.h>
#include <d3d11.h>
#include "Common.h"

template<UINT slot>
class SamplerState
{
public:
	SamplerState() :_Desc() { ZeroMemory(&_Desc, sizeof(D3D11_SAMPLER_DESC)); }

	HRESULT Delcare(ID3D11Device* device, const D3D11_SAMPLER_DESC& desc);
	void GetDesc(D3D11_SAMPLER_DESC& output) const { output = _Desc; }

	void VSBind(ID3D11DeviceContext* deviceContext);
	void GSBind(ID3D11DeviceContext* deviceContext);
	void PSBind(ID3D11DeviceContext* deviceContext);

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

template<UINT slot>
inline HRESULT SamplerState<slot>::Delcare(ID3D11Device* device, const D3D11_SAMPLER_DESC& desc)
{
	assert(device);
	_Desc = desc;
	return device->CreateSamplerState(desc, _pSamplerState.ReleaseAndGetAddressOf());
}

template<UINT slot>
inline void SamplerState<slot>::VSBind(ID3D11DeviceContext* deviceContext)
{
	assert(deviceContext);
	deviceContext->VSSetSamplers(slot, 1, _pSamplerState.GetAddressOf());
}

template<UINT slot>
inline void SamplerState<slot>::GSBind(ID3D11DeviceContext* deviceContext)
{
	assert(deviceContext);
	deviceContext->GSSetSamplers(slot, 1, _pSamplerState.GetAddressOf());
}

template<UINT slot>
inline void SamplerState<slot>::PSBind(ID3D11DeviceContext* deviceContext)
{
	assert(deviceContext);
	deviceContext->PSSetSamplers(slot, 1, _pSamplerState.GetAddressOf());
}
