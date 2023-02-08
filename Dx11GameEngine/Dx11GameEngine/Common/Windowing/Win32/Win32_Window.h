#pragma once

#include "../IWindow.h"
#include "Win32_Keyboard.h"
#include "Win32_Mouse.h"
#include <string>

namespace GNF::Common::Windowing::Win32
{
	class Win32_Window : public IWindow
	{
	public:
		Win32_Window(const char* windowText, UINT height, UINT width);

		//! Overrides

		//! Copy Move ctor
		Win32_Window(const Win32_Window&) = delete;
		Win32_Window&operator=(const Win32_Window&) = delete;

		inline virtual UINT GetWidth() const noexcept override;

		inline virtual UINT GetHeight() const noexcept override;

		inline virtual UINT GetDPI() const noexcept override;

		inline virtual void Init() override;

		inline virtual void HandleEventsIfAny() override;
		
		virtual bool IsOpen() const noexcept override ;

		virtual Keyboard::IKeyboard* GetKeyboard() override;
		virtual Mouse::IMouse* GetMouse() override;
		virtual void Minimize() override;
		virtual bool IsWindowMaximized() override;
		virtual void Maximize() override;
		virtual void Restore() override;


		RECT ClipCursorCenter(int);
		//!: Custom
	public:
		void SetOnCreatingWindowCallback(std::function<void(HWND,UINT width,UINT height, bool isFullScreen)>);
		void SetOnResizeWindowCallback(std::function<void(HWND, UINT width, UINT height, bool isFullScreen, UINT dpi)>);
		inline void Close() noexcept;
		void CloseWindow() override;
		void Move(int x, int y) override;
		//!: Static
	public:
		static LRESULT CALLBACK WndSetupProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		LRESULT HandleWindProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	private:
		std::unique_ptr<Keyboard::Win32::Win32_Keyboard> m_keyboard;
		std::unique_ptr<Mouse::Win32::Win32_Mouse> m_mouse;
		HWND m_hwnd;
		UINT m_width;
		UINT m_height;
		bool m_isFullScreen = false;
		std::string m_windowName;
		bool m_isOpen = true;
		bool m_isMaximized = false;
		std::function<void(HWND,UINT width, UINT height, bool isFullScreen)> m_windowCreatedCallback;
		std::function<void(HWND, UINT width, UINT height, bool isFullScreen,UINT dpi)> m_resizedCallback;

	};
}