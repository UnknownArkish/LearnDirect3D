#pragma once

#include <d3d11.h>
#include "Common.h"

class Renderer
{
private:
	struct RendererResource
	{
		RendererResource() : pVertexBuffer(nullptr), pIndexBuffer(nullptr), NumVertex(0), NumIndex(0) {}
		ComPtr<ID3D11Buffer> pVertexBuffer;
		ComPtr<ID3D11Buffer> pIndexBuffer;

		UINT NumVertex;
		UINT NumIndex;
	};

public:
	Renderer() :_bInit(false), _QuadResource(), _CubeResource(), _CylinderResource() {}

	void Init(ID3D11Device* device);

	void IASetInputLayout(ID3D11DeviceContext* deviceContext);

	void RenderQuad(ID3D11DeviceContext* deviceContext);
	void RenderQuadPoint(ID3D11DeviceContext* deviceContext);
	void RenderCube(ID3D11DeviceContext* deviceContext);
	void RenderCubePoint(ID3D11DeviceContext* deviceContext);
	void RenderCylinder(ID3D11DeviceContext* deviceContext);
	void RenderCylinderPoint(ID3D11DeviceContext* deviceContext);
private:
	void Render_Internal(ID3D11DeviceContext* deviceContext, const RendererResource& resource);
	void RenderPoint_Internal(ID3D11DeviceContext* deviceContext, const RendererResource& resource);

	void InitVertexLayout(ID3D11Device* device);
	void InitQuadResource(ID3D11Device* device);
	void InitCubeResource(ID3D11Device* device);
	void InitCylinderResource(ID3D11Device* device);

	void BindRendererResource(ID3D11DeviceContext* deviceContext, const RendererResource& resource);
private:
	bool _bInit;

	ComPtr<ID3D11InputLayout> _pUniversalInputLayout;

	RendererResource _QuadResource;
	RendererResource _CubeResource;
	RendererResource _CylinderResource;
};