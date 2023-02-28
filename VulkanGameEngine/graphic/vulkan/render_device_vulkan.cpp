#ifdef _WIN32
	#define VK_USE_PLATFORM_WIN32_KHR
#elif __linux__
	#define VK_USE_PLATFORM_XLIB_KHR
#elif __APPLE__
	#define VK_USE_PLATFORM_METAL_EXT
#endif


#include "../../imgui/imgui.h"
#include "render_device_vulkan.h"
#include "../../servers/logger_server.h"
#include "../../servers/configuration_server.h"
#include "../../servers/window_server.h"
#include "../../core/version.h"
#include "../../platform/GLFW/window_server_glfw.h"
#include "imgui_draw.h"
struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};


#include <boost/format.hpp>
#include <vector>
#include <unordered_map>
static VkBool32 VKAPI_CALL vk_debug_messenger_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
	(void)pUserData; // Unused argument

	std::string error = std::string("[VULKAN] [MESSENGER] Debug report from ObjectType: ") + std::to_string(uint32_t(pCallbackData->pObjects->objectType)) + "\n\nMessage: " + pCallbackData->pMessage + "\n\n";

	LoggerServer::get_singleton()->log_cout(RenderDevice::get_singleton(), error.c_str(), Logger::DEBUG);
	return VK_FALSE;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugReportCallback
(
	VkDebugReportFlagsEXT      flags,
	VkDebugReportObjectTypeEXT objectType,
	uint64_t                   object,
	size_t                     location,
	int32_t                    messageCode,
	const char* pLayerPrefix,
	const char* pMessage,
	void* UserData
)
{
	// https://github.com/zeux/niagara/blob/master/src/device.cpp   [ignoring performance warnings]
	// This silences warnings like "For optimal performance image layout should be VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL instead of GENERAL."
	if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
		return VK_FALSE;

	LoggerServer::get_singleton()->log_cout(RenderDevice::get_singleton(), boost::str(boost::format("[VULKAN] [REPORTER] [%1%] %2%") % pLayerPrefix % pMessage).c_str(),
		Logger::DEBUG);

	return VK_FALSE;
}


// Prototypes

bool create_debug_messenger(VkInstance instance, VkDebugUtilsMessengerEXT* pDebugMessenger,VkDebugReportCallbackEXT* pDebugReporter);

_INLINE_ bool supports_validation_layer(std::vector<VkLayerProperties>* layers, int& index);
_INLINE_ std::vector<VkLayerProperties> get_supported_instance_layers();
_INLINE_ std::vector<VkExtensionProperties> get_supported_instance_exs(const char* forLayer);
_INLINE_ std::unordered_map<std::string,std::vector<VkExtensionProperties>> get_all_supported_instance_exs(std::vector<const char*>* enabledLyers);
_INLINE_ bool get_all_physicalDevicesPropsFutures(const VkInstance* inst,std::vector<VkPhysicalDevice>& physicalDevs,
	std::vector<VkPhysicalDeviceProperties>& physicalDevProps, std::vector<VkPhysicalDeviceFeatures>& physicalDevFeatures);
_INLINE_ PhysicalDevice::PHYSICAL_DEVICE_TYPE vk_to_physical_device_type(VkPhysicalDeviceType type);
_INLINE_ std::vector<VkQueueFamilyProperties> get_queue_family_properties_from_device(VkPhysicalDevice dev);
_INLINE_ std::unordered_map<uint32_t, VkQueueFamilyProperties> find_queue_families(std::vector<VkQueueFamilyProperties>* queueFamilies,VkQueueFlags desiredFlags);
_INLINE_ bool get_supported_device_layers(const VkPhysicalDevice* dev, std::vector<VkLayerProperties>& props);
_INLINE_ bool get_supported_device_exs(const VkPhysicalDevice* dev, const char* enabledLyers, std::vector<VkExtensionProperties>& exs);
_INLINE_ bool get_all_supported_device_exs(const VkPhysicalDevice* dev, std::vector<const char*>* enabledLyers, std::unordered_map<std::string, std::vector<VkExtensionProperties>>& map);
_INLINE_ bool create_logical_device(VkPhysicalDevice physicalDev, const VkPhysicalDeviceFeatures* features,
	const PhysicalDeviceVulkan::QueueCreateInf* createInf, const std::vector<const char*>* props, const std::vector<const char*>* exs, VkDevice* device);
_INLINE_ bool check_device_extension_support(VkPhysicalDevice device, const std::vector<std::string>* deviceExtensions, std::vector<VkExtensionProperties>& deviceExtensionProps);
_INLINE_ bool sort_vk_ext_props(const VkExtensionProperties& p1, const VkExtensionProperties& p2);
_INLINE_ bool get_swap_chain_support_details(VkPhysicalDevice dev, VkSurfaceKHR surface, SwapChainSupportDetails& detail);


RenderDeviceVulkan::RenderDeviceVulkan()
{
}

VkCommandBuffer RenderDeviceVulkan::get_cmd_buffer()
{
	return buff;
}

void RenderDeviceVulkan::reset_cmd_pool()
{
	vkResetCommandPool(m_device, m_commandPool, 0);
	
}

RenderDeviceVulkan::~RenderDeviceVulkan()
{
	delete drawGui;

	if (m_instance != nullptr && isInstanceInitedSuccessfully)
	{
		if (m_swapChain != nullptr && isSwapchainInitialized)
		{
			vkDestroySwapchainKHR(m_device, m_swapChain, nullptr);
			vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
		}
		if (m_device != nullptr && isDeviceInitedSuccessfully)
		{
			vkDestroyDevice(m_device, nullptr);
		}
		if (isDebugMessengerCreated)
		{
			vkDestroyDebugUtilsMessengerEXT(m_instance, g_debugMessenger, nullptr);
			vkDestroyDebugReportCallbackEXT(m_instance, g_debugReporter, nullptr);
		}
		vkDestroyInstance(m_instance, nullptr);

	}
}

bool RenderDeviceVulkan::init()
{
	if (VK_SUCCESS != volkInitialize())
	{
		return false;
	}

	// Application Info

	VkApplicationInfo applicationInfo;
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pNext = nullptr;
	applicationInfo.pApplicationName = std::string(_STR_XDEF(GNF_APP_NAME_FULL)).c_str();
	applicationInfo.pEngineName = "NO ENGINE";
	applicationInfo.engineVersion = GNF_VERSION_AS_HEX;
	applicationInfo.apiVersion = VK_API_VERSION_1_2;

	// Supported Instance Layers
	std::vector<VkLayerProperties> instance_layer_props;
	try
	{
		instance_layer_props = get_supported_instance_layers();
	}
	catch (_UNUSED_ const std::exception& ex)
	{
		//X TOOD: LOGGER MAYBE ?
		return false;
	}

	//X TODO : This is where pluging comes to. Give it all instance layers

	// First check VkInstance should be created with validation layer
	std::string outStr;
	bool isFound = ConfigurationServer::get_singleton()->get_init_configuration("config.json", "DEBUG_LAYER", outStr);

	bool debugInstanceCreated = false;
	bool triedToInitDebug = false;
	bool isDebugEnabled = false;
	if (isFound && outStr == "TRUE")
	{
		isDebugEnabled = true;
		debugInstanceCreated = try_to_create_instance(&applicationInfo, &m_instance, &instance_layer_props, true);
		triedToInitDebug = true;
	}

	// Debug Instance couldn't created first log and create normal instance
	if (!debugInstanceCreated)
	{
		if (triedToInitDebug)
			log_couldnt_initialized_debug_mode();
		//X TODO : Add No Debug Instance
		bool isReleaseCreated = try_to_create_instance(&applicationInfo, &m_instance, &instance_layer_props);
		if (isReleaseCreated)
		{
			log_initialized_normal_mode();
		}
		else
		{
			log_couldnt_initialized_normal_mode();
			return false;
		}
	}
	else
	{
		log_initialized_debug_mode();
	}

	volkLoadInstance(m_instance);
	isInstanceInitedSuccessfully = true;



#ifdef VK_USE_PLATFORM_WIN32_KHR 
	auto pvkCreateWin32SurfaceKHR = PFN_vkCreateWin32SurfaceKHR(vkGetInstanceProcAddr(m_instance, "vkCreateWin32SurfaceKHR"));
	VkWin32SurfaceCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hwnd = (HWND)WindowServer::get_singleton()->get_native_handle();
	createInfo.hinstance = GetModuleHandle(nullptr);
	if (pvkCreateWin32SurfaceKHR == nullptr || pvkCreateWin32SurfaceKHR(m_instance, &createInfo, nullptr, &m_surface) != VK_SUCCESS) {
		return false;
	}

#elif defined VK_USE_PLATFORM_XLIB_KHR 
	NEED SUPPORT
#elif defined VK_USE_PLATFORM_METAL_EXT
	if (WindowServer::get_singleton()->get_window_supporter() != WindowServer::WINDOW_SUPPORTER_GLFW)
	{
		// MAC SUPPORT ONLY GLFW FOR VULKAN
		return false;
	}
	if (glfwCreateWindowSurface(m_instance, ((WindowServerGLFW*)((WindowServer::get_singleton())))->get_glfw_handle(), nullptr, &m_surface) != VK_SUCCESS)
	{
		return false;
	}
#endif

	//------------------------ PHYSICAL DEVICE

	if (isDebugEnabled)
	{
		if (create_debug_messenger(m_instance, &g_debugMessenger, &g_debugReporter))
			isDebugMessengerCreated = true;
	}
	std::string deviceTypeStr;
	isFound = ConfigurationServer::get_singleton()->get_init_configuration("config.json", "DEVICE_TYPE", deviceTypeStr);
	PhysicalDevice::PHYSICAL_DEVICE_TYPE physicalType = PhysicalDevice::PHYSICAL_DEVICE_TYPE_DISCRETE;

	if (isFound)
	{
		if (deviceTypeStr == "INTEGRATED")
		{
			physicalType = PhysicalDevice::PHYSICAL_DEVICE_TYPE_INTEGRATED;
		}
		else if (deviceTypeStr == "CPU")
		{
			physicalType = PhysicalDevice::PHYSICAL_DEVICE_TYPE_CPU;
		}
		else if (deviceTypeStr == "VIRTUAL")
		{
			physicalType = PhysicalDevice::PHYSICAL_DEVICE_TYPE_VIRTUAL;
		}
		else
		{
			physicalType = PhysicalDevice::PHYSICAL_DEVICE_TYPE_DISCRETE;
		}
	}


	// Get all Physical Devices

	std::vector<VkPhysicalDevice> physicalDevs;
	std::vector<VkPhysicalDeviceProperties> physicalDevProperties;
	std::vector<VkPhysicalDeviceFeatures> physicalDevFeatures;
	std::vector<uint32_t> supportedQueueIndexes;
	if (!get_all_physicalDevicesPropsFutures(&m_instance, physicalDevs, physicalDevProperties, physicalDevFeatures))
	{
		return false;
	}

	physicalDevProperties.clear();
	physicalDevFeatures.clear();

	// Selection between all current physical devices

	auto it = physicalDevs.begin();

	std::vector<std::string> requiredExtensionsForPhysicalDevice = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	std::vector<VkExtensionProperties> requiredExtensionPropsForPhysicalDevice;
	while (it != physicalDevs.end())
	{
		uint32_t count;
		vkGetPhysicalDeviceQueueFamilyProperties(*it.operator->(), &count, nullptr);
		std::vector<VkQueueFamilyProperties> families;
		vkGetPhysicalDeviceQueueFamilyProperties(*it.operator->(), &count, families.data());
		bool isThereAnySupported = false;
		for (unsigned int i = 0; i < count; i++)
		{
			VkBool32 isSupported;
			vkGetPhysicalDeviceSurfaceSupportKHR(*it.operator->(), i, m_surface, &isSupported);

			bool extensionsSupported = check_device_extension_support(*it.operator->(), &requiredExtensionsForPhysicalDevice, requiredExtensionPropsForPhysicalDevice);
			if (isSupported == VK_TRUE && extensionsSupported)
			{
				supportedQueueIndexes.push_back(i);
				isThereAnySupported = true;
				break;
			}
		}
		if (!isThereAnySupported)
		{
			it = physicalDevs.erase(it);
		}
		else
		{
			it++;
		}
	}


	if (physicalDevs.size() == 0)
		return false;

	for (const auto& dev : physicalDevs)
	{
		VkPhysicalDeviceProperties props;
		vkGetPhysicalDeviceProperties(dev, &props);
		physicalDevProperties.push_back(props);
		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceFeatures(dev, &features);
		physicalDevFeatures.push_back(features);
	}


	VkPhysicalDevice selectedDevice;
	VkPhysicalDeviceProperties selectedDeviceProperties;
	VkPhysicalDeviceFeatures selectedDeviceFeatures;
	PhysicalDevice::PHYSICAL_DEVICE_TYPE selectedType;
	uint32_t mustQueueIndex = 0;
	if (!try_to_select_physical_device(physicalDevs, physicalDevProperties, physicalDevFeatures, supportedQueueIndexes
		, physicalType, selectedDevice, selectedDeviceProperties, selectedDeviceFeatures, mustQueueIndex, selectedType))
	{
		return false;
	}

	auto physicalDevice = new PhysicalDeviceVulkan(selectedDevice, selectedDeviceProperties, selectedDeviceFeatures, selectedType, mustQueueIndex
		, selectedDeviceProperties.deviceName, selectedDeviceProperties.apiVersion,
		selectedDeviceProperties.driverVersion, selectedDeviceProperties.vendorID);

	m_physicalDevice.reset(physicalDevice);


	// Swap Chain Creation
	SwapChainSupportDetails swpDetail;
	
	// Here causes heap corruption
	if (!get_swap_chain_support_details(m_physicalDevice->m_physical_device, m_surface, swpDetail))
	{
		return false;
	}



	VkPresentModeKHR selectedPresentMode;
	VkPresentModeKHR desiredMode = VK_PRESENT_MODE_MAILBOX_KHR;
	bool isPresentModeSelected = false;	for (const auto& presentMode : swpDetail.presentModes)
	{
		if (presentMode == desiredMode)
		{
			selectedPresentMode = desiredMode;
			isPresentModeSelected = true;

		}
	}

	if (!isPresentModeSelected)
	{
		isPresentModeSelected = true;
		selectedPresentMode = VK_PRESENT_MODE_FIFO_KHR;
	}

	m_selected_present_mode = selectedPresentMode;

	auto min_number_of_images = swpDetail.capabilities.minImageCount + 1;
	if ((swpDetail.capabilities.maxImageCount > 0) &&
		(min_number_of_images > swpDetail.capabilities.maxImageCount)) {
		min_number_of_images = swpDetail.capabilities.maxImageCount;
	}


	bool formatFound = false;
	VkSurfaceFormatKHR selectedSurfaceFormat;
	for (const auto& availableFormat : swpDetail.formats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			selectedSurfaceFormat = availableFormat;
			formatFound = true;
		}
	}

	if (!formatFound)
		return false;

	m_selected_surface_format = selectedSurfaceFormat;

	VkExtent2D selectedExtend;

	if (swpDetail.capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		selectedExtend =  swpDetail.capabilities.currentExtent;
	}
	else {
		auto wndSize =  WindowServer::get_singleton()->get_framebuffer_size();
		VkExtent2D actualExtent = {
			static_cast<uint32_t>(wndSize.x),
			static_cast<uint32_t>(wndSize.y)
		};

		actualExtent.width = std::clamp(actualExtent.width, swpDetail.capabilities.minImageExtent.width, swpDetail.capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, swpDetail.capabilities.minImageExtent.height, swpDetail.capabilities.maxImageExtent.height);

		selectedExtend =  actualExtent;
	}

	m_selected_extend = selectedExtend;

	// Device Creation
	auto queueFamilyProps = get_queue_family_properties_from_device(m_physicalDevice->m_physical_device);
	
	//X TODO : Plugim to select queueFamilyProps
	

	auto mainFamily = find_queue_families(&queueFamilyProps, VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT | VK_QUEUE_COMPUTE_BIT);


	std::unordered_map<uint32_t,VkQueueFamilyProperties> selectedQueueFamilies;

	for (const auto& family : mainFamily)
	{
		if(selectedQueueFamilies.find(family.first) == selectedQueueFamilies.end())
			selectedQueueFamilies.emplace(family.first,family.second);
	}

	
	if (selectedQueueFamilies.size() == 0)
	{
		//X TODO : LOGGER MAYBE ? 
		return false;
	}

	// Give needed information to m_physicalDevice
	
	m_physicalDevice->select_queue_family_properties(selectedQueueFamilies);

	

	PhysicalDeviceVulkan::QueueCreateInf queueInf;
	m_physicalDevice->create_queue_create_infos_from_selected_queue_family_propperties(queueInf);

	//X MAKE READY THE DEVICE LAYERS

	std::vector<VkLayerProperties> allDeviceLayers;
	if (!get_supported_device_layers(&m_physicalDevice->m_physical_device,allDeviceLayers))
	{
		allDeviceLayers.clear();
	}
	else
	{
		//X PLUGIN 
		if (isDebugEnabled)
		{
			int index = -1;
			for (int i = 0; i < allDeviceLayers.size();i++)
			{
				if (strcmp(allDeviceLayers[i].layerName, "VK_LAYER_KHRONOS_validation") == 0)
				{
					index = i;
					break;
				}
			}
			if (index != -1)
			{
				m_enabledDeviceLayerProps.push_back(allDeviceLayers[index]);
			}
		}
	}

	std::vector<const char*> enabledDeviceLayers;

	for (const auto& enabledDeviceLayer : m_enabledDeviceLayerProps)
	{
		enabledDeviceLayers.push_back(enabledDeviceLayer.layerName);
	}


	//X MAKE READY THE DEVICE LAYER EXTENSIONS
	std::unordered_map<std::string, std::vector<VkExtensionProperties>> allExs;
	get_all_supported_device_exs(&m_physicalDevice->m_physical_device, &enabledDeviceLayers, allExs);

	//X PLUGIN


	// THIS IS WHERE ENGINE ADDS ITS OWN PHYSICAL EXTENSIONS 

	for (const auto& ex : allExs)
	{
		if (isDebugEnabled && ex.first == "VK_LAYER_KHRONOS_validation")
		{
			std::vector< VkExtensionProperties> props;
			for (const auto& exProp : ex.second)
			{
				props.push_back(exProp);
			}
			m_enabledDeviceExtensions.emplace(ex.first, props);
		}
	}
	m_enabledDeviceExtensions.emplace("engine", std::vector<VkExtensionProperties>());
	// WE CHECKED THAT THIS EXTENSIONS SUPPORTED BY SELECTED DEVICE
	for (const auto& ex : requiredExtensionPropsForPhysicalDevice)
	{
		m_enabledDeviceExtensions["engine"].push_back( ex);
	}

	// NOW ELECT THE REPEATED VALUES

	for (auto& it : m_enabledDeviceExtensions)
	{
		it.second.erase(std::unique(it.second.begin(),it.second.end(),sort_vk_ext_props),it.second.end());
	
	}

	std::vector<const char*> enabledDeviceExtensions;
	
	for (const auto& it : m_enabledDeviceExtensions)
	{
		for (const auto& ex : it.second)
		{
			enabledDeviceExtensions.push_back(ex.extensionName);
		}
	}

	
	if (!create_logical_device(m_physicalDevice->m_physical_device, &m_physicalDevice->m_physical_device_features, &queueInf,
		&enabledDeviceLayers, &enabledDeviceExtensions, &m_device))
	{
		return false;
	}

	volkLoadDevice(m_device);
	isDeviceInitedSuccessfully = true;
	
	auto currentTransform = swpDetail.capabilities.currentTransform;
	
	VkSwapchainCreateInfoKHR swapchain_create_info = {
	VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
	nullptr,
	0,
	m_surface,
	min_number_of_images,
	selectedSurfaceFormat.format,
	selectedSurfaceFormat.colorSpace,
	selectedExtend,
	1,
	VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT , // Image Usage
	VK_SHARING_MODE_EXCLUSIVE,
	0,
	nullptr,
	currentTransform,
	VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
	selectedPresentMode,
	VK_TRUE,
	VK_NULL_HANDLE	
	};

	if (auto sres = vkCreateSwapchainKHR(m_device, &swapchain_create_info, nullptr, &m_swapChain); sres != VK_SUCCESS) {
		return false;
	}
	isSwapchainInitialized = true;

	uint32_t swapChainImagesCount = 0;

	vkGetSwapchainImagesKHR(m_device, m_swapChain, &swapChainImagesCount, nullptr);

	swapChainImages = std::vector<VkImage>(swapChainImagesCount);
	vkGetSwapchainImagesKHR(m_device, m_swapChain, &swapChainImagesCount, swapChainImages.data());

	swapChainImageViews = std::vector<VkImageView>(swapChainImagesCount);
	
	for (uint32_t i = 0; i < swapChainImagesCount; i++)
	{
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = swapChainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = m_selected_surface_format.format;

		// Default Usage 
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		vkCreateImageView(m_device, &createInfo, nullptr, &swapChainImageViews[i]);
		
	}
	
	vkGetDeviceQueue(m_device, m_physicalDevice->get_surface_queue(), 0, &m_surfaceQueue);


	
	// Acquire the images

	//vkAcquireNextImageKHR(m_device, m_swapChain, 2000000000, semaphore, fence, &image_index);

	VkSemaphoreCreateInfo semaphore_create_info = {
		VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		nullptr,
		0
	};
	VkResult result = vkCreateSemaphore(m_device, &semaphore_create_info, nullptr, &m_semaphore);
	result = vkCreateSemaphore(m_device, &semaphore_create_info, nullptr, &m_surfaceSemaphore);
	
	if (VK_SUCCESS != result) {
		return false;
	}

	VkFenceCreateInfo fence_create_info = {
		VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		nullptr,
		0
	};

	result = vkCreateFence(m_device, &fence_create_info, nullptr, &fence);
	if (VK_SUCCESS != result) {
		return false;
	}

	// Create Render Pass

	// the renderpass will use this color attachment.
	VkAttachmentDescription color_attachment = {};
	//the attachment will have the format needed by the swapchain
	color_attachment.format = m_selected_surface_format.format;
	//1 sample, we won't be doing MSAA
	color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	// we Clear when this attachment is loaded
	color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	// we keep the attachment stored when the renderpass ends
	color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	//we don't care about stencil
	color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

	//we don't know or care about the starting layout of the attachment
	color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	//after the renderpass ends, the image has to be on a layout ready for display
	color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


	VkAttachmentReference color_attachment_ref = {};
	//attachment number will index into the pAttachments array in the parent renderpass itself
	color_attachment_ref.attachment = 0;
	color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	//we are going to create 1 subpass, which is the minimum you can do
	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &color_attachment_ref;



	VkRenderPassCreateInfo render_pass_info = {};
	render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

	//connect the color attachment to the info
	render_pass_info.attachmentCount = 1;
	render_pass_info.pAttachments = &color_attachment;
	//connect the subpass to the info
	render_pass_info.subpassCount = 1;
	render_pass_info.pSubpasses = &subpass;


	if(VK_SUCCESS != vkCreateRenderPass(m_device, &render_pass_info, nullptr, &m_render_pass))
	{
		return false;
	}


	//create the framebuffers for the swapchain images. This will connect the render-pass to the images for rendering
	VkFramebufferCreateInfo fb_info = {};
	fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	fb_info.pNext = nullptr;

	fb_info.renderPass = m_render_pass;
	fb_info.attachmentCount = 1;
	fb_info.width = m_selected_extend.width;
	fb_info.height = m_selected_extend.height;
	fb_info.layers = 1;

	//grab how many images we have in the swapchain
	const uint32_t swapchain_imagecount = swapChainImages.size();
	m_frameBuffers = std::vector<VkFramebuffer>(swapchain_imagecount);

	//create framebuffers for each of the swapchain image views
	for (int i = 0; i < swapchain_imagecount; i++) {

		fb_info.pAttachments = &swapChainImageViews[i];
		if (vkCreateFramebuffer(m_device, &fb_info, nullptr, &m_frameBuffers[i]) != VK_SUCCESS)
		{
			return false;
		}
	}




	// CMD POOL




	VkCommandPoolCreateInfo commandPoolInfo {};
	commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolInfo.pNext = nullptr;
	commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	commandPoolInfo.queueFamilyIndex = m_physicalDevice->get_surface_queue();


	if (VK_SUCCESS != vkCreateCommandPool(m_device, &commandPoolInfo, nullptr, &m_commandPool))
	{
		return false;
	}

	VkCommandBufferAllocateInfo buffInfo = {};
	buffInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	buffInfo.pNext = nullptr;
	buffInfo.commandPool = m_commandPool;
	buffInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	buffInfo.commandBufferCount = 1;

	if (VK_SUCCESS != vkAllocateCommandBuffers(m_device, &buffInfo, &buff))
	{
		return false;
	}
	drawGui = new ImGuiDraw();
	drawGui->init(this);
	//CMD BEGIN

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.pNext = nullptr;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;



	uint32_t imageIndex = 0;

	auto res2 = vkAcquireNextImageKHR(m_device, m_swapChain, 0, m_semaphore, VK_NULL_HANDLE, &imageIndex);
	if (res2 != VK_SUCCESS)
	{
		return false;
	}

	if (VK_SUCCESS != vkBeginCommandBuffer(buff, &beginInfo))
	{
		return false;
	}

	VkClearValue clearValue;
	clearValue.color = { { 0.0f, 0.0f, 1.0f, 1.0f } };


	VkRenderPassBeginInfo rpInfo = {};
	rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	rpInfo.pNext = nullptr;

	rpInfo.renderPass = m_render_pass;
	rpInfo.renderArea.offset.x = 0;
	rpInfo.renderArea.offset.y = 0;
	rpInfo.renderArea.extent = selectedExtend;
	rpInfo.framebuffer = m_frameBuffers[imageIndex];

	//connect clear values
	rpInfo.clearValueCount = 1;
	rpInfo.pClearValues = &clearValue;
	
	vkCmdBeginRenderPass(buff, &rpInfo, VK_SUBPASS_CONTENTS_INLINE);
	

	vkCmdEndRenderPass(buff);

	vkEndCommandBuffer(buff);

	
	VkSubmitInfo submit = {};
	submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit.pNext = nullptr;

	VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	submit.pWaitDstStageMask = &waitStage;

	submit.waitSemaphoreCount = 1;
	submit.pWaitSemaphores = &m_semaphore;

	submit.signalSemaphoreCount = 1;
	submit.pSignalSemaphores = &m_surfaceSemaphore;

	submit.commandBufferCount = 1;
	submit.pCommandBuffers = &buff;

	//submit command buffer to the queue and execute it.
	// _renderFence will now block until the graphic commands finish execution
	res2 = vkQueueSubmit(m_surfaceQueue, 1, &submit,fence);
	if (res2 != VK_SUCCESS)
	{
		return false;
	}

	vkResetFences(m_device, 0, &fence);
	//vkFreeCommandBuffers(m_device, m_commandPool, 1, &buff);


	// Swapchain
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = nullptr;

	presentInfo.pSwapchains = &m_swapChain;
	presentInfo.swapchainCount = 1;

	presentInfo.pWaitSemaphores = &m_surfaceSemaphore;
	presentInfo.waitSemaphoreCount = 1;

	presentInfo.pImageIndices = &imageIndex;

	res2 = vkQueuePresentKHR(m_surfaceQueue, &presentInfo);
	if (res2 != VK_SUCCESS)
	{
		return false;
	}

	return true;
}

GRAPHIC_API RenderDeviceVulkan::get_graphic_api() const noexcept
{
	return GRAPHIC_API_VULKAN;
}
void RenderDeviceVulkan::render_ui()
{	

	drawGui->begin();
	drawGui->show_demo();
	drawGui->end();







	uint32_t imageIndex = 0;

	auto res2 = vkAcquireNextImageKHR(m_device, m_swapChain, UINT64_MAX	, m_semaphore, VK_NULL_HANDLE, &imageIndex);
	if (res2 == VK_ERROR_OUT_OF_DATE_KHR || res2 == VK_SUBOPTIMAL_KHR)
	{
		auto ff = true;
		return;
	}

	auto ferr = vkWaitForFences(m_device, 1, &fence, VK_TRUE, UINT64_MAX);
	ferr = vkResetFences(m_device, 0, &fence);

	ferr = vkResetCommandPool(m_device, m_commandPool, 0);

		
	VkCommandBufferBeginInfo beginInf = {};
	beginInf.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInf.pNext = nullptr;
	beginInf.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	beginInf.pInheritanceInfo = nullptr;

	auto sss = vkBeginCommandBuffer(buff, &beginInf);

	VkClearValue clearValue;
	clearValue.color = { { 0.0f, 0.0f, 0.0f, 0.0f } };


	VkRenderPassBeginInfo rpInfo = {};
	rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	rpInfo.pNext = nullptr;

	rpInfo.renderPass = m_render_pass;
	rpInfo.renderArea.offset.x = 0;
	rpInfo.renderArea.offset.y = 0;
	rpInfo.renderArea.extent = m_selected_extend;
	rpInfo.framebuffer = m_frameBuffers[imageIndex];

	//connect clear values
	rpInfo.clearValueCount = 1;
	rpInfo.pClearValues = &clearValue;

	vkCmdBeginRenderPass(buff, &rpInfo, VK_SUBPASS_CONTENTS_INLINE);
	/*
	ImGui::Begin("Another Window");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	ImGui::Text("Hello from another window!");
	ImGui::Button("Close Me");
	ImGui::End();
	*/

	drawGui->fillCmd(buff);

	vkCmdEndRenderPass(buff);
	vkEndCommandBuffer(buff);

	VkSubmitInfo submit = {};
	submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit.pNext = nullptr;

	VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	submit.pWaitDstStageMask = &waitStage;

	submit.waitSemaphoreCount = 1;
	submit.pWaitSemaphores = &m_semaphore;

	submit.signalSemaphoreCount = 1;
	submit.pSignalSemaphores = &m_surfaceSemaphore;

	submit.commandBufferCount = 1;
	submit.pCommandBuffers = &buff;
	
	
	vkQueueSubmit(m_surfaceQueue, 1, &submit , fence);


	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = nullptr;

	presentInfo.pSwapchains = &m_swapChain;
	presentInfo.swapchainCount = 1;

	presentInfo.pWaitSemaphores = &m_surfaceSemaphore;
	presentInfo.waitSemaphoreCount = 1;

	presentInfo.pImageIndices = &imageIndex;

	auto sadasdas = vkQueuePresentKHR(m_surfaceQueue, &presentInfo);
	
}
PhysicalDevice* RenderDeviceVulkan::get_selected_physical_device() const noexcept
{
	return m_physicalDevice.get();
}

bool RenderDeviceVulkan::try_to_create_instance(VkApplicationInfo* appInfo,VkInstance* inst, std::vector<VkLayerProperties>* instance_layer_props, bool isDebugEnabled)
{
	int index = -1;
	if (isDebugEnabled)
	{
		bool found = supports_validation_layer(instance_layer_props, index);
		if (!found)
			return false;
	}
	

	//X In Here Plugin Will be Added and we will give them enabled Props and let them if any layer they wanted etc.

	
	//X Delete repeated values if any added from plugin
	
	m_enabledInstanceLayerProps.erase(std::unique(m_enabledInstanceLayerProps.begin(), m_enabledInstanceLayerProps.end(),
		[](const VkLayerProperties& lhs, const VkLayerProperties& rhs) {
			return std::strcmp(lhs.layerName, rhs.layerName) == 0;
		}),
		m_enabledInstanceLayerProps.end());


	//X Checks if any plugin already added debug layer
	bool isAdded = false;

	//X Make Ready LayerProps Data for Vulkan
	std::vector<const char*> enabledInstanceLayerProps;

	for (const auto& layerProp : m_enabledInstanceLayerProps)
	{
		//X TODO : LOGGER MAYBE ?
		enabledInstanceLayerProps.push_back(layerProp.layerName);
		if (isDebugEnabled)
		{
			if (strcmp(layerProp.layerName, (*instance_layer_props)[index].layerName) == 0)
			{
				isAdded = true;
			}
		}
		
	}
	if (!isAdded && isDebugEnabled)
	{
		m_enabledInstanceLayerProps.push_back((*instance_layer_props)[index]);
		enabledInstanceLayerProps.push_back(m_enabledInstanceLayerProps[m_enabledInstanceLayerProps.size() - 1].layerName);
		isAdded = true;
	}
	//X TODO : In Here Plugin Will be Added and we will give them enabled Props etc.

	auto allSupportedExtensions = get_all_supported_instance_exs(&enabledInstanceLayerProps);
	
	for (const auto& supportedExIterator : allSupportedExtensions)
	{
		for (const auto& supportedEx : supportedExIterator.second)
		{
			
			if (isDebugEnabled && strcmp((*instance_layer_props)[index].layerName, supportedExIterator.first.c_str()) == 0)
			{
				m_enabledInstanceExtensionProps.push_back(supportedEx);
				//X TODO : LOGGER MAYBE ?
			}
			
			
		}
	}
	
	//X Remove the repeated values

	m_enabledInstanceExtensionProps.erase(std::unique(m_enabledInstanceExtensionProps.begin(), m_enabledInstanceExtensionProps.end(),
		[](const VkExtensionProperties& lhs, const VkExtensionProperties& rhs) {
			return std::strcmp(lhs.extensionName, rhs.extensionName) == 0;
		}),
		m_enabledInstanceExtensionProps.end());
	

	//X Ready data for Vulkan

	//X Make Ready LayerProps Data for Vulkan
	std::vector<const char*> enabledInstanceExtensionProps(m_enabledInstanceExtensionProps.size());

	for (int i = 0;i< enabledInstanceExtensionProps.size();i++)
	{
		enabledInstanceExtensionProps[i] = m_enabledInstanceExtensionProps[i].extensionName;
	}

	enabledInstanceExtensionProps.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#ifdef VK_USE_PLATFORM_WIN32_KHR 
	enabledInstanceExtensionProps.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

#elif defined VK_USE_PLATFORM_XLIB_KHR 
	enabledDeviceExtensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);

#elif defined VK_USE_PLATFORM_METAL_EXT 
	enabledInstanceExtensionProps.push_back("VK_EXT_metal_surface");
	enabledInstanceExtensionProps.push_back("VK_KHR_portability_enumeration"); 
#else
	DONT COMPILE
#endif

	VkInstanceCreateInfo inf;
	inf.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	inf.pNext = nullptr;
#if defined VK_USE_PLATFORM_METAL_EXT 
	inf.flags = 1;
#else
	inf.flags = 0;
#endif
	inf.pApplicationInfo = appInfo;
	inf.enabledLayerCount = (uint32_t)enabledInstanceLayerProps.size();
	inf.ppEnabledLayerNames = enabledInstanceLayerProps.data();
	inf.enabledExtensionCount = (uint32_t)enabledInstanceExtensionProps.size();
	inf.ppEnabledExtensionNames = enabledInstanceExtensionProps.data();

	auto res = vkCreateInstance(&inf, nullptr, inst);
	if (res != VK_SUCCESS)
	{
		return false;
	}

	return true;
}

bool RenderDeviceVulkan::try_to_select_physical_device(
	const std::vector<VkPhysicalDevice>& physicalDevs,
	const std::vector<VkPhysicalDeviceProperties>& physicalDevProperties,
	const std::vector<VkPhysicalDeviceFeatures>& physicalDevFeatures,
	const std::vector<uint32_t>& surfaceSupportedQueueIndexes,
	PhysicalDevice::PHYSICAL_DEVICE_TYPE deviceType, VkPhysicalDevice& dev,
	VkPhysicalDeviceProperties& prop, VkPhysicalDeviceFeatures& feature,uint32_t& queueIndex,PhysicalDevice::PHYSICAL_DEVICE_TYPE& selectedType)
{
	/*
	std::vector<VkPhysicalDevice> physicalDevs;
	std::vector<VkPhysicalDeviceProperties> physicalDevProperties;
	std::vector<VkPhysicalDeviceFeatures> physicalDevFeatures;
	*/

	//X TODO : In here There have to be selection by supporting queue families

	if (physicalDevs.size() == 0)
		return false;

	//X There is already just one device. No need to for loop
	if (physicalDevs.size() == 1)
	{
		dev = physicalDevs[0];
		prop = physicalDevProperties[0];
		feature = physicalDevFeatures[0];
		queueIndex = surfaceSupportedQueueIndexes[0];
		selectedType = vk_to_physical_device_type(prop.deviceType);
		return true;
	}

	//X TODO : Plugin Physical Devices Found Event

	VkPhysicalDeviceType type = VK_PHYSICAL_DEVICE_TYPE_OTHER;
	
	switch (deviceType)
	{
	case PhysicalDevice::PHYSICAL_DEVICE_TYPE_UNDEFINED:
		type = VK_PHYSICAL_DEVICE_TYPE_OTHER;
		break;
	case PhysicalDevice::PHYSICAL_DEVICE_TYPE_CPU:
		type = VK_PHYSICAL_DEVICE_TYPE_CPU;
		break;
	case PhysicalDevice::PHYSICAL_DEVICE_TYPE_DISCRETE:
		type = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
		break;
	case PhysicalDevice::PHYSICAL_DEVICE_TYPE_INTEGRATED:
		type = VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;
		break;
	case PhysicalDevice::PHYSICAL_DEVICE_TYPE_VIRTUAL:
		type = VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU;
		break;
	default:
		type = VK_PHYSICAL_DEVICE_TYPE_OTHER;
		break;
	}

	if (type == VK_PHYSICAL_DEVICE_TYPE_OTHER)
	{
		dev = physicalDevs[0];
		prop = physicalDevProperties[0];
		feature = physicalDevFeatures[0];
		queueIndex = surfaceSupportedQueueIndexes[0];
		selectedType = vk_to_physical_device_type(prop.deviceType);
		return true;
	}
	bool isFound = false;


	for (int i = 0; i < physicalDevProperties.size(); i++)
	{
		if (physicalDevProperties[i].deviceType == type)
		{
			dev = physicalDevs[i];
			prop = physicalDevProperties[i];
			feature = physicalDevFeatures[i];
			queueIndex = surfaceSupportedQueueIndexes[i];
			selectedType = vk_to_physical_device_type(prop.deviceType);
			return true;
		}
	}
	
	//X Couldn't find requested type. First check if there is any dedicated gpu
	//X TODO : Logger Maybe ?

	for (int i = 0; i < physicalDevProperties.size(); i++)
	{
		if (physicalDevProperties[i].deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			dev = physicalDevs[i];
			prop = physicalDevProperties[i];
			feature = physicalDevFeatures[i];
			queueIndex = surfaceSupportedQueueIndexes[i];
			selectedType = vk_to_physical_device_type(prop.deviceType);
			return true;
		}
	}


	//X Couldn't find any dedicated gpu. Now check if there is any integrated gpu
	//X TODO : Logger Maybe ?
	for (int i = 0; i < physicalDevProperties.size(); i++)
	{
		if (physicalDevProperties[i].deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
		{
			dev = physicalDevs[i];
			prop = physicalDevProperties[i];
			feature = physicalDevFeatures[i];
			queueIndex = surfaceSupportedQueueIndexes[i];
			selectedType = vk_to_physical_device_type(prop.deviceType);
			return true;
		}
	}

	//X Couldn't find any integrated gpu. Just return first physical device
	//X TODO : Logger Maybe ?

	dev = physicalDevs[0];
	prop = physicalDevProperties[0];
	feature = physicalDevFeatures[0];
	queueIndex = surfaceSupportedQueueIndexes[0];
	selectedType = vk_to_physical_device_type(prop.deviceType);
	return true;
}

// HELPERS

bool create_debug_messenger(VkInstance instance, VkDebugUtilsMessengerEXT* pDebugMessenger, VkDebugReportCallbackEXT* pDebugReporter) {
	// Load the function pointer for vkCreateDebugUtilsMessengerEXT()
	PFN_vkCreateDebugUtilsMessengerEXT pfnCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
	if (pfnCreateDebugUtilsMessengerEXT == nullptr) {
		return false;
	}

	// Fill in the debug messenger creation info structure
	VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = vk_debug_messenger_callback;
	createInfo.pUserData = nullptr;
	// Create the debug messenger
	VkResult result = pfnCreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, pDebugMessenger);

	if (result != VK_SUCCESS)
		return false;
	const VkDebugReportCallbackCreateInfoEXT ci = {
			.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
			.pNext = nullptr,
			.flags =
				VK_DEBUG_REPORT_WARNING_BIT_EXT |
				VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
				VK_DEBUG_REPORT_ERROR_BIT_EXT |
				VK_DEBUG_REPORT_DEBUG_BIT_EXT,
			.pfnCallback = &VulkanDebugReportCallback,
			.pUserData = nullptr
	};

	result = vkCreateDebugReportCallbackEXT(instance, &ci, nullptr, pDebugReporter);
	return result == VK_SUCCESS;
}


_INLINE_ bool get_all_physicalDevicesPropsFutures(const VkInstance* inst, std::vector<VkPhysicalDevice>& physicalDevs,
	std::vector<VkPhysicalDeviceProperties>& physicalDevProps, std::vector<VkPhysicalDeviceFeatures>& physicalDevFeatures)
{
	uint32_t physicalDevCount = 0;
	if (vkEnumeratePhysicalDevices(*inst, &physicalDevCount, nullptr) != VK_SUCCESS)
	{
		return false;
	}
	
	physicalDevs = std::vector<VkPhysicalDevice>(physicalDevCount);

	if (vkEnumeratePhysicalDevices(*inst, &physicalDevCount, physicalDevs.data()) != VK_SUCCESS)
	{
		return false;
	}
	physicalDevProps = std::vector<VkPhysicalDeviceProperties>(physicalDevCount);
	physicalDevFeatures = std::vector<VkPhysicalDeviceFeatures>(physicalDevCount);
	
	for(unsigned int i = 0;i < physicalDevCount;i++)
	{
		vkGetPhysicalDeviceProperties(physicalDevs[i], &physicalDevProps[i]);
		vkGetPhysicalDeviceFeatures(physicalDevs[i],&physicalDevFeatures[i]);
	}

	return true;
}

_INLINE_ std::vector<VkExtensionProperties> get_supported_instance_exs(const char* forLayer)
{
	uint32_t ex_count = 0;
	if (vkEnumerateInstanceExtensionProperties(forLayer,&ex_count, nullptr) != VK_SUCCESS)
	{
		throw std::runtime_error("Couldn't get layer prop");
	}
	std::vector<VkExtensionProperties> m_exProps(ex_count);
	if (vkEnumerateInstanceExtensionProperties(forLayer ,&ex_count, m_exProps.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("Couldn't get layer prop");
	}
	return m_exProps;
}

_INLINE_ std::vector<VkLayerProperties> get_supported_instance_layers()
{
	uint32_t layer_count = 0;
	if (vkEnumerateInstanceLayerProperties(&layer_count, nullptr) != VK_SUCCESS)
	{
		throw std::runtime_error("Couldn't get layer prop");
	}
	std::vector<VkLayerProperties> m_layerProps(layer_count);
	if (vkEnumerateInstanceLayerProperties(&layer_count, m_layerProps.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("Couldn't get layer prop");
	}
	return m_layerProps;
}

_INLINE_ bool supports_validation_layer(std::vector<VkLayerProperties>* layers, int& index)
{
	for (int i = 0; i < layers->size(); i++)
	{
		if (strcmp((*layers)[i].layerName, "VK_LAYER_KHRONOS_validation") == 0)
		{
			index = i;
			return true;
		}
	}
	return false;
}

_INLINE_ std::unordered_map<std::string, std::vector<VkExtensionProperties>> get_all_supported_instance_exs(std::vector<const char*>* enabledLayers)
{
	std::unordered_map<std::string, std::vector<VkExtensionProperties>> map;
	for (int i = 0; i < enabledLayers->size(); i++)
	{
		map.emplace(std::string((*enabledLayers)[i]), get_supported_instance_exs((*enabledLayers)[i]));
	}
	return map;
}

_INLINE_ PhysicalDevice::PHYSICAL_DEVICE_TYPE vk_to_physical_device_type(VkPhysicalDeviceType type)
{
	switch (type)
	{
	case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
		return PhysicalDevice::PHYSICAL_DEVICE_TYPE_DISCRETE;
	case VK_PHYSICAL_DEVICE_TYPE_CPU:
		return PhysicalDevice::PHYSICAL_DEVICE_TYPE_CPU;
	case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
		return PhysicalDevice::PHYSICAL_DEVICE_TYPE_INTEGRATED;
	case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
		return PhysicalDevice::PHYSICAL_DEVICE_TYPE_VIRTUAL;
	case VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM:
	case VK_PHYSICAL_DEVICE_TYPE_OTHER:
		return PhysicalDevice::PHYSICAL_DEVICE_TYPE_UNDEFINED;
	default:
		return PhysicalDevice::PHYSICAL_DEVICE_TYPE_UNDEFINED;
	}
}

_INLINE_ std::vector<VkQueueFamilyProperties> get_queue_family_properties_from_device(VkPhysicalDevice dev)
{
	uint32_t queueCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(dev, &queueCount, nullptr);
	std::vector<VkQueueFamilyProperties> queuesssProps = std::vector<VkQueueFamilyProperties>(queueCount);
	vkGetPhysicalDeviceQueueFamilyProperties(dev, &queueCount, queuesssProps.data());
	return queuesssProps;
}
_INLINE_ std::unordered_map<uint32_t, VkQueueFamilyProperties> find_queue_families(std::vector<VkQueueFamilyProperties>* queueFamily, VkQueueFlags desiredFlags)
{
	std::unordered_map<uint32_t, VkQueueFamilyProperties> supportedQueueFamilies;
	for (uint32_t i = 0; i != (*queueFamily).size(); i++)
		if ((*queueFamily)[i].queueCount > 0 && ((*queueFamily)[i].queueFlags & desiredFlags) == desiredFlags)
			supportedQueueFamilies.emplace(i, (*queueFamily)[i]);

	return supportedQueueFamilies;
}

_INLINE_ bool get_supported_device_layers(const VkPhysicalDevice* dev, std::vector<VkLayerProperties>& props)
{
	uint32_t propCount = 0;
	if (VK_SUCCESS != vkEnumerateDeviceLayerProperties(*dev, &propCount, nullptr))
		return false;
	props = std::vector<VkLayerProperties>(propCount);
	if (VK_SUCCESS != vkEnumerateDeviceLayerProperties(*dev, &propCount, props.data()))
		return false;
	return true;
}

_INLINE_ bool get_all_supported_device_exs(const VkPhysicalDevice* dev, std::vector<const char*>* enabledLyers, std::unordered_map<std::string, std::vector<VkExtensionProperties>>& map)
{
	for (int i = 0;i < enabledLyers->size();i++)
	{
		std::vector< VkExtensionProperties> exs;
		if (get_supported_device_exs(dev, (*enabledLyers)[i], exs))
		{
			map.emplace(std::string((*enabledLyers)[i]), exs);
		}
	}
	return true;
}

_INLINE_ bool get_supported_device_exs(const VkPhysicalDevice* dev, const char* enabledLyers, std::vector<VkExtensionProperties>& exs)
{
	uint32_t size = 0;
	if(vkEnumerateDeviceExtensionProperties(*dev, enabledLyers, &size, nullptr) != VK_SUCCESS)
	{
		return false;
	}
	exs = std::vector<VkExtensionProperties>(size);
	if (vkEnumerateDeviceExtensionProperties(*dev, enabledLyers, &size, exs.data()) != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

_INLINE_ bool create_logical_device(VkPhysicalDevice physicalDev, const VkPhysicalDeviceFeatures* features,
	const PhysicalDeviceVulkan::QueueCreateInf* createInf, const std::vector<const char*>* props, const std::vector<const char*>* exs, VkDevice* device)
{

	VkDeviceCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.queueCreateInfoCount = createInf->get_queue_create_inf_count();
	createInfo.pQueueCreateInfos = createInf->data();
	createInfo.enabledLayerCount = (uint32_t)props->size();
	createInfo.ppEnabledLayerNames = props->data();
	createInfo.enabledExtensionCount = (uint32_t)exs->size();
	createInfo.ppEnabledExtensionNames = exs->data();
	createInfo.pEnabledFeatures = features;

	if (vkCreateDevice(physicalDev, &createInfo, nullptr, device) != VK_SUCCESS)
		return false;

	return true;
}

_INLINE_ bool check_device_extension_support(VkPhysicalDevice device,const std::vector<std::string>* deviceExtensions,std::vector<VkExtensionProperties>& deviceExtensionProps) 
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions->begin(), deviceExtensions->end());

	for (const auto& extension : availableExtensions) {
		if (requiredExtensions.find(extension.extensionName) != requiredExtensions.end())
		{
			requiredExtensions.erase(extension.extensionName);
			deviceExtensionProps.push_back(extension);
		}
	}

	return requiredExtensions.empty();
}
_INLINE_ bool sort_vk_ext_props(const VkExtensionProperties& p1, const VkExtensionProperties& p2)
{
	return strcmp(p1.extensionName, p2.extensionName) == 0;
}

_INLINE_ bool get_swap_chain_support_details(VkPhysicalDevice dev, VkSurfaceKHR surface, SwapChainSupportDetails& detail)
{
	uint32_t surfaceCount = 0;
	if (VK_SUCCESS != vkGetPhysicalDeviceSurfacePresentModesKHR(dev, surface, &surfaceCount, nullptr))
		return false;

	detail.presentModes = std::vector<VkPresentModeKHR>(surfaceCount);


	if (VK_SUCCESS != vkGetPhysicalDeviceSurfacePresentModesKHR(dev, surface, &surfaceCount, detail.presentModes.data()))
		return false;
	

	if (VK_SUCCESS != vkGetPhysicalDeviceSurfaceCapabilitiesKHR(dev, surface, &detail.capabilities))
		return false;


	uint32_t formatCount;
	if (VK_SUCCESS != vkGetPhysicalDeviceSurfaceFormatsKHR(dev, surface, &formatCount,nullptr))
		return false;
	

	detail.formats = std::vector<VkSurfaceFormatKHR>(formatCount);

	// Here causes heap corruption
	if (VK_SUCCESS != vkGetPhysicalDeviceSurfaceFormatsKHR(dev, surface, &formatCount, detail.formats.data()))
		return false;

	return true;
}