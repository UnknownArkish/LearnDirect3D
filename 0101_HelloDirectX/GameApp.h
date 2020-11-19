#pragma once

#include <D3DApp.h>

#include <Shader.h>
#include <ConstantBuffer.h>
#include <ConstantBufferObjectCommon.h>

class GameApp : public D3DApp
{
public:
	GameApp(HINSTANCE hInstance);
	~GameApp();

	virtual bool Init() override;
	virtual void OnResize() override;
	virtual void UpdateScene(float dt) override;
	virtual void DrawScene() override;

protected:
	virtual void InitShader();
	virtual void InitResource();

private:
	Shader _BasePassShader;
	ConstantBuffer<0, BasePassBuffer> _ConstantBuffer;
};