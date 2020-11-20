#pragma once

#include <D3DApp.h>

#include <Shader.h>
#include <ConstantBuffer.h>
#include <ConstantBufferObjectCommon.h>

#include <SamplerState.h>
#include <Texture2D.h>
#include <TextureView.h>

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

	Texture2D _Texture;

	SamplerState<0> _SamplerState;
	TextureView<0> _TextireView;
	ConstantBuffer<0, BasePassConstantBuffer> _ConstantBuffer;
};