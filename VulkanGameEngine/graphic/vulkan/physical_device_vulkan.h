#ifndef PHYSICAL_DEVICE_VULKAN_H
#define PHYSICAL_DEVICE_VULKAN_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../physical_device.h"

class RenderDeviceVulkan;

class PhysicalDeviceVulkan : public PhysicalDevice
{
public:
	PhysicalDeviceVulkan(VkPhysicalDevice dev,VkPhysicalDeviceProperties prop,VkPhysicalDeviceFeatures features,PHYSICAL_DEVICE_TYPE type, const String& deviceName,uint_fast32_t apiVersion, uint_fast32_t driverVersion, uint_fast32_t vendorId) :
		PhysicalDevice(type,deviceName,apiVersion, driverVersion, vendorId)
	{

	}
	PhysicalDeviceVulkan(const PhysicalDeviceVulkan&) = delete;

private:
	friend class RenderDeviceVulkan;

	VkPhysicalDevice m_physicalDevice;
	VkPhysicalDeviceProperties m_physicalDeviceProps;
	VkPhysicalDeviceFeatures m_physicalDeviceFeatures;
};
	
#endif // PHYSICAL_DEVICE_VULKAN_H