#pragma once

#include <D3DApp.h>

#include <ConstantBuffer.h>
#include <ConstantBufferObjectCommon.h>
#include <Shader.h>
#include <Texture.h>
#include <TextureView.h>
#include <SamplerState.h>
#include <Lighting.h>

struct LightingConstantBuffer
{
	LightingConstantBuffer()
		: NumDirectionLight(0), NumPointLight(0)
	{}

	DirectionLight DirectionLights[16];
	PointLight PointLights[16];
	int NumDirectionLight;
	int NumPointLight;
	float _pad_0; float _pad_1;
};

class GBufferSheet : public Texture
{
public:
	GBufferSheet();
	HRESULT Declare(ID3D11Device* Device, UINT Width, UINT Height);

	virtual UINT GetWidth() const override;
	virtual UINT GetHeight() const override;
	virtual DXGI_FORMAT GetFormat() const override;
public:
	D3D11_TEXTURE2D_DESC Desc;
	ComPtr<ID3D11RenderTargetView> pRTV;

	TextureView View;
};
class GBufferSheets
{
public:
	GBufferSheets();
	HRESULT Declare(ID3D11Device* Device, UINT Width, UINT Height);

	void Load(std::vector<GBufferSheet*>& Sheets);
public:
	ComPtr<ID3D11Resource> pDepthStencilResource;
	ComPtr<ID3D11DepthStencilView> pDepthStencilView;
	
	D3D11_VIEWPORT Viewport;

	GBufferSheet GBufferA;
	GBufferSheet GBufferB;
	GBufferSheet GBufferC;
	SamplerState GBufferSampler;
};

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
	void InitGBuffer();
	void InitLighting();

	void SetGBufferAsRenderTarget();
	void UnsetGBufferAsRenderTarget();
	void SetGBufferAsResourceView();
	void UnsetGBufferAsResourceView();
private:
	Shader _BasePassShader;
	Shader _DeferredPassShader;

	ConstantBuffer<ViewConstantBuffer> _ViewConstantBuffer;
	ConstantBuffer<ObjectConstantBuffer> _ObjectConstantBuffer;

	ConstantBuffer<LightingConstantBuffer> _LightingConstantBuffer;

	ComPtr<ID3D11RenderTargetView> _pCachedRTV;
	ComPtr<ID3D11DepthStencilView> _pCachedDSV;
	D3D11_VIEWPORT _CacheVP;

private:
	// GBuffer Start
	GBufferSheets GBuffer;
	// GBuffer End
};