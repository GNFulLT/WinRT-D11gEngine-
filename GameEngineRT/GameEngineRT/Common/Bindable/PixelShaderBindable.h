#pragma once

#include "IBindable.h"
#include "IShaderBindable.h"
#include "Common/IShaderManager.h"

#include <d3d11_3.h>
#include <d2d1_3.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <DirectXMath.h>

namespace GNF::Common::Bindable
{
	class PixelShaderBindable : public IShaderBindable
	{
	public:
		PixelShaderBindable(Common::CommonPixelShaders, DeviceResources*);
		PixelShaderBindable(const PixelShaderBindable&) = delete;

		~PixelShaderBindable() = default;
		virtual const void* GetShader() const;
		virtual UINT GetShaderSize() const;
		virtual void* GetD3DShader() const;
		virtual void Bind(DeviceResources* resources);
	private:
		winrt::com_ptr<ID3D11PixelShader> m_pixelShader;
		const void* m_pShader;
		UINT m_shaderSize;
	};
}