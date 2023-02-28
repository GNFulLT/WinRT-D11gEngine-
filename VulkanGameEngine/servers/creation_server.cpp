#include "creation_server.h"

#include "../graphic/vulkan/render_device_vulkan.h"

#include "configuration_server.h"
#include "logger_server.h"
EventBusServer* CreationServer::create_event_bus_server()
{
	return EventBusServer::create_singleton();
}

WindowServer* CreationServer::create_the_window_server()
{
	auto wind = WindowServer::create_singleton();
	auto conf = wind->config_creation();
	ConfigurationServer::get_singleton()->register_config("WindowServer",conf);
	return wind;
}

LoggerServer* CreationServer::create_logger_server()
{
	return LoggerServer::create_singleton();
}

ConfigurationServer* CreationServer::create_configuration()
{
	return ConfigurationServer::create_singleton();
}
/*
RenderDevice* CreationServer::create_render_device()
{
	std::string mod = "UNKNOWN";
	ConfigurationServer::get_singleton()->get_init_configuration("config.json", "GRAPHIC_API", mod);

	GRAPHIC_API selectedApi = GRAPHIC_API_VULKAN;

	if (mod == "VULKAN")
	{
		bool isSupported = WindowServer::singleton->is_graphic_api_supported(GRAPHIC_API_VULKAN);
		if (isSupported)
			selectedApi = GRAPHIC_API_VULKAN;
		else
			selectedApi = WindowServer::singleton->get_default_graphic_api();
	}
#ifdef _WIN32
	else if (mod == "DIRECTX12")
	{
		bool isSupported = WindowServer::singleton->is_graphic_api_supported(GRAPHIC_API_DIRECTX_12);
		if (isSupported)
			selectedApi = GRAPHIC_API_DIRECTX_12;
		else
			selectedApi = WindowServer::singleton->get_default_graphic_api();

	}
	else if (mod == "DIRECTX11")
	{
		bool isSupported = WindowServer::singleton->is_graphic_api_supported(GRAPHIC_API_DIRECTX_11);
		if (isSupported)
			selectedApi = GRAPHIC_API_DIRECTX_11;
		else
			selectedApi = WindowServer::singleton->get_default_graphic_api();

	}
#endif
	else
	{
		selectedApi = WindowServer::singleton->get_default_graphic_api();
	}

	bool created = true;
	switch (selectedApi)
	{
	case GRAPHIC_API_VULKAN:
		 created = create_vulkan_render_device();
		if (!created)
			throw std::runtime_error("Couldn't initialized vulkan for this window supporter");
		break;
	default:
		if (!WindowServer::singleton->is_graphic_api_supported(GRAPHIC_API_VULKAN))
			throw std::runtime_error("THERE IS NO SUPPORTED GRAPHIC API.PLEASE TRY TO CHANGE WINDOW SUPPORTER");
		 created = create_vulkan_render_device();
		if (!created)
			throw std::runtime_error("THERE IS NO SUPPORTED GRAPHIC API.PLEASE TRY TO CHANGE WINDOW SUPPORTER");
		break;
	}

	return RenderDevice::singleton;
}
*/
void CreationServer::destroy()
{
	delete singleton;
}


// Helper Funcs

bool CreationServer::create_vulkan_render_device()
{
	auto vulkanDev = new RenderDeviceVulkan();
	RenderDevice::singleton = vulkanDev;
	return true;
}