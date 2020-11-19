#pragma once

#include <d3d11.h>
#include "DXTrace.h"
#include "Common.h"

template<UINT startSlot, class T>
class ConstantBuffer {
public:
	ConstantBuffer() : _Data(), _Dirty(true) {}

	HRESULT Declare(ID3D11Device* device);

	void SetBuffer(const T& data)
	{ 
		_Data = data; 
		_Dirty = true; 
	}
	void GetBuffer(T& output) { output = _Data; }

	void VSBind(ID3D11DeviceContext* deviceContext);
	void GSBind(ID3D11DeviceContext* deviceContext);
	void PSBind(ID3D11DeviceContext* deviceContext);

	void Apply(ID3D11DeviceContext* deviceContext);

	template<UINT TNameLength>
	void SetDebugName(_In_ const char(&name)[TNameLength])
	{
		assert(_Buffer);
		D3D11SetDebugObjectName(_Buffer.Get(), name);
	}
private:
	ComPtr<ID3D11Buffer> _Buffer;
	T _Data;

	bool _Dirty;
};

template<UINT startSlot, class T>
inline HRESULT ConstantBuffer<startSlot, T>::Declare(ID3D11Device* device)
{
	if (_Buffer != nullptr)
		return S_OK;
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.ByteWidth = sizeof(T);
	return device->CreateBuffer(&cbd, nullptr, _Buffer.GetAddressOf());
}

template<UINT startSlot, class T>
inline void ConstantBuffer<startSlot, T>::VSBind(ID3D11DeviceContext* deviceContext)
{
	deviceContext->VSSetConstantBuffers(startSlot, 1, _Buffer.GetAddressOf());
}

template<UINT startSlot, class T>
inline void ConstantBuffer<startSlot, T>::GSBind(ID3D11DeviceContext* deviceContext)
{
	deviceContext->GSSetConstantBuffers(startSlot, 1, _Buffer.GetAddressOf());
}

template<UINT startSlot, class T>
inline void ConstantBuffer<startSlot, T>::PSBind(ID3D11DeviceContext* deviceContext)
{
	deviceContext->PSSetConstantBuffers(startSlot, 1, _Buffer.GetAddressOf());
}

template<UINT startSlot, class T>
inline void ConstantBuffer<startSlot, T>::Apply(ID3D11DeviceContext* deviceContext)
{
	if (_Dirty)
	{
		D3D11_MAPPED_SUBRESOURCE mappedData;
		deviceContext->Map(_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
		memcpy_s(mappedData.pData, sizeof(T), &_Data, sizeof(T));
		deviceContext->Unmap(_Buffer.Get(), 0);
		_Dirty = false;
	}
}
