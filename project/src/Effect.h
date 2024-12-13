#pragma once
#include <string>

#include "d3dx11effect.h"

namespace dae
{
	class Texture;
}

class Effect final
{
public:

	Effect(ID3D11Device* pDevice, const std::wstring& assetFile);
	~Effect();

	static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);

	ID3DX11Effect* GetEffect() const;
	ID3DX11EffectTechnique* GetTechnique() const;
	ID3DX11EffectMatrixVariable* GetMatrix() const;

	void SetDiffuseMap(const dae::Texture* pDiffuseTexture) const;
	void CycleTechniques();

	enum class TechniqueMode
	{
		Point,
		Linear,
		Anisotropic
	};

private:

	ID3DX11Effect* m_pEffect;
	ID3DX11EffectTechnique* m_pActiveTechnique;
	ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable;
	ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable;

	ID3DX11EffectTechnique* m_PointTechnique;
	ID3DX11EffectTechnique* m_LinearTechnique;
	ID3DX11EffectTechnique* m_AnisotropicTechnique;

	TechniqueMode m_TechniqueMode;
};
