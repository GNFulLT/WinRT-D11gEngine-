#pragma once

#include "IEntity.h"

namespace GNF::Entity
{
	class Triangle2D : public IEntity
	{
	public:
		Triangle2D(const float edgeLength,const DirectX::SimpleMath::Vector3& loc = DEFAULT_WORLD_POS);

		//!: This is for creation. Give Indices and Vertexes in this method. If you created with EntityManager the entity. Dont use it
		virtual void OnCreated() override;

		//!: Update Method Per Frame
		virtual void Update(float const deltaTime) override;

		//!: 
		virtual void Destroy() override;
	private:
		float m_edgeLength;
		DirectX::SimpleMath::Vector3 m_worldLocation;
	};
}