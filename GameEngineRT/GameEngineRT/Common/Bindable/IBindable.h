#pragma once
#include "Common/DeviceResources.h"

namespace GNF::Common::Bindable
{
	class IBindable
	{
		public:
			virtual void Bind(DeviceResources* resources) = 0;
			virtual ~IBindable() = default;
	};
}