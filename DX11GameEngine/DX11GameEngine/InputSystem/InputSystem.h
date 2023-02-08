#pragma once

#include <Windows.h>
#include "IInputSystem.h"

namespace GNF::Input
{
	#define KEYSIZE 256
	#define MOUSE_KEY_SIZE 3
	class InputSystem : public IInputSystem
	{
		public:
			InputSystem();
			void KeyPressed(UINT keyCode) override;
			void KeyReleased(UINT keyCode) override;
			bool IsKeyPressing(UINT keyCode) override;
			virtual int SystemID() override;
			virtual bool IsMouseKeyPressed(UINT mouseKeyCode) override;
			virtual void MouseKeyPressed(UINT mouseKeyCode) override;
			virtual void MouseKeyReleased(UINT mouseKeyCode) override;

			virtual void MouseMoved(int x, int y) override;
			virtual void GetMouseState(const MouseState** const state) override;
			virtual void GetPrevMouseState(const MouseState** const state) override;
			virtual void GetPrevCurrMouseState(const MouseState** const prev_state, const MouseState** const curr_state) override;

		private:
			bool m_keys[KEYSIZE];
			bool m_mouseKeys[MOUSE_KEY_SIZE];
			MouseState m_mouseState;
			MouseState m_prevMouseState;
	};
}