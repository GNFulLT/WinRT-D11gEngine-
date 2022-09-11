#pragma once

#include "IBindable.h"
#include "IShaderBindable.h"
#include "Common/ShaderManagement/ShaderTypes.h"
#include "Common/Device/IDeviceResources.h"

#include <d3d11_3.h>
#include <d2d1_3.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <DirectXMath.h>

#include "Common/Event/Event.h"
#include <wrl/client.h>

using namespace Microsoft::WRL;

namespace GNF::Common::Bindable
{
	class PixelShaderBindable : public IShaderBindable
	{
	public:
		PixelShaderBindable(const std::pair<const void*, UINT>& ,ShaderManagement::CommonPixelShaders, Device::IDeviceResources*);
		PixelShaderBindable(const PixelShaderBindable&) = delete;

		~PixelShaderBindable() override
		{
			int a = 5;
		}
		virtual const void* GetShader() const;
		virtual UINT GetShaderSize() const;
		virtual void* GetD3DShader() const;
		virtual void Bind();

		Event::Event<PixelShaderBindable> Binded;

	private:
		ShaderManagement::CommonPixelShaders shaderType;
		Device::IDeviceResources* pParentDevice;
		ComPtr<ID3D11PixelShader> m_pixelShader;
		const void* m_pShader;
		UINT m_shaderSize;
	};
}