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

class ImGuiDraw;

class RenderDeviceVulkan : public RenderDevice
{
	OBJECT_DEF(RenderDeviceVulkan,RenderDevice)

public:
	RenderDeviceVulkan();
	~RenderDeviceVulkan();
	bool init() override final;
	PhysicalDevice* get_selected_physical_device() const noexcept override final;
	GRAPHIC_API get_graphic_api() const noexcept override final;
	void render_ui() override final;
	VkCommandBuffer get_cmd_buffer();
	void reset_cmd_pool();
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
	bool isSwapchainInitialized = false;
	// VK INFOS
private:
	VkFence fence;
	VkCommandBuffer buff;

	ImGuiDraw* drawGui;
	VkSwapchainKHR m_swapChain;
	VkSurfaceKHR m_surface;
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;

	VkPresentModeKHR m_selected_present_mode;
	VkSurfaceFormatKHR m_selected_surface_format;
	VkExtent2D m_selected_extend;
	VkRenderPass m_render_pass;
	std::vector<VkFramebuffer> m_frameBuffers;

	VkCommandPool m_commandPool;
	VkQueue m_surfaceQueue;
	// Waits to swap chain image awailable
	VkSemaphore m_semaphore;
	// Swapchain waits to process of rendering is finished
	VkSemaphore m_surfaceSemaphore;
	// Dev

	VkInstance m_instance;
	VkDevice m_device;
	VkDebugUtilsMessengerEXT g_debugMessenger;
	VkDebugReportCallbackEXT g_debugReporter;
	std::vector<VkLayerProperties> m_enabledDeviceLayerProps;
	std::unordered_map<std::string, std::vector<VkExtensionProperties>> m_enabledDeviceExtensions;
	std::vector<VkLayerProperties> m_enabledInstanceLayerProps;
	std::vector<VkExtensionProperties> m_enabledInstanceExtensionProps;

	friend class ImGuiDraw;
};

#endif // RENDER_DEVICE_VULKAN_H