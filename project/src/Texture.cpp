#include "Texture.h"

#include <d3d11.h>
#include <iostream>

#include "Vector2.h"
#include <SDL_image.h>

namespace dae
{
	Texture::Texture(SDL_Surface* pSurface, ID3D11Device* pDevice) :
		m_pSurface{ pSurface },
		m_pSurfacePixels{ (uint32_t*)pSurface->pixels }
	{
		DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
		D3D11_TEXTURE2D_DESC desc{};
		desc.Width = pSurface->w;
		desc.Height = pSurface->h;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = format;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = pSurface->pixels;
		initData.SysMemPitch = static_cast<UINT>(pSurface->pitch);
		initData.SysMemSlicePitch = static_cast<UINT>(pSurface->h * pSurface->pitch);

		HRESULT hr = pDevice->CreateTexture2D(&desc, &initData, &m_pResource);

		if (FAILED(hr))
		{
			std::wcout << L"Resource creation for texture failed" << std::endl;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC SRVdesc{};
		SRVdesc.Format = format;
		SRVdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVdesc.Texture2D.MipLevels = 1;

		hr = pDevice->CreateShaderResourceView(m_pResource, &SRVdesc, &m_pSRV);

		if (FAILED(hr))
		{
			std::wcout << L"ShaderResourceView creation failed" << std::endl;
		}
	}

	Texture::~Texture()
	{
		if (m_pSurface)
		{
			SDL_FreeSurface(m_pSurface);
			m_pSurface = nullptr;
		}

		m_pSRV->Release();
		m_pResource->Release();
	}

	std::unique_ptr<Texture> Texture::LoadFromFile(const std::string& path, ID3D11Device* pDevice)
	{
		SDL_Surface* pSurface = IMG_Load(path.c_str());

		if (pSurface == nullptr)
		{
			return nullptr;
		}

		return std::unique_ptr<Texture>(new Texture(pSurface, pDevice));
	}

	ID3D11ShaderResourceView* Texture::GetSRV() const
	{
		return m_pSRV;
	}
}