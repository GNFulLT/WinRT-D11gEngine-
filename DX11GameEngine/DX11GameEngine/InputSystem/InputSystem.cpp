#include "InputSystem.h"

namespace GNF::Input
{
	InputSystem::InputSystem()
	{
		for (int i = 0; i < KEYSIZE; i++)
		{
			m_keys[i] = false;
		}
	}
	void InputSystem::KeyPressed(UINT keyCode)
	{
		m_keys[keyCode] = true;
	}
	int InputSystem::SystemID()
	{
		return 5;
	}

	void InputSystem::KeyReleased(UINT keyCode)
	{
		m_keys[keyCode] = false;
	}
	bool InputSystem::IsKeyPressing(UINT keyCode)
	{
		return m_keys[keyCode];
	}
}