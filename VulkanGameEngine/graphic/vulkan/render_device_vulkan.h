#ifndef RENDER_DEVICE_VULKAN_H
#define RENDER_DEVICE_VULKAN_H

#include <memory>
#include <vector>

#define VK_NO_PROTOTYPES

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "volk.h"

#include "../../servers/rendering/render_device.h"
#include "physical_device_vulkan.h"

class RenderDeviceVulkan : public RenderDevice
{
	OBJECT_DEF(RenderDeviceVulkan,RenderDevice)

public:
	RenderDeviceVulkan();
	~RenderDeviceVulkan();
	bool init() override final;
	PhysicalDevice* get_selected_physical_device() const noexcept override final;
	GRAPHIC_API get_graphic_api() const noexcept override final;
private:

	// PRIVATE METHODS
private:
	bool try_to_create_instance(VkApplicationInfo* appInfo, VkInstance* inst, std::vector<VkLayerProperties>* instance_layer_props, bool debugIsEnabled = false);
	bool try_to_select_physical_device(
		const std::vector<VkPhysicalDevice>& physicalDevs,
		const std::vector<VkPhysicalDeviceProperties>& physicalDevProperties,
		const std::vector<VkPhysicalDeviceFeatures>& physicalDevFeatures,
		const std::vector<uint32_t>& surfaceSupportedQueueIndexes,
		PhysicalDevice::PHYSICAL_DEVICE_TYPE deviceType, VkPhysicalDevice& dev,
		VkPhysicalDeviceProperties& prop, VkPhysicalDeviceFeatures& feature, uint32_t& index, PhysicalDevice::PHYSICAL_DEVICE_TYPE& selectedType);	// Info about device
private:
	std::unique_ptr<PhysicalDeviceVulkan> m_physicalDevice;
	bool isInstanceInitedSuccessfully = false;
	bool isDeviceInitedSuccessfully = false;
	bool isDebugMessengerCreated = false;
	// VK INFOS
private:
	uint32_t m_khrSurface;
	VkInstance m_instance;
	VkDevice m_device;
	VkSurfaceKHR m_surface;
	VkDebugUtilsMessengerEXT g_debugMessenger;
	VkDebugReportCallbackEXT g_debugReporter;
	std::vector<VkLayerProperties> m_enabledDeviceLayerProps;
	std::unordered_map<std::string, std::vector<VkExtensionProperties>> m_enabledDeviceExtensions;
	std::vector<VkLayerProperties> m_enabledInstanceLayerProps;
	std::vector<VkExtensionProperties> m_enabledInstanceExtensionProps;
};

#endif // RENDER_DEVICE_VULKAN_H