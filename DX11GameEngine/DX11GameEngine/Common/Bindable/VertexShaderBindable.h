#pragma once*
#include "IBindable.h"
#include "IShaderBindable.h"
#include <d3d11_3.h>
#include <d2d1_3.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <DirectXMath.h>
#include "Common/Event/Event.h"
#include <wrl/client.h>
#include "Common/Device/IDeviceResources.h"
#include "Common/ShaderManagement/ShaderTypes.h"

using namespace Microsoft::WRL;

namespace GNF::Common::Bindable
{
	class VertexShaderBindable : public IShaderBindable 
	{
		public:
			VertexShaderBindable(const std::pair<const void*, UINT>&,ShaderManagement::CommonVertexShaders,Device::IDeviceResources*);
			VertexShaderBindable(const VertexShaderBindable&) = delete;


			~VertexShaderBindable() = default;
			virtual const void* GetShader() const;
			virtual UINT GetShaderSize() const;
			virtual void* GetD3DShader() const;
			virtual void Bind();


			Event::Event<VertexShaderBindable> Binded;

		private:
			ComPtr<ID3D11VertexShader> m_vertexShader;
			Device::IDeviceResources* pParentDevice;
			ShaderManagement::CommonVertexShaders m_vertexShaderType;
			const void* m_pShader;
			UINT m_shaderSize;
	};
}