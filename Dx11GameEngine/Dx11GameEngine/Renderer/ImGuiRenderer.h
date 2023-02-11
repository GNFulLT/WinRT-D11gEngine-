#pragma once

#include <Windows.h>
#include "RendererBase.h"
#include <vector>
#include <functional>


namespace GNF::Renderer
{
	enum ImGuiFonts
	{
		FONT_DEFAULT = 0,
		FONT_HEADER,
		FONT_ENTITY_HEADER
	};
	class ImGuiRenderer : public RendererBase
	{
	public:
		ImGuiRenderer(HWND hwnd,ID3D11Device3* device,ID3D11DeviceContext3* deviceContext);
		~ImGuiRenderer();
		void Init();
		void Begin();
		void Draw();
		void Finish();

		inline void SetFont(ImGuiFonts font)
		{
			ImGui::PushFont(m_fonts[font].get());

		}
		inline void SetFontDefault()
		{
			ImGui::PopFont();
		}
		
		void PushRenderFunc(std::function<void()> func);
	private:
		void InitStyles();
		void InitFonts();
	private:
		HWND m_hwnd;
		std::vector<std::function<void()>> m_renderFunctions;
		std::unordered_map<ImGuiFonts, std::unique_ptr<ImFont>> m_fonts;
	};
}