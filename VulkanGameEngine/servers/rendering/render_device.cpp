#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#elif __linux__
#define VK_USE_PLATFORM_XLIB_KHR
#elif __APPLE__
#define VK_USE_PLATFORM_METAL_EXT
#endif


#define DEBUG_LAYER_NAME "VK_LAYER_KHRONOS_validation"

#include <boost/format.hpp>
#include "render_device.h"

#include "../logger_server.h"
#include "../configuration_server.h"
#include "../window_server.h"
#include "../../core/version.h"
#include "../../graphic/vulkan/utils_vulkan.h"


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



RenderDevice::~RenderDevice()
{
	if (m_instanceLoaded && m_instance.instance != nullptr)
	{
		if(m_instance.messenger != nullptr)
			vkDestroyDebugUtilsMessengerEXT(m_instance.instance, m_instance.messenger, nullptr);
		if(m_instance.reportCallback != nullptr)
			vkDestroyDebugReportCallbackEXT(m_instance.instance, m_instance.reportCallback, nullptr);
		vkDestroyInstance(m_instance.instance, nullptr);
	}
}

bool RenderDevice::init()
{
	m_instance.format.format = VK_FORMAT_B8G8R8A8_SRGB;
	m_instance.format.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	if (VK_SUCCESS != volkInitialize())
	{
		return false;
	}
	
	bool succeeded = true;
	succeeded = init_vk_instance();
	if (!succeeded)
		return false;
	succeeded = init_vk_device();
	if (!succeeded)
		return false;
	succeeded = save_vk_physical_device();
	if (!succeeded)
		return false;

	return true;
}

void RenderDevice::destroy()
{
	delete singleton;
}


//LoggerServer::get_singleton()->log_cout(this,"Couldn't initalize debug mode",Logger::WARNING);




bool RenderDevice::init_vk_instance()
{	
	{
		// Application Info
		VkApplicationInfo applicationInfo;
		applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		applicationInfo.pNext = nullptr;
		applicationInfo.pApplicationName = std::string(_STR_XDEF(GNF_APP_NAME_FULL)).c_str();
		applicationInfo.pEngineName = "NO ENGINE";
		applicationInfo.engineVersion = GNF_VERSION_AS_HEX;
		applicationInfo.apiVersion = VK_API_VERSION_1_2;


		// First get all instance layer

		std::vector<VkLayerProperties> allInstanceLayers;

		if (!get_all_instance_layers(allInstanceLayers))
		{
			return false;
		}

		std::string outStr;
		bool isFound = ConfigurationServer::get_singleton()->get_init_configuration("config.json", "DEBUG_LAYER", outStr);
		bool isDebugEnabled = false;
		if (isFound && outStr == "TRUE")
		{
			isDebugEnabled = true;
		}
		bool isDebugLayerAdded = false;
		if (isDebugEnabled)
		{
			int index;
			isDebugLayerAdded = find_layer_by_name(&allInstanceLayers, DEBUG_LAYER_NAME, index);
			if(!isDebugLayerAdded)
				LoggerServer::get_singleton()->log_cout(this, boost::str(boost::format("Debug Layer was enabled but there was no layer with named %1%") % DEBUG_LAYER_NAME).c_str(), Logger::WARNING);
			else
			{
				m_instance.enabledProps.enabledLayers.push_back(allInstanceLayers[index]);
				LoggerServer::get_singleton()->log_cout(this, boost::str(boost::format("Debug Layer added with named %1%") % DEBUG_LAYER_NAME).c_str(), Logger::INFO);
			}
		}

		//X TODO : Plugin Here


		//X Check the array if plugins added same thing more than once
		m_instance.enabledProps.enabledLayers.erase(std::unique(m_instance.enabledProps.enabledLayers.begin(), m_instance.enabledProps.enabledLayers.end(),
			[](const VkLayerProperties& lhs, const VkLayerProperties& rhs) {
				return std::strcmp(lhs.layerName, rhs.layerName) == 0;
			}),
			m_instance.enabledProps.enabledLayers.end());


		std::vector<const char*> enabledLayers(m_instance.enabledProps.enabledLayers.size());

		// Copy to enabledLayers

		for (int i = 0;i< m_instance.enabledProps.enabledLayers.size();i++)
		{
			enabledLayers[i] = m_instance.enabledProps.enabledLayers[i].layerName;
		}

		auto all_exs = get_all_instance_exs_by_layers(&enabledLayers);

		std::vector<VkExtensionProperties> implicitExs;

		get_instance_implicit_exs(implicitExs);
		all_exs.emplace(IMPLICIT_EXTENSIONS_NAME, std::vector<VkExtensionProperties>());
		for (const auto& ex : implicitExs)
		{	
			all_exs[IMPLICIT_EXTENSIONS_NAME].push_back(ex);
		}


		// NOW WE GOT ALL EXTENSIONS

		// Be sure that extensions of debug layer added
		if (isDebugEnabled && isDebugLayerAdded)
		{
			m_instance.enabledProps.enabledExtensions.emplace(std::string(DEBUG_LAYER_NAME), all_exs[DEBUG_LAYER_NAME]);
		}

		// Be sure for system surface extensions added
		// This is for surface creation
#ifdef VK_USE_PLATFORM_WIN32_KHR 
		std::vector<const char*> names = {
		 VK_KHR_SURFACE_EXTENSION_NAME ,VK_KHR_WIN32_SURFACE_EXTENSION_NAME
		};
		std::vector<int> indexes;
		bool allFounded = find_instance_exs_by_names(&all_exs[IMPLICIT_EXTENSIONS_NAME], &names, indexes);

		if (!allFounded)
			return false;
		m_instance.enabledProps.enabledExtensions[IMPLICIT_EXTENSIONS_NAME].push_back(all_exs[IMPLICIT_EXTENSIONS_NAME][0]);
		m_instance.enabledProps.enabledExtensions[IMPLICIT_EXTENSIONS_NAME].push_back(all_exs[IMPLICIT_EXTENSIONS_NAME][1]);

#elif defined VK_USE_PLATFORM_XLIB_KHR 
		std::vector<const char*> names = {
	 VK_KHR_SURFACE_EXTENSION_NAME ,VK_KHR_XLIB_SURFACE_EXTENSION_NAME
		};
		std::vector<int> indexes;
		bool allFounded = find_instance_exs_by_names(&all_exs[IMPLICIT_EXTENSIONS_NAME], &names, indexes);

		if (!allFounded)
			return false;
		m_instance.enabledProps.enabledExtensions[IMPLICIT_EXTENSIONS_NAME].push_back(all_exs[IMPLICIT_EXTENSIONS_NAME][0]);
		m_instance.enabledProps.enabledExtensions[IMPLICIT_EXTENSIONS_NAME].push_back(all_exs[IMPLICIT_EXTENSIONS_NAME][1]);

#elif defined VK_USE_PLATFORM_METAL_EXT 
		std::vector<const char*> names = {
	 VK_KHR_SURFACE_EXTENSION_NAME ,"VK_EXT_metal_surface","VK_KHR_portability_enumeration"
		};
		std::vector<int> indexes;
		bool allFounded = find_instance_exs_by_names(&all_exs[IMPLICIT_EXTENSIONS_NAME], &names, indexes);

		if (!allFounded)
			return false;
		m_instance.enabledProps.enabledExtensions[IMPLICIT_EXTENSIONS_NAME].push_back(all_exs[IMPLICIT_EXTENSIONS_NAME][0]);
		m_instance.enabledProps.enabledExtensions[IMPLICIT_EXTENSIONS_NAME].push_back(all_exs[IMPLICIT_EXTENSIONS_NAME][1]);
		m_instance.enabledProps.enabledExtensions[IMPLICIT_EXTENSIONS_NAME].push_back(all_exs[IMPLICIT_EXTENSIONS_NAME][2]);
#else
		DONT COMPILE
#endif 


			//X TODO : PLUGIN GOES HERE ...
			//X Check the array if plugins added same thing more than once
			for (auto& exs : m_instance.enabledProps.enabledExtensions)
			{
				exs.second.erase(std::unique(exs.second.begin(), exs.second.end(),
					[](const VkExtensionProperties& lhs, const VkExtensionProperties& rhs) {
						return std::strcmp(lhs.extensionName, rhs.extensionName) == 0;
					}),
					exs.second.end());
			}

		//X MAKE READY EXTENSIONS FOR VULKAN

		uint32_t vsize = 0;
		for (auto& exs : m_instance.enabledProps.enabledExtensions)
		{
			vsize += (uint32_t)exs.second.size();
		}

		std::vector<const char*> enabledExs(vsize);

		int vindex = 0;
		// Copy data to vector
		for (auto& exs : m_instance.enabledProps.enabledExtensions)
		{
			for (int i = 0; i < exs.second.size(); i++)
			{
				enabledExs[vindex] = exs.second[i].extensionName;
				vindex++;
			}
		}

		//X NOW WE ARE READY TO CREATE INSTANCE
		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pNext = nullptr;

		createInfo.pApplicationInfo = &applicationInfo;
		createInfo.enabledLayerCount = (uint32_t)enabledLayers.size();
		createInfo.ppEnabledLayerNames = enabledLayers.data();
		createInfo.enabledExtensionCount = (uint32_t)enabledExs.size();
		createInfo.ppEnabledExtensionNames = enabledExs.data();
#if defined VK_USE_PLATFORM_METAL_EXT 
		createInfo.flags = 1;
#else
		createInfo.flags = 0;
#endif
		auto res = vkCreateInstance(&createInfo, nullptr, &m_instance.instance);
		if (res != VK_SUCCESS)
			return false;

		//X Init Function pointers of instance
		volkLoadInstance(m_instance.instance);
		m_instanceLoaded = true;

		if (isDebugEnabled)
		{
			create_debug_messenger(m_instance.instance, &m_instance.messenger, &m_instance.reportCallback, 
				vk_debug_messenger_callback, VulkanDebugReportCallback);
		}
	}

	//X Create Surface
	{
	if (glfwCreateWindowSurface(m_instance.instance, WindowServer::get_singleton()->get_window(), nullptr, &m_instance.surface) != VK_SUCCESS)
		return false;
	}

	return true;
}

bool RenderDevice::init_vk_device()
{
	std::vector<VkPhysicalDevice> devs;
	if (!get_all_physical_devices(m_instance.instance,devs))
		return false;

	std::vector<std::string> requiredExtensionsForPhysicalDevice = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	std::vector<std::vector<VkExtensionProperties>> requiredExtensionPropsForPhysicalDevices;

	auto dev = devs.begin();
	while(dev != devs.end())
	{
		std::vector<VkExtensionProperties> requiredExtensionPropsForPhysicalDevice;

		if (check_device_extension_support(*dev.operator->(), &requiredExtensionsForPhysicalDevice, requiredExtensionPropsForPhysicalDevice))
		{
			dev++;
		}
		else
		{
			dev = devs.erase(dev);
		}
	}

	if (devs.size() == 0)
		return false;


	// Suface is supported now check its capabilities
	dev = devs.begin();

	while (dev != devs.end())
	{	
		SwapChainSupportDetails details;
		get_swap_chain_support_details(*dev.operator->(), m_instance.surface, details);

		bool doesSupportPresent= false;
		for (const auto& presentMode : details.presentModes)
		{
			if (presentMode == m_instance.presentMode)
			{
				doesSupportPresent = true;
			}
		}

		bool doesSupportTripleBuff = true;
		auto min_number_of_images = details.capabilities.minImageCount + 1;
		if ((details.capabilities.maxImageCount > 0) &&
			(min_number_of_images > details.capabilities.maxImageCount)) {
			doesSupportTripleBuff = false;
		}

		bool doesSupportFormat = false;
		for (const auto& availableFormat : details.formats) {
			if (availableFormat.format == m_instance.format.format && availableFormat.colorSpace == m_instance.format.colorSpace) {
				doesSupportFormat = true;
			}
		}


		bool doesSupportAllReq = doesSupportPresent && doesSupportTripleBuff && doesSupportFormat;

		if (doesSupportAllReq)
		{
			dev++;
		}
		else
		{
			dev = devs.erase(dev);
		}
	}
	
	// Surface capabilities is supported. Now check Queue Futures

	auto queueRequirements = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT;
	dev = devs.begin();
	std::vector<int> queueIndices;
	while (dev != devs.end())
	{
		int index = -1;
		if (check_queue_support(*dev.operator->(), queueRequirements,index))
		{
			VkBool32 supportSurface;
			vkGetPhysicalDeviceSurfaceSupportKHR(*dev.operator->(), index, m_instance.surface, &supportSurface);
			if (supportSurface == VK_TRUE)
			{
				queueIndices.push_back(index);
				dev++;
			}
			else
			{
				dev = devs.erase(dev);
			}
		}
		else
		{
			dev = devs.erase(dev);
		}
	}

	// Check config file 

	std::string deviceTypeStr;
	auto isFound = ConfigurationServer::get_singleton()->get_init_configuration("config.json", "DEVICE_TYPE", deviceTypeStr);
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
	
	if (devs.size() == 0)
		return false;

	bool physicalDeviceSelected = false;

	for (int i = 0; i < devs.size(); i++)
	{
		VkPhysicalDeviceProperties props;
		vkGetPhysicalDeviceProperties(devs[i], &props);
		if (props.deviceType == physicalType)
		{
			physicalDeviceSelected = true;
			m_renderDevice.physicalDev = devs[i];
			m_renderDevice.mainQueueFamilyIndex = queueIndices[i];
		}
	}

	return physicalDeviceSelected;
}


bool RenderDevice::save_vk_physical_device()
{
	bool getted = get_swap_chain_support_details(m_renderDevice.physicalDev, m_instance.surface,m_swapChainDetails);
	if (!getted)
		return false;
	m_instance.surfaceImageCount = m_swapChainDetails.capabilities.minImageCount + 1;
	return true;
}

RenderDevice* RenderDevice::create_singleton()
{
	singleton = new RenderDevice();
	return singleton;
}
