#include "FireMeshEffect.h"

FireMeshEffect::FireMeshEffect(ID3D11Device* pDevice, const std::wstring& assetFile)
{
	m_pEffect = LoadEffect(pDevice, assetFile);

	m_pActiveTechnique = m_pEffect->GetTechniqueByName("DefaultTechnique");

	if (!m_pActiveTechnique->IsValid())
	{
		std::wcout << L"Technique not valid\n";
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
	m_pActiveTechnique->Release();
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
	return;
}





