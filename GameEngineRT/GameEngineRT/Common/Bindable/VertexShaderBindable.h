#pragma once
#include "IBindable.h"
#include "IShaderBindable.h"
#include "Common/IShaderManager.h"
#include <d3d11_3.h>
#include <d2d1_3.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <DirectXMath.h>
#include "Common/Event/Event.h"

namespace GNF::Common::Bindable
{
	class VertexShaderBindable : public IShaderBindable 
	{
		public:
			VertexShaderBindable(Common::CommonVertexShaders,DeviceResources*);
			VertexShaderBindable(const VertexShaderBindable&) = delete;


			~VertexShaderBindable() = default;
			virtual const void* GetShader() const;
			virtual UINT GetShaderSize() const;
			virtual void* GetD3DShader() const;
			virtual void Bind(DeviceResources* resources);


			Event::Event<VertexShaderBindable> Binded;

		private:
			winrt::com_ptr<ID3D11VertexShader> m_vertexShader;
			const void* m_pShader;
			UINT m_shaderSize;
	};
}