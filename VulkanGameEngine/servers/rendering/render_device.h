#ifndef RENDER_DEVICE_H
#define RENDER_DEVICE_H

#include "../../core/object/object.h"
#include "../../graphic/graphic_api.h"
#include "../../graphic/physical_device.h"

class CreationServer;

class RenderDevice : public Object
{
	OBJECT_DEF(RenderDevice,Object)

public:
	virtual ~RenderDevice() = default;

	virtual GRAPHIC_API get_graphic_api() const noexcept = 0;
	virtual bool init() = 0;
	virtual PhysicalDevice* get_selected_physical_device() const noexcept = 0;

	virtual void destroy();


	_INLINE_ static RenderDevice* get_singleton()
	{
		return singleton;
	}

protected:

	void log_couldnt_initialized_debug_mode() const noexcept;
	void log_initialized_debug_mode() const noexcept;
	void log_couldnt_initialized_normal_mode() const noexcept;
	void log_initialized_normal_mode() const noexcept;

protected:
	friend class CreationServer;
	_INLINE_ static RenderDevice* singleton;
};

#endif // RENDER_DEVICE_H