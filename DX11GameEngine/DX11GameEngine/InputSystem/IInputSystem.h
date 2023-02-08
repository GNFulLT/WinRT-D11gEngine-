#pragma once

#include "Common/System/ISystem.h"

#include <Windows.h>

namespace GNF::Input
{
	struct MouseState
	{
		MouseState()
		{
			posx = 0;
			posy = 0;
		}
		int posx;
		int posy;
	};

	class IInputSystem : public Common::ISystem
	{
		public:
			virtual ~IInputSystem() {}
			virtual void KeyPressed(UINT keyCode) = 0;
			virtual void KeyReleased(UINT keyCode) = 0;
			virtual bool IsKeyPressing(UINT keyCode) = 0;
			virtual bool IsMouseKeyPressed(UINT mouseKeyCode) = 0;
			virtual int SystemID() = 0;

			virtual void MouseMoved(int x,int y) = 0;
			virtual void GetMouseState(const MouseState** const state) = 0;
			virtual void GetPrevMouseState(const MouseState** const state) = 0;
			virtual void GetPrevCurrMouseState(const MouseState** const prev_state, const MouseState** const curr_state) = 0;

			virtual void MouseKeyPressed(UINT mouseKeyCode) = 0;
			virtual void MouseKeyReleased(UINT mouseKeyCode) = 0;
	};
}