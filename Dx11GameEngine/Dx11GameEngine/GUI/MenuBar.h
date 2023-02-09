#pragma once

#include <string>
#include "Renderer/ImGuiRenderer.h"
#include "Common/Windowing/IMouse.h"

namespace GNF::GUI
{
	class MenuBar
	{
		struct ViewSettings
		{
			bool IsSceneOpen = true;
		};


	public:
		MenuBar() = default;

		//!: Initializing Images Needed for Buttons
		void Init(ID3D11DeviceContext* context);
		//!: Should Be Init after the loading fonts and inside PreRender
		void Init_PreRender();
		void RenderSGui();
		
	private:
		inline void RenderMenu();


	private:
		float m_leftAlignmentForId = 0.f;
		float m_topAlignmentForId = 0.f;
		std::weak_ptr<Common::Windowing::Mouse::IMouse> wp_mouse;
		std::weak_ptr<Renderer::ImGuiRenderer> wp_guiRenderer;	
		Common::Windowing::Mouse::MousePos m_lastMousePos;
		bool isFirstMouseDown = false;
		//!: Changable settings
	private:
		inline constexpr static float g_buttonWidth = 30.f;
		inline constexpr static float g_leftGap = 20.f;

	private:
		inline constexpr static std::string_view g_gameId = "GEngine v1";

		//!: Styles
	private:
		constexpr inline static ImVec4 g_btnStyle = ImVec4( 32.f / 255.f, 36.f / 255.f, 48.f / 255.f ,1.f);
		constexpr inline static ImVec4 g_btnHoveredStyle = ImVec4(30.f / 255.f, 30.f / 255.f, 40.f / 255.f, 1.f);
		constexpr inline static ImVec4 g_btnActiveStyle = ImVec4(40.f / 255.f, 40.f / 255.f, 60.f / 255.f, 1.f);
		constexpr inline static ImVec4 g_checkBg = ImVec4(0, 0, 0, 0);
		constexpr inline static ImVec4 g_checkHoverBg = ImVec4(81.f / 255.f, 81.f / 255.f, 81.f / 255.f, 1.f);
		//!: Settings
	private:
		ViewSettings m_viewSettings;
	};
}