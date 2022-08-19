#include "pch.h"
#include "Triangle.h"

using namespace GNF::Common::Bindable;

namespace GNF::Primitives
{

	Triangle::Triangle(ID3D11Device* device,const void* byteShader,UINT const& shaderSize)
	{
		VERTEX vertices[] = {
			{0.0f,0.5f,0.0f},//1.
			{0.5f,-0.5f,0.0f },//2.
			{-0.5f,-0.5f ,0.f}//3.
		};
		D3D11_BUFFER_DESC bd;
		bd.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = sizeof(vertices);
		bd.StructureByteStride = sizeof(VERTEX);
		D3D11_SUBRESOURCE_DATA srd = { vertices, 0, 0 };

		HRESULT rs = device->CreateBuffer(&bd, &srd, m_vertexBuffer.put());

		const D3D11_INPUT_ELEMENT_DESC ied[] =
		{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		rs = device->CreateInputLayout(ied, 1, byteShader, shaderSize, m_inputLayout.put());

	}

	void Triangle::Draw(ID3D11DeviceContext* deviceContext)
	{

		const UINT stride = sizeof(VERTEX);
		const UINT offset = 0;

		ID3D11Buffer* buffers[] = {m_vertexBuffer.get()};

		deviceContext->IASetVertexBuffers(0, 1, buffers, &stride, &offset);

		deviceContext->IASetInputLayout(m_inputLayout.get());

		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		deviceContext->Draw(3u, 0u);
	}

	ID3D11InputLayout* Triangle::GetInputLayout()
	{
		return m_inputLayout.get();
	}
}