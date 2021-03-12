#pragma once

#include <D3DApp.h>

#include <ConstantBuffer.h>
#include <ConstantBufferObjectCommon.h>

#include <Shader.h>

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
	Shader _BasePassShader;
	ConstantBuffer<ViewConstantBuffer> _ViewConstantBuffer;
	ConstantBuffer<ObjectConstantBuffer> _ObjectConstantBuffer;
};