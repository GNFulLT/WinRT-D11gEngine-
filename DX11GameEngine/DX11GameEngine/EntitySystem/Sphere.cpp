#include "Sphere.h"



namespace GNF::Entity
{


	Sphere::Sphere()
	{
		Geometry::GeometryGenerator::CreateBox(10.f, 10.f, 10.f, meshData);
	}

	void Sphere::Draw()
	{

	}
	
	void Sphere::Update(const float frameTime)
	{

	}
	
	void Sphere::InitResources()
	{

	}
}