#include "includes/D3DUtil.h"

// ------------------------------
// CreateBuffer函数
// ------------------------------
// 创建缓冲区
// [In]d3dDevice			D3D设备
// [In]data					初始化结构化数据
// [In]byteWidth			缓冲区字节数
// [Out]structuredBuffer	输出的结构化缓冲区
// [In]usage				资源用途
// [In]bindFlags			资源绑定标签
// [In]cpuAccessFlags		资源CPU访问权限标签
// [In]structuredByteStride 每个结构体的字节数
// [In]miscFlags			资源杂项标签
static HRESULT CreateBuffer(
	ID3D11Device* d3dDevice,
	void* data,
	UINT byteWidth,
	ID3D11Buffer** buffer,
	D3D11_USAGE usage,
	UINT bindFlags,
	UINT cpuAccessFlags,
	UINT structureByteStride,
	UINT miscFlags);

HRESULT CreateShaderFromFile(const WCHAR* csoFileNameInOut, const WCHAR* hlslFileName,
	LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	// 寻找是否有已经编译好的顶点着色器
	if (csoFileNameInOut && D3DReadFileToBlob(csoFileNameInOut, ppBlobOut) == S_OK)
	{
		return hr;
	}
	else
	{
		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
		// 设置 D3DCOMPILE_DEBUG 标志用于获取着色器调试信息。该标志可以提升调试体验，
		// 但仍然允许着色器进行优化操作
		dwShaderFlags |= D3DCOMPILE_DEBUG;

		// 在Debug环境下禁用优化以避免出现一些不合理的情况
		dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
		ID3DBlob* errorBlob = nullptr;
		hr = D3DCompileFromFile(hlslFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, shaderModel,
			dwShaderFlags, 0, ppBlobOut, &errorBlob);
		if (FAILED(hr))
		{
			if (errorBlob != nullptr)
			{
				OutputDebugStringA(reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
			}
			SAFE_RELEASE(errorBlob);
			return hr;
		}

		// 若指定了输出文件名，则将着色器二进制信息输出
		if (csoFileNameInOut)
		{
			return D3DWriteBlobToFile(*ppBlobOut, csoFileNameInOut, FALSE);
		}
	}

	return hr;
}

HRESULT CreateBuffer(
	ID3D11Device* d3dDevice,
	void* data,
	UINT byteWidth,
	ID3D11Buffer** buffer,
	D3D11_USAGE usage,
	UINT bindFlags,
	UINT cpuAccessFlags,
	UINT structureByteStride,
	UINT miscFlags)
{
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = usage;
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.BindFlags = bindFlags;
	bufferDesc.CPUAccessFlags = cpuAccessFlags;
	bufferDesc.StructureByteStride = structureByteStride;
	bufferDesc.MiscFlags = miscFlags;

	if (data)
	{
		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory(&initData, sizeof(initData));
		initData.pSysMem = data;
		return d3dDevice->CreateBuffer(&bufferDesc, &initData, buffer);
	}
	else
	{
		return d3dDevice->CreateBuffer(&bufferDesc, nullptr, buffer);
	}


}

HRESULT CreateVertexBuffer(
	ID3D11Device* d3dDevice,
	void* data,
	UINT byteWidth,
	ID3D11Buffer** vertexBuffer,
	bool dynamic,
	bool streamOutput)
{
	UINT bindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_USAGE usage;
	UINT cpuAccessFlags = 0;
	if (dynamic && streamOutput)
	{
		return E_INVALIDARG;
	}
	else if (!dynamic && !streamOutput)
	{
		usage = D3D11_USAGE_IMMUTABLE;
	}
	else if (dynamic)
	{
		usage = D3D11_USAGE_DYNAMIC;
		cpuAccessFlags |= D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		bindFlags |= D3D11_BIND_STREAM_OUTPUT;
		usage = D3D11_USAGE_DEFAULT;
	}

	return CreateBuffer(d3dDevice, data, byteWidth, vertexBuffer,
		usage, bindFlags, cpuAccessFlags, 0, 0);
}