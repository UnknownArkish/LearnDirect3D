#pragma once

#include <D3DApp.h>

#include <ConstantBuffer.h>
#include <ConstantBufferObjectCommon.h>

#include <Shader.h>
#include <Texture2D.h>
#include <RenderTexture.h>
#include <TextureView.h>
#include <SamplerState.h>

struct OffsetConstantBuffer
{
	DirectX::XMFLOAT4 Data;
};

class GameApp : public D3DApp
{
public:
	GameApp(HINSTANCE hInstance);
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
	Texture2D _MainTex;
	SamplerState _MainTexSampler;
	TextureView _MainTexView;
	ConstantBuffer<BasePassConstantBuffer> _ConstantBuffer;
	BasePassConstantBuffer _BufferData;


	Shader _CalculateOffsetShader;
	RenderTexture _OffsetTex;
	SamplerState _OffsetTexSampler;
	TextureView _OffsetTexView;
	ConstantBuffer<OffsetConstantBuffer> _OffsetConstantBuffer;
	OffsetConstantBuffer _OffsetBufferData;
};