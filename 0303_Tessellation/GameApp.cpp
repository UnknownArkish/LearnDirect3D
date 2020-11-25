#include "GameApp.h"
#include "D3DUtil.h"

GameApp::GameApp(HINSTANCE hInstance) : D3DApp(hInstance) {}
GameApp::~GameApp() {}

bool GameApp::Init()
{
	if (!D3DApp::Init()) return false;

	_pMouse->SetMode(DirectX::Mouse::MODE_ABSOLUTE);

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

	//BasePassConstantBuffer buffer;
	//_ConstantBuffer.GetBuffer(buffer);
	//buffer.World = XMMatrixTranspose(XMMatrixRotationX(phi) * XMMatrixRotationY(theta));
	//_ConstantBuffer.SetBuffer(buffer);
	//_ConstantBuffer.Apply(_pd3dDeviceContext.Get());
}

void GameApp::DrawScene()
{
	assert(_pd3dDeviceContext);
	assert(_pSwapChain);
	static float blue[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA = (0,0,255,255)
	_pd3dDeviceContext->ClearRenderTargetView(_pRenderTargetView.Get(), blue);
	_pd3dDeviceContext->ClearDepthStencilView(_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	HR(_pSwapChain->Present(0, 0));
}

void GameApp::InitShader()
{
	ShaderDeclareDesc desc;
	desc.CsoName = L"";
	desc.FileName = L"";
	desc.EntryPoint = "";
	desc.ShaderModel = "";
	_BasePassShader.VSDeclare(_pd3dDevice->)
}

void GameApp::InitResource()
{
	// VertexBuffer
	DirectX::XMFLOAT3 triVertices[3] = {
		DirectX::XMFLOAT3(-0.8f, -0.8f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 0.8f, 0.0f),
		DirectX::XMFLOAT3(0.8f, -0.8f, 0.0f)
	};
	HR(CreateVertexBuffer(_pd3dDevice.Get(), triVertices, sizeof triVertices, _pTriangleVertexBuffer.GetAddressOf()));


	// RasterizerState
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;
	HR(_pd3dDevice->CreateRasterizerState(&rasterizerDesc, _pRasterizerState.ReleaseAndGetAddressOf()));


	// InputLayout
	D3D11_INPUT_ELEMENT_DESC inputElemDesc[1] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	ComPtr<ID3DBlob> blob;
	CreateShaderFromFile(L"BasePassVS.cso", L"BasePassVS.hlsl", "main", "vs_5_0", blob.ReleaseAndGetAddressOf());
	HR(_pd3dDevice->CreateInputLayout(inputElemDesc, 1, blob->GetBufferPointer(), blob->GetBufferSize(), _pInputLayout.ReleaseAndGetAddressOf()));
}