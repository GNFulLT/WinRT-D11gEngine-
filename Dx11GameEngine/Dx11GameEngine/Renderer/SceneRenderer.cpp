#include "pch.h"
#include "SceneRenderer.h"

namespace GNF::Renderer
{
	SceneRenderer::SceneRenderer(UINT width, UINT height, DXGI_FORMAT swapChainFormat, ID3D11Device3* device, ID3D11DeviceContext3* deviceContext) :
		m_width(width),
		m_height(height),
		m_renderBufferFormat(swapChainFormat),
		RendererBase(device,deviceContext)
	{
		m_renderViewPort = CD3D11_VIEWPORT(0.f, 0.f, (float)width, (float)height);
	}
	void SceneRenderer::SetAsRenderTarget()
	{
		ID3D11RenderTargetView* targets[] = { m_renderTargetView.Get() };
		
		p_deviceContext->OMSetRenderTargets(1, targets,m_depthStencilView.Get());

		p_deviceContext->RSSetViewports(1, &m_renderViewPort);

		
	}

	void SceneRenderer::Clear()
	{
		float color[] = { 0,1,0,1 };
		p_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), color);
		p_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
	ID3D11ShaderResourceView* SceneRenderer::GetFrame()
	{
		return m_renderResourceView.Get();
	}

	//!:
	void SceneRenderer::ChangeResolution(UINT width, UINT height)
	{
		m_width = width;
		m_height = height;
		Destroy();
		Init();
	}

	//!: Initialize Resources
	void SceneRenderer::Init()
	{
		if (!m_msaaEnabled)
		{
			Init_Textures();
			Init_Views();
		}
		else
		{
			Init_Textures_MSAA();
			Init_Views_MSAA();
		}
		Init_ViewPorts();
	}

	void SceneRenderer::Init_Textures()
	{
		//!: If MSAA active MipMapLevel should be 1 and sample count and quality 
		CD3D11_TEXTURE2D_DESC desc(m_renderBufferFormat, m_width, m_height, 1U, 0U, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT, 0U, 1, 0, 0U);

		GThrowIfFailed(p_device->CreateTexture2D(&desc,NULL,m_renderTexture.GetAddressOf()));

		CD3D11_TEXTURE2D_DESC depthStencilDesc(
			m_depthStencilBufferFormat,
			m_width,
			m_height,
			1U, //! DSV only has one texture
			1U, //!: Single Mipmap
			D3D11_BIND_DEPTH_STENCIL
		);

		GThrowIfFailed(p_device->CreateTexture2D(&depthStencilDesc, NULL, m_depthStencilTexture.GetAddressOf()));

	}

	void SceneRenderer::Init_Textures_MSAA()
	{

	}

	void SceneRenderer::Init_Views()
	{
		//!: Depth Stencil View
		CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
		GThrowIfFailed(p_device->CreateDepthStencilView(
			m_depthStencilTexture.Get(),
			&depthStencilViewDesc,
			m_depthStencilView.GetAddressOf()
		));

		//!: Render Target View
		CD3D11_RENDER_TARGET_VIEW_DESC ftv(D3D11_RTV_DIMENSION_TEXTURE2D);
		GThrowIfFailed(p_device->CreateRenderTargetView(m_renderTexture.Get(),
			&ftv, m_renderTargetView.GetAddressOf()));

		//!: Shader Resource View
		CD3D11_SHADER_RESOURCE_VIEW_DESC frv(D3D11_SRV_DIMENSION_TEXTURE2D);
		GThrowIfFailed(p_device->CreateShaderResourceView(m_renderTexture.Get(),
			&frv, m_renderResourceView.GetAddressOf()));

	}

	void SceneRenderer::Init_Views_MSAA()
	{

	}

	void SceneRenderer::Init_ViewPorts()
	{
		m_renderViewPort = CD3D11_VIEWPORT(0.f, 0.f, (float)m_width, (float)m_height);
	}

	void SceneRenderer::Destroy()
	{
		ID3D11RenderTargetView* nullViews[] = { nullptr };
		p_deviceContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
		//!: Destroy Views
		m_renderResourceView.ReleaseAndGetAddressOf();
		m_renderTargetView.ReleaseAndGetAddressOf();
		m_depthStencilView.ReleaseAndGetAddressOf();
		//!: Destroy Textures
		m_renderTexture.ReleaseAndGetAddressOf();
		m_depthStencilTexture.ReleaseAndGetAddressOf();
	}
}