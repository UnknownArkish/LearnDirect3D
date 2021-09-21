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

	// You can Init Resources(shader, texture et) here

	return true;
}

void GameApp::UpdateScene(float dt)
{
	// Add your UpdateScene code here
}

void GameApp::DrawScene()
{
	// Add your DrawScene code here
}