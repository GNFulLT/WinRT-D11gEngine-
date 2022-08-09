#include "pch.h"
#include "GraphicsRenderer.h"


namespace GNF::Graphics
{
	GraphicsRenderer::GraphicsRenderer(Common::DeviceResources* resources) : s_deviceResources(resources)
	{
		s_deviceResources->GetD2DContext()->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Black, 1.0f),
			m_blackBrush.put()
		);
	}
	void GraphicsRenderer::DrawText(const WCHAR* text,UINT textSize)
	{
		winrt::Windows::Foundation::Size rtSize = s_deviceResources->GetRenderTargetSize();

		s_deviceResources->GetD2DContext()->DrawText(text, textSize, s_deviceResources->GetD2DTextFormat(), D2D1::RectF(0, 0, rtSize.Width, rtSize.Height),
			m_blackBrush.get());
	}


	void GraphicsRenderer::CleanUpBg(float r = 0, float g = 0, float b = 0)
	{
		const float colors[4] = {r,g,b,1.f};
		winrt::Windows::Foundation::Size rtSize = s_deviceResources->GetRenderTargetSize();
		D2D1_RECT_F rectangle1 = D2D1::RectF(
			rtSize.Width / 2 - 50.0f,
			rtSize.Height / 2 - 50.0f,
			rtSize.Width / 2 + 50.0f,
			rtSize.Height / 2 + 50.0f
		);

		D2D1_RECT_F rectangle2 = D2D1::RectF(
			rtSize.Width / 2 - 100.0f,
			rtSize.Height / 2 - 100.0f,
			rtSize.Width / 2 + 100.0f,
			rtSize.Height / 2 + 100.0f
		);
		
		s_deviceResources->GetD3DContext()->ClearRenderTargetView(s_deviceResources->GetD3DTargetView(), colors);
		// Draw a filled rectangle.
		s_deviceResources->GetD2DContext()->FillRectangle(&rectangle1, m_blackBrush.get());

		// Draw the outline of a rectangle.
		s_deviceResources->GetD2DContext()->DrawRectangle(&rectangle2, m_blackBrush.get());

	
		
	}

	void GraphicsRenderer::StartDraw()
	{
	    ID3D11RenderTargetView* targets[] = { s_deviceResources->GetD3DTargetView()};
		s_deviceResources->GetD3DContext()->OMSetRenderTargets(1, targets, nullptr);
		s_deviceResources->GetD2DContext()->BeginDraw();

	}

	void GraphicsRenderer::Render()
	{

	}

	void GraphicsRenderer::FinishDraw()
	{
		s_deviceResources->GetD2DContext()->EndDraw();
		s_deviceResources->GetSwapChain()->Present(1, 0);

	}
}