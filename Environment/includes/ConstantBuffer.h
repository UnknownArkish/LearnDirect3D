#pragma once

#include <d3d11.h>
#include "DXTrace.h"
#include "Common.h"

template<class T>
class ConstantBuffer {
public:
	ConstantBuffer() : _Data(), _Dirty(true), _Declare(false) {}

	HRESULT Declare(ID3D11Device* device);

	void SetBuffer(const T& data)
	{ 
		_Data = data; 
		_Dirty = true; 
	}
	void GetBuffer(T& output) { output = _Data; }

	void VSBind(ID3D11DeviceContext* deviceContext, UINT slot);
	void HSBind(ID3D11DeviceContext* deviceContext, UINT slot);
	void DSBind(ID3D11DeviceContext* deviceContext, UINT slot);
	void GSBind(ID3D11DeviceContext* deviceContext, UINT slot);
	void PSBind(ID3D11DeviceContext* deviceContext, UINT slot);

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
	bool _Declare;
};

template<class T>
inline HRESULT ConstantBuffer<T>::Declare(ID3D11Device* device)
{
	assert(_Declare == false);
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.ByteWidth = sizeof(T);

	HRESULT result = device->CreateBuffer(&cbd, nullptr, _Buffer.GetAddressOf());
	if (SUCCEEDED(result)) _Declare = true;
	else _Buffer.Reset();
	return result;
}

template<class T>
inline void ConstantBuffer<T>::VSBind(ID3D11DeviceContext* deviceContext, UINT slot)
{
	assert(_Declare);
	assert(deviceContext);
	deviceContext->VSSetConstantBuffers(slot, 1, _Buffer.GetAddressOf());
}

template<class T>
inline void ConstantBuffer<T>::HSBind(ID3D11DeviceContext* deviceContext, UINT slot)
{
	assert(_Declare);
	assert(deviceContext);
	deviceContext->HSSetConstantBuffers(slot, 1, _Buffer.GetAddressOf());
}

template<class T>
inline void ConstantBuffer<T>::DSBind(ID3D11DeviceContext* deviceContext, UINT slot)
{
	assert(_Declare);
	assert(deviceContext);
	deviceContext->DSSetConstantBuffers(slot, 1, _Buffer.GetAddressOf());
}

template<class T>
inline void ConstantBuffer<T>::GSBind(ID3D11DeviceContext* deviceContext, UINT slot)
{
	assert(_Declare);
	assert(deviceContext);
	deviceContext->GSSetConstantBuffers(slot, 1, _Buffer.GetAddressOf());
}

template<class T>
inline void ConstantBuffer<T>::PSBind(ID3D11DeviceContext* deviceContext, UINT slot)
{
	assert(_Declare);
	assert(deviceContext);
	deviceContext->PSSetConstantBuffers(slot, 1, _Buffer.GetAddressOf());
}

template<class T>
inline void ConstantBuffer<T>::Apply(ID3D11DeviceContext* deviceContext)
{
	assert(_Declare);
	if (_Dirty)
	{
		D3D11_MAPPED_SUBRESOURCE mappedData;
		deviceContext->Map(_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
		memcpy_s(mappedData.pData, sizeof(T), &_Data, sizeof(T));
		deviceContext->Unmap(_Buffer.Get(), 0);
		_Dirty = false;
	}
}
