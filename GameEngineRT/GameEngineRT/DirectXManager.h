#pragma once

#include <d3d11_1.h>
#include <d2d1_1.h>

using namespace winrt;
using namespace Windows::UI::Core;

class DirectXManager
{
	public:
		DirectXManager(CoreWindow const& window);
		void UpdateDependentResources();
		void Render();
	private:
		void InitializeResources();

	private:
		winrt::com_ptr<ID3D11Device> m_d3d_device;
		winrt::com_ptr<ID3D11DeviceContext> m_d3d_deviceContext;
		winrt::com_ptr<ID3D11RenderTargetView> m_d3d_targetView;

		winrt::com_ptr<IDXGISwapChain1> m_dxgi_swapChain;


		winrt::com_ptr<::IDXGIDevice> m_dxgi_device;
		winrt::com_ptr<IDXGIAdapter> m_dxgi_adapter;


	private:
		winrt::agile_ref<winrt::Windows::UI::Core::CoreWindow> s_window {nullptr};
};

