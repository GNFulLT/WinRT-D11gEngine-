#include "pch.h"
#include "DirectXManager.h"



DirectXManager::DirectXManager(CoreWindow const& window) : s_window(window)
{
    InitializeResources();
}



void DirectXManager::Render()
{

    //Set Targets
    ID3D11RenderTargetView* targets[] = { m_d3d_targetView.get() };
    m_d3d_deviceContext->OMSetRenderTargets(1, targets, nullptr);

    float color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
    m_d3d_deviceContext->ClearRenderTargetView(m_d3d_targetView.get(), color);

    m_dxgi_swapChain->Present(1, 0);
}




void DirectXManager::InitializeResources()
{

	//Creation Options
    D3D11_CREATE_DEVICE_FLAG creationFlag = static_cast<D3D11_CREATE_DEVICE_FLAG>(0);

		
#if _DEBUG
	creationFlag = static_cast<D3D11_CREATE_DEVICE_FLAG>(static_cast<UINT>(creationFlag) | static_cast<UINT>(D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG));
	creationFlag = static_cast<D3D11_CREATE_DEVICE_FLAG>(static_cast<UINT>(creationFlag) | static_cast<UINT>(D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUGGABLE));
#endif

    //Feature Levels
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
    HRESULT hr = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_WARP,
        0,
        creationFlag,
        featureLevels,
        ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION,
        m_d3d_device.put(),
        nullptr,
        m_d3d_deviceContext.put()
    );
    if (FAILED(hr))
    {
        LPTSTR errorText = NULL;

        FormatMessage(
            // use system message tables to retrieve error text
            FORMAT_MESSAGE_FROM_SYSTEM
            // allocate buffer on local heap for error text
            | FORMAT_MESSAGE_ALLOCATE_BUFFER
            // Important! will fail otherwise, since we're not 
            // (and CANNOT) pass insertion parameters
            | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,    // unused with FORMAT_MESSAGE_FROM_SYSTEM
            hr,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&errorText,  // output 
            0, // minimum size for output buffer
            NULL);   // arguments - see note 

        int a = 5;
    }

    HRESULT res = m_d3d_device->QueryInterface(IID_PPV_ARGS(m_dxgi_device.put()));

    m_dxgi_device->GetAdapter(m_dxgi_adapter.put());

    winrt::com_ptr<::IDXGIFactory2> dxgiFactory;
    dxgiFactory.capture(m_dxgi_adapter, &IDXGIAdapter::GetParent);


    DXGI_SWAP_CHAIN_DESC1 swapChainDesc{ 0 };
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.Stereo = false;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.Scaling = DXGI_SCALING_NONE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    swapChainDesc.Flags = 0;
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

    hr = dxgiFactory->CreateSwapChainForCoreWindow(m_d3d_device.get(), winrt::get_unknown(s_window.get()), &swapChainDesc, nullptr, m_dxgi_swapChain.put());
    if (FAILED(hr))
    {
        LPTSTR errorText = NULL;

        FormatMessage(
            // use system message tables to retrieve error text
            FORMAT_MESSAGE_FROM_SYSTEM
            // allocate buffer on local heap for error text
            | FORMAT_MESSAGE_ALLOCATE_BUFFER
            // Important! will fail otherwise, since we're not 
            // (and CANNOT) pass insertion parameters
            | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,    // unused with FORMAT_MESSAGE_FROM_SYSTEM
            hr,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&errorText,  // output 
            0, // minimum size for output buffer
            NULL);   // arguments - see note 

        int a = 5;
    }

    winrt::com_ptr<ID3D11Texture2D> backbuffer;
    m_dxgi_swapChain->GetBuffer(0, IID_PPV_ARGS(backbuffer.put()));

    // create a render target pointing to the back buffer
    m_d3d_device->CreateRenderTargetView(backbuffer.get(), nullptr, m_d3d_targetView.put());

    
}

void DirectXManager::UpdateDependentResources()
{

}