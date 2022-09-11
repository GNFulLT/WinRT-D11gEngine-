#pragma once

#include "IEntity.h"
#include <d3d11.h>
#include <wrl/client.h>

using namespace Microsoft::WRL;

namespace GNF::Entity
{
	class IDrawableEntity : public IEntity
	{
		public:
			RTTI_DECLARATIONS(IDrawableEntity, IEntity)
			virtual ~IDrawableEntity() {}
			virtual void Draw() = 0;
	};
}