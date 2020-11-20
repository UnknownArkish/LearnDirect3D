#pragma once

class Texture2D;

#include "Texture.h"
#include "Texture2D.h"
#include "TextureView.h"

#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"

class TextureLoader
{
public:
	static HRESULT LoadFromDDS(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const wchar_t* fileName, Texture2D* outputTex, ITextureView* outputView, DirectX::DDS_ALPHA_MODE* alphaMode = nullptr);
	static HRESULT LoadFromWIC(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const wchar_t* fileName, Texture2D* outputTex, ITextureView* outputView);
};