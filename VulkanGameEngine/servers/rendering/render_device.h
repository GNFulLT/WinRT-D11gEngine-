#ifndef RENDER_DEVICE_H
#define RENDER_DEVICE_H

#include <taskflow/taskflow.hpp>

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
		VkExtent2D surfaceExtent;
		EnabledProps enabledProps;
	};

	struct VulkanPhysicalDevice final
	{
		VkPhysicalDevice physicalDev;
		VkPhysicalDeviceFeatures physicalDevFeatures;
		VkPhysicalDeviceProperties physicalDevProperties;
	};

	struct VulkanSwapChain final
	{
		VkSwapchainKHR swapchain;

		//X This is images created with swapchain
		std::vector<VkImage> swapchainImages;

		//X This is image views for swapchainImages to able writing into image
		std::vector<VkImageView> swapchainImageViews;

		//X This is to make able imageview be targeted as a frame buffer by presentation
		std::vector<VkFramebuffer> frameBuffers;

		VkRenderPass renderPass;
	};

	struct VukanRenderDevice final
	{
		VulkanPhysicalDevice physicalDev;
		
		
		int mainQueueFamilyIndex;
		int mainQueueIndex;
		VkQueue mainQueue;

		int presentQueueFamilyIndex;
		int presentQueueIndex;
		VkQueue presentQueue;


		VkDevice logicalDevice;
		EnabledProps enabledProps;

		//! CMD

		std::vector<VkCommandPool> mainQueueCommandPools;

		VkCommandBuffer pMainCommandBuffer;

		std::unordered_map<unsigned int,std::vector<VkCommandBuffer>> commandBuffers;


		VkCommandPool presentCommandPool;
		VkCommandBuffer presentCommandBuffer;

		//! Sync
		VkSemaphore renderCompleteSemaphore;
		VkSemaphore imageAcquiredSemaphore;


		VkFence	mainQueueFinishedFence;
		VkFence presentQueueFinishedFence;
	};
	
	~RenderDevice();

	bool init();

	void destroy();


	_INLINE_ static RenderDevice* get_singleton()
	{
		return singleton;
	}
	void render_ui(tf::Subflow& subflow);
	
	bool render_things();
private:
	VulkanInstance m_instance;
	// Initialize with save physical dev
	VukanRenderDevice m_renderDevice;
	VulkanSwapChain m_swapchain;
	SwapChainSupportDetails m_swapChainDetails;
	bool m_instanceLoaded = false;
	bool m_isDebugEnabled = false;
	// These are loading methods for upper structs
private:
	bool init_vk_instance();
	bool init_vk_device();
	bool save_vk_physical_device();
	bool init_vk_logical_device();
	void expose_queues();
	bool create_command_pools();
	bool init_vk_swapchain();
	bool init_vk_syncs();
	bool init_command_buffers();
private:
	friend class CreationServer;
	
	tf::Taskflow taskFlow;

	static RenderDevice* create_singleton();
	
	_INLINE_ static RenderDevice* singleton;
};

#endif // RENDER_DEVICE_H