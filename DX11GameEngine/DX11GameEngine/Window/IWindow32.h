#pragma once
#include <Windows.h>
#include "Common/Event/Event.h"
#include "IWindow32Class.h"
#include "Common/Device/IDeviceResources.h"
namespace GNF::Window
{
	class IWindow32
	{
		public:
			virtual ~IWindow32() = default;

			virtual IWindow32Class* GetWindowClass() const noexcept = 0;

			virtual HWND GetHWnd() const noexcept = 0;

			virtual bool IsFullScreen() const noexcept = 0;

			virtual UINT GetWidth() const noexcept = 0;

			virtual UINT GetHeight() const noexcept  = 0;

			virtual UINT GetDPI() const noexcept = 0;

			//! For Event
		public:
			Common::Event::Event<void> WindowInactive;
			Common::Event::Event<void> WindowActive;
			Common::Event::Event<void> LeftKeyUp;
	};
}