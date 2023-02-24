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

	virtual ~PhysicalDevice() = default;

	PhysicalDevice(PHYSICAL_DEVICE_TYPE type, const String& deviceName, uint_fast32_t apiVersion, uint_fast32_t driverVersion, uint_fast32_t vendorId) :
		m_api_version(apiVersion),
		m_device_name(deviceName),
		m_driver_version(driverVersion),
		m_vendor_id(vendorId),
		m_type(type)
	{
		switch (m_vendor_id)
		{
		case 0x10DE:
			m_vendor_name = String("NVIDIA");
			break;
		case 0x1002:
		case 0x1022:
			m_vendor_name = String("AMD");
			break;
		case 0x8086:
		case 0x163C:
		case 0x8087:
			m_vendor_name = String("INTEL");
			break;
		case 0x13B5:
			m_vendor_name = String("ARM");
			break;
		case 0x5143:
			m_vendor_name = String("Qualcomm");
			break;
		case 0x106B:
			m_vendor_name = String("MAC");
			break;
		case 0x1010:
			m_vendor_name = String("ImgTec");
			break;
		}
	}
	PhysicalDevice(const PhysicalDevice&) = delete;
	PhysicalDevice& operator=(const PhysicalDevice&) = delete;

	_INLINE_ bool is_nvidia()   const noexcept { return m_vendor_id == 0x10DE; }
	_INLINE_ bool is_amd()      const noexcept { return m_vendor_id == 0x1002 || m_vendor_id == 0x1022; }
	_INLINE_ bool is_intel()    const noexcept { return m_vendor_id == 0x8086 || m_vendor_id == 0x163C || m_vendor_id == 0x8087; }
	_INLINE_ bool is_arm()      const noexcept { return m_vendor_id == 0x13B5; }
	_INLINE_ bool is_qualcomm() const noexcept { return m_vendor_id == 0x5143; }
	_INLINE_ bool is_mac() const noexcept { return m_vendor_id == 0x106B; }
	_INLINE_ bool is_imgtec() const noexcept { return m_vendor_id == 0x1010; }
	_INLINE_ const String& get_device_name() const noexcept { return m_device_name; }
	_INLINE_ const String& get_vendor_id() const noexcept { return m_vendor_name; }
	_INLINE_ PHYSICAL_DEVICE_TYPE get_device_type() const noexcept { return m_type; }

private:
	uint_fast32_t m_api_version = 0;
	uint_fast32_t m_driver_version = 0;	
	uint_fast32_t m_vendor_id = 0;
	String m_device_name = "UNKNOWN";
	String m_vendor_name = "UNKNOWN";
	PHYSICAL_DEVICE_TYPE m_type = PHYSICAL_DEVICE_TYPE_UNDEFINED;
};

#endif // PHYSICAL_DEVICE_H