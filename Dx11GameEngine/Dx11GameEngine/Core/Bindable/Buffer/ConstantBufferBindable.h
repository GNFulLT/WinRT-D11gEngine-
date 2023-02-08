#pragma once

#include "Core/Bindable/IBindable.h"
#include "DrawMode.h"
namespace GNF::Core::Bindable::Buffer
{
	class ConstantBufferBindable : public Bindable::IBindable
	{
	public:
		ConstantBufferBindable(DRAW_MODE mode, const void* data, UINT sizeOfData);
		ConstantBufferBindable(const ConstantBufferBindable&) = delete;
		ConstantBufferBindable& operator=(ConstantBufferBindable&) = delete;

		virtual void Bind() override;
		void Update(const void* data);
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	};
}