#ifdef _WIN32
	#define VK_USE_PLATFORM_WIN32_KHR
#elif __linux__
	#define VK_USE_PLATFORM_XLIB_KHR
#elif __APPLE__
	#define VK_USE_PLATFORM_METAL_EXT
#endif

#include "render_device_vulkan.h"
#include "../../servers/logger_server.h"
#include "../../servers/configuration_server.h"
#include "../../servers/window_server.h"
#include "../../core/version.h"
#include "../../platform/GLFW/window_server_glfw.h"




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


RenderDeviceVulkan::RenderDeviceVulkan()
{
}

RenderDeviceVulkan::~RenderDeviceVulkan()
{
	if (m_instance != nullptr && isInstanceInitedSuccessfully)
	{
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
		debugInstanceCreated = try_to_create_instance(&applicationInfo, &m_instance,&instance_layer_props,true);
		triedToInitDebug = true;
	}

	// Debug Instance couldn't created first log and create normal instance
	if (!debugInstanceCreated)
	{
		if(triedToInitDebug)
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
	if (WindowServer::get_singleton()->get_window_supporter() != WindowServer::WINDOW_SUPPORTER_GLFW)
	{
		// MAC SUPPORT ONLY GLFW FOR VULKAN
		return false;
	}
	if (glfwCreateWindowSurface(m_instance, ((WindowServerGLFW*)((WindowServer::get_singleton())))->get_glfw_handle(), nullptr, &m_surface) != VK_SUCCESS)
	{
		return false;
	}
#elif defined VK_USE_PLATFORM_METAL_EXT
	
#endif




//------------------------ PHYSICAL DEVICE

	if (isDebugEnabled)
	{
		if (create_debug_messenger(m_instance, &g_debugMessenger,&g_debugReporter))
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
	while (it != physicalDevs.end())
	{
		uint32_t count;
		vkGetPhysicalDeviceQueueFamilyProperties(*it.operator->(), &count, nullptr);
		std::vector<VkQueueFamilyProperties> families;
		vkGetPhysicalDeviceQueueFamilyProperties(*it.operator->(), &count, families.data());
		bool isThereAnySupported = false;
		for (int i = 0; i < count; i++)
		{
			VkBool32 isSupported;
			vkGetPhysicalDeviceSurfaceSupportKHR(*it.operator->(), i, m_surface, &isSupported);
			if (isSupported == VK_TRUE)
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
	if (!try_to_select_physical_device(physicalDevs,physicalDevProperties,physicalDevFeatures, supportedQueueIndexes
		,physicalType,selectedDevice, selectedDeviceProperties, selectedDeviceFeatures, mustQueueIndex, selectedType))
	{
		return false;
	}

	auto physicalDevice = new PhysicalDeviceVulkan(selectedDevice,selectedDeviceProperties,selectedDeviceFeatures,selectedType,mustQueueIndex
		,selectedDeviceProperties.deviceName,selectedDeviceProperties.apiVersion,
		selectedDeviceProperties.driverVersion,selectedDeviceProperties.vendorID);

	m_physicalDevice.reset(physicalDevice);

	auto queueFamilyProps = get_queue_family_properties_from_device(physicalDevice->m_physical_device);
	
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

	std::vector<const char*> enabledDeviceExtensions;

	for (const auto& ex : allExs)
	{
		if (isDebugEnabled && ex.first == "VK_LAYER_KHRONOS_validation")
		{
			std::vector< VkExtensionProperties> props;
			for (const auto& exProp : ex.second)
			{
				enabledDeviceExtensions.push_back(exProp.extensionName);
				props.push_back(exProp);
			}
			m_enabledDeviceExtensions.emplace(ex.first, props);
		}
	}


	
	if (!create_logical_device(m_physicalDevice->m_physical_device, &m_physicalDevice->m_physical_device_features, &queueInf,
		&enabledDeviceLayers, &enabledDeviceExtensions, &m_device))
	{
		return false;
	}

	volkLoadDevice(m_device);
	isDeviceInitedSuccessfully = true;



	return true;
}

GRAPHIC_API RenderDeviceVulkan::get_graphic_api() const noexcept
{
	return GRAPHIC_API_VULKAN;
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
	std::vector<VkQueueFamilyProperties> queueProps(queueCount);
	vkGetPhysicalDeviceQueueFamilyProperties(dev, &queueCount, queueProps.data());
	return queueProps;
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
	createInfo.enabledLayerCount = props->size();
	createInfo.ppEnabledLayerNames = props->data();
	createInfo.enabledExtensionCount = exs->size();
	createInfo.ppEnabledExtensionNames = exs->data();
	createInfo.pEnabledFeatures = features;

	if (vkCreateDevice(physicalDev, &createInfo, nullptr, device) != VK_SUCCESS)
		return false;

	return true;
}