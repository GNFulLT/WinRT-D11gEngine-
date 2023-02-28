#ifndef UTILS_VULKAN_H
#define UTILS_VULKAN_H


#define VK_NO_PROTOTYPES
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "volk.h"

#include <unordered_map>
#include <vector>

#include "../../core/typedefs.h"


_F_INLINE_ _IMP_RETURN_ bool get_all_instance_layers(std::vector<VkLayerProperties>& layerProps)
{
	uint32_t layer_count = 0;
	if (vkEnumerateInstanceLayerProperties(&layer_count, nullptr) != VK_SUCCESS)
	{
		return false;
	}
	layerProps = std::vector<VkLayerProperties>(layer_count);
	if (vkEnumerateInstanceLayerProperties(&layer_count, layerProps.data()) != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

_F_INLINE_ _IMP_RETURN_ bool find_layer_by_name(const std::vector<VkLayerProperties>* layerProps, const char* name, int& index)
{
	index = -1;

	for (int i = 0; i < layerProps->size(); i++)
	{
		if (strcmp((*layerProps)[i].layerName, name) == 0)
		{
			index = i;
			return true;
		}
	}


	return false;
}

_F_INLINE_ _IMP_RETURN_ bool get_instance_exs_by_layer(const char* forLayer, std::vector<VkExtensionProperties>& exs)
{
	uint32_t ex_count = 0;
	if (vkEnumerateInstanceExtensionProperties(forLayer, &ex_count, nullptr) != VK_SUCCESS)
	{
		return false;
	}
	exs = std::vector<VkExtensionProperties>(ex_count);
	if (vkEnumerateInstanceExtensionProperties(forLayer, &ex_count, exs.data()) != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

_F_INLINE_ bool get_instance_implicit_exs(std::vector<VkExtensionProperties>& exs)
{
	return get_instance_exs_by_layer(nullptr, exs);
}

_F_INLINE_ _IMP_RETURN_ std::unordered_map<std::string, std::vector<VkExtensionProperties>> get_all_instance_exs_by_layers(std::vector<const char*>* layers)
{
	std::unordered_map<std::string, std::vector<VkExtensionProperties>> map;
	for (int i = 0; i < layers->size(); i++)
	{
		std::vector<VkExtensionProperties> props;

		if (!get_instance_exs_by_layer((*layers)[i], props))
			continue;
		map.emplace(std::string((*layers)[i]), props);
	}
	return map;
}

_F_INLINE_ bool find_instance_exs_by_names(const std::vector<VkExtensionProperties>* exs,const std::vector<const char*>* names,std::vector<int>& indexes)
{
	std::set<std::string> searchedNames(names->begin(), names->end());

	for (int i = 0;i<exs->size();i++)
	{
		if (searchedNames.find((*exs)[i].extensionName) != searchedNames.end())
		{
			indexes.push_back(i);
			searchedNames.erase((*exs)[i].extensionName);
		}
	}

	return indexes.size() == names->size();
}

_F_INLINE_ _IMP_RETURN_ bool create_debug_messenger(VkInstance instance, VkDebugUtilsMessengerEXT* pDebugMessenger, VkDebugReportCallbackEXT* pDebugReporter,
	PFN_vkDebugUtilsMessengerCallbackEXT vk_debug_messenger_callback,PFN_vkDebugReportCallbackEXT VulkanDebugReportCallback) {
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
			.pfnCallback = VulkanDebugReportCallback,
			.pUserData = nullptr
	};

	result = vkCreateDebugReportCallbackEXT(instance, &ci, nullptr, pDebugReporter);
	return result == VK_SUCCESS;
}
#endif

