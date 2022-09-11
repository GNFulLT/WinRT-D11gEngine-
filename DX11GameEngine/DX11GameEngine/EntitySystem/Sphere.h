#pragma once

#include "IDrawableEntity.h"
#include "PrimitiveBatch.h"
#include "Geometry/GeometryGenerator.h"
#include "Common/VertexTypes.h"

namespace GNF::Entity
{

	class Sphere : public IDrawableEntity
	{
		public:
			RTTI_DECLARATIONS(Sphere, IDrawableEntity)

			Sphere();
			~Sphere() {};

			virtual void Draw() override;
			virtual void Update(const float& frameTime) override;
			virtual void InitResources() override;
		private:
			Geometry::MeshData meshData;
			ComPtr<ID3D11InputLayout> vertLayout;

	};
}
