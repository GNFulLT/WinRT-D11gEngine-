#include "DeviceFactory.h"

#include <exception>
#include "DeviceResources.h"
#include "DebugDeviceResources.h"
#define THROW_EX() 	throw std::exception("Cannot Create Device Resources");

GNF::Common::Device::IDeviceResources* DeviceFactory::BuildDevice()
{
	GNF::Common::Device::IDeviceResources* dev;

	switch (selectedDeviceType)
	{
		case VULKAN_1_2_DEBUG_DEVICE:
			if (build_vulkan_debug_device(&dev))
				return dev;
			THROW_EX();
		case D3D11_HARDWARE_OR_WARP_DEBUG_DEVICE:
			if (build_dx11_hardware_debug_device(&dev))
				return dev;
			if (build_dx11_warp_debug_device(&dev))
				return dev;
			THROW_EX();
		case D3D11_HARDWARE_PERFORMANCE_DEVICE:
			if (build_dx11_performance_device(&dev))
				return dev;
			THROW_EX();
		case OPENGL_NONDEBUG_DEVICE:
			//X TODO : CREATE OPENGL DEVICE RESOURCES
			THROW_EX();
	}		
	
}




inline bool build_vulkan_debug_device(GNF::Common::Device::IDeviceResources** res)
{

}

inline bool build_dx11_warp_debug_device(GNF::Common::Device::IDeviceResources** res)
{

}

inline bool build_dx11_performance_device(GNF::Common::Device::IDeviceResources** res)
{

}



inline bool build_dx11_hardware_debug_device(GNF::Common::Device::IDeviceResources** res)
{

	//!
	Device::IDeviceResources* dev = new Device::DebugDeviceResources(new Device::DeviceResources(nullptr));



}