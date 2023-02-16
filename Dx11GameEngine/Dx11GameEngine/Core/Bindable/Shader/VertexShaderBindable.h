#pragma once

#include "../IBindable.h"
#include <wrl/client.h>

namespace GNF::Core::Bindable::Shader
{
	class VertexShaderBindable : public Bindable::IBindable
	{
	public:
		VertexShaderBindable(const void* byteCode,size_t size);
		VertexShaderBindable(const VertexShaderBindable&) = delete;
		VertexShaderBindable& operator=(VertexShaderBindable&) = delete;
		virtual void Bind() override;
		virtual void Bind(ID3D11DeviceContext3* ctx) override;
	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_shader;
	};
}