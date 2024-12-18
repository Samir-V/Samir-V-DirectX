#pragma once
#include <string>

#include "d3dx11effect.h"

namespace dae
{
	struct Vector3;
}

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
	void SetNormalMap(const dae::Texture* pNormalMapTexture) const;
	void SetSpecularMap(const dae::Texture* pSpecularMapTexture) const;
	void SetGlossinessMap(const dae::Texture* pGlossMapTexture) const;
	void SetCameraPos(const dae::Vector3& cameraPos) const;
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

	ID3DX11EffectVariable* m_pCameraPositionVariable;

	ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable;
	ID3DX11EffectMatrixVariable* m_pMatWorldVariable;

	ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable;
	ID3DX11EffectShaderResourceVariable* m_pNormalMapVariable;
	ID3DX11EffectShaderResourceVariable* m_pSpecularMapVariable;
	ID3DX11EffectShaderResourceVariable* m_pGlossinessMapVariable;

	ID3DX11EffectTechnique* m_PointTechnique;
	ID3DX11EffectTechnique* m_LinearTechnique;
	ID3DX11EffectTechnique* m_AnisotropicTechnique;

	TechniqueMode m_TechniqueMode;
};
