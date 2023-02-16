#pragma once

#include "Core/Bindable/IBindable.h"
#include "DrawMode.h"
namespace GNF::Core::Bindable::Buffer
{
	class VertexBufferBindable : public Bindable::IBindable
	{
	public:
		VertexBufferBindable(DRAW_MODE mode,const void* data, UINT sizeOfData, UINT perVertexStride, ID3D11InputLayout* iptLayout,size_t index);
		VertexBufferBindable(const VertexBufferBindable&) = delete;
		VertexBufferBindable& operator=(VertexBufferBindable&) = delete;
		
		virtual void Bind() override;
		void Bind(ID3D11DeviceContext3* ctx) override;

	private:
		inline static UINT g_currentInputLayoutIndex = 0;

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
		size_t m_inputLayoutIndex;
		UINT m_stride;
	};
}