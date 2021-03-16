#pragma once

#include <DirectXMath.h>

using namespace DirectX;

struct DirectionLight
{
	DirectionLight()
		: _Param1(0.0f, 0.0f, 0.0f, 0.0f), _Param2(0.0f, 0.0f, 0.0f, 0.0f)
	{}

	void SetColor(const XMFLOAT3& color) 
	{
		_Param1.x = color.x;
		_Param1.y = color.y;
		_Param1.z = color.z;
	}
	void SetIntensity(const float& intensity)
	{
		_Param1.w = intensity;
	}
	void SetDirection(const XMFLOAT3& direction)
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
	void GetIntensity(float& Output)
	{
		Output = _Param1.w;
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
	PointLight()
		: _Param1(0.0f, 0.0f, 0.0f, 0.0f), _Param2(0.0f, 0.0f, 0.0f, 0.0f)
	{}

	void SetColor(const XMFLOAT3& color)
	{
		_Param1.x = color.x;
		_Param1.y = color.y;
		_Param1.z = color.z;
	}
	void SetIntensity(const float& Intensity)
	{
		_Param1.w = Intensity;
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