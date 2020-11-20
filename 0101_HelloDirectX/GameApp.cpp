#include <D3DUtil.h>
#include <DXTrace.h>

#include "GameApp.h"

#include <Texture.h>
#include <TextureView.h>

using namespace DirectX;

GameApp::GameApp(HINSTANCE hInstance)
	: D3DApp(hInstance), _FirstDraw(true)
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

	BasePassConstantBuffer buffer;
	_ConstantBuffer.GetBuffer(buffer);
	buffer.World = XMMatrixTranspose(XMMatrixRotationX(phi) * XMMatrixRotationY(theta));
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

	if (_FirstDraw)
	{
		_RenderTexture.BeginRender(_pd3dDeviceContext.Get());
	}

	_BasePassShader.Use(_pd3dDeviceContext.Get());
	_ConstantBuffer.VSBind(_pd3dDeviceContext.Get());

	_TextireView.PSBind(_pd3dDeviceContext.Get());
	_SamplerState.PSBind(_pd3dDeviceContext.Get());

	_pRenderer->RenderCube(_pd3dDeviceContext.Get());

	if (_FirstDraw)
	{
		_RenderTexture.EndRender(_pd3dDeviceContext.Get());
		_TextireView.Declare(&_RenderTexture);
		_FirstDraw = false;
	}

	HR(_pSwapChain->Present(0, 0));
}

void GameApp::InitShader()
{
	ComPtr<ID3DBlob> blob;

	ShaderDeclareDesc desc;
	ZeroMemory(&desc, sizeof(ShaderDeclareDesc));
	desc.CsoName = L"BasePassVS.cso";
	desc.FileName = L"shaders/BasePassVS.hlsl";
	desc.EntryPoint = "main";
	desc.ShaderModel = "vs_5_0";
	_BasePassShader.VSDeclare(_pd3dDevice.Get(), desc);
	desc.CsoName = L"BasePassPS.cso";
	desc.FileName = L"shaders/BasePassPS.hlsl";
	desc.EntryPoint = "main";
	desc.ShaderModel = "ps_5_0";
	_BasePassShader.PSDeclare(_pd3dDevice.Get(), desc);
}

void GameApp::InitResource()
{
	_Texture.DeclareWithWIC(_pd3dDevice.Get(), _pd3dDeviceContext.Get(), L"texture.png");
	_TextireView.Declare(&_Texture);

	_RenderTexture.Declare(_pd3dDevice.Get(), 512, 512, DXGI_FORMAT_R8G8B8A8_UNORM, false);
	_RenderTexture.SetDebugName("RenderTexture", "RenderTextureView");

	D3D11_SAMPLER_DESC desc;
	_SamplerState.GetDesc(desc);
	_SamplerState.Delcare(_pd3dDevice.Get(), desc);

	_ConstantBuffer.Declare(_pd3dDevice.Get());
	BasePassConstantBuffer buffer;
	buffer.World = XMMatrixIdentity();
	buffer.View = XMMatrixTranspose(XMMatrixLookAtLH(
		XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	));
	buffer.Projection = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));
	_ConstantBuffer.SetBuffer(buffer);
	_ConstantBuffer.Apply(_pd3dDeviceContext.Get());

	_ConstantBuffer.SetDebugName("ConstantBuffer");
	_BasePassShader.VSSetDebugName("CUBE_VS");
	_BasePassShader.PSSetDebugName("CUBE_VS");
}
