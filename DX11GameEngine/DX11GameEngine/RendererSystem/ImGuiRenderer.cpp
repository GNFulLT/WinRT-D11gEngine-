#include "ImGuiRenderer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "Common/imgui_wrapper.h"

namespace GNF::Renderer
{
	void ImGuiRenderer::InitResources()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplWin32_Init(m_hwnd);
		ImGui_ImplDX11_Init(p_res->GetD3DDevice(), p_res->GetD3DContext());
		ImGui::StyleColorsDark();

	}

	ImGuiRenderer::~ImGuiRenderer()
	{
		
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
	}

	void ImGuiRenderer::Render()
	{
	
		//X UPDATE TO BOOST FUNC
		for(auto func : m_funcList)
		{
			func();
		}
	}

	void ImGuiRenderer::UpdateThings()
	{

	}
	void ImGuiRenderer::Begin()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
	void ImGuiRenderer::Finish()
	{

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}

