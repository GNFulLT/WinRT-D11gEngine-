#include "pch.h"
#include "VertexBufferBindable.h"
#include "Common/DeviceResources.h"

namespace GNF::Common::Bindable
{
	VertexBufferBindable::VertexBufferBindable(ID3D11Device* device,const VERTEX* vertices,UINT verticesSize)
	{
		D3D11_BUFFER_DESC bd;
		bd.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = verticesSize;
		bd.StructureByteStride = sizeof(VERTEX);
		D3D11_SUBRESOURCE_DATA srd = { vertices, 0, 0 };

		HRESULT rs = device->CreateBuffer(&bd, &srd, m_vertexBuffer.put());

	}
	void VertexBufferBindable::Bind(DeviceResources* resources)
	{
		ID3D11Buffer* buffers[] = { m_vertexBuffer.get() };
		UINT stride = sizeof(VERTEX);
		UINT offset = 0;
		resources->GetD3DContext()->IASetVertexBuffers(0u, 1u, buffers, &stride, &offset);
	}

}