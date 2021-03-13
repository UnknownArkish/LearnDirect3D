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

#include "Plane.h"

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

	void InitForDeformationTex();
	void InitForDeformationMap();
private:
	Plane _Plane;

	Shader _BasePassShader;
	ConstantBuffer<ViewConstantBufferData> _ViewConstantBuffer;
	ConstantBuffer<ObjectConstantBufferData> _ObjectConstantBuffer;
	ConstantBuffer<PointLight> _LightConstantBuffer;
	ConstantBuffer<ParallelMapConstantBuffer> _ParallelMapConstantBuffer;
	Texture2D _MainTex;
	TextureView _MainTexView;
	SamplerState _MainTexSampler;
	Texture2D _NormalTex;
	TextureView _NormalTexView;
	SamplerState _NormalTexSampler;
	ComPtr<ID3D11RasterizerState> _pRasterizerState;

	// 变形贴图和变形参数
	ConstantBuffer<DeformationConstantBuffer> _DeformationConstantBuffer;
	Texture2D _DeformationTex;
	TextureView _DeformationTexView;
	SamplerState _DeformationTexSampler;

	// DeformationMap
	RenderTexture _DeformationMap;
	TextureView _DeformationMapView;
	SamplerState _DeformationMapSampler;
	Shader _CalculateDeformationMapShader;
	ComPtr<ID3D11RasterizerState> _DeformationMapRatserizerState;
	ComPtr<ID3D11BlendState> _DeformationMapBlendState;


	//Shader _CalculateOffsetMapShader;
	//RenderTexture _OffsetMap;
	//TextureView _OffsetMapView;
	//SamplerState _OffsetMapSampler;
	//ConstantBuffer<OffsetMapConstantBuffer> _OffsetMapConstantBuffer;
	//OffsetMapConstantBuffer _OffsetMapData;


	//Shader _CalculateParallelMapShader;
	//RenderTexture _ParallelMap;
	//TextureView _ParallelMapView;
	//SamplerState _ParallelMapSampler;
	//ComPtr<ID3D11RasterizerState> _ParallelMapRasterizerState;

};