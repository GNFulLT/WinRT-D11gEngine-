#include "creation_server.h"

#include "../platform/GLFW/window_server_glfw.h"
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

void CreationServer::destroy()
{
	delete singleton;
}