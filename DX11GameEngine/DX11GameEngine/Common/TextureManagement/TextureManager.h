#pragma once

#include "ITextureManager.h"
#include "Common/Device/IDeviceResources.h"
#include "Common/Bindable/TextureBindable.h"

#include <wrl/client.h>
#include <memory>
#include <map>

#include <Effects.h>
using namespace Microsoft::WRL;
namespace GNF::Texture
{
	class TextureManager : public ITextureManager
	{
		public:
			TextureManager(Common::Device::IDeviceResources* res);
			virtual ~TextureManager() {}
			virtual UINT CreateTexture(const WCHAR* path) override;
			virtual void BindTexture(UINT id) override;
		private:
			std::map<UINT,std::unique_ptr<Common::Bindable::TextureBindable>> m_textures;
			std::unique_ptr<DirectX::EffectFactory> m_effectFactory;
			int m_createdTextureCount;
			ComPtr<ID3D11SamplerState> m_samplerState;
			Common::Device::IDeviceResources* pResources;
			
	};
}