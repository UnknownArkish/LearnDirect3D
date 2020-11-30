#include <D3DUtil.h>
#include <DXTrace.h>

#include "GameApp.h"

#include <Texture.h>
#include <TextureView.h>

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
}

void GameApp::DrawScene()
{
	assert(_pd3dDeviceContext);
	assert(_pSwapChain);
	static float blue[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA = (0,0,255,255)
	_pd3dDeviceContext->ClearRenderTargetView(_pRenderTargetView.Get(), blue);
	_pd3dDeviceContext->ClearDepthStencilView(_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	_ShaderHair.Use(_pd3dDeviceContext.Get());
	_pRenderer->RenderCylinder(_pd3dDeviceContext.Get());

	HR(_pSwapChain->Present(0, 0));
}

void GameApp::InitShader()
{
	SHADER_DECLARE_DESC desc;
	ZeroMemory(&desc, sizeof(SHADER_DECLARE_DESC));
	desc.CsoName = L"HairVS.cso";
	desc.FileName = L"HairVS.cso";
	desc.EntryPoint = "main";
	desc.ShaderModel = "vs_5_0";
	_ShaderHair.VSDeclare(_pd3dDevice.Get(), desc);
	desc.CsoName = L"HairPS.cso";
	desc.FileName = L"HairPS.cso";
	desc.EntryPoint = "main";
	desc.ShaderModel = "vps_5_0";
	_ShaderHair.PSDeclare(_pd3dDevice.Get(), desc);
}

void GameApp::InitResource()
{
}
