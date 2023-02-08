#include "pch.h"

#include "IndexBufferBindable.h"
#include "Core/GraphicEngine/GraphicEngine.h"

namespace GNF::Core::Bindable::Buffer
{
	IndexBufferBindable::IndexBufferBindable(DRAW_MODE mode, const void* initialData, UINT sizeOfData,DXGI_FORMAT format) : m_format(format)
	{

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(UINT) * sizeOfData;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA sbData;
		sbData.pSysMem = initialData;
		
		switch (mode)
		{
		case STATIC_DYNAMIC_DRAW:
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.CPUAccessFlags = 0;
			break;
		default:
			Common::Logger::LogError("Unsupported draw mode for Index Buffer");
			throw std::runtime_error("Unsupported draw mode for Index Buffer");
			break;
		}
		GThrowIfFailed(GraphicEngine::GraphicEngine::GetInstance()->GetD3DDevice()->CreateBuffer(&desc, &sbData, m_buffer.GetAddressOf()));		
	}

	void IndexBufferBindable::Bind()
	{
		GraphicEngine::GraphicEngine::GetInstance()->GetD3DContext()->IASetIndexBuffer(m_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}	
}