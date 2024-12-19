#pragma once
#include <d3dcompiler.h>
#include <iostream>
#include <string>
#include <sstream>

#include "d3dx11effect.h"
#include "Vector3.h"
#include "Matrix.h"

namespace dae
{
	class Texture;
}

class EffectBase
{
public:

	EffectBase() = default;
	virtual ~EffectBase() = default;

	virtual void UpdateEffect(const dae::Matrix& WVPmatrix, const dae::Matrix& worldMatrix, const dae::Vector3& cameraPos) = 0;

	virtual void SetDiffuseMap(const dae::Texture* pDiffuseTexture) const = 0;
	virtual void SetNormalMap(const dae::Texture* pNormalMapTexture) const = 0;
	virtual void SetSpecularMap(const dae::Texture* pSpecularMapTexture) const = 0;
	virtual void SetGlossinessMap(const dae::Texture* pGlossMapTexture) const = 0;
	virtual void CycleTechniques() = 0;
	virtual ID3DX11EffectTechnique* GetTechnique() const = 0;


	static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile)
	{
		HRESULT result;
		ID3D10Blob* pErrorBlob{ nullptr };
		ID3DX11Effect* pEffect;

		DWORD shaderFlags = 0;

#if defined( DEBUG ) || defined( _DEBUG )
		shaderFlags |= D3DCOMPILE_DEBUG;
		shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		result = D3DX11CompileEffectFromFile(assetFile.c_str(),
			nullptr,
			nullptr,
			shaderFlags,
			0,
			pDevice,
			&pEffect,
			&pErrorBlob);

		if (FAILED(result))
		{
			if (pErrorBlob != nullptr)
			{
				const char* pErrors = static_cast<char*>(pErrorBlob->GetBufferPointer());

				std::wstringstream ss;
				for (unsigned int i = 0; i < pErrorBlob->GetBufferSize(); i++)
				{
					ss << pErrors[i];
				}

				OutputDebugStringW(ss.str().c_str());
				pErrorBlob->Release();
				pErrorBlob = nullptr;

				std::wcout << ss.str() << std::endl;
				return nullptr;
			}
		}

		return pEffect;
	}
};
