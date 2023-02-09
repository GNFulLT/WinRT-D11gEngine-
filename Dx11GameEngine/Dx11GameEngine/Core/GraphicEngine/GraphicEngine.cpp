#include "pch.h"

#include "Common/Utils/utils_dxd.h"
#include "GraphicEngine.h"
#include "../Game.h"
#include <DirectXTK/DirectXHelpers.h>

#define DEBUG_GRAPHIC_CARD
//#define SINGLE_THREADED_GPU
#ifdef _DEBUG
#define GIVE_DEBUG_NAME(dxobj,name) GThrowIfFailed( \
dxobj->SetPrivateData(WKPDID_D3DDebugObjectName,sizeof(name)-1,name)	\
); \

#else if
#define GIVE_DEBUG_NAME()
#endif

namespace GNF::Core::GraphicEngine
{
	GraphicEngine* GraphicEngine::GetInstance()
	{
		return &Core::Game::GetInstance()->engine;
	}
	GraphicEngine::GraphicEngine() noexcept
	{
		int a = 5;
	}
	GraphicEngine::~GraphicEngine()
	{
		
	}

	void GraphicEngine::Resize(UINT width, UINT height, bool isFullScreen, UINT dpi)
	{
		m_dpi = dpi;
		m_width = LogicalSizeToOutputSize(width);
		m_height = LogicalSizeToOutputSize(height);

		FlushAll();

		HRESULT hr = m_dxgi_swapChain->ResizeBuffers(m_backBufferCount, m_width, m_height, ms_backBufferFormat, 0);

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			//X Write HandleDeviceLost Func
			GThrowIfFailed(hr);
		}
		else
		{
			GThrowIfFailed(hr);
		}

		Init_RTV();
		Init_ViewPort();
		SetViewPort();
		//InitDSV();
	}

	void GraphicEngine::Destroy()
	{
		GNF::Common::Logger::LogDebug("Graphic Engine is destroying Calling FlushAll");
		FlushAll();
#if defined(DEBUG) || defined(_DEBUG)
		GNF::Common::Logger::LogTrace("ID3D11Debug is calling to call ReportLiveDeviceObjects");
		Microsoft::WRL::ComPtr<ID3D11Debug> debug;
		auto hr = m_d3d_device->QueryInterface(IID_PPV_ARGS(&debug));
		hr = debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif
	}

	/*void GraphicEngine::SetFrameScreen(int width,int height)
	{
		m_frameScreenWidth = width;
		m_frameScreenHeight = height;
	}*/

	void GraphicEngine::SetWindow(HWND hwnd,UINT width,UINT height, bool isFullScreen,UINT dpi)
	{
		m_width = width;
		m_height = height;
		m_dpi = dpi;
		DXGI_SWAP_CHAIN_DESC1 desc{};
		ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC1));
		desc.Height = height;
		desc.Width = width;
		desc.Format = ms_backBufferFormat;
		desc.Stereo = isFullScreen;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = m_backBufferCount;
		desc.Scaling = DXGI_SCALING_STRETCH;
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		desc.Flags = 0u;

		DXGI_SAMPLE_DESC sampleDesc;
		if (!m_msaaEnable)
		{
			sampleDesc.Count = 1;
			sampleDesc.Quality = 0;
		}
		else
		{
			sampleDesc.Count = 1;
			sampleDesc.Quality = 0;
		}

		desc.SampleDesc = sampleDesc;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC* descPtr = nullptr;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc;
		ZeroMemory(&fullScreenDesc, sizeof(DXGI_SWAP_CHAIN_FULLSCREEN_DESC));
		if (isFullScreen)
		{
			descPtr = &fullScreenDesc;
		}

		GNF::Common::Logger::LogInfo("Creating swap chain for hwnd");
		Microsoft::WRL::ComPtr<IDXGISwapChain1> swap;
		GThrowIfFailed(m_dxgi_factory->CreateSwapChainForHwnd(m_d3d_device.Get(),hwnd,&desc,
			descPtr,nullptr,swap.GetAddressOf()));
		GThrowIfFailed(swap.As(&m_dxgi_swapChain));
		GNF::Common::Logger::LogInfo("SwapChain Created");


		Init_RTV();
		Init_ViewPort();
		SetViewPort();
		//InitDSV();
	}

	void GraphicEngine::SetDPI(UINT dpi)
	{
		m_dpi = dpi;
	}

	/*void GraphicEngine::FrameBuffer_SetViewPort()
	{
		m_d3d_deviceContext->RSSetViewports(1, &m_d3d_frameScreenViewPort);

	}*/

	void GraphicEngine::SetViewPort()
	{
		m_d3d_deviceContext->RSSetViewports(1, &m_d3d_viewPort);
	}

	void GraphicEngine::SetRenderTarget()
	{
		ID3D11RenderTargetView* targetViews[] = {m_d3d_renderTargetView.Get()};
		m_d3d_deviceContext->OMSetRenderTargets(ARRAYSIZE(targetViews), targetViews, nullptr);
	}
	/*void GraphicEngine::FrameBuffer_SetRenderTarget()
	{
		ID3D11RenderTargetView* targetViews[] = { m_d3d_frameBufferTargetView.Get()};
		m_d3d_deviceContext->OMSetRenderTargets(ARRAYSIZE(targetViews), targetViews, m_d3d_depthStencilView.Get());
	}*/
	void
		GraphicEngine::DrawTriangleStrip(int count)
	{
		m_d3d_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_d3d_deviceContext->Draw(count, 0);
	}
	void GraphicEngine::SwapBuffers()
	{
	
		m_dxgi_swapChain->Present(1,0);
	}

	/*void GraphicEngine::FrameBuffer_ClearColor()
	{
		float color[] = { 0,1,0,1 };
		m_d3d_deviceContext->ClearRenderTargetView(m_d3d_frameBufferTargetView.Get(), color);
		m_d3d_deviceContext->ClearDepthStencilView(m_d3d_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	}*/

	
	void GraphicEngine::ClearColor(float r, float g, float b, float a)
	{
		float color[] = { r,g,b,a };
		m_d3d_deviceContext->ClearRenderTargetView(m_d3d_renderTargetView.Get(),color);
		//m_d3d_deviceContext->ClearDepthStencilView(m_d3d_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	Bindable::Shader::PixelShaderBindable* GraphicEngine::CreatePixelShader(const wchar_t* path)
	{
		auto pixelShaderByteCode = Common::Utils::ReadData(path);
		return new Bindable::Shader::PixelShaderBindable(pixelShaderByteCode.data(),pixelShaderByteCode.size());
	}

	void GraphicEngine::FlushAll()
	{
		ID3D11RenderTargetView* nullViews[] = { nullptr };
		m_d3d_deviceContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
		m_d2d_deviceContext->SetTarget(nullptr);
		m_d3d_renderTarget.ReleaseAndGetAddressOf();
		//m_d2d_targetBitmap.ReleaseAndGetAddressOf();
		//m_d3d_depthStencilTarget.ReleaseAndGetAddressOf();
		m_d3d_renderTargetView.ReleaseAndGetAddressOf();
		//m_d3d_depthStencilView.ReleaseAndGetAddressOf();
		m_d3d_deviceContext->Flush();



	}

	void GraphicEngine::Init()
	{
		InitIndependentResources();
	}

	void GraphicEngine::InitIndependentResources()
	{
		GNF::Common::Logger::LogInfo("InitIndependentResources Started");
		Init_IDR_DXGI_THINGS();
		Init_IDR_D3D_THINGS();
		Init_IDR_D2D_THINGS();
		GNF::Common::Logger::LogInfo("InitIndependentResources Finished");
	}

	void GraphicEngine::Init_IDR_DXGI_THINGS()
	{
		GNF::Common::Logger::LogInfo("Creating DXGI Things");

		//!: Firstly We created our factory
		auto factoryCreationFlag = 0;
#ifdef _DEBUG
		factoryCreationFlag = DXGI_CREATE_FACTORY_DEBUG;
		GNF::Common::Logger::LogDebug("DXGI_FACTORY CREATE WITH DEBUG FLAG");
#endif 
		Microsoft::WRL::ComPtr< IDXGIFactory2 > factor;
		GThrowIfFailed(CreateDXGIFactory2(0, IID_PPV_ARGS(factor.GetAddressOf())));
		factor.As(&m_dxgi_factory);
		
		GIVE_DEBUG_NAME(m_dxgi_factory, "DXGI FACTORY");
		
		//!: Select Physical Graphic Card
#ifdef DEBUG_GRAPHIC_CARD		
		GNF::Common::Logger::LogWarn("Selected WARP Device instead of a Graphic Card");
		m_dxgi_factory->EnumWarpAdapter(IID_PPV_ARGS(m_dxgi_adapter.GetAddressOf()));

#else if
		GNF::Common::Logger::LogInfo("Selected Fastest Graphic Card");
		GThrowIfFailed(m_dxgi_factory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(m_dxgi_adapter.GetAddressOf())));
#endif
		GIVE_DEBUG_NAME(m_dxgi_adapter, "DXGI ADAPTER");

		DXGI_ADAPTER_DESC3 desc;
		m_dxgi_adapter->GetDesc3(&desc);
		
		
		auto size_needed = WideCharToMultiByte(CP_UTF8,0,&desc.Description[0], 128, NULL, 0, NULL, NULL);
		std::string strTo(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, &desc.Description[0], (int)128, &strTo[0], size_needed, NULL, NULL);

		GNF::Common::Logger::LogDebug(std::format("Selected Graphic Card : \n{}",strTo.c_str()).c_str());
	}

	void GraphicEngine::Init_IDR_D3D_THINGS()
	{
		GNF::Common::Logger::LogInfo("Creating D3D Things");

		auto deviceCreationFlag = 0;
		deviceCreationFlag |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG
		deviceCreationFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif
#ifdef DEBUG_GRAPHIC_CARD	
		deviceCreationFlag |= D3D11_CREATE_DEVICE_DEBUGGABLE;
#endif
#ifdef SINGLE_THREADED_GPU
		deviceCreationFlag |= D3D11_CREATE_DEVICE_SINGLETHREADED;
#endif

		D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0,D3D_FEATURE_LEVEL_11_1 };
		Microsoft::WRL::ComPtr<ID3D11Device> device;
		GNF::Common::Logger::LogInfo("Creating D3D Device");
#ifndef DEBUG_GRAPHIC_CARD
		//!: Virtualizing Physical Graphic Card
		if (HRESULT(D3D11CreateDevice(m_dxgi_adapter.Get(), D3D_DRIVER_TYPE_UNKNOWN,
			0, deviceCreationFlag, levels, ARRAYSIZE(levels),
			D3D11_SDK_VERSION, device.GetAddressOf(), &m_selectedFeatureLevel, nullptr)))
		{
			GNF::Common::Logger::LogWarn("Hardware D3D Device couldn't created trying to create with warp device");

			GThrowIfFailed(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_WARP,
				0, deviceCreationFlag, levels, ARRAYSIZE(levels),
				D3D11_SDK_VERSION, device.GetAddressOf(), &m_selectedFeatureLevel, nullptr));
			GNF::Common::Logger::LogInfo("Warp Device created");

			device->QueryInterface(IID_PPV_ARGS(m_dxgi_device.ReleaseAndGetAddressOf()));
			Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
			m_dxgi_device->GetAdapter(adapter.GetAddressOf());
			m_dxgi_adapter.Reset();
			adapter.As(&m_dxgi_adapter);
		}
		else
		{
			GNF::Common::Logger::LogInfo("Hardware Device Created");
		}
#else if
		GThrowIfFailed(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_WARP,
			0, deviceCreationFlag, levels, ARRAYSIZE(levels),
			D3D11_SDK_VERSION, device.GetAddressOf(), &m_selectedFeatureLevel, nullptr));
		GNF::Common::Logger::LogInfo("Warp Device created");

		device->QueryInterface(IID_PPV_ARGS(m_dxgi_device.ReleaseAndGetAddressOf()));
		Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
		m_dxgi_device->GetAdapter(adapter.GetAddressOf());
		m_dxgi_adapter.Reset();
		adapter.As(&m_dxgi_adapter);
#endif

		device.As(&m_d3d_device);
		GIVE_DEBUG_NAME(m_d3d_device, "D3D Device");

		m_d3d_device->GetImmediateContext3(m_d3d_deviceContext.GetAddressOf());

		GThrowIfFailed(m_d3d_device->QueryInterface(IID_PPV_ARGS(m_dxgi_device.GetAddressOf())));

		GIVE_DEBUG_NAME(m_d3d_deviceContext, "D3D Device Context");

		GIVE_DEBUG_NAME(m_dxgi_device, "DXGI Device");


	}

	void GraphicEngine::Init_IDR_D2D_THINGS()
	{
		GNF::Common::Logger::LogInfo("Creating D2D Things");
		D2D1_FACTORY_OPTIONS factoryOptFlag;
		ZeroMemory(&factoryOptFlag, sizeof(D2D1_FACTORY_OPTIONS));
#ifdef _DEBUG
		factoryOptFlag.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif
		GNF::Common::Logger::LogInfo("Creating D2D Factory");
		GThrowIfFailed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory7), &factoryOptFlag, (void**)m_d2d_factory.GetAddressOf()));
		
		GNF::Common::Logger::LogInfo("Creating D2D Device");
		GThrowIfFailed(m_d2d_factory->CreateDevice(m_dxgi_device.Get(), m_d2d_device.GetAddressOf()));

		GNF::Common::Logger::LogInfo("Creating D2D Device Context");
		GThrowIfFailed(m_d2d_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, m_d2d_deviceContext.GetAddressOf()));
		
	}
	/*
	void GraphicEngine::InitDSV()
	{
		
		GNF::Common::Logger::LogInfo("Creating Depth Stencil Texture");
		//!: Texture Desc of DepthStencil
		CD3D11_TEXTURE2D_DESC depthStencilDesc(
			ms_depthStencilFormat,
			m_frameScreenWidth,
			m_frameScreenHeight,
			1U, //! DSV only has one texture
			1U, //!: Single Mipmap
			D3D11_BIND_DEPTH_STENCIL
		);
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		if (m_msaaEnable)
		{
			GNF::Common::Logger::LogDebug("MSAA ENABLE SO GIVE SAMPLE TO DEPTH STENCIL TEXTURE");
			depthStencilDesc.SampleDesc.Count = m_msaaCount;
			depthStencilDesc.SampleDesc.Quality = m_msaaQuality;
		}

		GThrowIfFailed(m_d3d_device->CreateTexture2D(&depthStencilDesc,NULL,m_d3d_depthStencilTarget.GetAddressOf()));
		
		auto viewTextureDim = D3D11_DSV_DIMENSION_TEXTURE2D;

		if (m_msaaEnable)
		{
			viewTextureDim = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		}

		GNF::Common::Logger::LogInfo("Creating Depth Stencil View");

		CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(viewTextureDim);
		
		GThrowIfFailed(m_d3d_device->CreateDepthStencilView(
			m_d3d_depthStencilTarget.Get(),
			&depthStencilViewDesc,
			m_d3d_depthStencilView.GetAddressOf()
		));
		
		//Describe our Depth/Stencil Buffer
		/*
		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = m_width;
		depthStencilDesc.Height = m_height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;
		GThrowIfFailed(m_d3d_device->CreateTexture2D(&depthStencilDesc, NULL, m_d3d_depthStencilTarget.GetAddressOf()));
		GThrowIfFailed(m_d3d_device->CreateDepthStencilView(m_d3d_depthStencilTarget.Get(), NULL, m_d3d_depthStencilView.GetAddressOf()));
	*/
	/*}*/

	void GraphicEngine::Init_MSAA_Features()
	{

	}

	void GraphicEngine::Init_RTV()
	{
		GNF::Common::Logger::LogInfo("Creating RenderTargetView");
		if (!m_msaaEnable)
		{
			GNF::Common::Logger::LogDebug("MSAA is Disable Creating RTV");
			GThrowIfFailed(m_dxgi_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(m_d3d_renderTarget.GetAddressOf())));
			GThrowIfFailed(m_d3d_device->CreateRenderTargetView(m_d3d_renderTarget.Get(), nullptr, m_d3d_renderTargetView.GetAddressOf()));
			GNF::Common::Logger::LogDebug("MSAA is Disable Creating Frame Buffer Target View");
			/*
			//!: If MSAA active MipMapLevel should be 1 and sample count and quality 
			CD3D11_TEXTURE2D_DESC desc(ms_backBufferFormat, m_frameScreenWidth, m_frameScreenHeight, 1U, 0U, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT, 0U, 1, 0, 0U);
			
			GThrowIfFailed(m_d3d_device->CreateTexture2D(&desc,nullptr,m_d3d_frameBufferTarget.GetAddressOf()));
			
			//!: For MSAA Should be 2dms
			CD3D11_RENDER_TARGET_VIEW_DESC ftv(m_d3d_frameBufferTarget.Get(), D3D11_RTV_DIMENSION_TEXTURE2D);
			
			GThrowIfFailed(m_d3d_device->CreateRenderTargetView(m_d3d_frameBufferTarget.Get(),&ftv,m_d3d_frameBufferTargetView.GetAddressOf()));
			
			CD3D11_SHADER_RESOURCE_VIEW_DESC frv(m_d3d_frameBufferTarget.Get(), D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D);

			GThrowIfFailed(m_d3d_device->CreateShaderResourceView(m_d3d_frameBufferTarget.Get(),&frv, m_d3d_frameResource.GetAddressOf()));
			*/
		}
		else
		{
			GNF::Common::Logger::LogDebug("MSAA is Enable Creating RTV");
		}
	}
	
	/*void GraphicEngine::FrameBuffer_ChangeViewPort(float width, float height)
	{
		m_d3d_frameScreenViewPort = CD3D11_VIEWPORT(
			0.0f,
			0.0f,
			m_frameScreenWidth,
			m_frameScreenHeight
		);
	}*/

	void GraphicEngine::Init_ViewPort()
	{
		/*m_d3d_frameScreenViewPort = CD3D11_VIEWPORT(
			0.0f,
			0.0f,
			m_frameScreenWidth,
			m_frameScreenHeight
		);*/

		m_d3d_viewPort = CD3D11_VIEWPORT(
			0.0f,
			0.0f,
			static_cast<float>(LogicalSizeToOutputSize(m_width)),
			static_cast<float>(LogicalSizeToOutputSize(m_height))
		);
	}

	//!: Size = Dips
	inline float GraphicEngine::LogicalSizeToOutputSize(float size)
	{
		return int(size * m_dpi / 96.f + 0.5f);
	}
	
}
