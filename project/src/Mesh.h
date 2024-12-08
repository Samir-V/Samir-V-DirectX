#pragma once
#include <d3d11.h>
#include <memory>
#include <vector>

#include "d3dx11effect.h"
#include "DataTypes.h"
#include "Effect.h"

namespace dae
{
	class Mesh final
	{
	public:

		Mesh(ID3D11Device* pDevice, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
		~Mesh();

		void Render(ID3D11DeviceContext* pDeviceContext);

	private:

		std::vector<Vertex> m_Vertices{};
		std::vector<uint32_t> m_Indices{};

		std::vector<Vertex_Out> m_Vertices_Out{};

		ID3D11InputLayout* m_pInputLayout{};

		ID3D11Buffer* m_pIndexBuffer{};
		ID3D11Buffer* m_pVertexBuffer{};
		std::unique_ptr<Effect> m_pEffect{};

		uint32_t m_NumIndices{};
	};
}
