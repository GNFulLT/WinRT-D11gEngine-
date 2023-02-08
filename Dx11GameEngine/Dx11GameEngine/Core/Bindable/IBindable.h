#pragma once

#include <d3d11.h>
#include <wrl/client.h>

namespace GNF::Core::Bindable
{
	class IBindable
	{
	public:
		virtual ~IBindable() = default;
		
		virtual void Bind() = 0;
	private:
	};
}