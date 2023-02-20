#include "window_server_glfw.h"

#include <exception>

WindowServerGLFW::~WindowServerGLFW()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void WindowServerGLFW::handle_events()
{
	glfwPollEvents();
}

void WindowServerGLFW::init()
{

	//X TODO : Custom Exception
	if (!glfwInit())
		throw std::runtime_error("Couldn't initalized");

	GLFWmonitor* monitor = nullptr;

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

}

WindowServerGLFW::WINDOW_SUPPORTER WindowServerGLFW::get_window_supporter() const noexcept 
{
	return WINDOW_SUPPORTER_GLFW;
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
