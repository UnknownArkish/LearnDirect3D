#pragma once

#include <D3DApp.h>

#include <ConstantBuffer.h>
#include <ConstantBufferObjectCommon.h>
#include <Shader.h>
#include <Texture.h>
#include <TextureView.h>
#include <SamplerState.h>
#include <Lighting.h>

struct LightingConstantBufferData
{
	LightingConstantBufferData()
		: NumDirectionLight(0), NumPointLight(0),
		_pad_0(), _pad_1()
	{}

	DirectionLight DirectionLights[16];
	PointLight PointLights[16];
	int NumDirectionLight;
	int NumPointLight;
	float _pad_0; float _pad_1;
};

struct MaterialConstantBufferData
{
	MaterialConstantBufferData()
		: BaseColor(0.0f, 0.0f, 0.0f)
	{}

	DirectX::XMFLOAT3 BaseColor;
	float _pad_0;
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

	GBufferSheet GBuffer0;
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
	void InitOCB();
	void InitMCB();

	void SetGBufferAsRenderTarget();
	void UnsetGBufferAsRenderTarget();
	void SetGBufferAsResourceView();
	void UnsetGBufferAsResourceView();

	void SetGbuffer0AsRenderTarget();
	void UnsetGBuffer0AsRenderTarget();
	void SetGbuffer0AsResourceView();
	void UnsetGBuffer0AsResourceView();

	void CacheGBuffer0();
private:
	Shader _BasePassShader;
	Shader _DeferredPassShader;
	Shader _SSGIPassShader;
	Shader _EndPassShader;

	ObjectConstantBufferData OCB_Center;
	ObjectConstantBufferData OCB_Left;
	ObjectConstantBufferData OCB_Down;
	ObjectConstantBufferData OCB_Right;

	MaterialConstantBufferData MCB_Red;
	MaterialConstantBufferData MCB_Green;
	MaterialConstantBufferData MCB_Blue;
	MaterialConstantBufferData MCB_White;

	ConstantBuffer<ViewConstantBufferData> _ViewConstantBuffer;
	ConstantBuffer<ObjectConstantBufferData> _ObjectConstantBuffer;

	ConstantBuffer<LightingConstantBufferData> _LightingConstantBuffer;
	ConstantBuffer<MaterialConstantBufferData> _MaterialConstantBuffer;

	ComPtr<ID3D11RenderTargetView> _pCachedRTV;
	ComPtr<ID3D11DepthStencilView> _pCachedDSV;
	D3D11_VIEWPORT _CacheVP;

private:
	// GBuffer Start
	GBufferSheet TempGBuffer0;
	GBufferSheets GBuffer;
	// GBuffer End
};