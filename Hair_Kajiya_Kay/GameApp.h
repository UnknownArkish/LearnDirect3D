#pragma once

#include <D3DApp.h>

#include <Shader.h>
#include <ConstantBuffer.h>
#include <ConstantBufferObjectCommon.h>

#include <SamplerState.h>
#include <TextureView.h>
#include <Texture2D.h>

#include <RenderTexture.h>

#include <Lighting.h>

struct LightConstantBufferData
{
	PointLight PointLight;
};

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
	Shader _ShaderHair;
	ConstantBuffer<ViewConstantBuffer> _ConstantBufferView;
	ConstantBuffer<ObjectConstantBuffer> _ConstantBufferObject;
	ConstantBuffer<LightConstantBufferData> _ConstantBufferLight;
};