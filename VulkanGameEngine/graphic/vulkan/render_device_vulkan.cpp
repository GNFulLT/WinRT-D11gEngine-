#include "render_device_vulkan.h"
#include "../../servers/logger_server.h"
#include "../../servers/configuration_server.h"
#include "../../core/version.h"




#include <boost/format.hpp>
#include <vector>
#include <unordered_map>

// Prototypes

_INLINE_ bool supports_validation_layer(std::vector<VkLayerProperties>* layers, int& index);
_INLINE_ std::vector<VkLayerProperties> get_supported_instance_layers();
_INLINE_ std::vector<VkExtensionProperties> get_supported_instance_exs(const char* forLayer);
_INLINE_ std::unordered_map<std::string,std::vector<VkExtensionProperties>> get_all_supported_instance_exs(std::vector<const char*>* enabledLyers);
_INLINE_ bool get_all_physicalDevicesPropsFutures(const VkInstance* inst,std::vector<VkPhysicalDevice>& physicalDevs,
	std::vector<VkPhysicalDeviceProperties>& physicalDevProps, std::vector<VkPhysicalDeviceFeatures>& physicalDevFeatures);
_INLINE_ PhysicalDevice::PHYSICAL_DEVICE_TYPE vk_to_physical_device_type(VkPhysicalDeviceType type);
RenderDeviceVulkan::RenderDeviceVulkan()
{
}

RenderDeviceVulkan::~RenderDeviceVulkan()
{
	if (m_instance != nullptr && isInstanceInitedSuccessfully)
	{
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
	if (isFound && outStr == "ENABLE")
	{
		debugInstanceCreated = try_to_create_instance(&applicationInfo, &m_instance,&instance_layer_props,true);
		triedToInitDebug = true;
	}

	// Debug Instance couldn't created first log and create normal instance
	if (!debugInstanceCreated)
	{
		if(triedToInitDebug)
			log_couldnt_initialized_debug_mode();
		//X TODO : Add No Debug Instance
		debugInstanceCreated = try_to_create_instance(&applicationInfo, &m_instance, &instance_layer_props);
		if (debugInstanceCreated)
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

	VkPhysicalDevice selectedDevice;
	VkPhysicalDeviceProperties selectedDeviceProperties;
	VkPhysicalDeviceFeatures selectedDeviceFeatures;
	PhysicalDevice::PHYSICAL_DEVICE_TYPE selectedType;
	if (!try_to_select_physical_device(physicalType,selectedDevice, selectedDeviceProperties, selectedDeviceFeatures, selectedType))
	{
		return false;
	}

	auto physicalDevice = new PhysicalDeviceVulkan(selectedDevice,selectedDeviceProperties,selectedDeviceFeatures,selectedType,selectedDeviceProperties.deviceName,selectedDeviceProperties.apiVersion,
		selectedDeviceProperties.driverVersion,selectedDeviceProperties.vendorID);

	m_physicalDevice.reset(physicalDevice);

	return true;
}

GRAPHIC_API RenderDeviceVulkan::get_graphic_api() const noexcept
{
	return GRAPHIC_API_VULKAN;
}
PhysicalDevice* RenderDeviceVulkan::get_selected_physical_device() const noexcept
{
	return nullptr;
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

	VkInstanceCreateInfo inf;
	inf.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	inf.pNext = nullptr;
	inf.flags = 0;
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

bool RenderDeviceVulkan::try_to_select_physical_device(PhysicalDevice::PHYSICAL_DEVICE_TYPE deviceType, VkPhysicalDevice& dev, VkPhysicalDeviceProperties& prop, VkPhysicalDeviceFeatures& feature, PhysicalDevice::PHYSICAL_DEVICE_TYPE& selectedType)
{
	std::vector<VkPhysicalDevice> physicalDevs;
	std::vector<VkPhysicalDeviceProperties> physicalDevProperties;
	std::vector<VkPhysicalDeviceFeatures> physicalDevFeatures;
	

	if (!get_all_physicalDevicesPropsFutures(&m_instance, physicalDevs, physicalDevProperties, physicalDevFeatures))
		return false;

	if (physicalDevs.size() == 0)
		return false;

	//X There is already just one device. No need to for loop
	if (physicalDevs.size() == 1)
	{
		dev = physicalDevs[0];
		prop = physicalDevProperties[0];
		feature = physicalDevFeatures[0];
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
		selectedType = vk_to_physical_device_type(prop.deviceType);
		return true;
	}
	bool isFound = false;
	int index = 0;


	for (int i = 0; i < physicalDevProperties.size(); i++)
	{
		if (physicalDevProperties[i].deviceType == type)
		{
			dev = physicalDevs[i];
			prop = physicalDevProperties[i];
			feature = physicalDevFeatures[i];
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
			selectedType = vk_to_physical_device_type(prop.deviceType);
			return true;
		}
	}

	//X Couldn't find any integrated gpu. Just return first physical device
	//X TODO : Logger Maybe ?

	dev = physicalDevs[0];
	prop = physicalDevProperties[0];
	feature = physicalDevFeatures[0];
	selectedType = vk_to_physical_device_type(prop.deviceType);
	return true;
}

// HELPERS

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

_INLINE_ bool supports_validation_layer(std::vector<VkLayerProperties>* layers,int& index)
{
	for (int i = 0;i<layers->size();i++)
	{
		if (strcmp((*layers)[i].layerName,"VK_LAYER_KHRONOS_validation") == 0)
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
		map.emplace(std::string((*enabledLayers)[i]),get_supported_instance_exs((*enabledLayers)[i]));
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