#pragma once

#include "pch.h"
#include "IBindable.h"

namespace GNF::Common::Bindable
{
	#pragma message("Before class in: " __FILE__)
	class IShaderBindable : public IBindable
	{
		public:
			virtual ~IShaderBindable() {}
			virtual void Bind(GNF::Common::DeviceResources* resources) = 0;

			virtual const void* GetShader() const = 0;
			virtual UINT GetShaderSize() const = 0;
			virtual void* GetD3DShader() const = 0;

	};

}