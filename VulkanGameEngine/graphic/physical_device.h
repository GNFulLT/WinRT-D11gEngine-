#ifndef PHYSICAL_DEVICE_H
#define PHYSICAL_DEVICE_H

#include "../core/string/string_types.h"
#include "../core/typedefs.h"

class PhysicalDevice
{
public:
	
	enum PHYSICAL_DEVICE_TYPE
	{
		PHYSICAL_DEVICE_TYPE_UNDEFINED = 0,
		PHYSICAL_DEVICE_TYPE_INTEGRATED,
		PHYSICAL_DEVICE_TYPE_DISCRETE,
		PHYSICAL_DEVICE_TYPE_VIRTUAL,
		PHYSICAL_DEVICE_TYPE_CPU
	};

	virtual ~PhysicalDevice() = 0;

	bool is_nvidia()   const noexcept { return m_vendor_id == 0x10DE; }
	bool is_amd()      const noexcept { return m_vendor_id == 0x1002 || m_vendor_id == 0x1022; }
	bool is_intel()    const noexcept { return m_vendor_id == 0x8086 || m_vendor_id == 0x163C || m_vendor_id == 0x8087; }
	bool is_arm()      const noexcept { return m_vendor_id == 0x13B5; }
	bool is_qualcomm() const noexcept { return m_vendor_id == 0x5143; }
	bool is_mac() const noexcept { return m_vendor_id == 0x106B; }
protected:
	String api_version = "UNKNONW";
	String driver_version = "UNKNOWN"; 
	uint32_t m_vendor_id = 0; 
	String vendor_name = "UNKNOWN";
};

#endif // PHYSICAL_DEVICE_H