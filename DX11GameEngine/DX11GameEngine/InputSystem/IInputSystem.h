#pragma once

#include "Common/System/ISystem.h"

namespace GNF::Input
{
	class IInputSystem : public Common::ISystem
	{
		public:
			virtual ~IInputSystem() {}
			virtual void KeyPressed(UINT keyCode) = 0;
			virtual void KeyReleased(UINT keyCode) = 0;
			virtual bool IsKeyPressing(UINT keyCode) = 0;
			virtual int SystemID() = 0;
	};
}