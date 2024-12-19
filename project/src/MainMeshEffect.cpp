#include "MainMeshEffect.h"

#include <d3dcompiler.h>
#include <iostream>
#include <sstream>

#include "Texture.h"

MainMeshEffect::MainMeshEffect(ID3D11Device* pDevice, const std::wstring& assetFile)
{
	m_pEffect = LoadEffect(pDevice, assetFile);

	m_TechniqueMode = TechniqueMode::Point;

	m_PointTechnique = m_pEffect->GetTechniqueByName("PointTechnique");
	m_LinearTechnique = m_pEffect->GetTechniqueByName("LinearTechnique");
	m_AnisotropicTechnique = m_pEffect->GetTechniqueByName("AnisotropicTechnique");

	m_pActiveTechnique = m_pEffect->GetTechniqueByName("PointTechnique");

	if (!m_pActiveTechnique->IsValid())
	{
		std::wcout << L"Technique not valid\n";
	}

	if (!m_PointTechnique->IsValid())
	{
		std::wcout << L"Point Technique not valid\n";
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

	m_pMatWorldVariable = m_pEffect->GetVariableByName("gWorldMatrix")->AsMatrix();

	if (!m_pMatWorldVariable->IsValid())
	{
		std::wcout << L"World Matrix not valid\n";
	}

	m_pCameraPositionVariable = m_pEffect->GetVariableByName("gCameraPosition")->AsVector();

	if (!m_pCameraPositionVariable->IsValid())
	{
		std::wcout << L"Camera Pos not valid\n";
	}

	m_pDiffuseMapVariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();

	if (!m_pDiffuseMapVariable->IsValid())
	{
		std::wcout << L"DiffuseMap variable not valid\n";
	}

	m_pNormalMapVariable = m_pEffect->GetVariableByName("gNormalMap")->AsShaderResource();

	if (!m_pNormalMapVariable->IsValid())
	{
		std::wcout << L"NormalMap variable not valid\n";
	}

	m_pSpecularMapVariable = m_pEffect->GetVariableByName("gSpecularMap")->AsShaderResource();

	if (!m_pSpecularMapVariable->IsValid())
	{
		std::wcout << L"SpecularMap variable not valid\n";
	}

	m_pGlossinessMapVariable = m_pEffect->GetVariableByName("gGlossinessMap")->AsShaderResource();

	if (!m_pGlossinessMapVariable->IsValid())
	{
		std::wcout << L"GlossinessMap variable not valid\n";
	}
}

MainMeshEffect::~MainMeshEffect()
{
	m_pGlossinessMapVariable->Release();
	m_pSpecularMapVariable->Release();
	m_pNormalMapVariable->Release();
	m_pDiffuseMapVariable->Release();
	m_pCameraPositionVariable->Release();
	m_pMatWorldVariable->Release();
	m_pMatWorldViewProjVariable->Release();
	//m_pActiveTechnique->Release();
	m_AnisotropicTechnique->Release();
	m_LinearTechnique->Release();
	m_PointTechnique->Release();
	m_pEffect->Release();
}

void MainMeshEffect::SetDiffuseMap(const dae::Texture* pDiffuseTexture) const
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

void MainMeshEffect::SetNormalMap(const dae::Texture* pNormalMapTexture) const
{
	if (m_pNormalMapVariable)
	{
		const HRESULT result = m_pNormalMapVariable->SetResource(pNormalMapTexture->GetSRV());

		if (FAILED(result))
		{
			std::wcout << L"Setting the normal resource failed\n";
		}
	}
}

void MainMeshEffect::SetSpecularMap(const dae::Texture* pSpecularMapTexture) const
{
	if (m_pSpecularMapVariable)
	{
		const HRESULT result = m_pSpecularMapVariable->SetResource(pSpecularMapTexture->GetSRV());

		if (FAILED(result))
		{
			std::wcout << L"Setting the specular resource failed\n";
		}
	}
}

void MainMeshEffect::SetGlossinessMap(const dae::Texture* pGlossMapTexture) const
{
	if (m_pGlossinessMapVariable)
	{
		const HRESULT result = m_pGlossinessMapVariable->SetResource(pGlossMapTexture->GetSRV());

		if (FAILED(result))
		{
			std::wcout << L"Setting the glossiness resource failed\n";
		}
	}
}


ID3DX11Effect* MainMeshEffect::GetEffect() const
{
	return m_pEffect;
}

ID3DX11EffectTechnique* MainMeshEffect::GetTechnique() const
{
	return m_pActiveTechnique;
}

ID3DX11EffectMatrixVariable* MainMeshEffect::GetWVPMatrix() const
{
	return m_pMatWorldViewProjVariable;
}

ID3DX11EffectMatrixVariable* MainMeshEffect::GetWorldMatrix() const
{
	return m_pMatWorldVariable;
}

ID3DX11EffectVariable* MainMeshEffect::GetCameraPos() const
{
	return m_pCameraPositionVariable;
}

void MainMeshEffect::CycleTechniques()
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

void MainMeshEffect::UpdateEffect(const dae::Matrix& WVPmatrix, const dae::Matrix& worldMatrix, const dae::Vector3& cameraPos)
{
	m_pMatWorldViewProjVariable->SetMatrix(reinterpret_cast<const float*>(&WVPmatrix));
	m_pMatWorldVariable->SetMatrix(reinterpret_cast<const float*>(&worldMatrix));
	m_pCameraPositionVariable->AsVector()->SetFloatVector(reinterpret_cast<const float*>(&cameraPos));
}



	




