#include "VertexShaderBindable.h"

namespace GNF::Common::Bindable
{
	VertexShaderBindable::VertexShaderBindable(const std::pair<const void*, UINT>& vertexShaderInfos, ShaderManagement::CommonVertexShaders stype, Device::IDeviceResources* resources)
	{
		pParentDevice = resources;
		m_pShader = vertexShaderInfos.first;
		m_shaderSize = vertexShaderInfos.second;
		auto dev = pParentDevice->GetD3DDevice();
		HRESULT rs = pParentDevice->GetD3DDevice()->CreateVertexShader(m_pShader, m_shaderSize, nullptr, m_vertexShader.GetAddressOf());
		m_vertexShaderType = stype;
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
		return m_vertexShader.Get();
	}
	
	void VertexShaderBindable::Bind()
	{

		pParentDevice->GetD3DContext()->VSSetShader(m_vertexShader.Get(), nullptr, 0);
		Binded(*this);
	}

	
}