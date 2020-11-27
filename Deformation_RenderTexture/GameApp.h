#pragma once

#include <D3DApp.h>

#include <ConstantBuffer.h>
#include <ConstantBufferObjectCommon.h>

#include <Shader.h>
#include <Texture2D.h>
#include <RenderTexture.h>
#include <TextureView.h>
#include <SamplerState.h>
#include <Lighting.h>

struct OffsetMapConstantBuffer
{
	DirectX::XMFLOAT4 Data;
};

struct DeformationConstantBuffer
{
	DirectX::XMFLOAT3 ForwardWS; float _pad_0;
	DirectX::XMFLOAT3 OriginWS; float _pad_1;

	// x : Width, y : Height z : Intensity
	DirectX::XMFLOAT4 Params;
};

struct ParallelMapConstantBuffer
{
	float ParallelStepScale;
	float ParallelIntensity;
	XMFLOAT2 _Pad_0;
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

	void InitForDeformation();
	void InitForOffsetMap();
	void InitForParallelMap();
private:
	Shader _BasePassShader;
	ConstantBuffer<ViewConstantBuffer> _ViewConstantBuffer;
	ConstantBuffer<ObjectConstantBuffer> _ObjectConstantBuffer;
	ConstantBuffer<PointLight> _LightConstantBuffer;
	ConstantBuffer<ParallelMapConstantBuffer> _ParallelMapConstantBuffer;
	Texture2D _MainTex;
	TextureView _MainTexView;
	SamplerState _MainTexSampler;
	Texture2D _NormalTex;
	TextureView _NormalTexView;
	SamplerState _NormalTexSampler;


	Shader _CalculateOffsetMapShader;
	RenderTexture _OffsetMap;
	TextureView _OffsetMapView;
	SamplerState _OffsetMapSampler;
	ConstantBuffer<OffsetMapConstantBuffer> _OffsetMapConstantBuffer;
	OffsetMapConstantBuffer _OffsetMapData;


	Shader _CalculateParallelMapShader;
	RenderTexture _ParallelMap;
	TextureView _ParallelMapView;
	SamplerState _ParallelMapSampler;
	ComPtr<ID3D11RasterizerState> _ParallelMapRasterizerState;

	// ParallelMap的ConstantBuffer
	ConstantBuffer<DeformationConstantBuffer> _DeformationConstantBuffer;
	Texture2D _DeformationMap;
	TextureView _DeformationMapView;
	SamplerState _DeformationMapSampler;
};