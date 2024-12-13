#pragma once
#include <d3d11.h>
#include <memory>
#include <SDL_surface.h>
#include <string>
#include "ColorRGB.h"
#include "SDL_system.h"

namespace dae
{
	struct Vector2;

	class Texture
	{
	public:
		~Texture();

		static std::unique_ptr<Texture> LoadFromFile(const std::string& path, ID3D11Device* pDevice);

		ID3D11ShaderResourceView* GetSRV() const;

	private:
		Texture(SDL_Surface* pSurface, ID3D11Device* pDevice);

		ID3D11Texture2D* m_pResource;
		ID3D11ShaderResourceView* m_pSRV;

		SDL_Surface* m_pSurface{ nullptr };
		uint32_t* m_pSurfacePixels{ nullptr };
	};
}
