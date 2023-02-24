#include "render_device.h"

#include "../logger_server.h"


void RenderDevice::destroy()
{
	delete singleton;
}

void RenderDevice::log_couldnt_initialized_debug_mode() const noexcept
{
	LoggerServer::get_singleton()->log_cout(this,"Couldn't initalize debug mode",Logger::WARNING);

}

void RenderDevice::log_initialized_debug_mode() const noexcept
{
	LoggerServer::get_singleton()->log_cout(this, "Debug mode initialized", Logger::WARNING);

}

void RenderDevice::log_couldnt_initialized_normal_mode() const noexcept
{
	LoggerServer::get_singleton()->log_cout(this, "Couldn't initalize normal mode", Logger::WARNING);

}

void RenderDevice::log_initialized_normal_mode() const noexcept
{
	LoggerServer::get_singleton()->log_cout(this, "Normal mode initialized", Logger::WARNING);

}