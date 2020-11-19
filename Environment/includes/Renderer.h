#pragma once

#include <d3d11.h>
#include "Common.h"

class Renderer
{
private:
	struct RendererResource
	{
		RendererResource() : _pVertexBuffer(nullptr), _pIndexBuffer(nullptr) {}
		ComPtr<ID3D11Buffer> _pVertexBuffer;
		ComPtr<ID3D11Buffer> _pIndexBuffer;
	};

public:
	Renderer() :_bInit(false), _QuadResource(), _CubeResource() {}

	void Init(ID3D11Device* device);
	void RenderQuad(ID3D11DeviceContext* deviceContext);
	void RenderCube(ID3D11DeviceContext* deviceContext);
private:
	void InitVertexLayout(ID3D11Device* device);
	void InitQuadResource(ID3D11Device* device);
	void InitCubeResource(ID3D11Device* device);

	void BindRendererResource(ID3D11DeviceContext* deviceContext, const RendererResource& resource);
private:
	bool _bInit;

	ComPtr<ID3D11InputLayout> _pUniversalInputLayout;

	RendererResource _QuadResource;
	RendererResource _CubeResource;
};