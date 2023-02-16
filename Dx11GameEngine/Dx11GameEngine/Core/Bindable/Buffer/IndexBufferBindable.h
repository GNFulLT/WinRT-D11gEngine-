#pragma once

#include "Core/Bindable/IBindable.h"
#include "DrawMode.h"
namespace GNF::Core::Bindable::Buffer
{
	class IndexBufferBindable : public Bindable::IBindable
	{
	public:
		IndexBufferBindable(DRAW_MODE mode,const void* initialData,UINT sizeOfData,DXGI_FORMAT format = DXGI_FORMAT_R32_UINT);

		void Bind() override;
		void Bind(ID3D11DeviceContext3* ctx) override;
	private:
		DXGI_FORMAT m_format;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	};
}