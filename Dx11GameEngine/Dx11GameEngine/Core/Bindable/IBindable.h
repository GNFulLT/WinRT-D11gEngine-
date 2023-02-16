#pragma once

#include <d3d11_3.h>
#include <wrl/client.h>

namespace GNF::Core::Bindable
{
	class IBindable
	{
	public:
		virtual ~IBindable() = default;
		
		virtual void Bind() = 0;

		virtual void Bind(ID3D11DeviceContext3* ctx) = 0;
	private:
	};
}