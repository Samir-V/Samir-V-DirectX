#include "Mesh.h"

#include <iostream>

#include "MainMeshEffect.h"
#include "Matrix.h"
#include "Camera.h"
#include "Texture.h"

namespace dae
{
	Mesh::Mesh(ID3D11Device* pDevice, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, std::unique_ptr<EffectBase> effect, const std::string& diffuseMapPath, const std::string& normalMapPath, const std::string& specularMapPath, const std::string& glossinessMapPath)
	{
		m_pEffect = std::move(effect);

		m_pDiffuseMap = Texture::LoadFromFile(diffuseMapPath, pDevice);
		m_pEffect->SetDiffuseMap(m_pDiffuseMap.get());

		if (!normalMapPath.empty())
		{
			m_pNormalMap = Texture::LoadFromFile(normalMapPath, pDevice);
			m_pEffect->SetNormalMap(m_pNormalMap.get());
		}

		if (!specularMapPath.empty())
		{
			m_pSpecularMap = Texture::LoadFromFile(specularMapPath, pDevice);
			m_pEffect->SetSpecularMap(m_pSpecularMap.get());
		}

		if (!glossinessMapPath.empty())
		{
			m_pGlossinessMap = Texture::LoadFromFile(glossinessMapPath, pDevice);
			m_pEffect->SetGlossinessMap(m_pGlossinessMap.get());
		}

		// Vertex Layout
		static constexpr uint32_t numElements{ 4 };
		D3D11_INPUT_ELEMENT_DESC vertexDesc[numElements]{};

		vertexDesc[0].SemanticName = "Position";
		vertexDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		vertexDesc[0].AlignedByteOffset = 0;
		vertexDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

		vertexDesc[1].SemanticName = "TEXCOORD";
		vertexDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		vertexDesc[1].AlignedByteOffset = 12;
		vertexDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

		vertexDesc[2].SemanticName = "Normal";
		vertexDesc[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		vertexDesc[2].AlignedByteOffset = 20;
		vertexDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

		vertexDesc[3].SemanticName = "Tangent";
		vertexDesc[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		vertexDesc[3].AlignedByteOffset = 32;
		vertexDesc[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

		// Input Layout
		D3DX11_PASS_DESC passDesc{};

		m_pEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);

		const HRESULT result1 = pDevice->CreateInputLayout(
			vertexDesc,
			numElements,
			passDesc.pIAInputSignature,
			passDesc.IAInputSignatureSize,
			&m_pInputLayout);

		if (FAILED(result1))
		{
			std::wcout << L"Input Layout failure \n";
			return;
		}

		// Vertex buffer

		D3D11_BUFFER_DESC bd = {};
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = sizeof(Vertex) * static_cast<uint32_t>(vertices.size());
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = vertices.data();

		const HRESULT result2 = pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);

		if (FAILED(result2))
		{
			std::wcout << L"Vertex buffer failure \n";
			return;
		}

		// Index buffer

		m_NumIndices = static_cast<uint32_t>(indices.size());
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = sizeof(uint32_t) * m_NumIndices;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		initData.pSysMem = indices.data();

		const HRESULT result3 = pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);

		if (FAILED(result3))
		{
			std::wcout << L"Index buffer failure \n";
			return;
		}
	}

	Mesh::~Mesh()
	{
		m_pVertexBuffer->Release();
		m_pIndexBuffer->Release();
		m_pInputLayout->Release();
	}

	void Mesh::Render(ID3D11DeviceContext* pDeviceContext, Camera* camera)
	{
		// 1. Topology

		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 2. Set Input Layout

		pDeviceContext->IASetInputLayout(m_pInputLayout);

		// 3. Set Vertex Buffer

		constexpr UINT stride = sizeof(Vertex);
		constexpr UINT offset = 0;

		pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

		// 4. Set the matrices and camera pos

		const Matrix view = camera->GetViewMatrix();
		const Matrix proj = camera->GetProjectionMatrix();
		const Matrix worldViewProjectionMatrix = WorldMatrix * view * proj;

		m_pEffect->UpdateEffect(worldViewProjectionMatrix, WorldMatrix, camera->origin);

		// 5. Set Index Buffer

		pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// 6. Generate Mips

		pDeviceContext->GenerateMips(m_pDiffuseMap->GetSRV());

		// 7. Draw

		D3DX11_TECHNIQUE_DESC techDesc{};
		m_pEffect->GetTechnique()->GetDesc(&techDesc);

		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			m_pEffect->GetTechnique()->GetPassByIndex(p)->Apply(0, pDeviceContext);
			pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
		}
	}

	void Mesh::CycleTechniques() const
	{
		m_pEffect->CycleTechniques();
	}
}
