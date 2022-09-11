#pragma once
#include "WindowDesc.h"
#include "Common/Event/Event.h"
#include <Windows.h>
#include <memory>
#include <optional>
#include "IWindow32.h"
#include "Common/Timer/GTimer.h"
#include "Common/Event/Event.h"
#include "Common/Device/IDeviceResources.h"
#include "InputSystem/IInputSystem.h"
namespace GNF::Window
{
	class MainWindow : public IWindow32
	{
		public:
			MainWindow(const MainWindow&) = delete;
			MainWindow& operator=(const MainWindow&) = delete;

			static MainWindow* CreateMainWindowInstance(const WindowDesc&);
			void CalculateFPS();
			IWindow32Class* GetWindowClass() const noexcept override;
			
			Common::Device::IDeviceResources* GetDeviceResources()
			{
				return m_deviceResources.get();
			}
 
			//! IWindow32 and IWindow32Class

			inline HWND GetHWnd() const noexcept override;
			inline bool IsFullScreen() const noexcept override;
			inline UINT GetWidth() const noexcept override;
			inline UINT GetHeight() const noexcept override;
			inline UINT GetDPI() const noexcept override;

			std::optional<UINT> ProcessWindowMessagesIfExist();
		//# NOTE : The Events When Application start (Setup is ready) and window is created (Ready)
			Common::Event::Event<void> Ready;
			Common::Event::Event<void> SetupStarted;
			//X Will be deleted it is just for testing To RendererSystem
			void Render();

		//# NOTE : For Handling Win32 API Message It divided two, First one is before window is created and second one is after window created
		private:
			MainWindow(const WindowDesc&);
			static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
			static LRESULT CALLBACK HandleMsgReady(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
			LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
			

			//X Put these to GameMain
			void OnWindowActivate();
			void OnWindowInactive();
		private:
			//X Put these to GameMain TOO
			void OnReady();
			void OnSetupStarted();
		private:
			class MainWindowClass : public IWindow32Class
			{
				public:
					MainWindowClass(const WindowDesc&);
					MainWindowClass(const MainWindowClass&) = delete;
					MainWindowClass& operator=(const MainWindowClass&) = delete;
					virtual ~MainWindowClass();
					HINSTANCE GetHInstance() const noexcept override;

				public:
					const wchar_t* m_windowClassName;
					const wchar_t* m_windowTitle;
					HINSTANCE m_hInstance;

			};
		public:
			//X Put this to GameMain
			void Tick();
			void Start();
		private:
			//X Put this to GameMain
			Input::IInputSystem* m_inputSystem;
			std::unique_ptr<MainWindowClass> m_windowClass;
			std::unique_ptr<Common::Device::IDeviceResources> m_deviceResources;
			unsigned int m_width;
			unsigned int m_height;
			HWND m_hWnd;
			bool m_isFullScreen = false;
			Common::Timer::GTimer m_timer;
	};
}