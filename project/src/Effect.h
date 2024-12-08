#pragma once
#include <string>

#include "d3dx11effect.h"

class Effect final
{
public:

	Effect(ID3D11Device* pDevice, const std::wstring& assetFile);
	~Effect();

	static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);

	ID3DX11Effect* GetEffect() const;
	ID3DX11EffectTechnique* GetTechnique() const;

private:

	ID3DX11Effect* m_pEffect;
	ID3DX11EffectTechnique* m_pTechnique;
};