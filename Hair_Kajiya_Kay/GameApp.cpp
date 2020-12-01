#include <D3DUtil.h>
#include <DXTrace.h>

#include "GameApp.h"

#include <Texture.h>
#include <TextureView.h>

using namespace DirectX;

GameApp::GameApp(HINSTANCE hInstance, int width, int height)
	: D3DApp(hInstance, width, height)
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
	static float intensity = 5.0f;
	if (_pInput->IsButtonDown(Input::LeftButton))
	{
		DirectX::XMFLOAT2 mouseOffset = _pInput->GetMousePositionOffset();
		phi += mouseOffset.y * dt * intensity;
		theta += mouseOffset.x * dt * intensity;
	}

	ObjectConstantBuffer buffer;
	_ConstantBufferObject.GetBuffer(buffer);
	buffer.Local2World = XMMatrixRotationX(phi) * XMMatrixRotationY(theta);
	buffer.World2Local = XMMatrixInverse(nullptr, buffer.Local2World);
	_ConstantBufferObject.SetBuffer(buffer);
	_ConstantBufferObject.Apply(_pd3dDeviceContext.Get());
}

void GameApp::DrawScene()
{
	assert(_pd3dDeviceContext);
	assert(_pSwapChain);
	static float blue[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA = (0,0,255,255)
	_pd3dDeviceContext->ClearRenderTargetView(_pRenderTargetView.Get(), blue);
	_pd3dDeviceContext->ClearDepthStencilView(_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	_ConstantBufferView.VSBind(_pd3dDeviceContext.Get(), 0);
	_ConstantBufferObject.VSBind(_pd3dDeviceContext.Get(), 1);

	_ConstantBufferView.PSBind(_pd3dDeviceContext.Get(), 0);
	_ConstantBufferObject.PSBind(_pd3dDeviceContext.Get(), 1);
	_ConstantBufferLight.PSBind(_pd3dDeviceContext.Get(), 2);

	_ShaderHair.Use(_pd3dDeviceContext.Get());
	_pRenderer->RenderQuad(_pd3dDeviceContext.Get());

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
	HR(_ConstantBufferView.Declare(_pd3dDevice.Get()));
	ViewConstantBuffer viewData;
	viewData.World2View = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixLookAtLH(
			DirectX::XMVectorSet(0.0f, 0.0f, -2.0f, 0.0f),
			DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
		));
	viewData.View2Proj = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));;
	viewData.ViewPosWS = DirectX::XMFLOAT3(0.0f, 0.0f, -2.0f);
	HR(_ConstantBufferView.Declare(_pd3dDevice.Get()));
	_ConstantBufferView.SetBuffer(viewData);
	_ConstantBufferView.Apply(_pd3dDeviceContext.Get());

	HR(_ConstantBufferObject.Declare(_pd3dDevice.Get()));

	HR(_ConstantBufferLight.Declare(_pd3dDevice.Get()));
	PointLight lightData;
	ZeroMemory(&lightData, sizeof(PointLight));
	lightData.SetColor(DirectX::XMFLOAT3(0.8f, 0.8f, 0.8f));
	lightData.SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, -3.0f));
	_ConstantBufferLight.SetBuffer(lightData);
	_ConstantBufferLight.Apply(_pd3dDeviceContext.Get());
}
