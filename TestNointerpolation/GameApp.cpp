#include "GameApp.h"
#include <DXTrace.h>
#include <MathUtil.h>

using namespace DirectX;

GameApp::GameApp(HINSTANCE hInstance, int width, int height) : D3DApp(hInstance, width, height)
{

}
GameApp::~GameApp()
{

}

bool GameApp::Init()
{
	if (!D3DApp::Init()) return false;

	InitShader();
	InitResource();

	return true;
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

	ObjectConstantBufferData buffer;
	_ObjectConstantBuffer.GetBuffer(buffer);
	buffer.Local2World = XMMatrixRotationX(phi) * XMMatrixRotationY(theta);
	buffer.World2Local = XMMatrixInverse(nullptr, buffer.Local2World);
	_ObjectConstantBuffer.SetBuffer(buffer);
	_ObjectConstantBuffer.Apply(_pd3dDeviceContext.Get());
}

static bool firstDraw = true;

void GameApp::DrawScene()
{
	assert(_pd3dDeviceContext);
	assert(_pSwapChain);
	static float blue[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	_pd3dDeviceContext->ClearRenderTargetView(_pRenderTargetView.Get(), blue);
	_pd3dDeviceContext->ClearDepthStencilView(_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	_ViewConstantBuffer.VSBind(_pd3dDeviceContext.Get(), 0);
	_ObjectConstantBuffer.VSBind(_pd3dDeviceContext.Get(), 1);
	_BasePassShader.Use(_pd3dDeviceContext.Get());
	_pRenderer->IASetInputLayout(_pd3dDeviceContext.Get());
	_pRenderer->RenderQuad(_pd3dDeviceContext.Get());

	HR(_pSwapChain->Present(0, 0));
}

void GameApp::InitShader()
{
	SHADER_DECLARE_DESC desc;
	ZeroMemory(&desc, sizeof(SHADER_DECLARE_DESC));
	desc.CsoName = L"BasePassVS.cso";
	desc.FileName = L"BasePassVS.hlsl";
	desc.ShaderModel = "vs_6_0";
	desc.EntryPoint = "main";
	HR(_BasePassShader.VSDeclare(_pd3dDevice.Get(), desc));
	desc.CsoName = L"BasePassPS.cso";
	desc.FileName = L"BasePassPS.hlsl";
	desc.ShaderModel = "ps_6_0";
	desc.EntryPoint = "main";
	HR(_BasePassShader.PSDeclare(_pd3dDevice.Get(), desc));
}

void GameApp::InitResource()
{
	HR(_ViewConstantBuffer.Declare(_pd3dDevice.Get()));
	ViewConstantBufferData viewData;
	viewData.World2View = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixLookAtLH(
			DirectX::XMVectorSet(0.0f, 0.0f, -2.25f, 0.0f),
			DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
		));
	viewData.View2Proj = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));;
	viewData.ViewPosWS = DirectX::XMFLOAT3(0.0f, 0.0f, -2.25f);
	_ViewConstantBuffer.SetBuffer(viewData);
	_ViewConstantBuffer.Apply(_pd3dDeviceContext.Get());

	HR(_ObjectConstantBuffer.Declare(_pd3dDevice.Get()));
}