#include "creation_server.h"

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

RenderDevice* CreationServer::create_render_device()
{
	return RenderDevice::create_singleton();
}
ThreadPoolServer* CreationServer::create_thread_pool()
{
	return ThreadPoolServer::create_singleton();
}
void CreationServer::destroy()
{
	delete singleton;
}


