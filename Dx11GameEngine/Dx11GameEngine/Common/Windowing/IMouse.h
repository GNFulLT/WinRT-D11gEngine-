#pragma once

#include "VirtualKeyCodes.h"

namespace GNF::Common::Windowing::Mouse
{
	enum MouseState
	{
		VISIBLE = 0,HIDDEN,DISABLE
	};
	
	struct MousePos
	{
		int x = 0;
		int y = 0;
		MousePos()
		{}
		MousePos(int x,int y) : x(x),y(y)
		{}
	};
	class IMouse
	{
	public:
		virtual ~IMouse() = default;
		virtual const MousePos& GetMousePos() = 0;
		virtual const MousePos& GetPrevMousePos() = 0;
		virtual bool IsMouseKeyPressing(VC::MOUSE_CODE mouseCode) = 0;
		virtual MouseState GetMouseState() = 0;
		virtual void SetMouseState(MouseState) = 0;
		virtual void CenterMouse() = 0;
	};
}