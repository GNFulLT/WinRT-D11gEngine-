#pragma once

#include "VirtualKeyCodes.h"

namespace GNF::Common::Windowing::Keyboard
{
	class IKeyboard
	{
	public:
		virtual ~IKeyboard() = default;

		virtual bool IsKeyPressed(VC::KEY_CODE code) = 0;
		//!: If In one loop key released returns true for once
		virtual bool IsKeyReleased(VC::KEY_CODE code) = 0;
	};
}