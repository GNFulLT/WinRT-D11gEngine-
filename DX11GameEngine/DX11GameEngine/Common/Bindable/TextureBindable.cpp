#include "TextureBindable.h"



using namespace DirectX;

namespace GNF::Common::Bindable
{
	TextureBindable::TextureBindable(Device::IDeviceResources* resources, const WCHAR* filePath, DirectX::EffectFactory* factory)
	{
		pResources = resources;
		factory->CreateTexture(filePath, resources->GetD3DContext(), m_texture.GetAddressOf());
	}

	void TextureBindable::Bind()
	{
		ID3D11ShaderResourceView* views[] = { m_texture.Get() };
		pResources->GetD3DContext()->PSSetShaderResources(0, 1, views);
	}

	

}