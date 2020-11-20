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
	deviceContext->Draw(24, 0);
}


void Renderer::InitVertexLayout(ID3D11Device* device)
{
	ComPtr<ID3DBlob> blob;

	HR(CreateShaderFromFile(L"VertexLayout.cso", L"shaders/VertexLayout.hlsl", "main", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(device->CreateInputLayout(UNIVERSAL_INPUT_LAYOUT, 3, blob->GetBufferPointer(), blob->GetBufferSize(), _pUniversalInputLayout.ReleaseAndGetAddressOf()));

	D3D11SetDebugObjectName(_pUniversalInputLayout.Get(), "UniversalInputLayout");
}

void Renderer::InitQuadResource(ID3D11Device* device)
{



}

void Renderer::InitCubeResource(ID3D11Device* device)
{
	UniversalVertexLayout vertices[24];

	float width = 2;
	float height = 2;
	float depth = 2;
	DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

	float w2 = width / 2, h2 = height / 2, d2 = depth / 2;

	// 右面(+X面)
	vertices[0].pos = XMFLOAT3(w2, -h2, -d2);
	vertices[1].pos = XMFLOAT3(w2, h2, -d2);
	vertices[2].pos = XMFLOAT3(w2, h2, d2);
	vertices[3].pos = XMFLOAT3(w2, -h2, d2);
	// 左面(-X面)
	vertices[4].pos = XMFLOAT3(-w2, -h2, d2);
	vertices[5].pos = XMFLOAT3(-w2, h2, d2);
	vertices[6].pos = XMFLOAT3(-w2, h2, -d2);
	vertices[7].pos = XMFLOAT3(-w2, -h2, -d2);
	// 顶面(+Y面)
	vertices[8].pos = XMFLOAT3(-w2, h2, -d2);
	vertices[9].pos = XMFLOAT3(-w2, h2, d2);
	vertices[10].pos = XMFLOAT3(w2, h2, d2);
	vertices[11].pos = XMFLOAT3(w2, h2, -d2);
	// 底面(-Y面)
	vertices[12].pos = XMFLOAT3(w2, -h2, -d2);
	vertices[13].pos = XMFLOAT3(w2, -h2, d2);
	vertices[14].pos = XMFLOAT3(-w2, -h2, d2);
	vertices[15].pos = XMFLOAT3(-w2, -h2, -d2);
	// 背面(+Z面)
	vertices[16].pos = XMFLOAT3(w2, -h2, d2);
	vertices[17].pos = XMFLOAT3(w2, h2, d2);
	vertices[18].pos = XMFLOAT3(-w2, h2, d2);
	vertices[19].pos = XMFLOAT3(-w2, -h2, d2);
	// 正面(-Z面)
	vertices[20].pos = XMFLOAT3(-w2, -h2, -d2);
	vertices[21].pos = XMFLOAT3(-w2, h2, -d2);
	vertices[22].pos = XMFLOAT3(w2, h2, -d2);
	vertices[23].pos = XMFLOAT3(w2, -h2, -d2);

	for (UINT i = 0; i < 4; ++i)
	{
		// 右面(+X面)
		//vertices[i].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
		//vertices[i].tangent = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
		vertices[i].color = color;
		// 左面(-X面)
		//vertices[i + 4].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
		//vertices[i + 4].tangent = XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f);
		vertices[i + 4].color = color;
		// 顶面(+Y面)
		//vertices[i + 8].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		//vertices[i + 8].tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		vertices[i + 8].color = color;
		// 底面(-Y面)
		//vertices[i + 12].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
		//vertices[i + 12].tangent = XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f);
		vertices[i + 12].color = color;
		// 背面(+Z面)
		//vertices[i + 16].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
		//vertices[i + 16].tangent = XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f);
		vertices[i + 16].color = color;
		// 正面(-Z面)
		//vertices[i + 20].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		//vertices[i + 20].tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		vertices[i + 20].color = color;
	
	}for (UINT i = 0; i < 6; ++i)
	{
		vertices[i * 4].uvs = XMFLOAT2(0.0f, 1.0f);
		vertices[i * 4 + 1].uvs = XMFLOAT2(0.0f, 0.0f);
		vertices[i * 4 + 2].uvs = XMFLOAT2(1.0f, 0.0f);
		vertices[i * 4 + 3].uvs = XMFLOAT2(1.0f, 1.0f);
	}

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
		0, 1, 2, 2, 3, 0,		// 右面(+X面)
		4, 5, 6, 6, 7, 4,		// 左面(-X面)
		8, 9, 10, 10, 11, 8,	// 顶面(+Y面)
		12, 13, 14, 14, 15, 12,	// 底面(-Y面)
		16, 17, 18, 18, 19, 16, // 背面(+Z面)
		20, 21, 22, 22, 23, 20	// 正面(-Z面)
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

	D3D11SetDebugObjectName(_CubeResource._pVertexBuffer.Get(), "CubeVeruvsBuffer");
	D3D11SetDebugObjectName(_CubeResource._pIndexBuffer.Get(), "CubeIndexBuffer");
}

void Renderer::BindRendererResource(ID3D11DeviceContext* deviceContext, const RendererResource& resource)
{
	UINT stride = sizeof(UniversalVertexLayout);
	UINT offset = 0;
	deviceContext->IASetIndexBuffer(resource._pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetVertexBuffers(0, 1, resource._pVertexBuffer.GetAddressOf(), &stride, &offset);
}
