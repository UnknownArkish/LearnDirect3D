#pragma once

#include <D3DApp.h>

#include <Shader.h>
#include <ConstantBuffer.h>
#include <ConstantBufferObjectCommon.h>

#include <SamplerState.h>
#include <TextureView.h>
#include <Texture2D.h>

#include <RenderTexture.h>

#include <Shader.h>

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
private:
	void InitShader();
	void InitResource();
private:
	ComPtr<ID3D11Buffer> _pTriangleVertexBuffer;

	ComPtr<ID3D11RasterizerState> _pRasterizerState;

	ComPtr<ID3D11InputLayout> _pInputLayout;

	Shader _BasePassShader;
};