#include "pch.h"
#include "VertexBufferBindable.h"
#include "Core/GraphicEngine/GraphicEngine.h"

namespace GNF::Core::Bindable::Buffer
{

	VertexBufferBindable::VertexBufferBindable(DRAW_MODE mode,const void* initData, UINT sizeOfData,UINT perVertexStride, ID3D11InputLayout* iptLayout, size_t index) :m_inputLayout(iptLayout), m_inputLayoutIndex(index)
	{
		D3D11_BUFFER_DESC bfd;
		ZeroMemory(&bfd, sizeof(D3D11_BUFFER_DESC));

		bfd.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		bfd.MiscFlags = 0u;
		bfd.ByteWidth = sizeOfData;
		bfd.StructureByteStride = perVertexStride;
		m_stride = perVertexStride;
		switch (mode)
		{
		case GNF::Core::Bindable::Buffer::STATIC_DRAW:
			bfd.Usage = D3D11_USAGE_IMMUTABLE;
			bfd.CPUAccessFlags = 0u;

			break;
		case GNF::Core::Bindable::Buffer::DYNAMIC_DRAW:
			bfd.Usage = D3D11_USAGE_DYNAMIC;
			bfd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			break;
		case STATIC_DYNAMIC_DRAW:
			bfd.Usage = D3D11_USAGE_DEFAULT;
			bfd.CPUAccessFlags = 0u;
			break;
		default:
			break;
		}
		if (initData == nullptr)
		{
			GraphicEngine::GraphicEngine::GetInstance()->GetD3DDevice()->CreateBuffer(&bfd,nullptr,m_buffer.GetAddressOf());

		}
		else
		{
			D3D11_SUBRESOURCE_DATA srd{ initData,0,0 };
			GraphicEngine::GraphicEngine::GetInstance()->GetD3DDevice()->CreateBuffer(&bfd,&srd,m_buffer.GetAddressOf());
		}
	}

	void VertexBufferBindable::Bind() 
	{
		ID3D11Buffer* buffers[] = { m_buffer.Get() };
		UINT strides[] = { m_stride };
		UINT offsets[] = { 0 };
		if (m_inputLayoutIndex != g_currentInputLayoutIndex)
		{
			g_currentInputLayoutIndex = m_inputLayoutIndex;
			GraphicEngine::GraphicEngine::GetInstance()->GetD3DContext()->IASetInputLayout(m_inputLayout.Get());
		}
		GraphicEngine::GraphicEngine::GetInstance()->GetD3DContext()->IASetVertexBuffers(0u, ARRAYSIZE(buffers), buffers, strides, offsets);
		
	}

}