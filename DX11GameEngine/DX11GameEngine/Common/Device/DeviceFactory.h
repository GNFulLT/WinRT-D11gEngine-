#pragma once

#include "IDeviceResources.h"

enum DEVICE_TYPE
{
	D3D11_HARDWARE_PERFORMANCE_DEVICE,
	D3D11_HARDWARE_OR_WARP_PERFORMANCE_DEVICE,
	D3D11_HARDWARE_MINIMAL_DEBUG_DEVICE,
	D3D11_HARDWARE_OR_WARP_DEBUG_DEVICE,
	VULKAN_1_2_DEBUG_DEVICE,
	OPENGL_NONDEBUG_DEVICE
};


class DeviceFactory
{
	public:
		DeviceFactory(DEVICE_TYPE deviceType) : selectedDeviceType(deviceType)
		{}
		GNF::Common::Device::IDeviceResources* BuildDevice();
		
	private:
		DEVICE_TYPE selectedDeviceType;
};