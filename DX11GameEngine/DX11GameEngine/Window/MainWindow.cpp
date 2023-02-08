#include "MainWindow.h"
#include "WindowsMessageMap.h"
#include <string>
#include "Common/Event/EventHandler.h",
#include "Common/Device/DeviceResources.h"
#include "InputSystem/InputSystem.h"
#include "Game/GameMain.h"
#include "InputSystem/VirtualKeyCodes.h"
#include "imgui/imgui_impl_win32.h"	
#include "imgui/imgui.h"
#include <windowsx.h>
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace GNF::Window
{
	 

	MainWindow* MainWindow::CreateMainWindowInstance(const WindowDesc& desc)
	{
		return new MainWindow(desc);
		
	}

	IWindow32Class* MainWindow::GetWindowClass() const noexcept
	{  
		return (IWindow32Class*)m_windowClass.get();
	}

	
	
	MainWindow::MainWindowClass::~MainWindowClass()
	{
		UnregisterClass(m_windowClassName, GetHInstance());
	}

	HINSTANCE MainWindow::MainWindowClass::GetHInstance() const noexcept
	{
		return m_hInstance;
	}

	void MainWindow::OnReady()
	{

	}

	void MainWindow::OnSetupStarted()
	{

	}

	MainWindow::MainWindowClass::MainWindowClass(const WindowDesc& desc) : m_windowClassName(desc.windowClassName), m_windowTitle(desc.windowTitle)
	{
		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = HandleMsgSetup;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = GetHInstance();
		wc.hIcon = nullptr;
		wc.hCursor = nullptr;
		wc.hIconSm = nullptr;
		wc.hbrBackground = nullptr;
		wc.lpszClassName = desc.windowClassName;
		wc.lpszMenuName = nullptr;
		RegisterClassEx(&wc);

	

	}

	MainWindow::MainWindow(const WindowDesc& desc) :
		m_height(desc.height),
		m_width(desc.width)
	{
		RECT wr;
		wr.left = 0;
		wr.top = 0;
		wr.right = desc.width + wr.left;
		wr.bottom = desc.height + wr.top;

		DWORD dStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;

		if (FAILED(AdjustWindowRect(&wr, dStyle, FALSE)))
		{
			throw std::exception("Can't create window rect");
		}

		m_windowClass.reset(new MainWindow::MainWindowClass(desc));

		m_hWnd = CreateWindowEx(NULL, desc.windowClassName, desc.windowTitle, dStyle, CW_USEDEFAULT, CW_USEDEFAULT,
			wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr, m_windowClass->GetHInstance(), this);
		if (m_hWnd == nullptr)
		{
			throw std::exception("Can't create window rect");
		}

		ShowWindow(m_hWnd, 10);

		m_deviceResources.reset(new Common::Device::DeviceResources(this));

		m_deviceResources->Init();

	}
	inline UINT MainWindow::GetDPI() const noexcept
	{
		return GetDpiForWindow(GetHWnd());
	}

	void MainWindow::CalculateFPS()
	{
		static int frameCnt = 0;
		static float timeElapsed = 0.0f;

		frameCnt++;

		// Compute averages over one second period.
		if ((m_timer.GetTotalTimeSeconds() - timeElapsed) >= 1.0f)
		{
			float fps = (float)frameCnt;
			float mspf = 1000.0f / fps;

			std::wstring fpsStr = std::to_wstring(fps);
			std::wstring mspfStr = std::to_wstring(mspf);

			std::wstring windowText =
				L"fps: " + fpsStr +
				L"   mspf: " + mspfStr;

			SetWindowText(GetHWnd(), windowText.c_str());

			// Reset for next average.
			frameCnt = 0;
			timeElapsed += 1.0f;
		}
	}

	void MainWindow::Tick()
	{
		m_timer.Tick();
	}
	void MainWindow::Start()
	{
		m_timer.Reset();
		m_timer.Start();
		
		WindowActive += Common::Event::EventHandler::Bind(&MainWindow::OnWindowActivate, this);

	}


	LRESULT MainWindow::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		
		if (msg == WM_NCCREATE)
		{
			const CREATESTRUCTW* const pcreate = reinterpret_cast<CREATESTRUCTW*>(lParam);

			MainWindow* const pWnd = static_cast<MainWindow*>(pcreate->lpCreateParams);

			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&MainWindow::HandleMsgReady));

			return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	

	LRESULT MainWindow::HandleMsgReady(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		MainWindow* const pWnd = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	

	LRESULT MainWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
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
			case WM_MOUSEMOVE:				
				Game::GameMain::GetInstance()->GetSystem<Input::IInputSystem>()->MouseMoved(GET_X_LPARAM(lParam) ,GET_Y_LPARAM(lParam));
				break;
			case WM_LBUTTONDOWN:
				switch (wParam)
				{
					case MK_LBUTTON:
						Game::GameMain::GetInstance()->GetSystem<Input::IInputSystem>()->MouseKeyPressed(Input::LEFT_BUTTON);
						break;
					case MK_RBUTTON:
						Game::GameMain::GetInstance()->GetSystem<Input::IInputSystem>()->MouseKeyPressed(Input::RIGHT_BUTTON);
						break;
				}
				break;
			case WM_RBUTTONUP:
				Game::GameMain::GetInstance()->GetSystem<Input::IInputSystem>()->MouseKeyReleased(Input::RIGHT_BUTTON);
				break;
			case WM_LBUTTONUP:
				Game::GameMain::GetInstance()->GetSystem<Input::IInputSystem>()->MouseKeyReleased(Input::LEFT_BUTTON);
				switch (wParam)
				{
				case MK_LBUTTON:
					Game::GameMain::GetInstance()->GetSystem<Input::IInputSystem>()->MouseKeyReleased(Input::LEFT_BUTTON);
					break;
				case MK_RBUTTON:
					Game::GameMain::GetInstance()->GetSystem<Input::IInputSystem>()->MouseKeyReleased(Input::RIGHT_BUTTON);
					break;
				}
				break;
			case WM_KEYUP:
				Game::GameMain::GetInstance()->GetSystem<Input::IInputSystem>()->KeyReleased(wParam);		
				break;
			case WM_KEYDOWN:
				Game::GameMain::GetInstance()->GetSystem<Input::IInputSystem>()->KeyPressed(wParam);
				switch (wParam)
				{
					case VK_RIGHT:
					{
						
					}
					break;
					case VK_LEFT:
					{
						LeftKeyUp();
					}
						break;
				}
			
				break;
			case WM_CLOSE:
				PostQuitMessage(0);
				return 0;
			case WM_SIZE:
					{
						int at = 5;
					}
				break;
			case WM_ENTERSIZEMOVE:
				{	
					int at = 5;
				}
				break;
			case WM_ACTIVATE:
				break;
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);

	}

	inline HWND MainWindow::GetHWnd() const noexcept
	{
		return m_hWnd;
	}

	inline UINT MainWindow::GetWidth() const noexcept
	{
		return (UINT)m_width;
	}

	inline UINT MainWindow::GetHeight() const noexcept
	{
		return m_height;
	}

	inline bool MainWindow::IsFullScreen() const noexcept
	{
		return m_isFullScreen;
	}

	void MainWindow::OnWindowActivate()
	{
		m_timer.Start();
	}

	void MainWindow::OnWindowInactive()
	{
		m_timer.Stop();
	}
	void MainWindow::Render()
	{
		m_deviceResources->ClearBackgroundColor();
	}

	std::optional<UINT> MainWindow::ProcessWindowMessagesIfExist()
	{

		MSG msg;
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{

			switch (msg.message)
			{
				case WM_QUIT:
					return msg.message;
				case WM_ACTIVATE:
					if (LOWORD(msg.wParam) == WA_INACTIVE)
					{
						WindowInactive();
					}
					else
					{
						WindowActive();
					}
					break;
			}
			

			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
		return {};
	}





}

