#pragma once

#include <Common.h>
#include <VertexLayoutCommon.h>

class Plane
{
public:
	HRESULT Declare(ID3D11Device* device, int vertexNumPerRowCol);

	UINT GetVertexNum()const;
	UINT GetIndexNum() const;

	void Render(ID3D11DeviceContext* deviceContext);
	void RenderPoint(ID3D11DeviceContext* deviceContext);
private:
	bool _IsDeclared;

	UINT _VertexNum;
	UINT _IndexNum;

	ComPtr<ID3D11Buffer> _pVertexBuffer;
	ComPtr<ID3D11Buffer> _pIndexBuffer;
};