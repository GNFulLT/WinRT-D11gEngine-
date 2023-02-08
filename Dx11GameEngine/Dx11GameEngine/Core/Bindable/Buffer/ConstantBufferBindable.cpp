#include "pch.h"

#include "ConstantBufferBindable.h"
#include "Core/GraphicEngine/GraphicEngine.h"
namespace GNF::Core::Bindable::Buffer
{
	ConstantBufferBindable::ConstantBufferBindable(DRAW_MODE mode, const void* data, UINT sizeOfData)
	{
		D3D11_BUFFER_DESC bfd;
		ZeroMemory(&bfd, sizeof(D3D11_BUFFER_DESC));

		bfd.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		bfd.MiscFlags = 0u;
		bfd.ByteWidth = sizeOfData;
		bfd.CPUAccessFlags = 0u;
		bfd.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA srd{ data,0,0 };
		GraphicEngine::GraphicEngine::GetInstance()->GetD3DDevice()->CreateBuffer(&bfd, nullptr, m_buffer.GetAddressOf());


	}
	void ConstantBufferBindable::Update(const void* data)
	{
		GraphicEngine::GraphicEngine::GetInstance()->GetD3DContext()->UpdateSubresource(m_buffer.Get(),0,NULL,data,0,0);
	}

	void ConstantBufferBindable::Bind() 
	{
		ID3D11Buffer* buffers[] = { m_buffer.Get() };

		GraphicEngine::GraphicEngine::GetInstance()->GetD3DContext()->VSSetConstantBuffers(0, 1, buffers);

	}


}