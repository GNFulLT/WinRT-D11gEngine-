#pragma once

#include <wrl/client.h>
#include "directx_wrapper.h"
#include "../Bindable/Shader/PixelShaderBindable.h"
#include "../Bindable/Shader/VertexShaderBindable.h"
#include "../Bindable/Buffer/VertexBufferBindable.h"
#include "../Bindable/Buffer/ConstantBufferBindable.h"
#include "../Bindable/Buffer/IndexBufferBindable.h"

#include "../VertexBuffer/IVertexBuffer.h"
#include "Common/Utils/utils_dxd.h"
#include <vector>
namespace GNF::Core::GraphicEngine
{
	class GraphicEngine
	{
	public:
		static GraphicEngine* GetInstance();
	public:
		GraphicEngine() noexcept;
		~GraphicEngine();
		void Init();
		void Destroy();
		//void SetFrameScreen(int width, int height);
		void SetWindow(HWND hwnd, UINT width, UINT height,bool isFullScreen, UINT dpi);
		void SetDPI(UINT dpi);
		void SetViewPort();
		void SetRenderTarget();
		DXGI_FORMAT GetSwapChainFormat()
		{
			return ms_backBufferFormat;
		}
		//void FrameBuffer_SetViewPort();
		
		/*
		void FrameBuffer_ChangeViewPort(float width,float height);

		void FrameBuffer_SetRenderTarget();
		void FrameBuffer_ClearColor();
		
		*/
		void SwapBuffers();
		void ClearColor(float r,float g,float b,float a);
		void Resize(UINT width, UINT height, bool isFullScreen,UINT dpi);
		Bindable::Shader::PixelShaderBindable* CreatePixelShader(const wchar_t* path);
		

		//void FrameBuffer_InitRTV_VP(float width,float height);

		template<typename T>
		inline Bindable::Shader::VertexShaderBindable* CreateVertexShader(const wchar_t* path)
		{
			GNF::Common::Logger::LogInfo("Compiling the vertex shader");
			auto pixelShaderByteCode = GNF::Common::Utils::ReadData(path);
			T::Init((const char*)pixelShaderByteCode.data(), pixelShaderByteCode.size());
			return new Bindable::Shader::VertexShaderBindable((void*)pixelShaderByteCode.data(), pixelShaderByteCode.size());
		}
		
		template<>
		inline Bindable::Shader::VertexShaderBindable* CreateVertexShader<void>(const wchar_t* path)
		{
			GNF::Common::Logger::LogInfo("Compiling the vertex shader");
			auto pixelShaderByteCode = GNF::Common::Utils::ReadData(path);
			return new Bindable::Shader::VertexShaderBindable((void*)pixelShaderByteCode.data(), pixelShaderByteCode.size());

		}

		template<typename T>
		inline Bindable::Buffer::VertexBufferBindable* CreateDefaultVertexBuff(const std::vector<T>& data)
		{
			ID3D11InputLayout* ipt = T::GetInputLayout();
			size_t index = T::GetInputLayoutIndex();
			return new Bindable::Buffer::VertexBufferBindable(GNF::Core::Bindable::Buffer::STATIC_DYNAMIC_DRAW,data.data(), data.size() * sizeof(T),sizeof(T),ipt,index);
		}

		template<typename T>
		inline Bindable::Buffer::ConstantBufferBindable* CreateDefaultConstantBuff(const T* data)
		{
			return new Bindable::Buffer::ConstantBufferBindable(GNF::Core::Bindable::Buffer::STATIC_DYNAMIC_DRAW, data, sizeof(T));
		}

		inline Bindable::Buffer::ConstantBufferBindable* CreateDefaultConstantBuff(const void* data,size_t sizeOfData)
		{
			return new Bindable::Buffer::ConstantBufferBindable(GNF::Core::Bindable::Buffer::STATIC_DYNAMIC_DRAW, data, sizeOfData);
		}

		inline Bindable::Buffer::IndexBufferBindable* CreateDefaultIndexBuff(const std::vector<UINT>& data)
		{
			return new Bindable::Buffer::IndexBufferBindable(GNF::Core::Bindable::Buffer::STATIC_DYNAMIC_DRAW, data.data(), data.size());
		}

		//! Getters
	public:
		inline ID3D11DeviceContext3* GetD3DContext()
		{
			return m_d3d_deviceContext.Get();
		}
		
		inline ID3D11Device3* GetD3DDevice()
		{
			return m_d3d_device.Get();
		}		
		void DrawTriangleStrip(int count);

	private:
		void InitIndependentResources();
		
		void FlushAll();

		void Init_IDR_DXGI_THINGS();
		void Init_IDR_D3D_THINGS();
		void Init_IDR_D2D_THINGS();

		void Init_MSAA_Features();

		//!: Create Render target view
		void Init_RTV();
		void Init_ViewPort();

		//!: Depth-Stencil 
		//void InitDSV();

		inline float LogicalSizeToOutputSize(float size);
	private:
		//!: DXGI THINGS
		Microsoft::WRL::ComPtr<IDXGIFactory6> m_dxgi_factory;
		Microsoft::WRL::ComPtr<IDXGIAdapter4> m_dxgi_adapter;
		Microsoft::WRL::ComPtr<IDXGIDevice3> m_dxgi_device;
		Microsoft::WRL::ComPtr<IDXGISwapChain4> m_dxgi_swapChain;

		//!: D3D Things
		Microsoft::WRL::ComPtr<ID3D11Device3> m_d3d_device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext3> m_d3d_deviceContext;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_d3d_renderTargetView;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_d3d_frameBufferTargetView;
		//Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_d3d_depthStencilView;

		//!: Resources
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_d3d_renderTarget;
		//Microsoft::WRL::ComPtr<ID3D11Texture2D> m_d3d_depthStencilTarget;
		//Microsoft::WRL::ComPtr<ID3D11Texture2D> m_d3d_frameBufferTarget;
		
		//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_d3d_frameResource;

		D3D11_VIEWPORT m_d3d_viewPort;
		//D3D11_VIEWPORT m_d3d_frameScreenViewPort;
		//!: D2D Things
		Microsoft::WRL::ComPtr<ID2D1Factory7> m_d2d_factory;
		Microsoft::WRL::ComPtr<ID2D1Device6> m_d2d_device;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext6> m_d2d_deviceContext;

	private:
		D3D_FEATURE_LEVEL m_selectedFeatureLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_1;
		DXGI_FORMAT ms_backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
		//DXGI_FORMAT_D32_FLOAT_S8X24_UINT
		DXGI_FORMAT ms_depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		
		int m_backBufferCount = 2;

		bool m_msaaEnable = false;

		int m_msaaCount = 1;
		int m_msaaQuality = 0;

		UINT m_dpi;

		int m_width;
		int m_height;

		int m_frameScreenWidth;
		int m_frameScreenHeight;
	};
}