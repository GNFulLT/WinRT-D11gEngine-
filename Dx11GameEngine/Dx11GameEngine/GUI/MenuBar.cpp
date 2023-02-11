#include "pch.h"
#include "MenuBar.h"

#include "imgui/imgui.h"
#include "Core/Game.h"
#include "Common/Utils/utils_imgui.h"
namespace GNF::GUI
{
	void MenuBar::Init(ID3D11DeviceContext* context)
	{
		wp_guiRenderer = Core::Game::GetInstance()->GetImGuiRenderer();
		m_icon.reset(new Icon(L"Assets/nameIcon.png", Core::Game::GetInstance()->GetCurrentTextureManager(), 128, 32));
		m_icon->SetSize(m_icon->GetFullWidth(), m_icon->GetFullHeight());
	}
	void MenuBar::Init_PreRender()
	{
		if (auto imgui = wp_guiRenderer.lock())
		{			
			if (ImGui::Begin("T"))
			{
				imgui->SetFont(Renderer::FONT_HEADER);

				m_leftAlignmentForId = ImGui::CalcTextSize(g_gameId.data()).x;
				m_topAlignmentForId = ImGui::CalcTextSize(g_gameId.data()).y / 2;

				imgui->SetFontDefault();
				ImGui::End();
			}
			
		}
		else
		{
			throw std::runtime_error("WEAK PTR! Couldn't get ImGuiRenderer MenuBar");
		}
	}

	void MenuBar::RenderSGui()
	{
		
		ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_FramePadding,{4.f,8.f});
		RenderMenu();
		ImGui::PopStyleVar();
	}

	inline void MenuBar::RenderMenu()
	{
		if (ImGui::BeginMainMenuBar())
		{
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, g_btnStyle);
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, g_btnHoveredStyle);
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, g_btnActiveStyle);

			bool isMouseActive = false;
			bool isClickedMenu = false;
			bool isAnyMenuEnabled = false;
			//!: No multi thread problem 


			auto imgui = wp_guiRenderer.lock();
			auto mouse = Core::Game::GetInstance()->GetMouse();
	
			isMouseActive = mouse->GetMouseState() != GNF::Common::Windowing::Mouse::DISABLE;		
			
			//!: Render Left Top Text
			imgui->SetFont(GNF::Renderer::FONT_HEADER);
			ImGui::Text(g_gameId.data());
			imgui->SetFontDefault();

			//!: Set Cursor to right
			ImGui::SameLine(ImGui::GetWindowSize().x - m_leftAlignmentForId / 2 - (19*g_buttonWidth)/8);
			
			//!: Draw Right Buttons
			if (ImGui::Button("-", { g_buttonWidth,0 }))
			{
				Core::Game::GetInstance()->GetWindow()->Minimize();
			}
			if (ImGui::Button("A", { g_buttonWidth,0 }))
			{
				isAnyMenuEnabled = true;
				if (Core::Game::GetInstance()->GetWindow()->IsWindowMaximized())
				{
					Core::Game::GetInstance()->GetWindow()->Restore();
				}
				else
				{
					Core::Game::GetInstance()->GetWindow()->Maximize();
				}
			}
			if (ImGui::Button("X", { g_buttonWidth,0 }))
			{
				isAnyMenuEnabled = true;
				Core::Game::GetInstance()->GetWindow()->CloseWindow();
			}

			//!: Set cursor down left after drawing Right side
			ImGui::SameLine(m_leftAlignmentForId + g_leftGap);
			ImGui::SetCursorPosY(16.f - m_topAlignmentForId);

			//!: Draw Menus
			if (ImGui::BeginMenu("View"))
			{
				
				ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_FramePadding, { 2.f,2.f });
				ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_FrameBorderSize, 1);
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg,g_checkBg);
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBgHovered, g_checkHoverBg);
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBgActive, g_checkHoverBg);

				ImGui::Checkbox("Scene", &m_viewSettings.IsSceneOpen);
				ImGui::PopStyleVar(2);
				ImGui::PopStyleColor(3);
				ImGui::EndMenu();
			}
			//! 
			if (!isClickedMenu && !isAnyMenuEnabled && mouse->IsMouseKeyPressing(GNF::Common::Windowing::VC::MOUSE_LEFT))
			{
				auto mousePos = mouse->GetMousePos();
				if (!isFirstMouseDown)
				{
					isFirstMouseDown = true;
					m_lastMousePos = mousePos;
				}

				if (isMouseActive && isFirstMouseDown && mousePos.y < 30.f)
				{
					GNF::Common::Logger::LogDebug("Clicked to bar");
					Core::Game::GetInstance()->GetWindow()->Move(mousePos.x - m_lastMousePos.x, mousePos.y - m_lastMousePos.y);
				}
			}
			else
			{
				if (isFirstMouseDown)
					isFirstMouseDown = false;
			}
			/*
			float aspect = m_icon->GetFullWidth() / m_icon->GetFullHeight();
			m_icon->SetSize(32*aspect,32);
			*/
			m_icon->Draw();

			ImGui::PopStyleColor(3);
			ImGui::EndMainMenuBar();
			/*
			ImGui::Begin("Image");
			m_icon->SetSize(m_icon->GetFullWidth(), m_icon->GetFullHeight());
			m_icon->Draw();
			ImGui::End();
			*/
		}
	}
}