#include "includes/Renderer.h"
#include "includes/DXTrace.h"
#include "includes/VertexLayoutCommon.h"
#include "includes/D3DUtil.h"
#include <assert.h>

using namespace DirectX;

void Renderer::Init(ID3D11Device* device)
{
	if (_bInit) return;
	assert(device);

	InitVertexLayout(device);
	InitQuadResource(device);
	InitCubeResource(device);
	_bInit = true;
}

void Renderer::RenderQuad(ID3D11DeviceContext* deviceContext)
{
}

void Renderer::RenderCube(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetInputLayout(_pUniversalInputLayout.Get());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	BindRendererResource(deviceContext, _CubeResource);
	deviceContext->DrawIndexed(36, 0, 0);
}

void Renderer::RenderCubePoint(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetInputLayout(_pUniversalInputLayout.Get());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	BindRendererResource(deviceContext, _CubeResource);
	deviceContext->Draw(8, 0);
}


void Renderer::InitVertexLayout(ID3D11Device* device)
{
	ComPtr<ID3DBlob> blob;

	HR(CreateShaderFromFile(L"VertexLayout.cso", L"shaders/VertexLayout.hlsl", "main", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(device->CreateInputLayout(UNIVERSAL_INPUT_LAYOUT, 2, blob->GetBufferPointer(), blob->GetBufferSize(), _pUniversalInputLayout.ReleaseAndGetAddressOf()));

	D3D11SetDebugObjectName(_pUniversalInputLayout.Get(), "UniversalInputLayout");
}

void Renderer::InitQuadResource(ID3D11Device* device)
{



}

void Renderer::InitCubeResource(ID3D11Device* device)
{
	UniversalVertexLayout vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) }
	};
	// 设置顶点缓冲区描述
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof vertices;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	// 新建顶点缓冲区
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	HR(device->CreateBuffer(&vbd, &InitData, _CubeResource._pVertexBuffer.ReleaseAndGetAddressOf()));

	DWORD indices[] = {
		// 正面
		0, 1, 2,
		2, 3, 0,
		// 左面
		4, 5, 1,
		1, 0, 4,
		// 顶面
		1, 5, 6,
		6, 2, 1,
		// 背面
		7, 6, 5,
		5, 4, 7,
		// 右面
		3, 2, 6,
		6, 7, 3,
		// 底面
		4, 0, 3,
		3, 7, 4
	};
	// 设置索引缓冲区描述
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof indices;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	// 新建索引缓冲区
	InitData.pSysMem = indices;
	HR(device->CreateBuffer(&ibd, &InitData, _CubeResource._pIndexBuffer.ReleaseAndGetAddressOf()));

	D3D11SetDebugObjectName(_CubeResource._pVertexBuffer.Get(), "CubeVertexBuffer");
	D3D11SetDebugObjectName(_CubeResource._pIndexBuffer.Get(), "CubeIndexBuffer");
}

void Renderer::BindRendererResource(ID3D11DeviceContext* deviceContext, const RendererResource& resource)
{
	UINT stride = sizeof(UniversalVertexLayout);
	UINT offset = 0;
	deviceContext->IASetIndexBuffer(resource._pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetVertexBuffers(0, 1, resource._pVertexBuffer.GetAddressOf(), &stride, &offset);
}
