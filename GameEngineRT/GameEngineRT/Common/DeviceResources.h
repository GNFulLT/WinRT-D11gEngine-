#pragma once
#include <d3d11_3.h>
#include <d2d1_3.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <DirectXMath.h>
#include "DeviceResources.fwd.h"

#include "ShaderManager.h"
#include "ShaderManager.fwd.h"

namespace GNF::Common {
	class DeviceResources
	{
	public:
		DeviceResources();
		DeviceResources(const DeviceResources&) = delete;
		DeviceResources& operator =(const DeviceResources&) = delete;
		void SetWindow(winrt::Windows::UI::Core::CoreWindow const& window);
		void HandleWindowSizeChanged(winrt::Windows::Foundation::Size newSize);
		float GetAspectRatio() const;
		ShaderManager* GetShaderManager() const;
		//Getters
	public:
		winrt::Windows::UI::Core::CoreWindow GetWindow() const noexcept;
		ID3D11DeviceContext* GetD3DContext() const noexcept;
		ID3D11Device* GetD3DDevice() const noexcept;
		ID3D11RenderTargetView* GetD3DTargetView() const noexcept;
		IDXGISwapChain1* GetSwapChain() const noexcept;
		ID2D1DeviceContext2* GetD2DContext() const noexcept;
		IDWriteTextFormat* GetD2DTextFormat() const noexcept;
		winrt::Windows::Foundation::Size GetRenderTargetSize() const noexcept;

		//For Methods
	private:
		void CreateIndependentResources();
		void CreateDependentResources();
		inline int ConvertDipsToPixels(float dips, float dpi) const;
		void HandleDeviceLost();

		//Check HRESULTS
		void ThrowIfFailed(HRESULT hr, const char* methodName);

		//For DirectX 
	private:
		//DirectX 2D Objects
		winrt::com_ptr<ID2D1Factory3> m_d2d_factory;
		winrt::com_ptr<ID2D1Device2> m_d2d_device;
		winrt::com_ptr<ID2D1DeviceContext2> m_d2d_deviceContext;

		// DirectX 2D rendering object. Required for 2D
		winrt::com_ptr<ID2D1Bitmap1> m_d2d_targetBitmap;

		//For text rendering
		winrt::com_ptr<IDWriteFactory3> m_dwrite_factory;
		//For Image Rendering
		winrt::com_ptr< IWICImagingFactory2> m_wic_factory;
		//For text
		winrt::com_ptr<IDWriteTextFormat> m_dwrite_textFormat;

		//DirectX 3D Objects
		winrt::com_ptr<ID3D11Device> m_d3d_device;
		winrt::com_ptr<ID3D11DeviceContext> m_d3d_deviceContext;

		// Direct3D rendering objects. Required for 3D.
		winrt::com_ptr<ID3D11Texture2D> m_d3d_renderTarget;
		winrt::com_ptr<ID3D11Texture2D> m_d3d_depthStencilTexture;
		winrt::com_ptr<ID3D11DepthStencilView>	m_d3d_depthStencilView;
		winrt::com_ptr<ID3D11RenderTargetView> m_d3d_targetView;
		D3D11_VIEWPORT m_d3d_screenViewport;


		//DirectX DXGI Objects
		winrt::com_ptr<IDXGISwapChain1> m_dxgi_swapChain;
		winrt::com_ptr<IDXGIDevice3> m_dxgi_device;
		winrt::com_ptr<IDXGIAdapter> m_dxgi_adapter;
		winrt::com_ptr<IDXGIFactory2> m_dxgi_factory;


		//Subsystems
		std::unique_ptr<ShaderManager> m_shaderManager;

		//For cached devices
	private:
		//Cached reference for main window
		winrt::agile_ref<winrt::Windows::UI::Core::CoreWindow> m_window{ nullptr };

		winrt::Windows::Foundation::Size m_d3d_renderTargetSize;
		winrt::Windows::Foundation::Size m_outputSize;
		RECT m_logicalSize;

		DXGI_FORMAT m_backBufferFormat;
		DXGI_FORMAT m_depthStencilFormat;
		UINT m_backBufferCount;
		float m_dpi;
	};

}