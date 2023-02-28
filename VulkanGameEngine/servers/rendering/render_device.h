#ifndef RENDER_DEVICE_H
#define RENDER_DEVICE_H

#include <vector>
#include <unordered_map>

#include "../../core/object/object.h"
#include "../../graphic/graphic_api.h"
#include "../../graphic/physical_device.h"

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
		EnabledProps enabledProps;
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
	bool m_instanceLoaded = false;
	// These are loading methods for upper structs
private:
	bool init_vk_instance();
private:
	friend class CreationServer;
	
	static RenderDevice* create_singleton();
	
	_INLINE_ static RenderDevice* singleton;
};

#endif // RENDER_DEVICE_H