#pragma once
#include "IWindow32.h"

namespace GNF::Window
{
	class IWindow32Class
	{
		public:
			virtual ~IWindow32Class() {};

			virtual HINSTANCE GetHInstance() const noexcept = 0;

	};
}