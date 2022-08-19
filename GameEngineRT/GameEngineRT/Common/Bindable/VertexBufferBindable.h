#pragma once
#include "Common/DeviceResources.h"
#include "IBindable.h"

namespace GNF::Common::Bindable
{
	class VertexBufferBindable : public IBindable
	{
		public:
			VertexBufferBindable(ID3D11Device*,const VERTEX* vertices,UINT verticesSize);
			virtual void Bind(DeviceResources* resources);
			virtual ~VertexBufferBindable() = default;
		private:
			winrt::com_ptr<ID3D11Buffer> m_vertexBuffer;
	};
}