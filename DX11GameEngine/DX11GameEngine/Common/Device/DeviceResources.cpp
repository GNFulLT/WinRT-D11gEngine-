#include "DeviceResources.h"
#include "Exception/DxException..h"
#include "Common/HRHelper.h"
#include "Common/ShaderManagement/ShaderManager.h"
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include "Graphic/CameraTestRenderer.h"
#include "Common/Event/EventHandler.h"
#include "Window/MainWindow.h"
#include "Common/TextureManagement/TextureManager.h"

#if defined(DEBUG) || defined(_DEBUG)
#include <initguid.h>
#include <dxgidebug.h>
#endif
namespace GNF::Common::Device
{
	DeviceResources::DeviceResources(Window::IWindow32* parentWindow) : pParentWindow(parentWindow), m_backBufferCount(2), m_backBufferFormat(DXGI_FORMAT_B8G8R8A8_UNORM), m_depthStencilFormat(DXGI_FORMAT_D24_UNORM_S8_UINT), m_renderer(new Graphic::CameraTestRenderer(this))
	{
		m_deviceSize.logicalWidth = parentWindow->GetHeight();
		m_deviceSize.logicalWidth = parentWindow->GetWidth();
		m_deviceSize.dpi = GetDpiForWindow(parentWindow->GetHWnd());
		m_deviceSize.SetOutputSizeWithDPI();

		m_d3d_screenViewport = {};

		m_shaderManager.reset(new ShaderManagement::ShaderManager(this));

		Geometry::GeometryGenerator::CreateBox(1.0f,1.0f,1.0f,meshData);

		(parentWindow->LeftKeyUp) += Event::EventHandler::Bind(&DeviceResources::OnLeftKeyUp, this);
	}
	

	void DeviceResources::OnLeftKeyUp()
	{
		((Graphic::CameraTestRenderer*)m_renderer.get())->TurnLeft();
	}

	void DeviceResources::HandleWindowSizeChanged()
	{
		CreateDependentResources();
	}
	ID3D11Device* DeviceResources::GetD3DDevice()
	{
		return m_d3d_device.Get();
	}

	ID3D11DeviceContext* DeviceResources::GetD3DContext()
	{
		return m_d3d_deviceContext.Get();
	}
	Texture::ITextureManager* DeviceResources::GetTextureManager()
	{
		return m_textureManager.get();
	}

	const char* DeviceResources::SelectFuture()
	{
		D3D11_CREATE_DEVICE_FLAG creationFlag = static_cast<D3D11_CREATE_DEVICE_FLAG>(D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_BGRA_SUPPORT);
		D2D1_FACTORY_OPTIONS options;
		ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));


#if _DEBUG
		creationFlag = static_cast<D3D11_CREATE_DEVICE_FLAG>(static_cast<UINT>(creationFlag) | static_cast<UINT>(D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG));
		creationFlag = static_cast<D3D11_CREATE_DEVICE_FLAG>(static_cast<UINT>(creationFlag) | static_cast<UINT>(D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUGGABLE));
		// If the project is in a debug build, enable Direct2D debugging via SDK Layers.
		options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

		//# Create D2D Factory

		GThrowIfFailed(D2D1CreateFactory(
			D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_SINGLE_THREADED
			, __uuidof(m_d2d_factory),
			&options
			, (void**)m_d2d_factory.GetAddressOf()));

		//# Create DirectWrite Factory

		GThrowIfFailed(DWriteCreateFactory(DWRITE_FACTORY_TYPE::DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_dwrite_factory), (IUnknown**)m_dwrite_factory.GetAddressOf()));

		//# Create IWICImagining factory for 

		GThrowIfFailed(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, __uuidof(m_wic_factory), (LPVOID*)m_wic_factory.GetAddressOf()));


		//# Set Feature Levels
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};

		D3D_FEATURE_LEVEL sLevel;
		ComPtr< ID3D11Device > d3d_device;

		HRESULT hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			0,
			creationFlag,
			featureLevels,
			ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION,
			d3d_device.GetAddressOf(),
			&sLevel,
			nullptr
		);
		//If failed try create with warp device
		if (FAILED(hr))
		{
			GThrowIfFailed(D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE_WARP,
				0,
				creationFlag,
				featureLevels,
				ARRAYSIZE(featureLevels),
				D3D11_SDK_VERSION,
				d3d_device.GetAddressOf(),
				&sLevel,
				nullptr
			));
		}

		//! Get DXGI Adapter and DXGI Device by D3D Device 
		GThrowIfFailed(d3d_device.As(&m_d3d_device));
		m_d3d_device->GetImmediateContext3(m_d3d_deviceContext.GetAddressOf());

		switch (sLevel)
		{
			case D3D_FEATURE_LEVEL_11_1:
				return "D3D_FEATURE_LEVEL_11_1";
			case D3D_FEATURE_LEVEL_11_0:
				return "D3D_FEATURE_LEVEL_11_0";
			case D3D_FEATURE_LEVEL_10_1:
				return "D3D_FEATURE_LEVEL_10_1";
			case D3D_FEATURE_LEVEL_10_0:
				return "D3D_FEATURE_LEVEL_10_0";
			case D3D_FEATURE_LEVEL_9_3:
				return "D3D_FEATURE_LEVEL_9_3";
			case D3D_FEATURE_LEVEL_9_2:
				return "D3D_FEATURE_LEVEL_9_2";
			case D3D_FEATURE_LEVEL_9_1:
				return "D3D_FEATURE_LEVEL_9_1";
			default:
				return "D3D_FEATURE_UNKNOWN_LEVEL";
		}
	}
	
	void DeviceResources::LoadShaderManager(Bindable::PixelShaderBindable* pixelShader,Bindable::VertexShaderBindable* vertexShader)
	{

		//! Load Shaders from shaderManager

		m_shaderManager->LoadAllShaderTypes();

		//! Bind Basic Shaders
		auto ps = m_shaderManager->GetShaderBindable(ShaderManagement::CameraTexturePixelShader);
		auto vs = m_shaderManager->GetShaderBindable(ShaderManagement::CameraTextureVertexShader);

		ps->Bind();
		vs->Bind();

		pixelShader = ps;
		vertexShader = vs;
	}

	bool DeviceResources::CheckMSAAFeatures(int* msaaMaxLevel,bool* msaaIsEnabled)
	{
		UINT maxQuality;
		auto hr = m_d3d_device->CheckMultisampleQualityLevels(m_backBufferFormat, m_sampleCount, &maxQuality);
		if (!FAILED(hr))
			m_multiSamplingSupported = true;
		m_maxQualityLevel = maxQuality;
		if (maxQuality == 0)
			m_multiSamplingSupported = false;

		if (!m_multiSamplingSupported)
			return false;
		if(msaaMaxLevel != nullptr)
			(*msaaMaxLevel) = m_maxQualityLevel;
		if(msaaIsEnabled != nullptr)
		(*msaaIsEnabled) = IsMultiSamplingEnabled();
	
		return true;
	}

	void DeviceResources::DevicePropsCheck(size_t* videoMem,size_t* sharedMem,wchar_t* deviceNameBuffer,int bufferSize)
	{
		DXGI_ADAPTER_DESC2 desc;
		m_dxgi_adapter->GetDesc2(&desc);


		if (videoMem != nullptr)
			(*videoMem) = desc.DedicatedVideoMemory;
		if (sharedMem != nullptr)
			(*sharedMem) = desc.SharedSystemMemory;
		if (deviceNameBuffer != nullptr)
		{
			for (int i = 0; i < bufferSize; i++)
			{
				if (i > 128)
					continue;
				*(deviceNameBuffer + i) = *(desc.Description+i);
			}
		}
	}

	void DeviceResources::CreateInDependentResources()
	{
		try
		{
			SelectFuture();

			GThrowIfFailed(m_d3d_device->QueryInterface(IID_PPV_ARGS(&m_dxgi_device)));

			ComPtr<IDXGIAdapter> adapter;

			GThrowIfFailed(m_dxgi_device->GetAdapter(adapter.GetAddressOf()));

			GThrowIfFailed(adapter.As(&m_dxgi_adapter));

			//! Create DXGI Adapter and later we will create swap chain with this factory
			GThrowIfFailed(m_dxgi_adapter->GetParent(IID_PPV_ARGS(&m_dxgi_factory)));
			
			//! Create D2D Device
			GThrowIfFailed(m_d2d_factory->CreateDevice(m_dxgi_device.Get(), m_d2d_device.GetAddressOf()));

			//! Create D2D Context
			
			GThrowIfFailed(m_d2d_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS::D2D1_DEVICE_CONTEXT_OPTIONS_NONE, m_d2d_deviceContext.GetAddressOf()));
			
			//X TODO:
			//! Create DWrite Device
			
			LoadShaderManager();


			m_renderer->InitResources();
			
			CheckMSAAFeatures();

			DevicePropsCheck();


			m_textureManager.reset(new Texture::TextureManager(this));

		}
		catch (Exception::DxException& e)
		{
			std::string msg = e.ToString();

			MessageBoxA(nullptr, msg.c_str(), "HRFailed", MB_OK);
			exit(-1);
		}
		catch (std::exception& e)
		{
			MessageBoxA(nullptr, "STD EXCEPTION", "UNHANDLED EXCEPTION", MB_OK);
			exit(-1);
		}
	}
	 void DeviceResources::Init()
	 {
		 CreateInDependentResources();
		 CreateDependentResources();
	 }

	 float DeviceResources::AspectRatio() const noexcept
	 {
		 return 1.0f;
	 }
	 HRESULT DeviceResources::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements, ID3D11InputLayout** vertLayout)
	 {
		return m_d3d_device->CreateInputLayout(layout, numElements, m_shaderManager->GetBindedVertexShader()->GetShader(),
			 m_shaderManager->GetBindedVertexShader()->GetShaderSize(), vertLayout);
	 }


	 void DeviceResources::SetTargetViews() 
	 {

		 ID3D11RenderTargetView* targetViews[] = { m_d3d_targetView.Get() };

		 if (m_depthStencilFormat == DXGI_FORMAT_UNKNOWN)
		 {
			 m_d3d_deviceContext->OMSetRenderTargets(ARRAYSIZE(targetViews), targetViews, nullptr);
		 }
		 else
		 {
			 m_d3d_deviceContext->OMSetRenderTargets(ARRAYSIZE(targetViews), targetViews, m_d3d_depthStencilView.Get());
			 m_d3d_deviceContext->ClearDepthStencilView(m_d3d_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		 }
		 if(IsMultiSamplingEnabled())
			m_d3d_deviceContext->ResolveSubresource(backBuffer.Get(), 0, m_d3d_renderTarget.Get(), 0, m_backBufferFormat);

	 }

	 void DeviceResources::Cls()
	 {
		 const float color[] = { 1.f,0.f,0.f,1.f };

		 m_d3d_deviceContext->ClearRenderTargetView(m_d3d_targetView.Get(), color);
	 }

	 void DeviceResources::ClearBackgroundColor()
	 {
		
		 
		/* ID3D11RenderTargetView* targetViews[] = {m_d3d_targetView.Get()};

		 if(m_depthStencilFormat == DXGI_FORMAT_UNKNOWN)
		 {
			 m_d3d_deviceContext->OMSetRenderTargets(ARRAYSIZE(targetViews), targetViews, nullptr);
		 }
		 else
		 {
			 m_d3d_deviceContext->OMSetRenderTargets(ARRAYSIZE(targetViews), targetViews, m_d3d_depthStencilView.Get());
			 m_d3d_deviceContext->ClearDepthStencilView(m_d3d_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		 }
		 const float color[] = { 1.f,0.f,0.f,1.f };

		 m_d3d_deviceContext->ClearRenderTargetView(m_d3d_targetView.Get(), color);
		 
		 int boxIndexCount = meshData.Indices.size();
		 int boxVertexCount = meshData.Vertices.size();

		 ComPtr<ID3D11InputLayout> inputLayout;
		 
		 D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		 {
		 {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		 D3D11_INPUT_PER_VERTEX_DATA, 0},
		 {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
		 D3D11_INPUT_PER_VERTEX_DATA, 0}
		 };
		 
		 std::vector<Vertex> vertices(boxVertexCount);

		 XMFLOAT4 black(0.0f, 0.0f, 0.0f, 1.0f);

		 for (size_t i = 0; i <  boxVertexCount; i++)
		 {
			 vertices[i].Pos = meshData.Vertices[i].Position;
		 }
		 
		 std::vector<Vertex> sq(4);

		 sq[0].Pos = XMFLOAT3(-0.5f,0.5f,0.0f);  // TOP LEFT
		 sq[1].Pos = XMFLOAT3(0.5f, 0.5f, 0.0f); // TOP RIGHT
		 sq[2].Pos = XMFLOAT3(0.5f, -0.5f, 0.0f); // BOTTOM RIGHT
		 sq[3].Pos = XMFLOAT3(0.5f, -0.5f, 0.0f); // BOTTOM LEFT


		 std::vector<UINT> iq(6);

		 iq[0] = 0;
		 iq[1] = 1;
		 iq[2] = 3;
		 iq[3] = 1;
		 iq[4] = 2;
		 iq[5] = 3;
		 

	



		 ComPtr<ID3D11Buffer> mVB;
		 ComPtr<ID3D11Buffer> mIB;
		 D3D11_BUFFER_DESC vbd;
		 ZeroMemory(&vbd, sizeof(vbd));
		 vbd.Usage = D3D11_USAGE_DEFAULT;
		 vbd.ByteWidth = sizeof(Vertex) * boxVertexCount;
		 vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		 vbd.CPUAccessFlags = 0;
		 vbd.MiscFlags = 0;
		 D3D11_SUBRESOURCE_DATA vinitData;
		 vinitData.pSysMem = &vertices[0];
		 m_d3d_device->CreateBuffer(&vbd, &vinitData, mVB.GetAddressOf());


		 auto hr = m_d3d_device->CreateInputLayout(vertexDesc, 2,
			 m_shaderManager->GetBindedVertexShader()->GetShader(),
			 m_shaderManager->GetBindedVertexShader()->GetShaderSize(),
			 inputLayout.GetAddressOf()
			 );
		 std::vector<UINT> indices(meshData.Indices);
		

		 D3D11_BUFFER_DESC ibd;

		 ZeroMemory(&ibd, sizeof(ibd));

		 ibd.Usage = D3D11_USAGE_DEFAULT;
		 ibd.ByteWidth = sizeof(UINT) * boxIndexCount;
		 ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		 ibd.CPUAccessFlags = 0;
		 ibd.MiscFlags = 0;
		 D3D11_SUBRESOURCE_DATA iinitData;
		 iinitData.pSysMem = &indices[0];
		 m_d3d_device->CreateBuffer(&ibd, &iinitData, mIB.GetAddressOf());
		 
		

		 

		 m_d3d_deviceContext->IASetInputLayout(inputLayout.Get());
		 m_d3d_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		 UINT stride = sizeof(Vertex);
		 UINT offset = 0;
		 
		 m_d3d_deviceContext->IASetVertexBuffers(0, 1, mVB.GetAddressOf(), &stride, &offset);
		 m_d3d_deviceContext->IASetIndexBuffer(mIB.Get(), DXGI_FORMAT_R32_UINT, 0);

	

		 m_d3d_deviceContext->DrawIndexed(boxIndexCount, 0, 0);*/
		 
		 
		//- XMFLOAT4 color(0.0f, 1.0f, 0.0f, 1.0f);

		 //Create the vertex buffer
		 /*Vertex v[] =
		 {
			 Vertex(0.0f, 0.5f, 0.5f),
			 Vertex(0.5f, -0.5f, 0.5f),
			 Vertex(-0.5f, -0.5f, 0.5f),
		 };

		 D3D11_BUFFER_DESC vertexBufferDesc;
		 ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

		 vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		 vertexBufferDesc.ByteWidth = sizeof(Vertex) * 3;
		 vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		 vertexBufferDesc.CPUAccessFlags = 0;
		 vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferData;

		 ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
		 vertexBufferData.pSysMem = v;

		 ComPtr<ID3D11Buffer> triangleVertBuffer;

		 auto hr = m_d3d_device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, triangleVertBuffer.GetAddressOf());

		 //Set the vertex buffer
		 UINT stride = sizeof(Vertex);
		 UINT offset = 0;

		 m_d3d_deviceContext->IASetVertexBuffers(0, 1, triangleVertBuffer.GetAddressOf(), &stride, &offset);

		 ComPtr<ID3D11InputLayout> vertLayout;

		 D3D11_INPUT_ELEMENT_DESC layout[] =
		 {
			 { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		 };
		 UINT numElements = ARRAYSIZE(layout);

		 //Create the Input Layout
		 hr = m_d3d_device->CreateInputLayout(layout, numElements, m_shaderManager->GetBindedVertexShader()->GetShader(),
			 m_shaderManager->GetBindedVertexShader()->GetShaderSize(), vertLayout.GetAddressOf());

		 //Set the Input Layout
		 m_d3d_deviceContext->IASetInputLayout(vertLayout.Get());

		 //Set Primitive Topology
		 m_d3d_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		 m_d3d_deviceContext->Draw(3, 0);*/
		
	
		 m_renderer->Render();

		 m_dxgi_swapChain->Present(1,0);


	 }
	 void DeviceResources::SwapChain()
	 {
		 m_dxgi_swapChain->Present(1, 0);

	 }

	

	 void DeviceResources::FlushAll()
	 {
		 //! Clear the previous window size specific context.

		 ID3D11RenderTargetView* nullViews[] = { nullptr };
		 m_d3d_deviceContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
		 m_d2d_deviceContext->SetTarget(nullptr);
		 m_d3d_renderTarget.ReleaseAndGetAddressOf();
		 m_d2d_targetBitmap.ReleaseAndGetAddressOf();
		 m_d3d_depthStencilTexture.ReleaseAndGetAddressOf();
		 m_d3d_targetView.ReleaseAndGetAddressOf();
		 m_d3d_depthStencilView.ReleaseAndGetAddressOf();
		 m_d3d_deviceContext->Flush();
	 }

	 bool DeviceResources::IsMultiSamplingEnabled()
	 {
		 return m_multiSamplingSupported && m_multiSampleEnabled;
	 }

	 DeviceResources::~DeviceResources()
	 {
		 FlushAll();
#if defined(DEBUG) || defined(_DEBUG)

		 ComPtr<ID3D11Debug> debug; 
		 auto hr = m_d3d_device->QueryInterface(IID_PPV_ARGS(&debug));
		 hr = debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif
	 }

	 void DeviceResources::CreateDependentResources()
	 {
		 try{
		 //! Clear the previous window size specific context.
		 ID3D11RenderTargetView* nullViews[] = { nullptr };
		 m_d3d_deviceContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
		 m_d2d_deviceContext->SetTarget(nullptr);
		 m_d3d_renderTarget.ReleaseAndGetAddressOf();
		 m_d2d_targetBitmap.ReleaseAndGetAddressOf();
		 m_d3d_depthStencilTexture.ReleaseAndGetAddressOf();
		 m_d3d_targetView.ReleaseAndGetAddressOf();
		 m_d3d_depthStencilView.ReleaseAndGetAddressOf();
		 m_d3d_deviceContext->Flush();

		 //! Recreate DeviceSize
		 m_deviceSize = {};
		 m_deviceSize.logicalWidth = pParentWindow->GetWidth();
		 m_deviceSize.logicalHeight = pParentWindow->GetHeight();
		 m_deviceSize.dpi = pParentWindow->GetDPI();
		 m_deviceSize.SetOutputSizeWithDPI();

		 //! If swap chain already created resize it
		 
		 if (m_dxgi_swapChain)
		 {
			 HRESULT hr = m_dxgi_swapChain->ResizeBuffers(m_backBufferCount, m_deviceSize.outputWidth, m_deviceSize.outputHeight, m_backBufferFormat, 0);
			
			 //! Device Deleted handle it
			 if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
			 {
				 //X Write HandleDeviceLost Func
				 GThrowIfFailed(hr);
			 }
			 else
			 {
				 GThrowIfFailed(hr);
			 }
		 }
		 else
		 {
			 //! Describe our Buffer
			 DXGI_MODE_DESC bufferDesc;

			 ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

			 bufferDesc.Width = m_deviceSize.outputWidth;
			 bufferDesc.Height = m_deviceSize.outputHeight;
			 bufferDesc.RefreshRate.Numerator = 60;
			 bufferDesc.RefreshRate.Denominator = 1;
			 bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			 bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			 bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;


			 //Describe our SwapChain
			 DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
			 swapChainDesc.Width = m_deviceSize.outputWidth;
			 swapChainDesc.Height = m_deviceSize.outputHeight;
			 swapChainDesc.Format = m_backBufferFormat;
			 swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			 swapChainDesc.BufferCount = m_backBufferCount;
			 swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
			 swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			 swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			 swapChainDesc.Flags = 0u;
			 
			 swapChainDesc.SampleDesc.Count = 1;
			 swapChainDesc.SampleDesc.Quality = 0;
			 //X 4xMsaa Quality
			 if (m_multiSampleEnabled && m_multiSamplingSupported)
			 { 
				swapChainDesc.SampleDesc.Count = m_sampleCount;
				swapChainDesc.SampleDesc.Quality = m_maxQualityLevel-1;
			 }
			 //! For 4XMSAA
			 if (m_multiSampleEnabled && m_multiSamplingSupported)
			 {
				 swapChainDesc.SampleDesc.Count = 1;
				 swapChainDesc.SampleDesc.Quality = 0;

			 }
			 //! Create SwapChain
			 GThrowIfFailed(m_dxgi_factory->CreateSwapChainForHwnd(m_d3d_device.Get(), pParentWindow->GetHWnd(), &swapChainDesc, NULL, NULL, m_dxgi_swapChain.GetAddressOf()));
			
			 //
			 // GThrowIfFailed(m_dxgi_device->SetMaximumFrameLatency(1));

			
		 }

		 //! Create Backbuffers and textures

		 DXGI_SAMPLE_DESC sampleDesc;
		 UINT mipmapLevel = 0;
		 sampleDesc.Count =1;
		 sampleDesc.Quality = 0;
		 if ((m_multiSampleEnabled && m_multiSamplingSupported))
		 {
			 sampleDesc.Count =  m_sampleCount ;
			 sampleDesc.Quality =  m_maxQualityLevel -1 ;
			 mipmapLevel = 1;
		 }

		 
		 if (IsMultiSamplingEnabled())
		 {
			 CD3D11_TEXTURE2D_DESC rtvDesc(m_backBufferFormat, m_deviceSize.outputWidth,
				 m_deviceSize.outputHeight, 1U, mipmapLevel, D3D11_BIND_RENDER_TARGET, D3D11_USAGE_DEFAULT, 0U, sampleDesc.Count, sampleDesc.Quality, 0U);

			 GThrowIfFailed(m_d3d_device->CreateTexture2D(
				 &rtvDesc,
				 nullptr,
				 m_d3d_renderTarget.GetAddressOf()
			 ));

			 auto rtv_dim = (m_multiSampleEnabled && m_multiSamplingSupported) ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;

			 D3D11_RENDER_TARGET_VIEW_DESC rtv_desc = CD3D11_RENDER_TARGET_VIEW_DESC(rtv_dim, m_backBufferFormat);

			 GThrowIfFailed(m_d3d_device->CreateRenderTargetView(m_d3d_renderTarget.Get(), &rtv_desc, m_d3d_targetView.GetAddressOf()));

			 m_dxgi_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));


			 m_d3d_deviceContext->ResolveSubresource(backBuffer.Get(), 0, m_d3d_renderTarget.Get(), 0, m_backBufferFormat);
		 }
		 else
		 {
			 m_dxgi_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(m_d3d_renderTarget.GetAddressOf()));
			 GThrowIfFailed(m_d3d_device->CreateRenderTargetView(m_d3d_renderTarget.Get(), nullptr, m_d3d_targetView.GetAddressOf()));

		 }

		 if (m_depthStencilFormat != DXGI_FORMAT_UNKNOWN)
		 {
			 CD3D11_TEXTURE2D_DESC depthStencilDesc(
				 m_depthStencilFormat,
				 m_deviceSize.outputWidth,
				 m_deviceSize.outputHeight,
				 1, // This depth stencil view has only one texture.
				 1, // Use a single mipmap level.
				 D3D11_BIND_DEPTH_STENCIL
			 );

			 //X 4xMsaa Quality
			
			 depthStencilDesc.SampleDesc.Count = 1;
			 depthStencilDesc.SampleDesc.Quality = 0;

			 auto viewTexture = D3D11_DSV_DIMENSION_TEXTURE2D;

			 //! For 4XMSAA
			 if (m_multiSampleEnabled && m_multiSamplingSupported)
			 {
				 depthStencilDesc.SampleDesc.Count = m_sampleCount;
				 depthStencilDesc.SampleDesc.Quality = m_maxQualityLevel-1;
				 viewTexture = D3D11_DSV_DIMENSION_TEXTURE2DMS;
			 }

			 GThrowIfFailed(m_d3d_device->CreateTexture2D(
				 &depthStencilDesc,
				 nullptr,
				 m_d3d_depthStencilTexture.GetAddressOf()
			 ));
		
			 CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(viewTexture);
			 GThrowIfFailed(m_d3d_device->CreateDepthStencilView(
				 m_d3d_depthStencilTexture.Get(),
				 &depthStencilViewDesc,
				 m_d3d_depthStencilView.GetAddressOf()
			 ));
		 }


		 m_d3d_screenViewport = CD3D11_VIEWPORT(
			 0.0f,
			 0.0f,
			 static_cast<float>(m_deviceSize.outputWidth),
			 static_cast<float>(m_deviceSize.outputHeight)
		 );

		 m_d3d_deviceContext->RSSetViewports(1, &m_d3d_screenViewport);

		 //Now struggling with DirectX 2D Render targets 

		 D2D1_BITMAP_PROPERTIES1 bitmapProperties =
			 D2D1::BitmapProperties1(
				 D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
				 D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED
				 ),
				 m_deviceSize.dpi,
				 m_deviceSize.dpi
			 );


		 ComPtr<IDXGIResource1> dxgiBackBuffer;
		
		 m_dxgi_swapChain->GetBuffer(0,IID_PPV_ARGS(&dxgiBackBuffer));
		 //! Create Surface For d2d
		 ComPtr<IDXGISurface2> dxgiSurface;
		 GThrowIfFailed(dxgiBackBuffer->CreateSubresourceSurface(0, dxgiSurface.GetAddressOf()));
		 GThrowIfFailed(m_d2d_deviceContext->CreateBitmapFromDxgiSurface(
			 dxgiSurface.Get(),
			 &bitmapProperties,
			 m_d2d_targetBitmap.GetAddressOf()
		 ));

		 m_d2d_deviceContext->SetTarget(m_d2d_targetBitmap.Get());

		 //! Grayscale text anti-aliasing
		 m_d2d_deviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
		 
		
		 


		}
		 catch (Exception::DxException& e)
		 {
			 std::string msg = e.ToString();
			 MessageBoxA(nullptr, msg.c_str() , "HRFailed", MB_OK);
			 exit(-1);
		 }
		 catch (std::exception& e)
		 {
			 MessageBoxA(nullptr, "STD EXCEPTION", "UNHANDLED EXCEPTION", MB_OK);
			 exit(-1);
		 }
	 }
}