#include "GameApp.h"
#include <DXTrace.h>
#include <MathUtil.h>
#include <vector>

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
	InitGBuffer();
	InitLighting();
	InitOCB();
	InitMCB();

	return true;
}

void GameApp::UpdateScene(float dt)
{
	//static float phi = 0.0f, theta = 0.0f;
	//static float intensity = 5.0f;
	//if (_pInput->IsButtonDown(Input::LeftButton))
	//{
	//	DirectX::XMFLOAT2 mouseOffset = _pInput->GetMousePositionOffset();
	//	phi += mouseOffset.y * dt * intensity;
	//	theta += mouseOffset.x * dt * intensity;
	//}

	//ObjectConstantBuffer buffer;
	//_ObjectConstantBuffer.GetBuffer(buffer);
	//buffer.Local2World = XMMatrixRotationX(phi) * XMMatrixRotationY(theta);
	//buffer.World2Local = XMMatrixInverse(nullptr, buffer.Local2World);
	//_ObjectConstantBuffer.SetBuffer(buffer);
	//_ObjectConstantBuffer.Apply(_pd3dDeviceContext.Get());
}

void GameApp::DrawScene()
{
	assert(_pd3dDeviceContext);
	assert(_pSwapChain);
	static float blue[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	_pd3dDeviceContext->ClearRenderTargetView(_pRenderTargetView.Get(), blue);
	_pd3dDeviceContext->ClearDepthStencilView(_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// 缓存屏幕RT
	UINT NumViewPort = 1;
	_pd3dDeviceContext->OMGetRenderTargets(1, _pCachedRTV.GetAddressOf(), _pCachedDSV.GetAddressOf());
	_pd3dDeviceContext->RSGetViewports(&NumViewPort, &_CacheVP);

	// 设置GBuffer为目标
	SetGBufferAsRenderTarget();
	{
		// 1. BasePass
		_ViewConstantBuffer.VSBind(_pd3dDeviceContext.Get(), 0);
		_ObjectConstantBuffer.VSBind(_pd3dDeviceContext.Get(), 1);
		_ViewConstantBuffer.PSBind(_pd3dDeviceContext.Get(), 0);
		_MaterialConstantBuffer.PSBind(_pd3dDeviceContext.Get(), 1);

		_BasePassShader.Use(_pd3dDeviceContext.Get());

		// Center
		_ObjectConstantBuffer.SetBuffer(OCB_Center);
		_ObjectConstantBuffer.Apply(_pd3dDeviceContext.Get());
		_MaterialConstantBuffer.SetBuffer(MCB_White);
		_MaterialConstantBuffer.Apply(_pd3dDeviceContext.Get());
		_pRenderer->RenderSphere(_pd3dDeviceContext.Get());

		// Left
		_ObjectConstantBuffer.SetBuffer(OCB_Left);
		_ObjectConstantBuffer.Apply(_pd3dDeviceContext.Get());
		_MaterialConstantBuffer.SetBuffer(MCB_Red);
		_MaterialConstantBuffer.Apply(_pd3dDeviceContext.Get());
		_pRenderer->RenderCube(_pd3dDeviceContext.Get());

		// Down
		_ObjectConstantBuffer.SetBuffer(OCB_Down);
		_ObjectConstantBuffer.Apply(_pd3dDeviceContext.Get());
		_MaterialConstantBuffer.SetBuffer(MCB_Green);
		_MaterialConstantBuffer.Apply(_pd3dDeviceContext.Get());
		_pRenderer->RenderCube(_pd3dDeviceContext.Get());

		// Right
		_ObjectConstantBuffer.SetBuffer(OCB_Right);
		_ObjectConstantBuffer.Apply(_pd3dDeviceContext.Get());
		_MaterialConstantBuffer.SetBuffer(MCB_Blue);
		_MaterialConstantBuffer.Apply(_pd3dDeviceContext.Get());
		_pRenderer->RenderCube(_pd3dDeviceContext.Get());
	}
	UnsetGBufferAsRenderTarget();


	SetGbuffer0AsRenderTarget();
	{
		SetGBufferAsResourceView();
		// 2. DeferredPass
		{
			_ViewConstantBuffer.PSBind(_pd3dDeviceContext.Get(), 0);
			_LightingConstantBuffer.PSBind(_pd3dDeviceContext.Get(), 1);

			GBuffer.GBufferSampler.PSBind(_pd3dDeviceContext.Get(), 0);

			_DeferredPassShader.Use(_pd3dDeviceContext.Get());
			_pRenderer->RenderQuad(_pd3dDeviceContext.Get());
		}
		CacheGBuffer0();

		// 3. SSGI Pass
		{

		}
		CacheGBuffer0();
		UnsetGBufferAsResourceView();
	}
	UnsetGBuffer0AsRenderTarget();

	// 恢复屏幕RT
	_pd3dDeviceContext->OMSetRenderTargets(1, _pCachedRTV.GetAddressOf(), _pCachedDSV.Get());
	_pd3dDeviceContext->RSSetViewports(NumViewPort, &_CacheVP);
	_pCachedRTV.Reset();
	_pCachedDSV.Reset();

	SetGbuffer0AsResourceView();
	{
		GBuffer.GBufferSampler.PSBind(_pd3dDeviceContext.Get(), 0);
		_EndPassShader.Use(_pd3dDeviceContext.Get());
		_pRenderer->RenderQuad(_pd3dDeviceContext.Get());
	}
	UnsetGBuffer0AsResourceView();

	HR(_pSwapChain->Present(0, 0));
}

void GameApp::InitShader()
{
	SHADER_DECLARE_DESC Desc;
	ZeroMemory(&Desc, sizeof(SHADER_DECLARE_DESC));
	Desc.FileName = L"BasePassVS.hlsl";
	Desc.CsoName = L"BasePassVS.cso";
	Desc.EntryPoint = "main";
	Desc.ShaderModel = "vs_4_0";
	HR(_BasePassShader.VSDeclare(_pd3dDevice.Get(), Desc));
	Desc.FileName = L"BasePassPS.hlsl";
	Desc.CsoName = L"BasePassPS.cso";
	Desc.EntryPoint = "main";
	Desc.ShaderModel = "ps_4_0";
	HR(_BasePassShader.PSDeclare(_pd3dDevice.Get(), Desc));

	Desc.FileName = L"DeferredPassVS.hlsl";
	Desc.CsoName = L"DeferredPassVS.cso";
	Desc.EntryPoint = "main";
	Desc.ShaderModel = "vs_4_0";
	HR(_DeferredPassShader.VSDeclare(_pd3dDevice.Get(), Desc));
	Desc.FileName = L"DeferredPassPS.hlsl";
	Desc.CsoName = L"DeferredPassPS.cso";
	Desc.EntryPoint = "main";
	Desc.ShaderModel = "ps_4_0";
	HR(_DeferredPassShader.PSDeclare(_pd3dDevice.Get(), Desc));

	Desc.FileName = L"EndPassVS.hlsl";
	Desc.CsoName = L"EndPassVS.cso";
	Desc.EntryPoint = "main";
	Desc.ShaderModel = "vs_4_0";
	HR(_EndPassShader.VSDeclare(_pd3dDevice.Get(), Desc));
	Desc.FileName = L"EndPassPS.hlsl";
	Desc.CsoName = L"EndPassPS.cso";
	Desc.EntryPoint = "main";
	Desc.ShaderModel = "ps_4_0";
	HR(_EndPassShader.PSDeclare(_pd3dDevice.Get(), Desc));

	_BasePassShader.VSSetDebugName("BasePassVS");
	_BasePassShader.PSSetDebugName("BasePassPS");
	_DeferredPassShader.VSSetDebugName("DeferredPassVS");
	_DeferredPassShader.PSSetDebugName("DeferredPassPS");
	_EndPassShader.VSSetDebugName("EndPassVS");
	_EndPassShader.PSSetDebugName("EndPassPS");
}

void GameApp::InitResource()
{
	ViewConstantBufferData viewData;
	viewData.World2View = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixLookAtLH(
			DirectX::XMVectorSet(0.0f, 7, -10.0f, 0.0f),
			DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
		));
	viewData.View2Proj = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PI / 2, AspectRatio(), 1.0f, 1000.0f));;
	viewData.ViewPosWS = DirectX::XMFLOAT3(0.0f, 3.0f, -8.0f);
	HR(_ViewConstantBuffer.Declare(_pd3dDevice.Get()));
	_ViewConstantBuffer.SetBuffer(viewData);
	_ViewConstantBuffer.Apply(_pd3dDeviceContext.Get());

	_ViewConstantBuffer.SetDebugName("ViewConstantBuffer");
}

void GameApp::InitGBuffer()
{
	HR(TempGBuffer0.Declare(_pd3dDevice.Get(), m_ClientWidth, m_ClientHeight));
	HR(GBuffer.Declare(_pd3dDevice.Get(), m_ClientWidth, m_ClientHeight));
}

void GameApp::InitLighting()
{
	HR(_LightingConstantBuffer.Declare(_pd3dDevice.Get()));
	LightingConstantBufferData LightingData;
	_LightingConstantBuffer.GetBuffer(LightingData);

	LightingData.DirectionLights[0].SetColor(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	LightingData.DirectionLights[0].SetDirection(DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f));

	LightingData.PointLights[0].SetColor(DirectX::XMFLOAT3(0.5f, 0.6f, 0.8f));
	LightingData.PointLights[0].SetIntensity(4);
	LightingData.PointLights[0].SetPosition(DirectX::XMFLOAT3(0.0f, 3.5f, 3.0f));
	LightingData.PointLights[0].SetRadius(10.0f);

	LightingData.NumDirectionLight = 1;
	LightingData.NumPointLight = 1;
	_LightingConstantBuffer.SetBuffer(LightingData);
	_LightingConstantBuffer.Apply(_pd3dDeviceContext.Get());

	_LightingConstantBuffer.SetDebugName("LightingConstantBuffer");
}

void GameApp::InitOCB()
{
	ZeroMemory(&OCB_Left, sizeof(ObjectConstantBufferData));
	ZeroMemory(&OCB_Down, sizeof(ObjectConstantBufferData));
	ZeroMemory(&OCB_Right, sizeof(ObjectConstantBufferData));

	OCB_Center.Local2World = XMMatrixTranspose(XMMatrixScaling(3, 3, 3) * XMMatrixRotationZ(0.0f) * XMMatrixTranslation(0.0f, 0.0f, 0.0f));
	OCB_Center.World2Local = XMMatrixInverse(nullptr, OCB_Center.Local2World);

	OCB_Left.Local2World = XMMatrixTranspose( XMMatrixScaling(5, 1, 5) * XMMatrixRotationZ(XM_PI / 2) * XMMatrixTranslation(-5.0f, 0.0f, 0.0f));
	OCB_Left.World2Local = XMMatrixInverse(nullptr, OCB_Left.Local2World);

	OCB_Down.Local2World = XMMatrixTranspose( XMMatrixScaling(5, 1, 5) * XMMatrixTranslation(0.0f, -5.0f, 0.0f));
	OCB_Down.World2Local = XMMatrixInverse(nullptr, OCB_Down.Local2World);

	OCB_Right.Local2World = XMMatrixTranspose( XMMatrixScaling(5, 1, 5) * XMMatrixRotationZ(XM_PI / 2) * XMMatrixTranslation(5.0f, 0.0f, 0.0f));
	OCB_Right.World2Local = XMMatrixInverse(nullptr, OCB_Right.Local2World);

	HR(_ObjectConstantBuffer.Declare(_pd3dDevice.Get()));
	_ObjectConstantBuffer.SetDebugName("ObjectConstantBuffer");
}

void GameApp::InitMCB()
{
	MCB_Red.BaseColor = XMFLOAT3(1.0f, 0.0f, 0.0f);
	MCB_Green.BaseColor = XMFLOAT3(0.0f, 1.0f, 0.0f);
	MCB_Blue.BaseColor = XMFLOAT3(0.0f, 0.0f, 1.0f);
	MCB_White.BaseColor = XMFLOAT3(1.0f, 1.0f, 1.0f);

	HR(_MaterialConstantBuffer.Declare(_pd3dDevice.Get()));
	_MaterialConstantBuffer.SetDebugName("MaterialConstantBuffer");
}

void GameApp::SetGBufferAsRenderTarget()
{
	std::vector<GBufferSheet*> Sheets;
	GBuffer.Load(Sheets);

	std::vector<ID3D11RenderTargetView*> pRenderTargetViews;
	for (GBufferSheet* Sheet : Sheets)
	{
		static float black[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		_pd3dDeviceContext->ClearRenderTargetView(Sheet->pRTV.Get(), black);
		pRenderTargetViews.push_back(Sheet->pRTV.Get());
	}
	// Clear Depth & Stencil
	_pd3dDeviceContext->ClearDepthStencilView(GBuffer.pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	_pd3dDeviceContext->OMSetRenderTargets(Sheets.size(), pRenderTargetViews.data(), GBuffer.pDepthStencilView.Get());
	_pd3dDeviceContext->RSSetViewports(1, &GBuffer.Viewport);
}

void GameApp::UnsetGBufferAsRenderTarget()
{
	std::vector<GBufferSheet*> Sheets;
	GBuffer.Load(Sheets);

	std::vector<ID3D11RenderTargetView*> pRenderTargetViews;
	for (GBufferSheet* Sheet : Sheets)
	{
		pRenderTargetViews.push_back(nullptr);
	}
	
	_pd3dDeviceContext->OMSetRenderTargets(Sheets.size(), pRenderTargetViews.data(), nullptr);
}

void GameApp::SetGbuffer0AsRenderTarget()
{
	static float black[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	_pd3dDeviceContext->ClearRenderTargetView(TempGBuffer0.pRTV.Get(), black);
	_pd3dDeviceContext->ClearRenderTargetView(GBuffer.GBuffer0.pRTV.Get(), black);
	_pd3dDeviceContext->ClearDepthStencilView(GBuffer.pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	_pd3dDeviceContext->OMSetRenderTargets(1, GBuffer.GBuffer0.pRTV.GetAddressOf(), GBuffer.pDepthStencilView.Get());
	_pd3dDeviceContext->RSSetViewports(1, &GBuffer.Viewport);
}

void GameApp::UnsetGBuffer0AsRenderTarget()
{
	std::vector<ID3D11RenderTargetView*> pRenderTargetViews;
	pRenderTargetViews.push_back(nullptr);
	
	_pd3dDeviceContext->OMSetRenderTargets(1, pRenderTargetViews.data(), nullptr);
}

void GameApp::SetGbuffer0AsResourceView()
{
	GBuffer.GBuffer0.View.PSBind(_pd3dDeviceContext.Get(), 0);
}

void GameApp::UnsetGBuffer0AsResourceView()
{
	std::vector<ID3D11ShaderResourceView*> _pSRVs;
	_pSRVs.push_back(nullptr);
	_pd3dDeviceContext->PSSetShaderResources(0, 1, _pSRVs.data());
}

void GameApp::CacheGBuffer0()
{
	ID3D11Resource* ToCopy, * ToPaste;
	GBuffer.GBuffer0.pRTV->GetResource(&ToCopy);
	TempGBuffer0.pRTV->GetResource(&ToPaste);
	_pd3dDeviceContext->CopyResource(ToPaste, ToCopy);
}


void GameApp::SetGBufferAsResourceView()
{
	std::vector<GBufferSheet*> Sheets;
	Sheets.push_back(&TempGBuffer0);
	GBuffer.Load(Sheets);

	for (int i = 0; i < Sheets.size(); i++)
	{
		Sheets[i]->View.PSBind(_pd3dDeviceContext.Get(), i);
	}
}

void GameApp::UnsetGBufferAsResourceView()
{
	std::vector<GBufferSheet*> Sheets;
	Sheets.push_back(&TempGBuffer0);
	GBuffer.Load(Sheets);

	std::vector<ID3D11ShaderResourceView*> _pSRVs;
	for (int i = 0; i < Sheets.size(); i++) _pSRVs.push_back(nullptr);

	_pd3dDeviceContext->PSSetShaderResources(0, Sheets.size(), _pSRVs.data());
}

GBufferSheet::GBufferSheet()
	: Desc(), pRTV()
{}

HRESULT GBufferSheet::Declare(ID3D11Device* Device, UINT Width, UINT Height)
{
	HRESULT Result = E_NOTIMPL;

	// Resource
	ZeroMemory(&Desc, sizeof(D3D11_TEXTURE2D_DESC));
	Desc.Width = Width;
	Desc.Height = Height;
	Desc.MipLevels = 1;
	Desc.ArraySize = 1;
	Desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.Usage = D3D11_USAGE_DEFAULT;
	Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	Desc.CPUAccessFlags = 0;

	ComPtr<ID3D11Texture2D> pTexture2D;
	Result = Device->CreateTexture2D(&Desc, nullptr, pTexture2D.GetAddressOf());
	if (FAILED(Result)) return Result;

	_pResource = pTexture2D;

	// RTV
	D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
	ZeroMemory(&RTVDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	RTVDesc.Format = Desc.Format;
	RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	RTVDesc.Texture2D.MipSlice = 0;
	Result = Device->CreateRenderTargetView(_pResource.Get(), &RTVDesc, pRTV.GetAddressOf());
	if (FAILED(Result))
	{
		pRTV.Reset();
		_pResource.Reset();
		return Result;
	}

	// SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	SRVDesc.Format = Desc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = -1;
	Result = Device->CreateShaderResourceView(_pResource.Get(), &SRVDesc, _pResourceView.GetAddressOf());
	if (FAILED(Result))
	{
		pRTV.Reset();
		_pResourceView.Reset();
		_pResource.Reset();
		return Result;
	}

	View.Declare(this);
	return Result;
}

UINT GBufferSheet::GetWidth() const
{
	return Desc.Width;
}

UINT GBufferSheet::GetHeight() const
{
	return Desc.Height;
}

DXGI_FORMAT GBufferSheet::GetFormat() const
{
	return Desc.Format;
}

GBufferSheets::GBufferSheets()
	: pDepthStencilResource(), pDepthStencilView(),
	Viewport(),
	GBufferA(), GBufferB()
{}

void GBufferSheets::Load(std::vector<GBufferSheet*>& Sheets)
{
	Sheets.push_back(&GBufferA);
	Sheets.push_back(&GBufferB);
	Sheets.push_back(&GBufferC);
}

#define DECLARE_GBUFFER_SHEET( Sheet, Device, Width, Height )	\
	Result = Sheet.Declare( Device, Width, Height );			\
	if(FAILED(Result)) goto GBUFFER_SHEETS_DECLARE_END;			\
	Sheet.SetDebugName(#Sheet, #Sheet"SRV");

HRESULT GBufferSheets::Declare(ID3D11Device* Device, UINT Width, UINT Height)
{
	HRESULT Result = E_NOTIMPL;
	ComPtr<ID3D11Texture2D> TempTex;

	DECLARE_GBUFFER_SHEET(GBuffer0, Device, Width, Height);
	DECLARE_GBUFFER_SHEET(GBufferA, Device, Width, Height);
	DECLARE_GBUFFER_SHEET(GBufferB, Device, Width, Height);
	DECLARE_GBUFFER_SHEET(GBufferC, Device, Width, Height);

	// Depth Resource
	D3D11_TEXTURE2D_DESC DepthResourceDesc;
	DepthResourceDesc.Width = Width;
	DepthResourceDesc.Height = Height;
	DepthResourceDesc.MipLevels = 0;
	DepthResourceDesc.ArraySize = 1;
	DepthResourceDesc.SampleDesc.Count = 1;
	DepthResourceDesc.SampleDesc.Quality = 0;
	DepthResourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthResourceDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthResourceDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthResourceDesc.CPUAccessFlags = 0;
	DepthResourceDesc.MiscFlags = 0;
	
	Result = Device->CreateTexture2D(&DepthResourceDesc, nullptr, TempTex.GetAddressOf());
	if (FAILED(Result))
	{
		goto GBUFFER_SHEETS_DECLARE_END;
	}
	pDepthStencilResource = TempTex;

	// DSV
	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;
	ZeroMemory(&DSVDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	DSVDesc.Format = DepthResourceDesc.Format;
	DSVDesc.Flags = 0;
	DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DSVDesc.Texture2D.MipSlice = 0;
	Result = Device->CreateDepthStencilView(pDepthStencilResource.Get(), &DSVDesc, pDepthStencilView.GetAddressOf());
	if (FAILED(Result)) goto GBUFFER_SHEETS_DECLARE_END;

	// Viewport
	Viewport.TopLeftX = 0.0f;
	Viewport.TopLeftY = 0.0f;
	Viewport.Width = static_cast<float>(Width);
	Viewport.Height = static_cast<float>(Height);
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;

	D3D11_SAMPLER_DESC SamplerDesc;
	GBufferSampler.GetDesc(SamplerDesc);
	SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	Result = GBufferSampler.Delcare(Device, SamplerDesc);

GBUFFER_SHEETS_DECLARE_END:
	return Result;
}