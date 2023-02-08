#pragma once

#include <Windows.h>
#include <functional>
#include "IKeyboard.h"
#include "IMouse.h"

namespace GNF::Common::Windowing
{
	class IWindow
	{
	public:
		virtual ~IWindow() = default;

		virtual UINT GetWidth() const noexcept = 0;

		virtual UINT GetHeight() const noexcept = 0;

		virtual UINT GetDPI() const noexcept = 0;

		virtual void Init() = 0;

		virtual void HandleEventsIfAny() = 0;

		virtual bool IsOpen() const noexcept = 0;
		virtual void Restore() = 0;
		virtual void Maximize() = 0;
		virtual Keyboard::IKeyboard* GetKeyboard() = 0;
		virtual Mouse::IMouse* GetMouse() = 0;
		virtual void Minimize() = 0;
		virtual bool IsWindowMaximized() = 0;
		virtual void CloseWindow() = 0;
		virtual void Move(int x, int y) = 0;
	};
}