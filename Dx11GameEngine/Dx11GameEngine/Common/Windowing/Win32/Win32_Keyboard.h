#pragma once
#include "../IKeyboard.h"
#include "Common/Logger.h"
namespace GNF::Common::Windowing::Keyboard::Win32
{
	class Win32_Keyboard : public Keyboard::IKeyboard
	{
	public:
		Win32_Keyboard()
		{
			for (int i = 0; i < SUPPORTED_KEY_NUM; i++)
			{
				keys[i] = false;
				releasedKeys[i] = false;
				releasedKeyReturned[i] = false;
			}
		}

		void KeyPressed(VC::KEY_CODE code)
		{
			Logger::LogDebug(std::format("Key   {}    pressed",VK_TO_STR(code)).c_str());
			keys[code] = true;
		}

		void KeyReleased(VC::KEY_CODE code)
		{
			
			Logger::LogDebug(std::format("Key   {}    released", VK_TO_STR(code)).c_str());
			keys[code] = false;
			releasedKeys[code] = true;
		}

		virtual bool IsKeyPressed(VC::KEY_CODE code) override
		{
			return keys[code];
		}
		virtual bool IsKeyReleased(VC::KEY_CODE code) override
		{
			if (releasedKeys[code])
			{
				if (releasedKeyReturned[code])
				{
					releasedKeys[code] = false;
					releasedKeyReturned[code] = false;
					return false;
				}
				else
				{
					releasedKeyReturned[code] = true;
					return true;
				}
			}
			else
			{
				return false;
			}
		}

	private:
		bool keys[SUPPORTED_KEY_NUM];
		bool releasedKeys[SUPPORTED_KEY_NUM];
		bool releasedKeyReturned[SUPPORTED_KEY_NUM];
	};
}