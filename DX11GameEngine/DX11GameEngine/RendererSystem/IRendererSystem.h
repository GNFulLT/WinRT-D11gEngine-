#pragma once

#include "Graphic/IRenderer3.h"
#include "EntitySystem/IDrawableEntity.h"
#include "Common/System/ISystem.h"
namespace GNF::Renderer
{
	class IRendererSystem : public Graphic::IRenderer3, public Common::ISystem
	{
		public:
			virtual ~IRendererSystem() {};
			virtual void Cls() = 0;
			//! Set Render Target
			virtual void StartDraw() = 0;
			virtual int SystemID() = 0;

			virtual void InitResources() = 0;

			virtual void UpdateThings() = 0;

			virtual void Draw( Entity::IDrawableEntity* ) = 0;

			virtual void Render() = 0;

			//! Swap Chain Flip
			virtual void EndDraw() = 0;
	};
}