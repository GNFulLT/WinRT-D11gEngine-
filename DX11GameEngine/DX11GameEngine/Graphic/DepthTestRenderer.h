#pragma once
#include "IRenderer.h"
#include "Common/Device/IDeviceResources.h"

namespace GNF::Graphic
{

	
	class DepthTestRenderer : public IRenderer
	{
		public:
			DepthTestRenderer(Common::Device::IDeviceResources* res);
			virtual Common::Device::IDeviceResources* GetResources() override
			{
				return pParentDevice;
			}
			virtual void Render() override;

		private:
			Common::Device::IDeviceResources* pParentDevice;

	};
}