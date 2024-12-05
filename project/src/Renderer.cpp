#include "pch.h"
#include "Renderer.h"

#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dx11effect.h>

namespace dae {

	Renderer::Renderer(SDL_Window* pWindow) :
		m_pWindow(pWindow)
	{
		//Initialize
		SDL_GetWindowSize(pWindow, &m_Width, &m_Height);

		//Initialize DirectX pipeline
		const HRESULT result = InitializeDirectX();
		if (result == S_OK)
		{
			m_IsInitialized = true;
			std::cout << "DirectX is initialized and ready!\n";
		}
		else
		{
			std::cout << "DirectX initialization failed!\n";
		}
	}

	Renderer::~Renderer()
	{
		
	}

	void Renderer::Update(const Timer* pTimer)
	{

	}


	void Renderer::Render() const
	{
		if (!m_IsInitialized)
			return;

	}

	HRESULT Renderer::InitializeDirectX()
	{
		// Creating the Device and DeviceContext

		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_1;
		uint32_t createdDeviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
		createdDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		HRESULT result = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, createdDeviceFlags,
			&featureLevel, 1, D3D11_SDK_VERSION, &m_pDevice, nullptr, &m_pDeviceContext);

		if (FAILED(result))
		{
			return result;
		}

		return S_OK;
	}
}
