#include "pch.h"

#include "ImGuiRenderer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui_wrapper.h"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace GNF::Renderer
{
	
		ImGuiRenderer::ImGuiRenderer(HWND hwnd, ID3D11Device3* device, ID3D11DeviceContext3* deviceContext) : RendererBase(device,deviceContext)
		{
			m_hwnd = hwnd;
		}
		ImGuiRenderer::~ImGuiRenderer()
		{
			ImGui_ImplDX11_Shutdown();
			ImGui_ImplWin32_Shutdown();
		}
		void ImGuiRenderer::Init()
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			ImGui_ImplWin32_Init(m_hwnd);
			ImGui_ImplDX11_Init(p_device, p_deviceContext);
			InitStyles();
			InitFonts();
			ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		}
		void ImGuiRenderer::Begin()
		{
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
		}
		void ImGuiRenderer::Draw()
		{
			for (const auto& func : m_renderFunctions)
			{
				func();
			}
		}

		void ImGuiRenderer::PushRenderFunc(std::function<void()> func)
		{
			m_renderFunctions.push_back(func);
		}

		void ImGuiRenderer::InitStyles()
		{
			ImGui::StyleColorsDark();
			auto& style = ImGui::GetStyle();
			//!: Styles
			style.ScrollbarSize = 8.f;
			style.WindowRounding = 3.f;
			//!: Colors
			auto colors = style.Colors;
			colors[ImGuiCol_::ImGuiCol_WindowBg] = ImVec4(38.f / 255.f, 44.f / 255.f, 60.f / 255.f, 1.f);
			colors[ImGuiCol_::ImGuiCol_TitleBg] = ImVec4(51.f/255.f, 57.f/255.f, 79.f/255.f, 1.f);
			colors[ImGuiCol_::ImGuiCol_TitleBgActive] = ImVec4(51.f/255.f, 57.f/255.f, 79.f / 255.f, 1.f);
			colors[ImGuiCol_::ImGuiCol_MenuBarBg] = ImVec4(32.f / 255.f, 36.f / 255.f, 48.f / 255.f, 1.f);
			colors[ImGuiCol_::ImGuiCol_CheckMark] = ImVec4(1.f, 1.f, 1.f, 1.f);
			colors[ImGuiCol_::ImGuiCol_Border] = ImVec4(1.f, 1.f, 1.f, 0.f);
			style.WindowBorderSize = 0;
			style.WindowRounding = 0;
			
		}

		void ImGuiRenderer::InitFonts()
		{
			m_fonts.emplace(ImGuiFonts::FONT_DEFAULT, ImGui::GetIO().Fonts->AddFontFromFileTTF("./fonts/DefaultFont.ttf", 16.0f));
			m_fonts.emplace(ImGuiFonts::FONT_HEADER, ImGui::GetIO().Fonts->AddFontFromFileTTF("./fonts/HeaderFont.ttf", 24.0f));
			ImGui::GetIO().FontDefault = m_fonts[FONT_DEFAULT].get();
			ImGui::GetIO().Fonts->Build();
			ImGui_ImplDX11_InvalidateDeviceObjects();
		}

		void ImGuiRenderer::Finish()
		{
			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		}
		
}