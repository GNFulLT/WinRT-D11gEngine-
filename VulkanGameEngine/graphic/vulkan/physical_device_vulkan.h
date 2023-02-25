#ifndef PHYSICAL_DEVICE_VULKAN_H
#define PHYSICAL_DEVICE_VULKAN_H

#include <vector>
#include <utility>
#include <cassert>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../physical_device.h"
#include "../../core/typedefs.h"
class RenderDeviceVulkan;

class PhysicalDeviceVulkan : public PhysicalDevice
{
public:

	struct QueueCreateInf
	{
	public:
		QueueCreateInf()
		{
			m_priorities = new std::vector<float>();
		}
		~QueueCreateInf()
		{
			if(m_priorities)
				delete m_priorities;
		}
		
		_INLINE_ uint32_t get_queue_create_inf_count() const
		{
			return (uint32_t)infos.size();
		}

		_INLINE_ const VkDeviceQueueCreateInfo* data() const
		{
			return infos.data();
		}

		_INLINE_ void add_create_info(uint32_t index, const std::vector<float>& priorities)
		{
			for (int i = 0; i < priorities.size(); i++)
			{
				m_priorities->push_back(priorities[i]);
			}

			VkDeviceQueueCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			info.pNext = nullptr;
			info.queueFamilyIndex = index;
			info.queueCount = (uint32_t)priorities.size();
			info.pQueuePriorities = &(*m_priorities)[m_priorities->size() - priorities.size()];
			infos.push_back(info);

			
		}

	private:
		std::vector<VkDeviceQueueCreateInfo> infos;
		std::vector<float>* m_priorities;

	};

	PhysicalDeviceVulkan(VkPhysicalDevice dev,VkPhysicalDeviceProperties prop,VkPhysicalDeviceFeatures features,PHYSICAL_DEVICE_TYPE type,
		const String& deviceName,uint_fast32_t apiVersion, uint_fast32_t driverVersion, uint_fast32_t vendorId) :
		PhysicalDevice(type,deviceName,apiVersion, driverVersion, vendorId),m_physical_device(dev), m_physical_device_props(prop), m_physical_device_features(features)
	{

	}
	PhysicalDeviceVulkan(const PhysicalDeviceVulkan&) = delete;

private:


	_INLINE_ void select_queue_family_properties(const std::unordered_map<uint32_t, VkQueueFamilyProperties>& props) noexcept
	{
		m_selectedQueueFamilyProps = props;
	}

	_INLINE_ void create_queue_create_infos_from_selected_queue_family_propperties(
		QueueCreateInf& inf,
		const std::unordered_map<uint32_t,std::vector<float>>* prioritiesByIndex = nullptr
	) const noexcept
	{
		if (prioritiesByIndex == nullptr)
		{
			for (const auto& queueFamilyPropIterator : m_selectedQueueFamilyProps)
			{
				inf.add_create_info(queueFamilyPropIterator.first, { 1.f });
			}
		}
		else
		{
			assert(false);
		}

	}

private:
	friend class RenderDeviceVulkan;
	std::unordered_map<uint32_t,VkQueueFamilyProperties> m_selectedQueueFamilyProps;
	VkPhysicalDevice m_physical_device;
	VkPhysicalDeviceProperties m_physical_device_props;
	VkPhysicalDeviceFeatures m_physical_device_features;
};
	
#endif // PHYSICAL_DEVICE_VULKAN_H