#include "pch.h"
#include "Triangle2D.h"

using namespace GNF::Core::VertexBuffer;

namespace GNF::Entity
{
	Triangle2D::Triangle2D(const float edgeLength, const DirectX::SimpleMath::Vector3& loc) : IEntity("Triangle2D"), m_edgeLength(edgeLength), m_worldLocation(loc)
	{

	}
	void Triangle2D::Update(float const deltaTime)
	{

	}

	void Triangle2D::Destroy()
	{

	}

	void Triangle2D::OnCreated()
	{
		float halfLength = m_edgeLength / 2.0f;
		float height = std::sqrt(3.0f) / 2.0f * m_edgeLength;

		auto positions = m_worldLocation + DirectX::SimpleMath::Vector3(-halfLength, -height, 0.0f);
		auto positions2 = m_worldLocation + DirectX::SimpleMath::Vector3(halfLength, -height, 0.0f);

		std::vector<OnlyVertexBuffer> vertices{
			{m_worldLocation.x,m_worldLocation.y,m_worldLocation.z,0.5f,0},
			{positions.x,positions.y,positions.z,0,1.f},
			{positions2.x,positions2.y,positions2.z,1.f,1.f}
		};

		std::vector<UINT> indices{
			2,1,0
		};
		
		SetIndices(indices);
		SetVertices(vertices);
	}

}