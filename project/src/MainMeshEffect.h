#pragma once
#include <string>

#include "d3dx11effect.h"
#include "EffectBase.h"

namespace dae
{
	struct Vector3;
	class Texture;
}

class MainMeshEffect final : public EffectBase
{
public:

	MainMeshEffect(ID3D11Device* pDevice, const std::wstring& assetFile);
	~MainMeshEffect() override;

	ID3DX11Effect* GetEffect() const;
	ID3DX11EffectTechnique* GetTechnique() const override;
	ID3DX11EffectMatrixVariable* GetWVPMatrix() const;
	ID3DX11EffectMatrixVariable* GetWorldMatrix() const;

	ID3DX11EffectVariable* GetCameraPos() const;

	void SetDiffuseMap(const dae::Texture* pDiffuseTexture) const override;
	void SetNormalMap(const dae::Texture* pNormalMapTexture) const override;
	void SetSpecularMap(const dae::Texture* pSpecularMapTexture) const override;
	void SetGlossinessMap(const dae::Texture* pGlossMapTexture) const override;
	void CycleTechniques() override;

	void UpdateEffect(const dae::Matrix& WVPmatrix, const dae::Matrix& worldMatrix, const dae::Vector3& cameraPos) override;

private:

	ID3DX11Effect* m_pEffect;
	ID3DX11EffectTechnique* m_pActiveTechnique;

	ID3DX11EffectTechnique* m_PointTechnique;
	ID3DX11EffectTechnique* m_LinearTechnique;
	ID3DX11EffectTechnique* m_AnisotropicTechnique;

	ID3DX11EffectVariable* m_pCameraPositionVariable;

	ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable;
	ID3DX11EffectMatrixVariable* m_pMatWorldVariable;

	ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable;
	ID3DX11EffectShaderResourceVariable* m_pNormalMapVariable;
	ID3DX11EffectShaderResourceVariable* m_pSpecularMapVariable;
	ID3DX11EffectShaderResourceVariable* m_pGlossinessMapVariable;

	TechniqueMode m_TechniqueMode;
};
