#include "TextureManager.h"

#include "Common/HRHelper.h"
#include <filesystem>
namespace fs = std::filesystem;

namespace GNF::Texture
{

	TextureManager::TextureManager(Common::Device::IDeviceResources* res)
	{
		pResources = res;
		m_createdTextureCount = 0;
		m_effectFactory.reset(new DirectX::EffectFactory(res->GetD3DDevice()));

		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	
		Common::GThrowIfFailed(res->GetD3DDevice()->CreateSamplerState(&samplerDesc, m_samplerState.GetAddressOf()));
		
	
	}

	UINT TextureManager::CreateTexture(const WCHAR* path)
	{
		
		m_textures.emplace(m_createdTextureCount, new Common::Bindable::TextureBindable(pResources, path, m_effectFactory.get()));
		m_createdTextureCount++;

		return m_createdTextureCount - 1;
	}

	void TextureManager::BindTexture(UINT textureID)
	{
		auto texture = m_textures.find(textureID);
		if (texture  == m_textures.end())
		{
			throw std::exception("There is no texture with that ID");
		}

		ID3D11SamplerState* samps[] = { m_samplerState.Get() };
		pResources->GetD3DContext()->PSSetSamplers(0, 1, samps);
		texture->second.get()->Bind();
	}
}