#include "creation_server.h"

#include "../platform/GLFW/window_server_glfw.h"


EventBusServer* CreationServer::create_event_bus_server()
{
	return EventBusServer::create_singleton();
}
WindowServer* CreationServer::create_the_window_server()
{
	
	WindowServerGLFW* glfwServer = new WindowServerGLFW("WindowServerGLFW");
	auto conf = glfwServer->config_creation();
	ConfigurationServer::get_singleton()->register_config("WindowServer", conf );
	WindowServer::singleton = glfwServer;
	return glfwServer;
}

PluginServer* CreationServer::create_plugin_server()
{
	return PluginServer::create_singleton();
}

ConfigurationServer* CreationServer::create_configuration_server()
{
	return ConfigurationServer::create_singleton();
}

void CreationServer::destroy()
{
	delete singleton;
}