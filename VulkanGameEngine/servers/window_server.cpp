#include "window_server.h"


Config* WindowServer::config_creation()
{
	ConfigPropRegistery size("size", m_size);
	ConfigPropRegistery windowMode("window_mode", m_windowMode);
	ConfigPropRegistery monitorSelection("monitor_selection", m_monitorSelection);
	ConfigPropRegistery windowInitialPos("window_initial_position", m_windowInitialPosition);
	Config* conf = new Config(this,size,windowMode,monitorSelection, windowInitialPos);
	return conf;
}

WindowServer* WindowServer::create_singleton()
{
	singleton = new WindowServer("WindowServer");
	return singleton;
}

bool WindowServer::init()
{
	if (!glfwInit())
		return false;

	switch (*m_monitorSelection->get_prop())
	{
	case MONITOR_SELECTION_PRIMARY:
		m_monitor = nullptr;
		break;
	default:
		m_monitor = nullptr;
		break;
	}

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	// Vulkan
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	
	switch (*m_windowMode->get_prop())
	{
	case WINDOW_MODE_WINDOWED:
		m_window = glfwCreateWindow(m_size->get_prop()->x, m_size->get_prop()->y, "GNF Engine", m_monitor, NULL);
		break;
		// Not Supported Yet
	default:
		m_window = glfwCreateWindow(m_size->get_prop()->x, m_size->get_prop()->y, "GNF Engine", m_monitor, NULL);
		break;
	}
	//m_window = glfwCreateWindow(m_size->get_prop()->x, m_size->get_prop()->y, "GNF Engine", monitor, NULL);

	return true;
}

void WindowServer::destroy()
{
	delete singleton;
}