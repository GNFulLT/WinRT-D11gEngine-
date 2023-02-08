#pragma once
#include <DirectXMath.h>
#include "Common/RTTI/RTTI.h"

using namespace GNF::Common::RTTI;
namespace GNF::Entity
{
	class IEntity :public RTTI
	{
		RTTI_DECLARATIONS(IEntity, RTTI)
		
		public:
			virtual ~IEntity() {};
			virtual void Update(const float frameTime) = 0;
			virtual void InitResources() = 0;
	/*	
		protected:
			DirectX::XMMATRIX m_rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(0,0,0);
			DirectX::XMMATRIX m_scaleMatrix = DirectX::XMMatrixScaling(1,1,1);
			DirectX::XMMATRIX m_translationMatrix = DirectX::XMMatrixTranslation(0,0,0);
	*/
	};
}