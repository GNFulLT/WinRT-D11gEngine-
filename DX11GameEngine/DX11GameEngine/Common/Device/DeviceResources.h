#pragma once

#include "IDeviceResources.h"
#include "D3D11Depends.h"
#include "Window/IWindow32.h"
#include <Windows.h>
#include <wrl/client.h>
#include <memory>
#include "Common/ShaderManagement/IShaderManager.h"
#include "Geometry/GeometryGenerator.h"
#include "Graphic/IRenderer2.h"
#include "Common/TextureManagement/ITextureManager.h"

using namespace Microsoft::WRL;

namespace GNF::Common::Device
{
	struct DeviceSize
	{
		public:
			UINT logicalWidth;
			UINT logicalHeight;
			UINT outputWidth;
			UINT outputHeight;
			UINT dpi;

			inline void SetOutputSizeWithDPI() noexcept
			{
				outputWidth = ConvertDipsToPixels(logicalWidth, dpi);
				outputHeight = ConvertDipsToPixels(logicalHeight, dpi);
			}

		private:
			static inline int ConvertDipsToPixels(float dips, float dpi)
			{
				return int(dips * dpi / 96.f + 0.5f);
			}

	};

	class DeviceResources : public IDeviceResources
	{
		public:
			DeviceResources(Window::IWindow32* parentWindow);

			DeviceResources(const DeviceResources&) = delete;
			DeviceResources& operator =(const DeviceResources&) = delete;
			~DeviceResources();
			virtual void Init() override;


			virtual ID3D11Device* GetD3DDevice() override;

			virtual ID3D11DeviceContext* GetD3DContext() override;

			virtual float AspectRatio() const noexcept override;
			virtual void Cls() override;
			virtual void SwapChain() override;

			//X Put this Vertex class

			virtual HRESULT CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC*, UINT numElements, ID3D11InputLayout**) override;
			virtual Texture::ITextureManager* GetTextureManager() override;

			virtual void SetTargetViews()  override;

			//X Put This to renderer 
			void ClearBackgroundColor() override;

			void HandleWindowSizeChanged() override;

			inline virtual bool IsMultiSamplingEnabled() override;

		protected:
			inline void DevicePropsCheck(size_t* videoMem = nullptr, size_t* sharedMem = nullptr, wchar_t* deviceNameBuffer = nullptr, int bufferSize = 0);
			inline const char* SelectFuture();
			inline void LoadShaderManager(Bindable::PixelShaderBindable* pixelShader = nullptr, Bindable::VertexShaderBindable* vertexShader = nullptr);
			inline bool CheckMSAAFeatures(int* msaaMaxLevel = nullptr, bool* msaaIsEnabled = nullptr);
		private:
			virtual void CreateDependentResources() override;

			virtual void CreateInDependentResources() override;

			void FlushAll();

			void OnLeftKeyUp();
			//! DirectX Things
		private:
			//! Direct2D Things
			ComPtr< ID2D1Factory7 > m_d2d_factory;
			ComPtr< ID2D1Device6 > m_d2d_device;
			ComPtr< ID2D1DeviceContext6 > m_d2d_deviceContext;


			//! DirectWrite Things
			ComPtr < IDWriteFactory7 > m_dwrite_factory;
			ComPtr < IWICImagingFactory > m_wic_factory;
			

			//! Direct3D Things
			ComPtr < ID3D11Device3 > m_d3d_device;
			ComPtr < ID3D11DeviceContext3 > m_d3d_deviceContext;

			//! DXGI Things
			ComPtr < IDXGIDevice3 > m_dxgi_device;
			ComPtr < IDXGIAdapter2 > m_dxgi_adapter;
			ComPtr < IDXGIFactory3 > m_dxgi_factory;
			ComPtr < IDXGISwapChain1 > m_dxgi_swapChain;

			Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;

			//! Resources that is used by DirectX Things
		private:
			//! Direct3D rendering objects. Required for 3D.
			ComPtr < ID3D11Texture2D > m_d3d_renderTarget;
			ComPtr < ID3D11Texture2D > m_d3d_depthStencilTexture;
			ComPtr < ID3D11DepthStencilView >	m_d3d_depthStencilView;
			ComPtr < ID3D11RenderTargetView > m_d3d_targetView;
			D3D11_VIEWPORT m_d3d_screenViewport;
			
			//! DirectX 2D rendering object. Required for 2D
			ComPtr < ID2D1Bitmap1 > m_d2d_targetBitmap;

			//! DirectX Formats
			DXGI_FORMAT m_backBufferFormat;
			DXGI_FORMAT m_depthStencilFormat;

			//! Infos
			UINT m_backBufferCount = 2;
		private:
			bool m_multiSamplingSupported = false;
			int m_sampleCount = 4;
			int m_maxQualityLevel = 0;
			bool m_multiSampleEnabled = false;
			Geometry::MeshData meshData;
			DeviceSize m_deviceSize;
			Window::IWindow32* pParentWindow;
			std::unique_ptr<ShaderManagement::IShaderManager> m_shaderManager;
			std::unique_ptr<Graphic::IRenderer2>  m_renderer;
			std::unique_ptr<Texture::ITextureManager> m_textureManager;
	};
}