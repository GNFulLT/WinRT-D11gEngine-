#pragma once

#include "RendererBase.h"
#include <wrl/client.h>
#include "Common/IResource.h"

namespace GNF::Renderer
{
	class SceneRenderer : public RendererBase, public Common::IResource
	{
	public:
		SceneRenderer(UINT width,UINT height,DXGI_FORMAT swapChainFormat,ID3D11Device3* device, ID3D11DeviceContext3* deviceContext);
		SceneRenderer(const SceneRenderer&) = delete;
		SceneRenderer& operator=(const SceneRenderer&) = delete;
		//!: Set Render Target 
		void SetAsRenderTarget(ID3D11DeviceContext3* ctx);
		
		//!:
		void ChangeResolution(UINT width, UINT height);

		//!: Initialize Resources
		void Init() override;

		//!: Clears RTV DTV
		void Clear(ID3D11DeviceContext3* ctx);

		//!: Destroys resources
		void Destroy();

		//!: Basically Render Method. It is specifically rendering into a 2d texture another of swap chain's
		ID3D11ShaderResourceView* GetFrame();
	private:
		void Init_Textures();
		void Init_Views();
		void Init_ViewPorts();

		//!: MSAA
		
		void Init_Textures_MSAA();
		void Init_Views_MSAA();

		//!: Device Resources for rendering
	private:
		UINT m_width;
		UINT m_height;
		
		//!: ViewPort of render target
		D3D11_VIEWPORT m_renderViewPort;
		
		//!: It is for drawing on render target 
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;

		//! It is for displaying as texture
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_renderResourceView;

		//!: For Z axis 
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;

		//!: It is resource of RenderTargetView and ShaderResourceView
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_renderTexture;

		//!: For Depth Stencil View
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthStencilTexture;

		DXGI_FORMAT m_renderBufferFormat;
		DXGI_FORMAT m_depthStencilBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

		bool m_msaaEnabled = false;
	};
}