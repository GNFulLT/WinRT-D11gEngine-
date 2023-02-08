#include "pch.h"
#include "VertexShaderBindable.h"
#include "Core/GraphicEngine/GraphicEngine.h"

namespace GNF::Core::Bindable::Shader
{

	VertexShaderBindable::VertexShaderBindable(const void* byteCode, size_t size)
	{
		GraphicEngine::GraphicEngine::GetInstance()->GetD3DDevice()->CreateVertexShader(byteCode, size, nullptr, m_shader.GetAddressOf());
	}

	void VertexShaderBindable::Bind()
	{
		GraphicEngine::GraphicEngine::GetInstance()->GetD3DContext()->VSSetShader(m_shader.Get(), nullptr, 0);
	}

}

