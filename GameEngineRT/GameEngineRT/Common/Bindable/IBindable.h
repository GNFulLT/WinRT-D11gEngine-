#pragma once
#pragma message("Top in: " __FILE__)

#include "Common/DeviceResources.fwd.h"

namespace GNF::Common::Bindable
{
	struct VERTEX { FLOAT X, Y, Z; };    // a struct to define a vertex
	#pragma message("Before class in Bindable: " __FILE__)
	class IBindable
	{
		public:
			virtual void Bind(DeviceResources* resources) = 0;
			virtual ~IBindable() {}
	};

}