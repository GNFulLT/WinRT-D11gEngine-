#include "window_server_glfw.h"
#include "../../servers/rendering/render_device.h"
#include <stdexcept>

#ifdef _WIN32
	#define GLFW_EXPOSE_NATIVE_WIN32
	#include <GLFW/glfw3native.h>
#elif __linux__
	NOT SUPPORTED
#elif __APPLE__
	NOT SUPPORTED
#endif

WindowServerGLFW::~WindowServerGLFW()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void WindowServerGLFW::handle_events()
{
	glfwPollEvents();
}

void* WindowServerGLFW::get_native_handle()
{
	return glfwGetWin32Window(m_window);
}

bool WindowServerGLFW::init()
{

	//X TODO : Custom Exception
	if (!glfwInit())
		return false;

	GLFWmonitor* monitor = nullptr;

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	if (GRAPHIC_API_VULKAN == RenderDevice::get_singleton()->get_graphic_api())
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	}
	
	//X TODO : Logger Needed
	// This will be when window mode is full screen
	/*
	switch(*m_monitorSelection->get_prop())
	{
	case MONITOR_SELECTION_PRIMARY:
		monitor = glfwGetPrimaryMonitor();
		break;
	default:
		monitor = glfwGetPrimaryMonitor();
		break;
	};
	*/
	switch (*m_windowMode->get_prop())
	{
	case WINDOW_MODE_WINDOWED:
		m_window = glfwCreateWindow(m_size->get_prop()->x, m_size->get_prop()->y, "GNF Engine", monitor, NULL);
		break;
	// Not Supported Yet
	default:
		m_window = glfwCreateWindow(m_size->get_prop()->x, m_size->get_prop()->y, "GNF Engine", monitor, NULL);
		break;
	}
	//m_window = glfwCreateWindow(m_size->get_prop()->x, m_size->get_prop()->y, "GNF Engine", monitor, NULL);

	return true;
}

WindowServerGLFW::WINDOW_SUPPORTER WindowServerGLFW::get_window_supporter() const noexcept 
{
	return WINDOW_SUPPORTER_GLFW;
}

GRAPHIC_API WindowServerGLFW::get_default_graphic_api() const noexcept
{
	return GRAPHIC_API_VULKAN;
}

bool WindowServerGLFW::is_graphic_api_supported(GRAPHIC_API api) const noexcept
{
	return api == GRAPHIC_API_VULKAN;
}


bool WindowServerGLFW::should_close()
{
	assert(m_window != nullptr);

	return glfwWindowShouldClose(m_window);
}

void WindowServerGLFW::show()
{
	assert(m_window != nullptr);
	

	glfwShowWindow(m_window);
}
void WindowServerGLFW::hide()
{
	assert(m_window != nullptr);

	glfwHideWindow(m_window);
	m_windowMode->set_prop(WINDOW_MODE_MINIMIZED);
}
