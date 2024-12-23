#pragma once
#include <d3d11.h>
#include <memory>
#include <vector>

#include "d3dx11effect.h"
#include "DataTypes.h"
#include "MainMeshEffect.h"
#include "Matrix.h"
#include "EffectBase.h"

namespace dae
{
	struct Camera;

	class Mesh final
	{
	public:

		Mesh(ID3D11Device* pDevice, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, std::unique_ptr<EffectBase> effect, const std::string& diffuseMapPath, const std::string& normalMapPath = {}, const std::string& specularMapPath = {}, const std::string& glossinessMapPath = {});
		~Mesh();

		void Render(ID3D11DeviceContext* pDeviceContext, Camera* camera);

		void CycleTechniques() const;

		Matrix WorldMatrix{ Matrix::CreateTranslation(0, 0, 50.0f)};

	private:

		std::vector<Vertex_Out> m_Vertices_Out{};

		ID3D11InputLayout* m_pInputLayout{};

		ID3D11Buffer* m_pIndexBuffer{};
		ID3D11Buffer* m_pVertexBuffer{};

		std::unique_ptr<EffectBase> m_pEffect{};
		std::unique_ptr<Texture> m_pDiffuseMap;
		std::unique_ptr<Texture> m_pNormalMap;
		std::unique_ptr<Texture> m_pSpecularMap;
		std::unique_ptr<Texture> m_pGlossinessMap;

		uint32_t m_NumIndices{};
	};
}
