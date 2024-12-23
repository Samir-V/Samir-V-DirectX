#include "FireMeshEffect.h"

FireMeshEffect::FireMeshEffect(ID3D11Device* pDevice, const std::wstring& assetFile)
{
	m_pEffect = LoadEffect(pDevice, assetFile);

	m_PointTechnique = m_pEffect->GetTechniqueByName("PointTechnique");
	m_LinearTechnique = m_pEffect->GetTechniqueByName("LinearTechnique");
	m_AnisotropicTechnique = m_pEffect->GetTechniqueByName("AnisotropicTechnique");

	m_pActiveTechnique = m_PointTechnique;

	if (!m_pActiveTechnique->IsValid())
	{
		std::wcout << L"Active (Point) Technique not valid\n";
	}

	if (!m_LinearTechnique->IsValid())
	{
		std::wcout << L"Linear Technique not valid\n";
	}

	if (!m_AnisotropicTechnique->IsValid())
	{
		std::wcout << L"Anisotropic Technique not valid\n";
	}

	m_pMatWorldViewProjVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();

	if (!m_pMatWorldViewProjVariable->IsValid())
	{
		std::wcout << L"WVP Matrix not valid\n";
	}

	m_pDiffuseMapVariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();

	if (!m_pDiffuseMapVariable->IsValid())
	{
		std::wcout << L"DiffuseMap variable not valid\n";
	}
}

FireMeshEffect::~FireMeshEffect()
{
	m_pDiffuseMapVariable->Release();
	m_pMatWorldViewProjVariable->Release();
	m_AnisotropicTechnique->Release();
	m_LinearTechnique->Release();
	m_PointTechnique->Release();
	m_pEffect->Release();
}

void FireMeshEffect::SetDiffuseMap(const dae::Texture* pDiffuseTexture) const
{
	if (m_pDiffuseMapVariable)
	{
		const HRESULT result = m_pDiffuseMapVariable->SetResource(pDiffuseTexture->GetSRV());

		if (FAILED(result))
		{
			std::wcout << L"Setting the diffuse resource failed\n";
		}
	}
}

void FireMeshEffect::UpdateEffect(const dae::Matrix& WVPmatrix, const dae::Matrix& worldMatrix, const dae::Vector3& cameraPos)
{
	m_pMatWorldViewProjVariable->SetMatrix(reinterpret_cast<const float*>(&WVPmatrix));
}

ID3DX11EffectTechnique* FireMeshEffect::GetTechnique() const
{
	return m_pActiveTechnique;
}


void FireMeshEffect::SetGlossinessMap(const dae::Texture* pGlossMapTexture) const
{
	return;
}

void FireMeshEffect::SetNormalMap(const dae::Texture* pNormalMapTexture) const
{
	return;
}

void FireMeshEffect::SetSpecularMap(const dae::Texture* pSpecularMapTexture) const
{
	return;
}

void FireMeshEffect::CycleTechniques()
{
	if (m_TechniqueMode == TechniqueMode::Point)
	{
		m_pActiveTechnique = m_LinearTechnique;
		m_TechniqueMode = TechniqueMode::Linear;
	}
	else if (m_TechniqueMode == TechniqueMode::Linear)
	{
		m_pActiveTechnique = m_AnisotropicTechnique;
		m_TechniqueMode = TechniqueMode::Anisotropic;
	}
	else if (m_TechniqueMode == TechniqueMode::Anisotropic)
	{
		m_pActiveTechnique = m_PointTechnique;
		m_TechniqueMode = TechniqueMode::Point;
	}
}





