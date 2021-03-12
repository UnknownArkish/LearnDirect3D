#include "GameApp.h"
#include <DXTrace.h>
#include <MathUtil.h>

using namespace DirectX;

const UINT VERTEX_NUM_PER_ROW_COL = 16;
const float DEFORMATION_TEX_WIDTH = 2;
const float DEFORMATION_TEX_HEIGHT = 2;
const float DEFORMATION_TEX_INTENSITY = 1;

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
}

static bool firstDraw = true;

void GameApp::DrawScene()
{
	assert(_pd3dDeviceContext);
	assert(_pSwapChain);
	static float blue[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	_pd3dDeviceContext->ClearRenderTargetView(_pRenderTargetView.Get(), blue);
	_pd3dDeviceContext->ClearDepthStencilView(_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	HR(_pSwapChain->Present(0, 0));
}

void GameApp::InitShader()
{

}

void GameApp::InitResource()
{
}
