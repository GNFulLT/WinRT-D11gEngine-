#include "creation_server.h"

#include "../platform/GLFW/window_server_glfw.h"
#include "../graphic/vulkan/render_device_vulkan.h"

#include "configuration_server.h"
#include "logger_server.h"
EventBusServer* CreationServer::create_event_bus_server()
{
	return EventBusServer::create_singleton();
}

WindowServer* CreationServer::create_the_window_server()
{
	std::string mod = "UNKNOWN";
	ConfigurationServer::get_singleton()->get_init_configuration("config.json", "WINDOW_SUPPORTER", mod);
	if (mod == "GLFW")
	{
		LoggerServer::get_singleton()->log_cout("Selected Window Supporter is GLFW",Logger::DEBUG);
		WindowServerGLFW* glfwServer = new WindowServerGLFW("WindowServerGLFW");
		auto conf = glfwServer->config_creation();
		ConfigurationServer::get_singleton()->register_config("WindowServer", conf);
		WindowServer::singleton = glfwServer;
	}
	else if (mod == "WIN32")
	{
		LoggerServer::get_singleton()->log_cout("Selected Window Supporter is WIN32 but WIN32 is not supported. Selected GLFW", Logger::DEBUG);
		//X TODO : Need Support
		WindowServerGLFW* glfwServer = new WindowServerGLFW("WindowServerGLFW");
		auto conf = glfwServer->config_creation();
		ConfigurationServer::get_singleton()->register_config("WindowServer", conf);
		WindowServer::singleton = glfwServer;
	}
	else if (mod == "UWP")
	{
		LoggerServer::get_singleton()->log_cout("Selected Window Supporter is UWP but UWP is not supported. Selected GLFW", Logger::DEBUG);
		//X TODO : Need support
		WindowServerGLFW* glfwServer = new WindowServerGLFW("WindowServerGLFW");
		auto conf = glfwServer->config_creation();
		ConfigurationServer::get_singleton()->register_config("WindowServer", conf);
		WindowServer::singleton = glfwServer;
	}
	else
	{
		LoggerServer::get_singleton()->log_cout("Selected Default GLFW", Logger::DEBUG);
		//X TODO : Maybe fix the config file 
		WindowServerGLFW* glfwServer = new WindowServerGLFW("WindowServerGLFW");
		auto conf = glfwServer->config_creation();
		ConfigurationServer::get_singleton()->register_config("WindowServer", conf);
		WindowServer::singleton = glfwServer;
	}
	
	return WindowServer::singleton;
}

LoggerServer* CreationServer::create_logger_server()
{
	return LoggerServer::create_singleton();
}

ConfigurationServer* CreationServer::create_configuration()
{
	return ConfigurationServer::create_singleton();
}

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