#include "GameApp.h"
#include <DXTrace.h>
#include <MathUtil.h>

using namespace DirectX;

const UINT VERTEX_NUM_PER_ROW_COL = 2;
const float DEFORMATION_TEX_WIDTH = 2;
const float DEFORMATION_TEX_HEIGHT = 2;
const float DEFORMATION_TEX_INTENSITY = 1;

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

	InitForDeformationTex();
	InitForDeformationMap();

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

	ObjectConstantBuffer buffer;
	_ObjectConstantBuffer.GetBuffer(buffer);
	buffer.Local2World = XMMatrixRotationX(phi) * XMMatrixRotationY(theta);
	buffer.World2Local = XMMatrixInverse(nullptr, buffer.Local2World);
	_ObjectConstantBuffer.SetBuffer(buffer);
	_ObjectConstantBuffer.Apply(_pd3dDeviceContext.Get());

	if (_pInput->IsKeyPressed(Input::KeyCode::S))
	{
		D3D11_RASTERIZER_DESC desc;
		_pRasterizerState->GetDesc(&desc);
		if (desc.FillMode == D3D11_FILL_SOLID) desc.FillMode = D3D11_FILL_WIREFRAME;
		else desc.FillMode = D3D11_FILL_SOLID;
		HR(_pd3dDevice->CreateRasterizerState(&desc, _pRasterizerState.ReleaseAndGetAddressOf()));
	}
}

static bool firstDraw = true;

void GameApp::DrawScene()
{
	assert(_pd3dDeviceContext);
	assert(_pSwapChain);
	static float blue[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	_pd3dDeviceContext->ClearRenderTargetView(_pRenderTargetView.Get(), blue);
	_pd3dDeviceContext->ClearDepthStencilView(_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	if (firstDraw) 
	{
		_DeformationMap.BeginRender(_pd3dDeviceContext.Get());
		{
			_ObjectConstantBuffer.VSBind(_pd3dDeviceContext.Get(), 0);
			_DeformationConstantBuffer.VSBind(_pd3dDeviceContext.Get(), 1);
			_DeformationTexView.VSBind(_pd3dDeviceContext.Get(), 0);
			_DeformationTexSampler.VSBind(_pd3dDeviceContext.Get(), 0);

			_pd3dDeviceContext->RSSetState(_DeformationMapRatserizerState.Get());
				
			_ObjectConstantBuffer.PSBind(_pd3dDeviceContext.Get(), 0);
			_DeformationConstantBuffer.PSBind(_pd3dDeviceContext.Get(), 1);
			_DeformationTexView.PSBind(_pd3dDeviceContext.Get(), 0);
			_DeformationTexSampler.PSBind(_pd3dDeviceContext.Get(), 0);
			
			_CalculateDeformationMapShader.Use(_pd3dDeviceContext.Get());
			_pRenderer->IASetInputLayout(_pd3dDeviceContext.Get());
			_Plane.Render(_pd3dDeviceContext.Get());
		}
		_DeformationMap.EndRender(_pd3dDeviceContext.Get());

		firstDraw = false;
	}
	else
	{
		_ViewConstantBuffer.VSBind(_pd3dDeviceContext.Get(), 0);
		_ObjectConstantBuffer.VSBind(_pd3dDeviceContext.Get(), 1);
		_DeformationMapView.VSBind(_pd3dDeviceContext.Get(), 0);
		_DeformationMapSampler.VSBind(_pd3dDeviceContext.Get(), 0);

		_pd3dDeviceContext->RSSetState(_pRasterizerState.Get());

		_ViewConstantBuffer.PSBind(_pd3dDeviceContext.Get(), 0);
		_ObjectConstantBuffer.PSBind(_pd3dDeviceContext.Get(), 1);
		_LightConstantBuffer.PSBind(_pd3dDeviceContext.Get(), 2);
		_ParallelMapConstantBuffer.PSBind(_pd3dDeviceContext.Get(), 3);

		_MainTexView.PSBind(_pd3dDeviceContext.Get(), 0);
		_MainTexSampler.PSBind(_pd3dDeviceContext.Get(), 0);
		_NormalTexView.PSBind(_pd3dDeviceContext.Get(), 1);
		_NormalTexSampler.PSBind(_pd3dDeviceContext.Get(), 1);
		_DeformationMapView.PSBind(_pd3dDeviceContext.Get(), 2);
		_DeformationMapSampler.PSBind(_pd3dDeviceContext.Get(), 2);

		_BasePassShader.Use(_pd3dDeviceContext.Get());
		_pRenderer->IASetInputLayout(_pd3dDeviceContext.Get());
		_Plane.Render(_pd3dDeviceContext.Get());
	}

	HR(_pSwapChain->Present(0, 0));
}

void GameApp::InitShader()
{
	SHADER_DECLARE_DESC desc;
	desc.CsoName = L"BasePassVS.cso";
	desc.FileName = L"shaders/BasePassVS.hlsl";
	desc.EntryPoint = "main";
	desc.ShaderModel = "vs_5_0";
	HR(_BasePassShader.VSDeclare(_pd3dDevice.Get(), desc));
	desc.CsoName = L"BasePassPS.cso";
	desc.FileName = L"shaders/BasePassPS.hlsl";
	desc.EntryPoint = "main";
	desc.ShaderModel = "ps_5_0";
	HR(_BasePassShader.PSDeclare(_pd3dDevice.Get(), desc));

	_BasePassShader.VSSetDebugName("BasePassVS");
	_BasePassShader.PSSetDebugName("BasePassPS");
}

void GameApp::InitResource()
{
	_Plane.Declare(_pd3dDevice.Get(), VERTEX_NUM_PER_ROW_COL);

	D3D11_SAMPLER_DESC desc;

	HR(_MainTex.DeclareWithWIC(_pd3dDevice.Get(), _pd3dDeviceContext.Get(), L"maintex.png"));
	_MainTexView.Declare(&_MainTex);
	_MainTexSampler.GetDesc(desc);
	HR(_MainTexSampler.Delcare(_pd3dDevice.Get(), desc));

	HR(_NormalTex.DeclareWithWIC(_pd3dDevice.Get(), _pd3dDeviceContext.Get(), L"normalTex.png"));
	_NormalTexView.Declare(&_NormalTex);
	_NormalTexSampler.GetDesc(desc);
	HR(_NormalTexSampler.Delcare(_pd3dDevice.Get(), desc));

	HR(_ViewConstantBuffer.Declare(_pd3dDevice.Get()));
	ViewConstantBuffer viewData;
	viewData.World2View = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixLookAtLH(
			DirectX::XMVectorSet(0.0f, 0.0f, -2.25f, 0.0f),
			DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
		));
	viewData.View2Proj = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));;
	viewData.ViewPosWS = DirectX::XMFLOAT3(0.0f, 0.0f, -2.25f);
	HR(_ViewConstantBuffer.Declare(_pd3dDevice.Get()));
	_ViewConstantBuffer.SetBuffer(viewData);
	_ViewConstantBuffer.Apply(_pd3dDeviceContext.Get());

	HR(_ObjectConstantBuffer.Declare(_pd3dDevice.Get()));

	HR(_LightConstantBuffer.Declare(_pd3dDevice.Get()));
	PointLight lightData;
	ZeroMemory(&lightData, sizeof(PointLight));
	lightData.SetColor(DirectX::XMFLOAT3(0.8f, 0.8f, 0.8f));
	lightData.SetPosition(DirectX::XMFLOAT3(1.5f, 1.0f, -3.0f));
	_LightConstantBuffer.SetBuffer(lightData);
	_LightConstantBuffer.Apply(_pd3dDeviceContext.Get());

	HR(_ParallelMapConstantBuffer.Declare(_pd3dDevice.Get()));
	ParallelMapConstantBuffer parallelMapData;
	ZeroMemory(&parallelMapData, sizeof(ParallelMapConstantBuffer));
	parallelMapData.ParallelStepScale = 0.3f;
	parallelMapData.ParallelIntensity = 1.0f;
	_ParallelMapConstantBuffer.SetBuffer(parallelMapData);
	_ParallelMapConstantBuffer.Apply(_pd3dDeviceContext.Get());

	D3D11_RASTERIZER_DESC rasterizerState;
	rasterizerState.CullMode = D3D11_CULL_BACK;
	rasterizerState.FillMode = D3D11_FILL_SOLID;
	rasterizerState.FrontCounterClockwise = FALSE;
	rasterizerState.DepthBias = 0;
	rasterizerState.DepthBiasClamp = 0.0f;
	rasterizerState.SlopeScaledDepthBias = 0.0f;
	rasterizerState.DepthClipEnable = true;
	rasterizerState.ScissorEnable = false;
	rasterizerState.MultisampleEnable = false;
	rasterizerState.AntialiasedLineEnable = false;
	HR(_pd3dDevice->CreateRasterizerState(&rasterizerState, _pRasterizerState.ReleaseAndGetAddressOf()));
}

void GameApp::InitForDeformationTex()
{
	HR(_DeformationTex.DeclareWithWIC(_pd3dDevice.Get(), _pd3dDeviceContext.Get(), L"heightmap.png"));

	_DeformationTexView.Declare(&_DeformationTex);

	D3D11_SAMPLER_DESC samplerDesc;
	_DeformationTexSampler.GetDesc(samplerDesc);
	HR(_DeformationTexSampler.Delcare(_pd3dDevice.Get(), samplerDesc));

	HR(_DeformationConstantBuffer.Declare(_pd3dDevice.Get()));
	DeformationConstantBuffer data;
	ZeroMemory(&data, sizeof(DeformationConstantBuffer));
	data.ForwardWS = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
	data.OriginWS = DirectX::XMFLOAT3(0.0f, 0.0f, -5.0f);
	data.Params = DirectX::XMFLOAT4(DEFORMATION_TEX_WIDTH, DEFORMATION_TEX_HEIGHT, DEFORMATION_TEX_INTENSITY, 0.0f);
	_DeformationConstantBuffer.SetBuffer(data);
	_DeformationConstantBuffer.Apply(_pd3dDeviceContext.Get());

	_DeformationTex.SetDebugName("DeformationMap", "DeformationMapSRV");
	_DeformationTexSampler.SetDebugName("DeformationMapSampler");
	_DeformationConstantBuffer.SetDebugName("DeformationConstantBuffer");
}

void GameApp::InitForDeformationMap()
{
	HR(_DeformationMap.Declare(_pd3dDevice.Get(), 512, 512, DXGI_FORMAT_R32G32B32A32_FLOAT, false));
	_DeformationMapView.Declare(&_DeformationMap);
	
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	_DeformationMapSampler.GetDesc(samplerDesc);
	HR(_DeformationMapSampler.Delcare(_pd3dDevice.Get(), samplerDesc));

	_DeformationMap.SetDebugName("DeformationMap", "DeformationMapSRV");
	_DeformationMapSampler.SetDebugName("DeformationMapSampler");

	SHADER_DECLARE_DESC shaderDesc;
	ZeroMemory(&shaderDesc, sizeof(SHADER_DECLARE_DESC));
	shaderDesc.CsoName = L"CalculateDeformationMapVS.cso";
	shaderDesc.FileName = L"shaders/CalculateDeformationMapVS.hlsl";
	shaderDesc.EntryPoint = "main";
	shaderDesc.ShaderModel = "vs_5_0";
	HR(_CalculateDeformationMapShader.VSDeclare(_pd3dDevice.Get(), shaderDesc));
	shaderDesc.CsoName = L"CalculateDeformationMapPS.cso";
	shaderDesc.FileName = L"shaders/CalculateDeformationMapPS.hlsl";
	shaderDesc.EntryPoint = "main";
	shaderDesc.ShaderModel = "ps_5_0";
	HR(_CalculateDeformationMapShader.PSDeclare(_pd3dDevice.Get(), shaderDesc));

	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	HR(_pd3dDevice->CreateRasterizerState(&rasterizerDesc, _DeformationMapRatserizerState.ReleaseAndGetAddressOf()));
}
