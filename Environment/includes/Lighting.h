#pragma once

#include <DirectXMath.h>

using namespace DirectX;

struct DirectionLight
{
	void SetColor(XMFLOAT3& color) 
	{
		_Param1.x = color.x;
		_Param1.y = color.y;
		_Param1.z = color.z;
	}
	void SetDirection(XMFLOAT3& direction)
	{
		_Param2.x = direction.x;
		_Param2.y = direction.y;
		_Param2.z = direction.z;
	}

	void GetColor(XMFLOAT3& output)
	{
		output.x = _Param1.x;
		output.y = _Param1.y;
		output.z = _Param1.z;
	}
	void GetDirection(XMFLOAT3& output)
	{
		output.x = _Param2.x;
		output.y = _Param2.y;
		output.z = _Param2.z;
	}
private:
	XMFLOAT4 _Param1;
	XMFLOAT4 _Param2;
};

struct PointLight
{
	void SetColor(const XMFLOAT3& color)
	{
		_Param1.x = color.x;
		_Param1.y = color.y;
		_Param1.z = color.z;
	}
	void SetPosition(const XMFLOAT3& position)
	{
		_Param2.x = position.x;
		_Param2.y = position.y;
		_Param2.z = position.z;
	}
	void SetRadius(const float& radius)
	{
		_Param2.w = radius;
	}

	void GetColor(XMFLOAT3& output)
	{
		output.x = _Param1.x;
		output.y = _Param1.y;
		output.z = _Param1.z;
	}
	void GetPosition(XMFLOAT3& output)
	{
		output.x = _Param2.x;
		output.y = _Param2.y;
		output.z = _Param2.z;
	}
	void GetRadius(float& radius)
	{
		radius - _Param2.w;
	}
private:
	XMFLOAT4 _Param1;
	XMFLOAT4 _Param2;
};