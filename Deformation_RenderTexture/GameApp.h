#pragma once

#include <D3DApp.h>

#include <ConstantBuffer.h>
#include <ConstantBufferObjectCommon.h>

#include <Shader.h>
#include <Texture2D.h>
#include <RenderTexture.h>
#include <TextureView.h>
#include <SamplerState.h>

struct OffsetMapConstantBuffer
{
	DirectX::XMFLOAT4 Data;
};

struct DeformationConstantBuffer
{
	DirectX::XMFLOAT4 ForwardWS;
	DirectX::XMFLOAT4 OriginWS;

	// x : Width, y : Height
	DirectX::XMFLOAT4 Params;
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
	Texture2D _MainTex;
	SamplerState _MainTexSampler;
	TextureView _MainTexView;


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

	// ParallelMap的ConstantBuffer
	ConstantBuffer<DeformationConstantBuffer> _DeformationConstantBuffer;
	Texture2D _DeformationMap;
	TextureView _DeformationMapView;
	SamplerState _DeformationMapSampler;
};