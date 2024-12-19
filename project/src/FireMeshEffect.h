#pragma once
#include "EffectBase.h"
#include "Texture.h"

namespace dae
{
	struct Vector3;
	class Texture;
}

class FireMeshEffect final : public EffectBase
{
public:

	FireMeshEffect(ID3D11Device* pDevice, const std::wstring& assetFile);
	~FireMeshEffect() override;

	void SetDiffuseMap(const dae::Texture* pDiffuseTexture) const override;
	void SetNormalMap(const dae::Texture* pNormalMapTexture) const override;
	void SetSpecularMap(const dae::Texture* pSpecularMapTexture) const override;
	void SetGlossinessMap(const dae::Texture* pGlossMapTexture) const override;
	void CycleTechniques() override;

	void UpdateEffect(const dae::Matrix& WVPmatrix, const dae::Matrix& worldMatrix = {}, const dae::Vector3& cameraPos = {}) override;
	ID3DX11EffectTechnique* GetTechnique() const override;

private:

	ID3DX11Effect* m_pEffect;
	ID3DX11EffectTechnique* m_pActiveTechnique;

	ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable;
	ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable;
};
