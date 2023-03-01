#ifndef RENDER_DEVICE_H
#define RENDER_DEVICE_H

#include <vector>
#include <unordered_map>

#include "../../core/object/object.h"
#include "../../graphic/graphic_api.h"
#include "../../graphic/physical_device.h"
#include "../../graphic/vulkan/utils_vulkan.h"

#define IMPLICIT_EXTENSIONS_NAME "ENGINE_EXS"


#define VK_NO_PROTOTYPES

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "../../graphic/vulkan/volk.h"

class CreationServer;

class RenderDevice : public Object
{
	OBJECT_DEF(RenderDevice,Object)

public:

	struct EnabledProps final
	{
		std::vector<VkLayerProperties> enabledLayers;
		std::unordered_map<std::string, std::vector<VkExtensionProperties>> enabledExtensions;
	};

	struct VulkanInstance final
	{
		VkInstance instance = nullptr;
		VkSurfaceKHR surface = nullptr;
		VkDebugUtilsMessengerEXT messenger = nullptr;
		VkDebugReportCallbackEXT reportCallback = nullptr;
		VkPresentModeKHR presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
		VkSurfaceFormatKHR format;
		int surfaceImageCount;
		EnabledProps enabledProps;
	};

	struct VukanRenderDevice final
	{
		VkPhysicalDevice physicalDev;
		int mainQueueFamilyIndex;
	};

	~RenderDevice();

	bool init();

	void destroy();


	_INLINE_ static RenderDevice* get_singleton()
	{
		return singleton;
	}


private:
	VulkanInstance m_instance;
	// Initialize with save physical dev
	VukanRenderDevice m_renderDevice;
	SwapChainSupportDetails m_swapChainDetails;
	bool m_instanceLoaded = false;
	// These are loading methods for upper structs
private:
	bool init_vk_instance();
	bool init_vk_device();
	bool save_vk_physical_device();
private:
	friend class CreationServer;
	
	static RenderDevice* create_singleton();
	
	_INLINE_ static RenderDevice* singleton;
};

#endif // RENDER_DEVICE_H