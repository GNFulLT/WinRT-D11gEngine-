#include "Graphic/DepthTestRenderer.h"
#include <wrl/client.h>
#include <Common/VertexTypes.h>



using namespace Microsoft::WRL;
using namespace GNF::Common;
namespace GNF::Graphic
{
	DepthTestRenderer::DepthTestRenderer(Common::Device::IDeviceResources* res) : pParentDevice(res)
	{}

	void DepthTestRenderer::Render()
	{
		auto m_d3d_device = pParentDevice->GetD3DDevice();
		auto m_d3d_deviceContext = pParentDevice->GetD3DContext();


		ComPtr<ID3D11Buffer> squareVertBuffer;
		int sizeOfV = 8;
		Vertex v[] =
		{
			Vertex(-0.3f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f), // Bottom left
			Vertex(-0.3f,  0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f), // Top left
			Vertex(0.7f,  0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f), // Top Right
			Vertex(0.7f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f), // Bottom Right


			Vertex(-0.7f, -0.5f, 0.7f, 0.0f, 0.0f, 0.0f, 1.0f), // Bottom left
			Vertex(-0.7f,  0.5f, 0.7f, 0.0f, 0.0f, 0.0f, 1.0f), // Top left
			Vertex(0.3f,  0.5f, 0.7f, 0.0f, 0.0f, 0.0f, 1.0f), // Top Right
			Vertex(0.3f, -0.5f, 0.7f, 0.0f, 0.0f, 0.0f, 1.0f), // Bottom Right
		};

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(Vertex) * sizeOfV;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA vertexBufferData;

		ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
		vertexBufferData.pSysMem = v;

		m_d3d_device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, squareVertBuffer.GetAddressOf());

		int sizeOfInd = 12;
		DWORD indices[] = {
						0, 1, 2,
						0, 2, 3,

						4,5,6,
						4,6,7
		};

		ComPtr<ID3D11Buffer> squareIndexBuffer;

		D3D11_BUFFER_DESC indexBufferDesc;
		ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(DWORD) * sizeOfInd;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = indices;
		m_d3d_device->CreateBuffer(&indexBufferDesc, &iinitData, squareIndexBuffer.GetAddressOf());

		ComPtr<ID3D11InputLayout> vertLayout;

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = ARRAYSIZE(layout);


		auto hr = pParentDevice->CreateInputLayout(layout, numElements,vertLayout.GetAddressOf());

		//Set the vertex buffer
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		m_d3d_deviceContext->IASetVertexBuffers(0, 1, squareVertBuffer.GetAddressOf(), &stride, &offset);

		m_d3d_deviceContext->IASetIndexBuffer(squareIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		//Set the Input Layout
		m_d3d_deviceContext->IASetInputLayout(vertLayout.Get());

		//Set Primitive Topology
		m_d3d_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		m_d3d_deviceContext->DrawIndexed(sizeOfInd, 0, 0);
	}
}