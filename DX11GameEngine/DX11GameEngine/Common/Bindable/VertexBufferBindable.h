#pragma once
#include <Windows.h>
#include "IBindable.h"
#include "Common/Device/IDeviceResources.h"
#include "Common/VertexTypes.h"
#include <wrl/client.h>
#include <d3d11.h>

using namespace Microsoft::WRL;

namespace GNF::Common::Bindable
{
	class VertexBufferBindable : public IBindable
	{
		public:
			VertexBufferBindable(Device::IDeviceResources*, const Common::VertexTextured* vertices, UINT byteWidth);
			VertexBufferBindable(Device::IDeviceResources*,const Common::GVertex1& vertices,UINT byteWidth);
			VertexBufferBindable(Device::IDeviceResources*, const Common::Vertex* vertices, UINT byteWidth);
			VertexBufferBindable(VertexBufferBindable&) = delete;
			virtual void Bind();
			virtual ~VertexBufferBindable()
			{
				int a = 5;
			}
		private:
			Device::IDeviceResources* pParentDevice;
			ComPtr<ID3D11Buffer> m_vertexBuffer;
			UINT vertexStride;
	};
}