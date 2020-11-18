#include <D3DUtil.h>
#include <DXTrace.h>

#include "GameApp.h"
#include "VertexLayout.h"

using namespace DirectX;

GameApp::GameApp(HINSTANCE hInstance)
	: D3DApp(hInstance)
{
}

GameApp::~GameApp()
{
}



bool GameApp::Init()
{
	if (!D3DApp::Init())
		return false;

	InitShader();
	InitResource();

	return true;
}

void GameApp::OnResize()
{
	D3DApp::OnResize();
}

void GameApp::UpdateScene(float dt)
{
	static float phi = 0.0f, theta = 0.0f;
	phi += 0.0001f, theta += 0.00015f;
	_ConstantBuffer.World = XMMatrixTranspose(XMMatrixRotationX(phi) * XMMatrixRotationY(theta));
	// ���³�������������������ת����
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(_pd3dImmediateContext->Map(_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(_ConstantBuffer), &_ConstantBuffer, sizeof(_ConstantBuffer));
	_pd3dImmediateContext->Unmap(_pConstantBuffer.Get(), 0);
}

void GameApp::DrawScene()
{
	assert(_pd3dImmediateContext);
	assert(_pSwapChain);
	static float blue[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA = (0,0,255,255)
	_pd3dImmediateContext->ClearRenderTargetView(_pRenderTargetView.Get(), blue);
	_pd3dImmediateContext->ClearDepthStencilView(_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	_pd3dImmediateContext->DrawIndexed(36, 0, 0);

	HR(_pSwapChain->Present(0, 0));
}

void GameApp::InitShader()
{
	ComPtr<ID3DBlob> blob;

	HR(CreateShaderFromFile(nullptr, L"TriangleVS.hlsl", "main", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(_pd3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, _pVertexShader.GetAddressOf()));

	_pd3dDevice->CreateInputLayout(VertexLayout::BASE_PASS_INPUT_LAYOUT, 2, blob->GetBufferPointer(), blob->GetBufferSize(), _pInputLayout.GetAddressOf());

	HR(CreateShaderFromFile(nullptr, L"TrianglePS.hlsl", "main", "ps_5_0", blob.ReleaseAndGetAddressOf()));
	HR(_pd3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, _pPixelShader.GetAddressOf()));
}

void GameApp::InitResource()
{
	VertexLayout::BasePassVertexLayout vertices[] =
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
	// ���ö��㻺��������
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof vertices;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	// �½����㻺����
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	HR(_pd3dDevice->CreateBuffer(&vbd, &InitData, _pVertexBuffer.GetAddressOf()));

	// ******************
	// ��������
	//
	DWORD indices[] = {
		// ����
		0, 1, 2,
		2, 3, 0,
		// ����
		4, 5, 1,
		1, 0, 4,
		// ����
		1, 5, 6,
		6, 2, 1,
		// ����
		7, 6, 5,
		5, 4, 7,
		// ����
		3, 2, 6,
		6, 7, 3,
		// ����
		4, 0, 3,
		3, 7, 4
	};
	// ������������������
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof indices;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	// �½�����������
	InitData.pSysMem = indices;
	HR(_pd3dDevice->CreateBuffer(&ibd, &InitData, _pIndexBuffer.GetAddressOf()));
	// ����װ��׶ε���������������
	_pd3dImmediateContext->IASetIndexBuffer(_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);


	// ******************
	// ���ó�������������
	//
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.ByteWidth = sizeof(ConstantBuffer::BasePassBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	// �½���������������ʹ�ó�ʼ����
	HR(_pd3dDevice->CreateBuffer(&cbd, nullptr, _pConstantBuffer.GetAddressOf()));


	// ��ʼ��������������ֵ
	// ����㲻��Ϥ��Щ���󣬿����Ⱥ��ԣ�����������º��ٻ�ͷ�����޸�
	_ConstantBuffer.World = XMMatrixIdentity();	// ��λ�����ת����������
	_ConstantBuffer.View = XMMatrixTranspose(XMMatrixLookAtLH(
		XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	));
	_ConstantBuffer.Projection = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));


	// ******************
	// ����Ⱦ���߸����׶ΰ󶨺�������Դ
	//

	// ����װ��׶εĶ��㻺��������
	UINT stride = sizeof(VertexLayout::BasePassVertexLayout);	// ��Խ�ֽ���
	UINT offset = 0;						// ��ʼƫ����

	_pd3dImmediateContext->IASetVertexBuffers(0, 1, _pVertexBuffer.GetAddressOf(), &stride, &offset);
	// ����ͼԪ���ͣ��趨���벼��
	_pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_pd3dImmediateContext->IASetInputLayout(_pInputLayout.Get());
	// ����ɫ���󶨵���Ⱦ����
	_pd3dImmediateContext->VSSetShader(_pVertexShader.Get(), nullptr, 0);
	// �����ºõĳ����������󶨵�������ɫ��
	_pd3dImmediateContext->VSSetConstantBuffers(0, 1, _pConstantBuffer.GetAddressOf());

	_pd3dImmediateContext->PSSetShader(_pPixelShader.Get(), nullptr, 0);

	// ******************
	// ���õ��Զ�����
	//
	D3D11SetDebugObjectName(_pInputLayout.Get(), "VertexPosColorLayout");
	D3D11SetDebugObjectName(_pVertexBuffer.Get(), "VertexBuffer");
	D3D11SetDebugObjectName(_pIndexBuffer.Get(), "IndexBuffer");
	D3D11SetDebugObjectName(_pConstantBuffer.Get(), "ConstantBuffer");
	D3D11SetDebugObjectName(_pVertexShader.Get(), "Cube_VS");
	D3D11SetDebugObjectName(_pPixelShader.Get(), "Cube_PS");
}
