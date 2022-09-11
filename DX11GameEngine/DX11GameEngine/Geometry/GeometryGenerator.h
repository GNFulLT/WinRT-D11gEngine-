#pragma once
#include "Common/VertexTypes.h"
#include <Windows.h>
#include <vector>

namespace GNF::Geometry
{
	struct MeshData
	{
		std::vector<Common::GVertex5> Vertices;
		std::vector<UINT> Indices;
	};

	class GeometryGenerator
	{
		public:
			static void CreateBox(float width, float height, float depth, MeshData& meshData);
		private:
	};
}