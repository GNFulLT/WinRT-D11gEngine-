#pragma once

#include "IDrawableEntity.h"
#include <memory>
#include "Common/Bindable/VertexBufferBindable.h"
#include <SimpleMath.h>
namespace GNF::Entity
{
	class Cube : public IDrawableEntity
	{
		public:
			RTTI_DECLARATIONS(Cube, IDrawableEntity)

			Cube();
			Cube(float edgeL,const DirectX::SimpleMath::Vector3& worldPos) : m_worldPos(worldPos),m_edgeL(edgeL)
			{
			}
			~Cube()
			{
				int a = 5;
			}
			virtual void Draw() override;
			virtual void Update(const float frameTime) override;
			virtual void InitResources() override;

		private:
			std::unique_ptr<Common::Bindable::VertexBufferBindable> m_vertexBufferBindable;
			ComPtr<ID3D11InputLayout> vertLayout;
			ComPtr<ID3D11Buffer> squareIndexBuffer;
			UINT sizeOfInd;
			UINT textureID;
			//! For Texture
			ComPtr<ID3D11SamplerState> m_samplerState;

			DirectX::SimpleMath::Vector3 m_worldPos = {0,0,0};
			float m_edgeL = 2.f;
			
	};
}