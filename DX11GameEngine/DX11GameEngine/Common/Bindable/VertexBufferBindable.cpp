#include "VertexBufferBindable.h"
#include "Common/Device/IDeviceResources.h"
namespace GNF::Common::Bindable
{
	VertexBufferBindable::VertexBufferBindable(Device::IDeviceResources* resources, const Common::VertexTextured* vertices, UINT byteWidth)
	{
		vertexStride = sizeof(Common::VertexTextured);
		D3D11_BUFFER_DESC bd;
		bd.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = byteWidth;
		bd.StructureByteStride = vertexStride;
		D3D11_SUBRESOURCE_DATA srd = { vertices, 0, 0 };

		auto device = resources->GetD3DDevice();
		HRESULT rs = device->CreateBuffer(&bd, &srd, m_vertexBuffer.GetAddressOf());

		pParentDevice = resources;
	}
	VertexBufferBindable::VertexBufferBindable(Device::IDeviceResources* resources, const Common::GVertex1& vertices, UINT verticesSize)
	{
		vertexStride = sizeof(Common::GVertex1);
		D3D11_BUFFER_DESC bd;
		bd.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = verticesSize;
		bd.StructureByteStride = vertexStride;
		D3D11_SUBRESOURCE_DATA srd = { &vertices, 0, 0 };

		auto device = resources->GetD3DDevice();
		HRESULT rs = device->CreateBuffer(&bd, &srd, m_vertexBuffer.GetAddressOf());

		pParentDevice = resources;
	}
	VertexBufferBindable::VertexBufferBindable(Device::IDeviceResources* resources, const Common::Vertex* vertices, UINT verticesSize)
	{
		vertexStride = sizeof(Common::Vertex);
		D3D11_BUFFER_DESC bd;
		bd.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = verticesSize;
		bd.StructureByteStride = vertexStride;
		D3D11_SUBRESOURCE_DATA srd = { vertices, 0, 0 };

		auto device = resources->GetD3DDevice();
		HRESULT rs = device->CreateBuffer(&bd, &srd, m_vertexBuffer.GetAddressOf());

		pParentDevice = resources;

	}
	void VertexBufferBindable::Bind()
	{
		ID3D11Buffer* buffers[] = { m_vertexBuffer.Get() };
		UINT stride = vertexStride;
		UINT offset = 0;
		pParentDevice->GetD3DContext()->IASetVertexBuffers(0u, 1u, buffers, &stride, &offset);
	}

}