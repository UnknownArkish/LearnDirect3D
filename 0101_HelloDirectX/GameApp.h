#pragma once

#include <D3DApp.h>

#include <Shader.h>
#include "ConstantBuffer.h"

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
	ComPtr<ID3D11InputLayout> _pInputLayout;
	ComPtr<ID3D11Buffer> _pVertexBuffer;
	ComPtr<ID3D11Buffer> _pIndexBuffer;
	ComPtr<ID3D11Buffer> _pConstantBuffer;

	//ComPtr<ID3D11VertexShader> _pVertexShader;
	//ComPtr<ID3D11PixelShader> _pPixelShader;
	Shader _BasePassShader;

	ConstantBuffer::BasePassBuffer _ConstantBuffer;
};