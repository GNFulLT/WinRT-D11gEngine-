#include "pch.h"
#include "VertexShaderBindable.h"
#include "Common/DeviceResources.h"
#include "Common/Event/EventHandler.h"
namespace GNF::Common::Bindable
{
	VertexShaderBindable::VertexShaderBindable(Common::CommonVertexShaders vertexShaderType, DeviceResources* resources)
	{
		std::pair<const void*, UINT> vertexShaderInfos = ShaderManager::VertexShaders[vertexShaderType];
		m_pShader = vertexShaderInfos.first;
		m_shaderSize = vertexShaderInfos.second;
		HRESULT rs = resources->GetD3DDevice()->CreateVertexShader(m_pShader, m_shaderSize, nullptr, m_vertexShader.put());

		OnBind += Event::EventHandler::Bind<int, VertexShaderBindable>(&VertexShaderBindable::Velth, this);
	}


	void VertexShaderBindable::Velth(const int&)
	{
		int a = 5;
		throw std::exception("Çalýþýyo");
	}

	const void* VertexShaderBindable::GetShader() const
	{
		return m_pShader;
	}
	UINT VertexShaderBindable::GetShaderSize() const
	{
		return m_shaderSize;
	}
	void* VertexShaderBindable::GetD3DShader() const
	{
		return m_vertexShader.get();
	}
	
	void VertexShaderBindable::Bind(DeviceResources* resources)
	{
		int a = 5;
		OnBind(a);
		resources->GetD3DContext()->VSSetShader(m_vertexShader.get(), nullptr, 0);
	}

	
}