#include "creation_server.h"

#include "platform/GLFW/window_server_glfw.h"

WindowServer* CreationServer::create_the_window_server()
{
	
	WindowServerGLFW* glfwServer = new WindowServerGLFW("WindowServerGLFW");
	auto conf = glfwServer->config_creation();
	ConfigurationServer::get_singleton()->register_config("WindowServer", conf );
	WindowServer::singleton = glfwServer;
	return glfwServer;
}

ConfigurationServer* CreationServer::create_configuration()
{
	return ConfigurationServer::create_singleton();
}

void CreationServer::destroy()
{
	delete singleton;
}