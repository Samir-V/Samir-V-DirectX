#include "pch.h"
#include "Renderer.h"
#include "Utils.h"

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

		m_Camera = std::make_unique<Camera>();

		m_Camera->Initialize(45.0f, { 0, 0, -50.0f });

		m_Camera->aspectRatio = static_cast<float>(m_Width) / static_cast<float>(m_Height);

		/*std::vector<Vertex> vertices
		{
			Vertex{{-3,3,-2},{colors::White},{0,0}},
				Vertex{{0,3,-2},{colors::White},{0.5f,0}},
				Vertex{{3,3,-2},{colors::White},{1,0}},
				Vertex{{-3,0,-2},{colors::White},{0,.5f}},
				Vertex{{0,0,-2},{colors::White},{.5f,.5f}},
				Vertex{{3,0,-2},{colors::White},{1,.5f}},
				Vertex{{-3,-3,-2},{colors::White},{0,1}},
				Vertex{{0,-3,-2},{colors::White},{.5f,1}},
				Vertex{{3,-3,-2},{colors::White},{1,1}}
		};*/

		/*std::vector<uint32_t> indices
		{	3,0,1, 1,4,3, 4,1,2,
			2,5,4, 6,3,4, 4,7,6,
			7,4,5, 5,8,7
		};*/

		std::vector<Vertex> vertices
		{
		};

		std::vector<uint32_t> indices
		{ 
		};

		Utils::ParseOBJ("resources/vehicle.obj", vertices, indices);

		m_Mesh = std::make_unique<Mesh>(m_pDevice, vertices, indices, 
			"resources/vehicle_diffuse.png",
			"resources/vehicle_normal.png",
			"resources/vehicle_specular.png",
			"resources/vehicle_gloss.png");
	}

	Renderer::~Renderer()
	{
		m_pRenderTargetView->Release();
		m_pRenderTargetBuffer->Release();
		m_pDepthStencilView->Release();
		m_pDepthStencilBuffer->Release();
		m_pSwapChain->Release();

		if (m_pDeviceContext)
		{
			m_pDeviceContext->ClearState();
			m_pDeviceContext->Flush();
			m_pDeviceContext->Release();
		}

		m_pDevice->Release();
	}

	void Renderer::Update(const Timer* pTimer)
	{
		m_Camera->Update(pTimer);

		yaw = PI_DIV_2 * pTimer->GetElapsed();
		const Matrix rotationMatrix = Matrix::CreateRotationY(yaw);
		m_Mesh->WorldMatrix *= rotationMatrix;
	}


	void Renderer::Render() const
	{
		if (!m_IsInitialized)
			return;

		// Clear RTV and DSV

		constexpr float color[4] = { 0.0f, 0.0f, 0.3f, 1.0f };
		m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
		m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		// Actually render

		m_Mesh->Render(m_pDeviceContext, m_Camera.get());

		// Present backbuffer
		m_pSwapChain->Present(0, 0);
	}

	HRESULT Renderer::InitializeDirectX()
	{
		//1. Creating the Device and DeviceContext

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

		//1.2 Creating the factory 

		IDXGIFactory1* pDxgiFactory{};
		result = CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&pDxgiFactory));	
		if (FAILED(result))
		{
			return result;
		}

		//2. Creating the swapchain

		// Filling the description
		DXGI_SWAP_CHAIN_DESC swapChainDesc{};
		swapChainDesc.BufferDesc.Width = m_Width;
		swapChainDesc.BufferDesc.Height = m_Height;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 1;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 60;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.Windowed = true;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = 0;

		// Getting the handle
		SDL_SysWMinfo sysWMinfo{};
		SDL_GetVersion(&sysWMinfo.version);
		SDL_GetWindowWMInfo(m_pWindow, &sysWMinfo);
		swapChainDesc.OutputWindow = sysWMinfo.info.win.window;

		// Creating the chain
		result = pDxgiFactory->CreateSwapChain(m_pDevice, &swapChainDesc, &m_pSwapChain);
		if (FAILED(result))
		{
			return result;
		}

		//3. Creating DepthStencil (DS) & DepthStencilView (DSV) (We use it for z-test)
		//A resource

		D3D11_TEXTURE2D_DESC depthStencilDesc{};
		depthStencilDesc.Width = m_Width;
		depthStencilDesc.Height = m_Height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
		depthStencilViewDesc.Format = depthStencilDesc.Format;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		result = m_pDevice->CreateTexture2D(&depthStencilDesc, nullptr, &m_pDepthStencilBuffer);
		if (FAILED(result))
		{
			return result;
		}

		result = m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, nullptr, &m_pDepthStencilView);
		if (FAILED(result))
		{
			return result;
		}


		//4. Creating the render target and render target view
		result = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_pRenderTargetBuffer));
		if (FAILED(result))
		{
			return result;
		}

		result = m_pDevice->CreateRenderTargetView(m_pRenderTargetBuffer, nullptr, &m_pRenderTargetView);
		if (FAILED(result))
		{
			return result;
		}

		//5. Bind RTV and DSV to Output Merger Stage

		m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

		//6. Set Viewport
		D3D11_VIEWPORT viewport{};

		viewport.Width = static_cast<float>(m_Width);
		viewport.Height = static_cast<float>(m_Height);
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		m_pDeviceContext->RSSetViewports(1, &viewport);

		pDxgiFactory->Release();

		return S_OK;
	}

	void Renderer::CycleTechniques() const
	{
		m_Mesh->CycleTechniques();
	}

}
