#pragma once

#include "../IMouse.h"

#include <Windows.h>
#include <functional>

namespace GNF::Common::Windowing::Mouse::Win32
{
	class Win32_Mouse : public GNF::Common::Windowing::Mouse::IMouse
	{
	public:
		Win32_Mouse(HWND hwnd, std::function<RECT(int)> clipCursorCenterCallback) : m_clipCursorCenterCallback(clipCursorCenterCallback), m_mouseState(MouseState::VISIBLE), parent_hwnd(hwnd)
		{
			for (int i = 0; i < SUPPORTED_MOUSE_KEY_NUM; i++)
			{
				keys[i] = false;
			}
		}
		
		virtual void CenterMouse() override
		{
			RECT rect;
			GetWindowRect(parent_hwnd, &rect);
			SetCursorPos((rect.right + rect.left) /2,(rect.bottom+rect.top)/2);
			m_mousePos.x = (rect.right - rect.left) / 2, m_mousePos.y = (rect.bottom - rect.top) / 2;
			mouseCenteredAccepted = false;
		}

		void MouseMoved(int x,int y)
		{
			
			m_mousePrevPos.x = m_mousePos.x;
			m_mousePrevPos.y = m_mousePos.y;
			m_mousePos.x = x;
			m_mousePos.y = y;
		}
		void MouseKeyPressed(VC::MOUSE_CODE mouseCode)
		{
			keys[mouseCode] = true;
		}

		void MouseKeyReleased(VC::MOUSE_CODE mouseCode)
		{
			keys[mouseCode] = false;
		}

		virtual MouseState GetMouseState() override
		{
			return m_mouseState;
		}

		virtual const Windowing::Mouse::MousePos& GetMousePos() override
		{
			return m_mousePos;
		}
		virtual const Windowing::Mouse::MousePos& GetPrevMousePos() override
		{
			return m_mousePrevPos;
		}
		virtual bool IsMouseKeyPressing(VC::MOUSE_CODE mouseCode) override
		{
			return keys[mouseCode];
		}
		
		virtual void SetMouseState(MouseState state)
		{
			if (state == m_mouseState)
				return;
			switch (state)
			{
			case VISIBLE:
				ShowCursor(true);
				break;
			case HIDDEN:
				ShowCursor(false);
				break;
			case DISABLE:
				ShowCursor(false);
				break;
			
			}

			m_mouseState = state;
		}



	private:

		

		std::function<RECT(int)> m_clipCursorCenterCallback;

		bool mouseCenteredAccepted = false;
		int radius = 20;
		bool keys[SUPPORTED_MOUSE_KEY_NUM];
		Windowing::Mouse::MousePos m_mousePos;
		Windowing::Mouse::MousePos m_mousePrevPos;
		Windowing::Mouse::MousePos m_disabledPos;
		MouseState m_mouseState;
		HWND parent_hwnd;
		bool mouseCentered = true;
	};
}