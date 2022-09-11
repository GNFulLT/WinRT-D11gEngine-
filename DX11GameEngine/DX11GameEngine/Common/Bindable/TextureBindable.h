#pragma once

#include "IBindable.h"
#include "Common/Device/IDeviceResources.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <Effects.h>

using namespace Microsoft::WRL;
namespace GNF::Common::Bindable
{
	class TextureBindable : public IBindable
	{
		public:
			TextureBindable(Device::IDeviceResources* resources, const WCHAR * filePath,DirectX::EffectFactory* factory );
			TextureBindable(TextureBindable&) = delete;
			virtual void Bind() override;
		private:
			ComPtr<ID3D11ShaderResourceView> m_texture;
			Device::IDeviceResources* pResources;
	};
}