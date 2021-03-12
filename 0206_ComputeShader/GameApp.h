#pragma once

#include <D3DApp.h>

class GameApp : public D3DApp
{
public:
	GameApp(HINSTANCE hInstance, int width = 800, int height = 600);
	~GameApp();

	virtual bool Init() override;
	virtual void UpdateScene(float dt) override;
	virtual void DrawScene() override;
protected:
private:
	void InitShader();
	void InitResource();
private:
};