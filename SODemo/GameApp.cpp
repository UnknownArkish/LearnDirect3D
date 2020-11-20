#include "GameApp.h"
#include <VertexLayoutCommon.h>
#include <DXTrace.h>
using namespace DirectX;

const D3D11_INPUT_ELEMENT_DESC BASE_PASS_INPUT_LAYOUT_DESC[3] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},

	// Offset
	{ "COLOR", 1, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

GameApp::GameApp(HINSTANCE hInstance)
	: D3DApp(hInstance), _IsFirstDraw(true)
{
}

GameApp::~GameApp()
{
}


bool GameApp::Init()
{
	if (!D3DApp::Init()) return false;

	InitInputLayout();
	InitVertexBuffer();
	InitShader();
	InitOther();

	return true;
}

void GameApp::UpdateScene(float dt)
{

}
void GameApp::DrawScene()
{
	assert(_pd3dDeviceContext);
	assert(_pSwapChain);
	static float blue[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA = (0,0,255,255)
	_pd3dDeviceContext->ClearRenderTargetView(_pRenderTargetView.Get(), blue);
	_pd3dDeviceContext->ClearDepthStencilView(_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	if (_IsFirstDraw) DoFirstDraw();

	// 绑定InputLayout
	_pd3dDeviceContext->IASetInputLayout(_pBasePassInputLayout.Get());
	_pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// VertxBuffer
	UINT stride = sizeof(UniversalVertexLayout);
	UINT offset = 0;
	_pd3dDeviceContext->IASetVertexBuffers(0, 1, _pVBBase.GetAddressOf(), &stride, &offset);
	stride = sizeof(VertexOffset);
	offset = 0;
	_pd3dDeviceContext->IASetVertexBuffers(1, 1, _pVBOffset.GetAddressOf(), &stride, &offset);
	// IndexBuffer
	_pd3dDeviceContext->IASetIndexBuffer(_pIB.Get(), DXGI_FORMAT_R32_UINT, 0);
	// Shader
	_BasePassShader.Use(_pd3dDeviceContext.Get());
	_ConstantBuffer.VSBind(_pd3dDeviceContext.Get());

	_pd3dDeviceContext->DrawIndexed(36, 0, 0);
	//_pRenderer->RenderCube(_pd3dDeviceContext.Get());

	HR(_pSwapChain->Present(0, 0));
}

void GameApp::InitInputLayout()
{
	ComPtr<ID3DBlob> blob;

	HR(CreateShaderFromFile(L"BasePassVS.cso", L"shaders/BasePassVS.hlsl", "main", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(_pd3dDevice->CreateInputLayout(BASE_PASS_INPUT_LAYOUT_DESC, 3, blob->GetBufferPointer(), blob->GetBufferSize(), _pBasePassInputLayout.ReleaseAndGetAddressOf()));
}

void GameApp::InitVertexBuffer()
{
	UniversalVertexLayout vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) }
	};
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(D3D11_BUFFER_DESC));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(vertices);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
	initData.pSysMem = vertices;
	HR(_pd3dDevice->CreateBuffer(&vbd, &initData, _pVBBase.ReleaseAndGetAddressOf()));

	VertexOffset verticesOffset[] =
	{
		{XMFLOAT3(0.0f, 0.0f, 0.0f)},
		{XMFLOAT3(0.0f, 0.0f, 0.0f)},
		{XMFLOAT3(0.0f, 0.0f, 0.0f)},
		{XMFLOAT3(0.0f, 0.0f, 0.0f)},
		{XMFLOAT3(0.0f, 0.0f, 0.0f)},
		{XMFLOAT3(0.0f, 0.0f, 0.0f)},
		{XMFLOAT3(0.0f, 0.0f, 0.0f)},
		{XMFLOAT3(0.0f, 0.0f, 0.0f)}
	};
	// GPU read/write
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(verticesOffset);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
	initData.pSysMem = verticesOffset;
	HR(_pd3dDevice->CreateBuffer(&vbd, &initData, _pVBOffset.ReleaseAndGetAddressOf()));

	DWORD indices[] = {
		// 正面
		0, 1, 2,
		2, 3, 0,
		// 左面
		4, 5, 1,
		1, 0, 4,
		// 顶面
		1, 5, 6,
		6, 2, 1,
		// 背面
		7, 6, 5,
		5, 4, 7,
		// 右面
		3, 2, 6,
		6, 7, 3,
		// 底面
		4, 0, 3,
		3, 7, 4
	};
	// 设置索引缓冲区描述
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof indices;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	// 新建索引缓冲区
	initData.pSysMem = indices;
	HR(_pd3dDevice->CreateBuffer(&ibd, &initData, _pIB.ReleaseAndGetAddressOf()));
}

void GameApp::InitShader()
{
	ShaderDeclareDesc desc;
	ZeroMemory(&desc, sizeof ShaderDeclareDesc);
	desc.EntryPoint = "main";

	desc.CsoName = L"BasePassVS.cso";
	desc.FileName = L"shaders/BasePassVS.hlsl";
	desc.ShaderModel = "vs_5_0";
	_BasePassShader.VSDeclare(_pd3dDevice.Get(), desc);
	desc.CsoName = L"BasePassPS.cso";
	desc.FileName = L"shaders/BasePassPS.hlsl";
	desc.ShaderModel = "ps_5_0";
	_BasePassShader.PSDeclare(_pd3dDevice.Get(), desc);

	desc.CsoName = L"CalculateOffsetPassVS.cso";
	desc.FileName = L"shaders/CalculateBasePassVS.hlsl";
	desc.ShaderModel = "vs_5_0";
	_CalculateOffsetPassShader.VSDeclare(_pd3dDevice.Get(), desc);
	desc.CsoName = L"CalculateOffsetPassGS.cso";
	desc.FileName = L"shaders/CalculateBasePassGS.hlsl";
	desc.ShaderModel = "gs_5_0";
	D3D11_SO_DECLARATION_ENTRY pDecal[] =
	{
		{ 0, "COLOR", 1, 0, 3, 0}
	};
	_CalculateOffsetPassShader.GSDeclareWithStreamOut(_pd3dDevice.Get(), desc, pDecal, 1);
	desc.CsoName = L"CalculateOffsetPassPS.cso";
	desc.FileName = L"shaders/CalculateBasePassPS.hlsl";
	desc.ShaderModel = "ps_5_0";
	_CalculateOffsetPassShader.PSDeclare(_pd3dDevice.Get(), desc);
}

void GameApp::InitOther()
{
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
}

void GameApp::DoFirstDraw()
{
	UINT offset[1] = { 0 };

	_pd3dDeviceContext->SOSetTargets(1, _pVBOffset.GetAddressOf(), offset);
	_CalculateOffsetPassShader.Use(_pd3dDeviceContext.Get());
	_ConstantBuffer.GSBind(_pd3dDeviceContext.Get());

	_pRenderer->RenderCubePoint(_pd3dDeviceContext.Get());
	_pd3dDeviceContext->SOSetTargets(0, { nullptr }, offset);

	_IsFirstDraw = false;
}