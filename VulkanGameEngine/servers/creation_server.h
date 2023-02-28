#ifndef CREATION_SERVER_H
#define CREATION_SERVER_H

#include "../core/object/object.h"
#include "window_server.h"
#include "configuration_server.h"
#include "../config/config.h"
#include "logger_server.h"
#include "event_bus_server.h"
#include "rendering/render_device.h"

class CreationServer : public Object
{
	OBJECT_DEF(CreationServer,Object)

public:
	CreationServer(const CreationServer&) = delete;

	CreationServer& operator=(const CreationServer&) = delete;



	static _INLINE_ CreationServer* create_creation()
	{
		return get_singleton();
	}

	static _INLINE_ CreationServer* get_singleton()
	{
		return singleton;
	}

	ConfigurationServer* create_configuration();
	WindowServer* create_the_window_server();
	LoggerServer* create_logger_server();
	EventBusServer* create_event_bus_server();
	//RenderDevice* create_render_device();

	void destroy();	

private:
	CreationServer()
	{}

	bool create_vulkan_render_device();

private:

	_INLINE_ static CreationServer* singleton;
};

#endif // CREATION_SERVER_H