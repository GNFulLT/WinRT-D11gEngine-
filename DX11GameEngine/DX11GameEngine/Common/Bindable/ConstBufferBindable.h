#pragma once

#include "IBindable.h"
#include "Common/Device/IDeviceResources.h"
#include <wrl/client.h>

using namespace Microsoft::WRL;

namespace GNF::Common::Bindable
{
	class ConstBufferBindable : public IBindable
	{
		public:
			ConstBufferBindable(Device::IDeviceResources* resources,const void* data,UINT byteWidth);
			~ConstBufferBindable()
			{
				int a = 5;
			}
			void Update(const void* data);
			virtual void Bind() override;
		private:
			Device::IDeviceResources* pParentDevice;
			ComPtr<ID3D11Buffer> cbPerObjectBuffer;

	};
}
