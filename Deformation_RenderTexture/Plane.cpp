#include "Plane.h"
#include <assert.h>
#include <vector>
#include <DXTrace.h>
using namespace DirectX;

HRESULT Plane::Declare(ID3D11Device* device, int vertexNumPerRowCol)
{
	assert(device);
	assert(!_IsDeclared);
	assert(vertexNumPerRowCol > 1);

	XMVECTOR posBase = XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f);
	XMVECTOR uvsBase = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR posDelta = XMVectorSet(2.0f / (vertexNumPerRowCol - 1), 2.0f / (vertexNumPerRowCol - 1), 0.0f, 0.0f);
	XMVECTOR uvsDelta = XMVectorSet(1.0f / (vertexNumPerRowCol - 1), 1.0f / (vertexNumPerRowCol - 1), 0.0f, 0.0f);

	std::vector<UniversalVertexLayout> vertices;
	for (int y = 0; y < vertexNumPerRowCol; y++)
	{
		for (int x = 0; x < vertexNumPerRowCol; x++)
		{
			XMVECTOR location = XMVectorSet(x, y, 0.0f, 0.0f);

			XMVECTOR pos = posBase + posDelta * location;
			XMVECTOR uvs = uvsBase + uvsDelta * location;

			UniversalVertexLayout vertex;
			XMStoreFloat3(&vertex.pos, pos);
			XMStoreFloat2(&vertex.uvs, uvs);
			vertex.uvs.y = 1.0f - vertex.uvs.y;
			vertex.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex.normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
			vertex.tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

			vertices.push_back(vertex);
		}
	}

	std::vector<DWORD> indices;
	for (int row = 0; row < vertexNumPerRowCol - 1; row++)
	{
		for (int col = 0; col < vertexNumPerRowCol - 1; col++)
		{
			int x = row * vertexNumPerRowCol + col;
			indices.push_back(x);
			indices.push_back(x + vertexNumPerRowCol + 1);
			indices.push_back(x + 1);

			indices.push_back(x);
			indices.push_back(x + vertexNumPerRowCol);
			indices.push_back(x + vertexNumPerRowCol + 1);
		}
	}

	_VertexNum = vertices.size();
	_IndexNum = indices.size();

	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(D3D11_BUFFER_DESC));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = vertices.size() * sizeof(UniversalVertexLayout);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
	initData.pSysMem = vertices.data();
	HR(device->CreateBuffer(&vbd, &initData, _pVertexBuffer.ReleaseAndGetAddressOf()));

	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(D3D11_BUFFER_DESC));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = indices.size() * sizeof(DWORD);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	initData.pSysMem = indices.data();
	HR(device->CreateBuffer(&ibd, &initData, _pIndexBuffer.ReleaseAndGetAddressOf()));

	_IsDeclared = true;
	return 0;
}

UINT Plane::GetVertexNum() const
{
	assert(_IsDeclared);
	return _VertexNum;
}

UINT Plane::GetIndexNum() const
{
	assert(_IsDeclared);
	return _IndexNum;
}

void Plane::Render(ID3D11DeviceContext* deviceContext)
{
	assert(deviceContext);
	assert(_IsDeclared);

	UINT stride = sizeof(UniversalVertexLayout);
	UINT offset = 0;
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetVertexBuffers(0, 1, _pVertexBuffer.GetAddressOf(), &stride, &offset);
	deviceContext->IASetIndexBuffer(_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->DrawIndexed(_IndexNum, 0, 0);
}

void Plane::RenderPoint(ID3D11DeviceContext* deviceContext)
{
	assert(deviceContext);
	assert(_IsDeclared);

	UINT stride = sizeof(UniversalVertexLayout);
	UINT offset = 0;
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	deviceContext->IASetVertexBuffers(0, 1, _pVertexBuffer.GetAddressOf(), &stride, &offset);
	deviceContext->IASetIndexBuffer(_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->Draw(_VertexNum, 0);
}
