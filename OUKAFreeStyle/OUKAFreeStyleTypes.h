#pragma once

#include <vector>
#include <DirectXMath.h>

struct OUKAMesh
{
public:

	void SetNumVertex(int NewNumVertex) { NumVertex = (int)fmaxf(NewNumVertex, 0.0f); }

	int GetNumVertex() { return NumVertex; }
	int GetNumIndex() { return NumIndex; }
private:
	int NumVertex;
	int NumIndex;

	std::vector<DirectX::XMFLOAT3> Positions;
	std::vector<DirectX::XMFLOAT2> UVs;
	std::vector<DirectX::XMFLOAT3> Colors;
	std::vector<DirectX::XMFLOAT3> Normals;
	std::vector<DirectX::XMFLOAT4> Tangents;
};


struct OUKAEdge
{
	int PointAIndex;
	int PointBIndex;

	/// <summary>
	/// 法线，Silhouette类型的描边
	/// </summary>
	DirectX::XMFLOAT3 Normal;

	/// <summary>
	// Crease边类型的角度
	/// </summary>
	float CreaseDegree;

	/// <summary>
	/// Border类型描边
	/// </summary>
	bool IsBorder;
};