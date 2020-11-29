#include "GameApp.h"
#include "D3DUtil.h"

GameApp::GameApp(HINSTANCE hInstance) : D3DApp(hInstance) {}
GameApp::~GameApp() {}

bool GameApp::Init()
{
	if (!D3DApp::Init()) return false;

	//_pMouse->SetMode(DirectX::Mouse::MODE_ABSOLUTE);

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

	// VertexBuffer
	UINT stride = sizeof(DirectX::XMFLOAT3), offset = 0;
	_pd3dDeviceContext->IASetVertexBuffers(0, 1, _pTriangleVertexBuffer.GetAddressOf(), &stride, &offset);
	_pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	//_pd3dDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	_pd3dDeviceContext->IASetInputLayout(_pInputLayout.Get());
	// Shader
	_BasePassShader.Use(_pd3dDeviceContext.Get());
	// Rasterizer State
	_pd3dDeviceContext->RSSetState(_pRasterizerState.Get());


	_pd3dDeviceContext->Draw(3, 0);


	HR(_pSwapChain->Present(0, 0));
}

void GameApp::InitShader()
{
	SHADER_DECLARE_DESC desc;
	desc.CsoName = L"BasePassVS.cso";
	desc.FileName = L"BasePassVS.hlsl";
	desc.EntryPoint = "main";
	desc.ShaderModel = "vs_5_0";
	HR(_BasePassShader.VSDeclare(_pd3dDevice.Get(), desc));
	desc.CsoName = L"BasePassHS.cso";
	desc.FileName = L"BasePassHS.hlsl";
	desc.EntryPoint = "main";
	desc.ShaderModel = "hs_5_0";
	_BasePassShader.HSDeclare(_pd3dDevice.Get(), desc);
	desc.CsoName = L"BasePassDS.cso";
	desc.FileName = L"BasePassDS.hlsl";
	desc.EntryPoint = "main";
	desc.ShaderModel = "ds_5_0";
	_BasePassShader.DSDeclare(_pd3dDevice.Get(), desc);
	desc.CsoName = L"BasePassPS.cso";
	desc.FileName = L"BasePassPS.hlsl";
	desc.EntryPoint = "main";
	desc.ShaderModel = "ps_5_0";
	HR(_BasePassShader.PSDeclare(_pd3dDevice.Get(), desc));
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
	HR(CreateShaderFromFile(L"BasePaseVS.cso", L"BasePassVS.hlsl", "main", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(_pd3dDevice->CreateInputLayout(inputElemDesc, 1, blob->GetBufferPointer(), blob->GetBufferSize(), _pInputLayout.ReleaseAndGetAddressOf()));
}