#include "GameApp.h"
#include <DXTrace.h>

using namespace DirectX;

GameApp::GameApp(HINSTANCE hInstance) : D3DApp(hInstance)
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
	phi += 0.0001f, theta += 0.00015f;

	BasePassConstantBuffer buffer;
	_ConstantBuffer.GetBuffer(buffer);
	buffer.World = XMMatrixRotationX(phi) * XMMatrixRotationY(theta);
	_ConstantBuffer.SetBuffer(buffer);
	_ConstantBuffer.Apply(_pd3dDeviceContext.Get());
}

static bool firstDraw = true;

void GameApp::DrawScene()
{
	assert(_pd3dDeviceContext);
	assert(_pSwapChain);
	static float blue[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA = (0,0,255,255)
	_pd3dDeviceContext->ClearRenderTargetView(_pRenderTargetView.Get(), blue);
	_pd3dDeviceContext->ClearDepthStencilView(_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	if (firstDraw) 
	{
		_OffsetTex.BeginRender(_pd3dDeviceContext.Get());
		{
			_OffsetConstantBuffer.VSBind(_pd3dDeviceContext.Get(), 1);
			_CalculateOffsetShader.Use(_pd3dDeviceContext.Get());
			_pRenderer->RenderCubePoint(_pd3dDeviceContext.Get());
		}
		_OffsetTex.EndRender(_pd3dDeviceContext.Get());
		firstDraw = false;
	}
	else
	{
		_ConstantBuffer.VSBind(_pd3dDeviceContext.Get(), 0);
		_OffsetConstantBuffer.VSBind(_pd3dDeviceContext.Get(), 1);


		_OffsetTexSampler.VSBind(_pd3dDeviceContext.Get(), 0);
		_OffsetTexView.VSBind(_pd3dDeviceContext.Get(), 0);

		_MainTexView.PSBind(_pd3dDeviceContext.Get(), 0);
		_MainTexSampler.PSBind(_pd3dDeviceContext.Get(), 0);

		_BasePassShader.Use(_pd3dDeviceContext.Get());
		_pRenderer->RenderCube(_pd3dDeviceContext.Get());
	}

	HR(_pSwapChain->Present(0, 0));
}

void GameApp::InitShader()
{
	ShaderDeclareDesc desc;
	desc.CsoName = L"CalculateOffsetPassVS.cso";
	desc.FileName = L"shaders/CalculateOffsetPassVS.hlsl";
	desc.EntryPoint = "main";
	desc.ShaderModel = "vs_5_0";
	_CalculateOffsetShader.VSDeclare(_pd3dDevice.Get(), desc);
	desc.CsoName = L"CalculateOffsetPassPS.cso";
	desc.FileName = L"shaders/CalculateOffsetPassPS.hlsl";
	desc.EntryPoint = "main";
	desc.ShaderModel = "ps_5_0";
	_CalculateOffsetShader.PSDeclare(_pd3dDevice.Get(), desc);

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
	// tex
	_OffsetTex.Declare(_pd3dDevice.Get(), 8, 4, DXGI_FORMAT_R8G8B8A8_SNORM, false);
	// sampler state
	D3D11_SAMPLER_DESC desc;
	_OffsetTexSampler.GetDesc(desc);
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;		// 确保按照点采样
	_OffsetTexSampler.Delcare(_pd3dDevice.Get(), desc);
	// texture view
	_OffsetTexView.Declare(&_OffsetTex);


	_MainTex.DeclareWithWIC(_pd3dDevice.Get(), _pd3dDeviceContext.Get(), L"texture.png");
	_MainTexSampler.GetDesc(desc);
	_MainTexSampler.Delcare(_pd3dDevice.Get(), desc);
	_MainTexView.Declare(&_MainTex);

	// constant buffer
	_OffsetBufferData.Data = DirectX::XMFLOAT4(8, 4, 0, 0);
	_OffsetConstantBuffer.Declare(_pd3dDevice.Get());
	_OffsetConstantBuffer.SetBuffer(_OffsetBufferData);
	_OffsetConstantBuffer.Apply(_pd3dDeviceContext.Get());

	_ConstantBuffer.Declare(_pd3dDevice.Get());
	_BufferData.World = DirectX::XMMatrixIdentity();
	_BufferData.View = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixLookAtLH(
			DirectX::XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
			DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	));
	_BufferData.Projection = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));
	_ConstantBuffer.SetBuffer(_BufferData);
	_ConstantBuffer.Apply(_pd3dDeviceContext.Get());
}