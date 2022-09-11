#pragma once

#include <DirectXMath.h>

using namespace DirectX;

namespace GNF::Common
{

	struct GVertex1
	{
		GVertex1() = default;
		GVertex1(float x, float y, float z)
			: pos(x, y, z) {}
		XMFLOAT3 pos;
	};

	struct GVertex2
	{
		GVertex2() = default;
		GVertex2(float x, float y, float z,float r,float g, float b)
			: pos(x, y, z), color(r,g,b) {}
		XMFLOAT3 pos;
		XMFLOAT3 color;

	};

	struct GVertex3
	{
		GVertex3() = default;
		GVertex3(float x, float y, float z, float r, float g, float b,float texX,float texY)
			: pos(x, y, z), color(r, g, b), tex0(texX,texY) {}
		XMFLOAT3 pos;
		XMFLOAT3 color;
		XMFLOAT2 tex0;
	};

	struct GVertex4
	{
		GVertex4() = default;
		GVertex4(float x, float y, float z, float r, float g, float b, float tex0X, float tex0Y,float tex1X,float tex1Y)
			: pos(x, y, z), color(r, g, b), tex0(tex0X, tex0Y), tex1(tex1X,tex1Y) {}
		XMFLOAT3 pos;
		XMFLOAT3 color;
		XMFLOAT2 tex0;
		XMFLOAT2 tex1;
	};

	struct GVertex5
	{
		GVertex5() {}
		GVertex5(const XMFLOAT3& p, const XMFLOAT3& n, const XMFLOAT3& t, const XMFLOAT2& uv)
			: Position(p), Normal(n), TangentU(t), TexC(uv) {}
		GVertex5(
			float px, float py, float pz,
			float nx, float ny, float nz,
			float tx, float ty, float tz,
			float u, float v)
			: Position(px, py, pz), Normal(nx, ny, nz),
			TangentU(tx, ty, tz), TexC(u, v) {}

		XMFLOAT3 Position;
		XMFLOAT3 Normal;
		XMFLOAT3 TangentU;
		XMFLOAT2 TexC;
	};


	struct Vertex
	{
		Vertex() = default;
		Vertex(float x, float y, float z) : Pos(x, y, z) {}
		Vertex(float x, float y, float z,
			float cr, float cg, float cb, float ca)
			: Pos(x, y, z), Color(cr, cg, cb, ca) {}
		XMFLOAT3 Pos;
		XMFLOAT4 Color;
	};

	struct VertexTextured
	{
		VertexTextured() = default;
		VertexTextured(float x,float y,float z, float u,float v) : Pos(x,y,z), tex0(u,v)
		{}
		XMFLOAT3 Pos;
		XMFLOAT2 tex0;
	};
}