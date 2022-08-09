#include "pch.h"
#include "DeviceResources.h"
#include <winrt/Windows.UI.Popups.h>
#include <winrt/Windows.Graphics.Display.h>

namespace GNF::Common {

    winrt::Windows::UI::Core::CoreWindow DeviceResources::GetWindow() const noexcept
    {
        return m_window.get();
    }
    ID3D11DeviceContext* DeviceResources::GetD3DContext() const noexcept
    {
        return m_d3d_deviceContext.get();
    }
    ID3D11Device* DeviceResources::GetD3DDevice() const noexcept
    {
        return m_d3d_device.get();
    }
    ID3D11RenderTargetView* DeviceResources::GetD3DTargetView() const noexcept
    {
        return m_d3d_targetView.get();
    }

    ID2D1DeviceContext2* DeviceResources::GetD2DContext() const noexcept
    {
        return m_d2d_deviceContext.get();
    }
    winrt::Windows::Foundation::Size DeviceResources::GetRenderTargetSize() const noexcept
    {
       
        return m_d3d_renderTargetSize;
    }
    IDWriteTextFormat* DeviceResources::GetD2DTextFormat() const noexcept
    {
        return m_dwrite_textFormat.get();
    }


    DeviceResources::DeviceResources() :m_dpi(-1.0f), m_backBufferCount(2), m_backBufferFormat(DXGI_FORMAT_B8G8R8A8_UNORM), m_depthStencilFormat(DXGI_FORMAT_UNKNOWN)
    {
        CreateIndependentResources();
    }

    void DeviceResources::HandleDeviceLost()
    {

    }

    void DeviceResources::SetWindow(winrt::Windows::UI::Core::CoreWindow const& window)
    {
        winrt::Windows::Graphics::Display::DisplayInformation currentDisplayInformation = winrt::Windows::Graphics::Display::DisplayInformation::GetForCurrentView();

        m_window = window;

        m_logicalSize.left = m_logicalSize.top = 0;
        m_logicalSize.bottom = window.Bounds().Height;
        m_logicalSize.right = window.Bounds().Width;

        m_dpi = currentDisplayInformation.LogicalDpi();

        CreateDependentResources();
    }

    void DeviceResources::CreateIndependentResources()
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
        //Create D2D Factory

        ThrowIfFailed(D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED,
            __uuidof(m_d2d_factory),
            &options,
            m_d2d_factory.put_void()
        ),"Cannot create d2d1 factory");

        //Create DirectWrite Factory
        ThrowIfFailed(DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(m_dwrite_factory),
            reinterpret_cast<IUnknown**>(m_dwrite_factory.put())
        ),"Direct Write factory cannot created");

        m_wic_factory = winrt::create_instance<IWICImagingFactory2>(CLSID_WICImagingFactory2);

        //Set Feature Levels
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
        //Create D3D Device
        HRESULT hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
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
            //Create Device with warp
            ThrowIfFailed(D3D11CreateDevice(
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
            ), "Cannot DirectX Device");
        }
        hr = m_d3d_device->QueryInterface(IID_PPV_ARGS(m_dxgi_device.put()));
        ThrowIfFailed(hr, "Cannot create DXGI Device");
        m_dxgi_device->GetAdapter(m_dxgi_adapter.put());
        m_dxgi_factory.capture(m_dxgi_adapter, &IDXGIAdapter::GetParent);


        //Create D2D Device
        ThrowIfFailed(m_d2d_factory->CreateDevice(m_dxgi_device.get(), m_d2d_device.put()), "Cannot Create D2D Device");
        ThrowIfFailed(m_d2d_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, m_d2d_deviceContext.put()),"Cannot Create D2D Device Context");
    }

    void DeviceResources::CreateDependentResources()
    {
        // Clear the previous window size specific context.
        ID3D11RenderTargetView* nullViews[] = { nullptr };
        m_d3d_deviceContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
        m_d2d_deviceContext->SetTarget(nullptr);
        m_d3d_renderTarget = nullptr;
        m_d2d_targetBitmap = nullptr;
        m_d3d_depthStencilTexture = nullptr;
        m_d3d_targetView.put();
        m_d3d_depthStencilView.put();
        m_d3d_deviceContext->Flush();



        //Calculate the real backBuffer Size
        m_outputSize.Width = std::max<UINT>(static_cast<UINT>(ConvertDipsToPixels(m_logicalSize.right - m_logicalSize.left, m_dpi)), 1u);
        m_outputSize.Height = std::max<UINT>(static_cast<UINT>(ConvertDipsToPixels(m_logicalSize.bottom - m_logicalSize.top, m_dpi)), 1u);

        m_d3d_renderTargetSize = m_outputSize;

        //If swap chain already created resize it
        if (m_dxgi_swapChain)
        {
            HRESULT hr = m_dxgi_swapChain->ResizeBuffers(m_backBufferCount, m_d3d_renderTargetSize.Width, m_d3d_renderTargetSize.Height, m_backBufferFormat, 0);
            if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
            {
                // If the device was removed for any reason, a new device and swap chain will need to be created.
                HandleDeviceLost();

                // Everything is set up now. Do not continue execution of this method. HandleDeviceLost will reenter this method 
                // and correctly set up the new device.
                return;
            }
            else
            {
                ThrowIfFailed(hr, "Cannot resize swapchain");
            }
        }
        else
        {
            DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
            swapChainDesc.Width = m_d3d_renderTargetSize.Width;
            swapChainDesc.Height = m_d3d_renderTargetSize.Height;
            swapChainDesc.Format = m_backBufferFormat;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.BufferCount = m_backBufferCount;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.Scaling = DXGI_SCALING_ASPECT_RATIO_STRETCH;
            swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
            swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
            swapChainDesc.Flags = 0u;

            HRESULT hr = m_dxgi_factory->CreateSwapChainForCoreWindow(m_d3d_device.get(), winrt::get_unknown(m_window.get()), &swapChainDesc, nullptr, m_dxgi_swapChain.put());

            ThrowIfFailed(hr, "Cannot Create Swap Chain");

            ThrowIfFailed(m_dxgi_device->SetMaximumFrameLatency(1), "Cannot Set Dxgi maximum frame latency");

            
        }
        //Create Backbuffer
        //ThrowIfFailed(m_dxgi_swapChain->GetBuffer(0, IID_PPV_ARGS(m_d3d_renderTarget.put())), "Swap Chain GetBuffer");
        m_d3d_renderTarget = winrt::capture<ID3D11Texture2D>(m_dxgi_swapChain, &IDXGISwapChain1::GetBuffer, 0);

        ThrowIfFailed(m_d3d_device->CreateRenderTargetView(m_d3d_renderTarget.get(), nullptr, m_d3d_targetView.put()), "Create RenderTargetView");

        if (m_depthStencilFormat != DXGI_FORMAT_UNKNOWN)
        {
            CD3D11_TEXTURE2D_DESC depthStencilDesc(
                m_depthStencilFormat,
                m_d3d_renderTargetSize.Width,
                m_d3d_renderTargetSize.Height,
                1, // This depth stencil view has only one texture.
                1, // Use a single mipmap level.
                D3D11_BIND_DEPTH_STENCIL
            );

            ThrowIfFailed(m_d3d_device->CreateTexture2D(
                &depthStencilDesc,
                nullptr,
                m_d3d_depthStencilTexture.put()
            ), "Depth StencilView");

            CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
            ThrowIfFailed(m_d3d_device->CreateDepthStencilView(
                m_d3d_depthStencilTexture.get(),
                &depthStencilViewDesc,
                m_d3d_depthStencilView.put()
            ), "Depth Stencil view");
        }

        m_d3d_screenViewport = CD3D11_VIEWPORT(
            0.0f,
            0.0f,
            static_cast<float>(m_d3d_renderTargetSize.Width),
            static_cast<float>(m_d3d_renderTargetSize.Height)
        );

        m_d3d_deviceContext->RSSetViewports(1, &m_d3d_screenViewport);


        //Now struggling with DirectX 2D Render targets 

        D2D1_BITMAP_PROPERTIES1 bitmapProperties =
            D2D1::BitmapProperties1(
                D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
                D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED
                ),
                m_dpi,
                m_dpi
            );

        winrt::com_ptr<IDXGIResource1> dxgiBackBuffer = winrt::capture<IDXGIResource1>(m_dxgi_swapChain, &IDXGISwapChain1::GetBuffer, 0);
        winrt::com_ptr<IDXGISurface2> dxgiSurface;
        ThrowIfFailed(dxgiBackBuffer->CreateSubresourceSurface(0, dxgiSurface.put()), "Subresource Surface");
        ThrowIfFailed(m_d2d_deviceContext->CreateBitmapFromDxgiSurface(
            dxgiSurface.get(),
            &bitmapProperties,
            m_d2d_targetBitmap.put()
        ),"Cannot createbitmapfromdxgisurface");

        m_d2d_deviceContext->SetTarget(m_d2d_targetBitmap.get());

        // Grayscale text anti-aliasing is recommended for all Windows Store apps.
        m_d2d_deviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

        //D2D WRITE DEVICE
        ThrowIfFailed(m_dwrite_factory->CreateTextFormat(L"Verdana", NULL, DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL, 50, L"", m_dwrite_textFormat.put()),"Write Device couldnt be created");
        
    }
    IDXGISwapChain1* DeviceResources::GetSwapChain() const noexcept
    {
        return m_dxgi_swapChain.get();
    }

    void DeviceResources::HandleWindowSizeChanged(winrt::Windows::Foundation::Size newSize)
    {
        //If size is same with prev do nothing
        if (newSize.Width == m_logicalSize.right && newSize.Height == m_logicalSize.bottom)
            return;

        RECT newRect;
        newRect.left = newRect.top = 0;
        newRect.right = newSize.Width;
        newRect.bottom = newSize.Height;

        m_logicalSize = newRect;

        CreateDependentResources();
    }


    float DeviceResources::GetAspectRatio() const
    {
        return static_cast<float>(m_d3d_renderTargetSize.Width) / (m_d3d_renderTargetSize.Height);

    }


    void DeviceResources::ThrowIfFailed(HRESULT hr, const char* methodName)
    {
        if (!FAILED(hr))
            return;

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


        winrt::hstring Title = L"Hello";
        winrt::hstring Content = errorText;
        winrt::Windows::UI::Popups::MessageDialog dialog(Content, L"Throwed Exception " + *methodName);

        dialog.ShowAsync();
        throw std::exception("Throwed Exception");
    }

    inline int DeviceResources::ConvertDipsToPixels(float dips, float dpi) const
    {
        return int(dips * dpi / 96.f + 0.5f);
    }
}