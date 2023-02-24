#ifndef RENDER_DEVICE_VULKAN_H
#define RENDER_DEVICE_VULKAN_H

#define VK_NO_PROTOTYPES

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "volk.h"

#include "../../servers/rendering/render_device.h"


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
	
	// Info about device
private:
	bool isInitedSuccessfully = false;
	// VK INFOS
private:
	VkInstance m_instance;
	std::vector<VkLayerProperties> m_enabledInstanceLayerProps;
	std::vector<VkExtensionProperties> m_enabledInstanceExtensionProps;
};

#endif // RENDER_DEVICE_VULKAN_H