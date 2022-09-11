#pragma once

#include <Windows.h>
#include "IInputSystem.h"

namespace GNF::Input
{
	#define KEYSIZE 256
	
	class InputSystem : public IInputSystem
	{
		public:
			InputSystem();
			void KeyPressed(UINT keyCode) override;
			void KeyReleased(UINT keyCode) override;
			bool IsKeyPressing(UINT keyCode) override;
			virtual int SystemID() override;
		private:
			bool m_keys[KEYSIZE];
	};
}