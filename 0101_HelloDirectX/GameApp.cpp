#include <D3DUtil.h>
#include <DXTrace.h>

#include "GameApp.h"
#include <VertexLayout.h>

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

	// 更新常量缓冲区，让立方体转起来
	BasePassBuffer buffer;
	_ConstantBuffer.GetBuffer(buffer);
	buffer.World = XMMatrixTranspose(XMMatrixRotationX(phi) * XMMatrixRotationY(theta));
	_ConstantBuffer.SetBuffer(buffer);
	_ConstantBuffer.Apply(_pd3dDeviceContext.Get());
}

void GameApp::DrawScene()
{
	assert(_pd3dDeviceContext);
	assert(_pSwapChain);
	static float blue[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA = (0,0,255,255)
	_pd3dDeviceContext->ClearRenderTargetView(_pRenderTargetView.Get(), blue);
	_pd3dDeviceContext->ClearDepthStencilView(_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// 设置图元类型，设定输入布局
	_BasePassShader.Use(_pd3dDeviceContext.Get());
	_ConstantBuffer.VSBind(_pd3dDeviceContext.Get());
	_pRenderer->RenderCube(_pd3dDeviceContext.Get());

	HR(_pSwapChain->Present(0, 0));
}

void GameApp::InitShader()
{
	ComPtr<ID3DBlob> blob;

	ShaderDeclareDesc desc;
	ZeroMemory(&desc, sizeof(ShaderDeclareDesc));
	desc.CsoName = L"TriangleVS.cso";
	desc.FileName = L"shaders/TriangleVS.hlsl";
	desc.EntryPoint = "main";
	desc.ShaderModel = "vs_5_0";
	_BasePassShader.VSDeclare(_pd3dDevice.Get(), desc);
	desc.CsoName = L"TrianglePS.cso";
	desc.FileName = L"shaders/TrianglePS.hlsl";
	desc.EntryPoint = "main";
	desc.ShaderModel = "ps_5_0";
	_BasePassShader.PSDeclare(_pd3dDevice.Get(), desc);
}

void GameApp::InitResource()
{
	_ConstantBuffer.Declare(_pd3dDevice.Get());
	BasePassBuffer buffer;
	buffer.World = XMMatrixIdentity();
	buffer.View = XMMatrixTranspose(XMMatrixLookAtLH(
		XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	));
	buffer.Projection = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));
	_ConstantBuffer.SetBuffer(buffer);

	// ******************
	// 设置调试对象名
	//
	_ConstantBuffer.SetDebugName("ConstantBuffer");
	_BasePassShader.VSSetDebugName("CUBE_VS");
	_BasePassShader.PSSetDebugName("CUBE_VS");
}
