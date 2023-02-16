#pragma once

#include "../IBindable.h"

namespace GNF::Core::Bindable::Shader
{
	class PixelShaderBindable : public Bindable::IBindable
	{
	public:
		PixelShaderBindable(const void* byteCode,size_t size);
		PixelShaderBindable(const PixelShaderBindable&) = delete;
		PixelShaderBindable& operator=(PixelShaderBindable&) = delete;
		virtual void Bind() override;
		virtual void Bind(ID3D11DeviceContext3* ctx) override;
	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_shader;
	};
}