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
	GameApp(HINSTANCE hInstance, int width, int height);
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
	ConstantBuffer<ViewConstantBufferData> _ConstantBufferView;
	ConstantBuffer<ObjectConstantBufferData> _ConstantBufferObject;
	ConstantBuffer<PointLight> _ConstantBufferLight;
};