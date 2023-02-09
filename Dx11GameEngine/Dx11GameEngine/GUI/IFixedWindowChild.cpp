#include "pch.h"
#include "IFixedWindowChild.h"
#include <boost/bind.hpp>

#include "Core/Game.h"

namespace GNF::GUI
{
	IFixedWindowChild::IFixedWindowChild(const char* name,int top, int left, float widthP, float heightP) : m_top(top),
		m_left(left),m_name(name)
	{
		m_widthP = widthP > 1 ? 1 : widthP;
		m_heightP = heightP > 1 ? 1 : heightP;
	}

	void IFixedWindowChild::Init()
	{
		sizeChangedConnection = Core::Game::GetInstance()->AddWindowSizeChanged(boost::bind(&IFixedWindowChild::OnWindowSizeChanged, this, _1, _2,_3));
		m_height = Core::Game::GetInstance()->GetWindow()->GetHeight()* m_heightP;
		m_width = Core::Game::GetInstance()->GetWindow()->GetHeight() * m_widthP;
		
	}

	void IFixedWindowChild::OnWindowSizeChanged(int sizeX, int sizeY, bool isFullScreen)
	{
		m_height = sizeY * m_heightP;
		m_width = sizeX * m_widthP;
	}

	void IFixedWindowChild::RenderSGui()
	{

		ImVec2 locVec2 = { (float)m_top,(float)m_left } ;
		ImVec2 sizeVec2 = { (float)m_width,(float)m_height };
		auto cursorP = ImGui::GetCursorPos();
		ImGui::SetCursorPos(locVec2);
		if (ImGui::Begin(m_name,0,ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse))
		{
			ImGui::End();
		}
		ImGui::SetCursorPos(cursorP);
	}
}