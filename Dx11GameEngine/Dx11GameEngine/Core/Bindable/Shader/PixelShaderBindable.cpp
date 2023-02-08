#include "pch.h"
#include "PixelShaderBindable.h"
#include "Core/GraphicEngine/GraphicEngine.h"

namespace GNF::Core::Bindable::Shader
{
	PixelShaderBindable::PixelShaderBindable(const void* byteCode, size_t size)
	{
		GThrowIfFailed(GraphicEngine::GraphicEngine::GetInstance()->GetD3DDevice()->CreatePixelShader(byteCode, size,
			nullptr, m_shader.GetAddressOf()));
	}

	void PixelShaderBindable::Bind()
	{
		GraphicEngine::GraphicEngine::GetInstance()->GetD3DContext()->PSSetShader(m_shader.Get(),nullptr,0);
	}





}