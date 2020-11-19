#pragma once

#include <D3DApp.h>

#include <Shader.h>
#include <ConstantBuffer.h>
#include <ConstantBufferObjectCommon.h>

struct VertexOffset
{
	DirectX::XMFLOAT3 Offset;
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
	void InitInputLayout();
	void InitVertexBuffer();
	void InitShader();
	void InitOther();

	void DoFirstDraw();
private:
	bool _IsFirstDraw;

	// 第一个Pass使用的Shader，计算Offset
	Shader _CalculateOffsetPassShader;

	Shader _BasePassShader;
	ConstantBuffer<0, BasePassConstantBuffer> _ConstantBuffer;
	ComPtr<ID3D11Buffer> _pVBBase;				// Slot 0，IA_INPUT
	ComPtr<ID3D11Buffer> _pVBOffset;			// Slot 1，SO_TARGET | IA_INPUT
	ComPtr<ID3D11Buffer> _pIB;					// IndexBuffer
	ComPtr<ID3D11InputLayout> _pBasePassInputLayout;
};