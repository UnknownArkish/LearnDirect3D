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

	InitForDeformation();

	InitShader();
	InitResource();

	return true;
}

void GameApp::UpdateScene(float dt)
{
	static float phi = 0.0f, theta = 0.0f;
	//phi += 0.0001f, theta += 0.00015f;

	ObjectConstantBuffer buffer;
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
	static float blue[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	_pd3dDeviceContext->ClearRenderTargetView(_pRenderTargetView.Get(), blue);
	_pd3dDeviceContext->ClearDepthStencilView(_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	if (firstDraw) 
	{
		_OffsetMap.BeginRender(_pd3dDeviceContext.Get());
		{
			_OffsetMapConstantBuffer.VSBind(_pd3dDeviceContext.Get(), 0);
			_CalculateOffsetMapShader.Use(_pd3dDeviceContext.Get());
			_pRenderer->RenderQuadPoint(_pd3dDeviceContext.Get());
		}
		_OffsetMap.EndRender(_pd3dDeviceContext.Get());

		_ParallelMap.BeginRender(_pd3dDeviceContext.Get());
		{
			_ViewConstantBuffer.VSBind(_pd3dDeviceContext.Get(), 0);
			_ObjectConstantBuffer.VSBind(_pd3dDeviceContext.Get(), 1);

			_pd3dDeviceContext->RSSetState(_ParallelMapRasterizerState.Get());

			_DeformationConstantBuffer.PSBind(_pd3dDeviceContext.Get(), 0);
			_DeformationMapView.PSBind(_pd3dDeviceContext.Get(), 0);
			_DeformationMapSampler.PSBind(_pd3dDeviceContext.Get(), 0);

			_CalculateParallelMapShader.Use(_pd3dDeviceContext.Get());
			_pRenderer->RenderQuad(_pd3dDeviceContext.Get());
		}
		_ParallelMap.EndRender(_pd3dDeviceContext.Get());

		firstDraw = false;
	}
	else
	{
		_ViewConstantBuffer.VSBind(_pd3dDeviceContext.Get(), 0);
		_ObjectConstantBuffer.VSBind(_pd3dDeviceContext.Get(), 1);
		_OffsetMapConstantBuffer.VSBind(_pd3dDeviceContext.Get(), 2);


		_OffsetMapSampler.VSBind(_pd3dDeviceContext.Get(), 0);
		_OffsetMapView.VSBind(_pd3dDeviceContext.Get(), 0);

		_pd3dDeviceContext->RSSetState(nullptr);


		_ViewConstantBuffer.PSBind(_pd3dDeviceContext.Get(), 0);
		_ObjectConstantBuffer.PSBind(_pd3dDeviceContext.Get(), 1);
		_LightConstantBuffer.PSBind(_pd3dDeviceContext.Get(), 2);
		_MainTexView.PSBind(_pd3dDeviceContext.Get(), 0);
		_MainTexSampler.PSBind(_pd3dDeviceContext.Get(), 0);

		_DebugView.PSBind(_pd3dDeviceContext.Get(), 3);

		_BasePassShader.Use(_pd3dDeviceContext.Get());
		_pRenderer->RenderQuad(_pd3dDeviceContext.Get());
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
	D3D11_SAMPLER_DESC desc;
	HR(_MainTex.DeclareWithWIC(_pd3dDevice.Get(), _pd3dDeviceContext.Get(), L"maintex.png"));
	HR(_NormalTex.DeclareWithWIC(_pd3dDevice.Get(), _pd3dDeviceContext.Get(), L"normalTex.png"));
	_MainTexSampler.GetDesc(desc);
	HR(_MainTexSampler.Delcare(_pd3dDevice.Get(), desc));
	_MainTexView.Declare(&_MainTex);

	HR(_ViewConstantBuffer.Declare(_pd3dDevice.Get()));
	ViewConstantBuffer viewData;
	viewData.World2View = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixLookAtLH(
			DirectX::XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
			DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
		));
	viewData.View2Proj = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));;
	HR(_ViewConstantBuffer.Declare(_pd3dDevice.Get()));
	_ViewConstantBuffer.SetBuffer(viewData);
	_ViewConstantBuffer.Apply(_pd3dDeviceContext.Get());

	HR(_ObjectConstantBuffer.Declare(_pd3dDevice.Get()));

	HR(_LightConstantBuffer.Declare(_pd3dDevice.Get()));
	PointLight lightData;
	ZeroMemory(&lightData, sizeof(PointLight));
	lightData.SetColor(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	lightData.SetPosition(DirectX::XMFLOAT3(0.0f, 1.0f, -3.0f));
	_LightConstantBuffer.SetBuffer(lightData);
	_LightConstantBuffer.Apply(_pd3dDeviceContext.Get());

	_DebugView.Declare(&_ParallelMap);
}

void GameApp::InitForDeformation()
{
	HR(_DeformationMap.DeclareWithWIC(_pd3dDevice.Get(), _pd3dDeviceContext.Get(), L"heightmap.png"));

	_DeformationMapView.Declare(&_DeformationMap);

	D3D11_SAMPLER_DESC samplerDesc;
	_DeformationMapSampler.GetDesc(samplerDesc);
	HR(_DeformationMapSampler.Delcare(_pd3dDevice.Get(), samplerDesc));

	HR(_DeformationConstantBuffer.Declare(_pd3dDevice.Get()));
	DeformationConstantBuffer data;
	ZeroMemory(&data, sizeof(DeformationConstantBuffer));
	data.ForwardWS = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
	data.OriginWS = DirectX::XMFLOAT3(0.0f, 0.0f, -5.0f);
	data.Params = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f);
	_DeformationConstantBuffer.SetBuffer(data);
	_DeformationConstantBuffer.Apply(_pd3dDeviceContext.Get());

	_DeformationMap.SetDebugName("DeformationMap", "DeformationMapSRV");
	_DeformationMapSampler.SetDebugName("DeformationMapSampler");
	_DeformationConstantBuffer.SetDebugName("DeformationConstantBuffer");

	InitForOffsetMap();
	InitForParallelMap();
}

void GameApp::InitForOffsetMap()
{
	SHADER_DECLARE_DESC shaderDesc;
	shaderDesc.CsoName = L"CalculateOffsetPassVS.cso";
	shaderDesc.FileName = L"shaders/CalculateOffsetPassVS.hlsl";
	shaderDesc.EntryPoint = "main";
	shaderDesc.ShaderModel = "vs_5_0";
	HR(_CalculateOffsetMapShader.VSDeclare(_pd3dDevice.Get(), shaderDesc));
	shaderDesc.CsoName = L"CalculateOffsetPassPS.cso";
	shaderDesc.FileName = L"shaders/CalculateOffsetPassPS.hlsl";
	shaderDesc.EntryPoint = "main";
	shaderDesc.ShaderModel = "ps_5_0";
	HR(_CalculateOffsetMapShader.PSDeclare(_pd3dDevice.Get(), shaderDesc));

	HR(_OffsetMap.Declare(_pd3dDevice.Get(), 2, 2, DXGI_FORMAT_R8G8B8A8_SNORM, false));
	_OffsetMapView.Declare(&_OffsetMap);

	D3D11_SAMPLER_DESC samplerDesc;
	_OffsetMapSampler.GetDesc(samplerDesc);
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;		// 确保按照点采样
	HR(_OffsetMapSampler.Delcare(_pd3dDevice.Get(), samplerDesc));


	_OffsetMapData.Data = DirectX::XMFLOAT4(2, 2, 0, 0);
	HR(_OffsetMapConstantBuffer.Declare(_pd3dDevice.Get()));
	_OffsetMapConstantBuffer.SetBuffer(_OffsetMapData);
	_OffsetMapConstantBuffer.Apply(_pd3dDeviceContext.Get());


	_CalculateOffsetMapShader.VSSetDebugName("CalculateOffsetMapVS");
	_CalculateOffsetMapShader.PSSetDebugName("CalculateOffsetMapVS");
	_OffsetMap.SetDebugName("OffsetMap", "OffsetMapSRV");
	_OffsetMapSampler.SetDebugName("OffsetMapSampler");
	_OffsetMapConstantBuffer.SetDebugName("OffsetMapConstantBuffer");
}

void GameApp::InitForParallelMap()
{
	HR(_ParallelMap.Declare(_pd3dDevice.Get(), 512, 512, DXGI_FORMAT_R8G8B8A8_SNORM, false));
	_ParallelMapView.Declare(&_ParallelMap);

	D3D11_SAMPLER_DESC samplerDesc;
	_ParallelMapSampler.GetDesc(samplerDesc);
	HR(_ParallelMapSampler.Delcare(_pd3dDevice.Get(), samplerDesc));

	SHADER_DECLARE_DESC shaderDesc;
	shaderDesc.CsoName = L"CalculateParallelMapVS.cso";
	shaderDesc.FileName = L"CalculateParallelMapVS.hlsl";
	shaderDesc.EntryPoint = "main";
	shaderDesc.ShaderModel = "vs_5_0";
	HR(_CalculateParallelMapShader.VSDeclare(_pd3dDevice.Get(), shaderDesc));
	shaderDesc.CsoName = L"CalculateParallelMapPS.cso";
	shaderDesc.FileName = L"CalculateParallelMapPS.hlsl";
	shaderDesc.EntryPoint = "main";
	shaderDesc.ShaderModel = "ps_5_0";
	HR(_CalculateParallelMapShader.PSDeclare(_pd3dDevice.Get(), shaderDesc));


	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	_pd3dDevice->CreateRasterizerState(&rasterizerDesc, _ParallelMapRasterizerState.ReleaseAndGetAddressOf());

	_CalculateParallelMapShader.VSSetDebugName("CalculateParallelMapVS");
	_CalculateParallelMapShader.PSSetDebugName("CalculateParallelMapVS");
	_ParallelMap.SetDebugName("ParallelMap", "ParallelMapSRV");
	_ParallelMapSampler.SetDebugName("ParallelMapSampler");
}
