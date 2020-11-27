#pragma once

#include <d3d11.h>

namespace MathUtil
{
	UINT NextPowerOf2(UINT value)
	{
		value--;
		value |= value >> 1;
		value |= value >> 2;
		value |= value >> 4;
		value |= value >> 8;
		value |= value >> 16;
		value++;
		return value;
	}
}