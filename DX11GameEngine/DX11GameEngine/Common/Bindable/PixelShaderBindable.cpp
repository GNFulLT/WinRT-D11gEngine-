#include "PixelShaderBindable.h"


namespace GNF::Common::Bindable
{
	PixelShaderBindable::PixelShaderBindable( const std::pair<const void*, UINT>& vertexShaderInfos,ShaderManagement::CommonPixelShaders vertexShaderType, Device::IDeviceResources* resources)
	{
		pParentDevice = resources;
		m_pShader = vertexShaderInfos.first;
		m_shaderSize = vertexShaderInfos.second;
		HRESULT rs = resources->GetD3DDevice()->CreatePixelShader(m_pShader, m_shaderSize, nullptr, m_pixelShader.GetAddressOf());
		shaderType = vertexShaderType;
	}
	const void* PixelShaderBindable::GetShader() const
	{
		return m_pShader;
	}
	UINT PixelShaderBindable::GetShaderSize() const
	{
		return m_shaderSize;
	}
	void* PixelShaderBindable::GetD3DShader() const
	{
		return m_pixelShader.Get();
	}

	void PixelShaderBindable::Bind()
	{
		pParentDevice->GetD3DContext()->PSSetShader(m_pixelShader.Get(), nullptr, 0);
		Binded(*this);
	}


}