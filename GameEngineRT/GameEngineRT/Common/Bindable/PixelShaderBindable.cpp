#include "pch.h"
#include "PixelShaderBindable.h"
#include "Common/DeviceResources.h"


namespace GNF::Common::Bindable
{
	PixelShaderBindable::PixelShaderBindable(Common::CommonPixelShaders vertexShaderType, DeviceResources* resources)
	{
		std::pair<const void*, UINT> vertexShaderInfos = ShaderManager::PixelShaders[vertexShaderType];
		m_pShader = vertexShaderInfos.first;
		m_shaderSize = vertexShaderInfos.second;
		HRESULT rs = resources->GetD3DDevice()->CreatePixelShader(m_pShader, m_shaderSize, nullptr, m_pixelShader.put());

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
		return m_pixelShader.get();
	}

	void PixelShaderBindable::Bind(DeviceResources* resources)
	{
		resources->GetD3DContext()->PSSetShader(m_pixelShader.get(), nullptr, 0);
		Binded(*this);
	}


}