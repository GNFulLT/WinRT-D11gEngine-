#include "pch.h"

#include "Win32_Window.h"
#include "Windowsx.h"
#include "imgui/imgui_impl_win32.h"	
#include "imgui/imgui.h"
#include "WindowsMessageMap.h"
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#define new DEBUG_NEW
#endif
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace GNF::Common::Windowing::Win32
{
	Win32_Window::Win32_Window(const char* windowText,UINT width,UINT height) : m_windowName(windowText),m_height(height),m_width(width),m_hwnd(0)
	{
		m_keyboard.reset(new Keyboard::Win32::Win32_Keyboard());
	

	}

	Mouse::IMouse* Win32_Window::GetMouse()
	{
		return m_mouse.get();
	}

	void Win32_Window::Minimize()
	{
		ShowWindow(m_hwnd, SW_MINIMIZE);
	}

	bool Win32_Window::IsWindowMaximized()
	{
		return m_isMaximized;
	}

	void Win32_Window::Maximize()
	{
		ShowWindow(m_hwnd, SW_MAXIMIZE);
		m_isMaximized = true;
		HandleEventsIfAny();
	}

	void Win32_Window::Restore()
	{
		ShowWindow(m_hwnd, SW_RESTORE);
		m_isMaximized = false;
	}

	RECT Win32_Window::ClipCursorCenter(int a)
	{	
		RECT rect;
		GetWindowRect(m_hwnd, &rect);
		rect.left += m_width / 2 - a;
		rect.right = rect.left + (a << 1);
		rect.top += m_height / 2 - a;
		rect.bottom =  rect.top + (a << 1);
		if (ClipCursor(&rect))
		{
			return rect;
		}
		else
		{
			return RECT{ 0,0,0,0 };
		}
	}

	inline UINT Win32_Window::GetWidth() const noexcept
	{
		return m_width;
	}

	inline UINT Win32_Window::GetHeight() const noexcept
	{
		return m_height;
	}

	inline UINT Win32_Window::GetDPI() const noexcept
	{
		return GetDpiForWindow(m_hwnd);
	}
	void Win32_Window::Move(int x, int y)
	{
		if (m_hwnd != 0)
		{
			RECT rect;
			GetWindowRect(m_hwnd, &rect);
			::SetWindowPos(m_hwnd, 0, rect.left+x, rect.top+y, 0, 0,
				SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
		}
	}
	void Win32_Window::Init()
	{
		auto hInstance = GetModuleHandle(NULL);

		int size_needed = MultiByteToWideChar(CP_UTF8, 0, m_windowName.c_str(), (int)m_windowName.size(), NULL, 0);
		std::wstring wstrTo(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, m_windowName.c_str(), (int)m_windowName.size(), &wstrTo[0], size_needed);

		WNDCLASSEX wndclass{};
		ZeroMemory(&wndclass, sizeof(WNDCLASSEX));
		wndclass.cbSize = sizeof(WNDCLASSEX);
		wndclass.style = CS_HREDRAW | CS_VREDRAW;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
		wndclass.lpszClassName = wstrTo.c_str();
		wndclass.hInstance = hInstance;
		wndclass.lpfnWndProc = WndSetupProc;

		//!: Say this is my unique class (window) to operating system
		RegisterClassEx(&wndclass);
		GCheckAnyError();
		
		DWORD exStyle = 0;
		DWORD style = WS_POPUP;
		UINT x = GetSystemMetrics(SM_CXSCREEN);
		UINT y = GetSystemMetrics(SM_CYSCREEN);

		//!: Set Rect
		RECT windowRect;
		windowRect.left = ((int)x - (int)m_width) / 2;
		windowRect.top = ((int)y - (int)m_height) / 2;
		windowRect.right = ((int)x + (int)m_width) / 2;
		windowRect.bottom = ((int)y + (int)m_height)/ 2;

		RECT rcDesktop;
		::SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcDesktop, NULL);

		if (windowRect.top < rcDesktop.top)
		{
			windowRect.bottom += (windowRect.top - rcDesktop.top) ;
			windowRect.top = rcDesktop.top;
		}
		if (windowRect.left < 0)
		{
			windowRect.right += windowRect.left - rcDesktop.left;
			windowRect.left = 0;
		}

		if (windowRect.bottom > rcDesktop.bottom)
		{
			windowRect.bottom = rcDesktop.bottom;
		}
		if (windowRect.right > rcDesktop.right)
		{
			windowRect.right = rcDesktop.right;
		}

		//!: Set New Rect with Style
		AdjustWindowRectEx(&windowRect, style, FALSE, 0);
		
		m_hwnd = CreateWindowEx(exStyle, wstrTo.c_str(), wstrTo.c_str(), style, windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL, NULL, hInstance, this);
		GCheckAnyError();
		ShowWindow(m_hwnd, SW_SHOWNORMAL);
		UpdateWindow(m_hwnd);

	}
	Keyboard::IKeyboard* Win32_Window::GetKeyboard()
	{
		return m_keyboard.get();
	}
	inline void Win32_Window::HandleEventsIfAny()
	{
		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				Common::Logger::LogInfo("Win32 Closing Event Inside EventsIfany");
				this->Close();
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	bool Win32_Window::IsOpen() const noexcept
	{

		return m_isOpen;
	}
	void Win32_Window::SetOnCreatingWindowCallback(std::function<void(HWND, UINT width, UINT height, bool isFullScreen)> callback)
	{
		m_windowCreatedCallback = callback;
	}
	inline void Win32_Window::Close() noexcept
	{
		m_isOpen = false;
	}
	void Win32_Window::CloseWindow()
	{
		SendMessage(m_hwnd, WM_CLOSE, 0, 0);
	}
	void Win32_Window::SetOnResizeWindowCallback(std::function<void(HWND, UINT width, UINT height, bool isFullScreen, UINT dpi)> callback)
	{
		
		m_resizedCallback = callback;
	}

	LRESULT Win32_Window::HandleWindProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
#if defined _DEBUG
		static WindowsMessageMap mm;
		std::string str = mm(msg, lParam, wParam);
		OutputDebugString(std::wstring(str.begin(), str.end()).c_str());
#endif
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;
		switch (msg)
		{
		case WM_CLOSE:
			Common::Logger::LogInfo("Win32 Window Wanted to Close");
			PostQuitMessage(0);
			break;
		case WM_DESTROY:
			Common::Logger::LogInfo("Win32 Window Wanted to Destroying");
			break;
		case WM_MOUSEMOVE:
		{
			
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			m_mouse->MouseMoved(xPos, yPos);
		}
			break;

		case WM_LBUTTONDOWN:
			m_mouse->MouseKeyPressed(VC::MOUSE_LEFT);
			break;
		case WM_LBUTTONUP:
			m_mouse->MouseKeyReleased(VC::MOUSE_LEFT);
			break;
		case WM_RBUTTONDOWN:
			m_mouse->MouseKeyPressed(VC::MOUSE_RIGHT);
			break;
		case WM_RBUTTONUP:
			m_mouse->MouseKeyReleased(VC::MOUSE_RIGHT);
			break;

		case WM_SIZE:
		{
			if (wParam == SIZE_MINIMIZED)
				break;
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);
			if (m_width == width && m_height == height)
				break;
			m_width = width;
			m_height = height;
			m_resizedCallback(hWnd, m_width, m_height, wParam == SIZE_MAXIMIZED, GetDpiForWindow(hWnd));
		}
			break;
		case WM_KEYDOWN:
			{
				m_keyboard->KeyPressed((VC::KEY_CODE)wParam);
				break;
			}
		case WM_KEYUP:
			{
				m_keyboard->KeyReleased((VC::KEY_CODE)wParam);
				break;
			}
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);

	}
	LRESULT Win32_Window::WndSetupProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		if (msg == WM_NCCREATE)
		{
			GNF::Common::Logger::LogInfo("Window is in creating phase");
			const CREATESTRUCTW* const pcreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			Win32_Window* const gApplication = static_cast<Win32_Window*>(pcreate->lpCreateParams);
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Win32_Window::WndProc));
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(gApplication));
			gApplication->m_mouse.reset(new Mouse::Win32::Win32_Mouse(hWnd,std::bind(&Win32_Window::ClipCursorCenter,gApplication, std::placeholders::_1)));
			if (gApplication->m_windowCreatedCallback)
			{
				GNF::Common::Logger::LogInfo("Calling Created Callback");
				gApplication->m_windowCreatedCallback(hWnd, gApplication->m_width, gApplication->m_height,gApplication->m_isFullScreen);
			}

			return gApplication->HandleWindProc(hWnd, msg, wParam, lParam);
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT Win32_Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{

		return ((Win32_Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->HandleWindProc(hWnd,msg,wParam,lParam);
	}
}


