#pragma once

#include "IBindable.h"
#include <Windows.h>

namespace GNF::Common::Bindable
{
	#pragma message("Before class in: " __FILE__)
	class IShaderBindable : public IBindable
	{
		public:
			virtual ~IShaderBindable() override
			{
				int a = 5;
			}
			virtual void Bind() = 0;

			virtual const void* GetShader() const = 0;
			virtual UINT GetShaderSize() const = 0;

	};

}