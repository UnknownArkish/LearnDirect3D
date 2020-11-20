#pragma once

#include <D3DApp.h>

#include <Shader.h>
#include <ConstantBuffer.h>
#include <ConstantBufferObjectCommon.h>

#include <SamplerState.h>
#include <TextureView.h>
#include <Texture2D.h>

#include <RenderTexture.h>

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
	bool _FirstDraw;

	Shader _BasePassShader;

	Texture2D _Texture;
	RenderTexture _RenderTexture;
	TextureView<0> _TextireView;
	SamplerState<0> _SamplerState;

	ConstantBuffer<BasePassConstantBuffer> _ConstantBuffer;
};