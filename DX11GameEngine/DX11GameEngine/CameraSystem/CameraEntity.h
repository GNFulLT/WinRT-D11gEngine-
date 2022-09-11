#pragma once

#include "EntitySystem/IEntity.h"
#include <DirectXMath.h>
#include "Common/Bindable/ConstBufferBindable.h"
#include <memory>

using namespace DirectX;

namespace GNF::Camera
{
	struct cbPerObject
	{
		XMMATRIX  WVP;
	};


	class CameraEntity : public Entity::IEntity
	{
		public:
			RTTI_DECLARATIONS(CameraEntity, IEntity)

			CameraEntity();
			
			virtual void Update(const float& frameTime) override;

			virtual void InitResources() override;

		protected:
			inline void UpdateWVP();
		private:
			std::unique_ptr<Common::Bindable::ConstBufferBindable> constBuffer;
			cbPerObject cbPerObj;

			//! World View Matrix
			XMMATRIX WVP;
			XMMATRIX World;
			XMMATRIX camView;
			XMMATRIX camProjection;

			//! For Camera Position
			XMVECTOR camPosition;
			XMVECTOR camTarget;
			XMVECTOR camUp;
	};
}