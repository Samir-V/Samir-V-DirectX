#pragma once
#include <d3d11.h>
#include <memory>
#include <vector>

#include "d3dx11effect.h"
#include "DataTypes.h"
#include "Effect.h"
#include "Matrix.h"

namespace dae
{
	struct Camera;

	class Mesh final
	{
	public:

		Mesh(ID3D11Device* pDevice, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::string& texturePath);
		~Mesh();

		void Render(ID3D11DeviceContext* pDeviceContext, Camera* camera);

		void CycleTechniques() const;

		Matrix WorldMatrix{};

	private:

		std::vector<Vertex_Out> m_Vertices_Out{};

		ID3D11InputLayout* m_pInputLayout{};

		ID3D11Buffer* m_pIndexBuffer{};
		ID3D11Buffer* m_pVertexBuffer{};

		std::unique_ptr<Effect> m_pEffect{};
		std::unique_ptr<Texture> m_pTexture;

		uint32_t m_NumIndices{};
	};
}
