#pragma once
#include <wrl/client.h>
#include "Common/Device/IDeviceResources.h"
using namespace Microsoft::WRL;
namespace GNF::Graphic
{
	class IRenderer
	{
		public:
			virtual ~IRenderer() {};
		
			virtual void Render() = 0;

			virtual Common::Device::IDeviceResources* GetResources() = 0;
	};
}