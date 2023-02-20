#include "window_server.h"


Config* WindowServer::config_creation()
{
	ConfigPropRegistery size("size", m_size);
	ConfigPropRegistery windowMode("window_mode", m_windowMode);
	ConfigPropRegistery monitorSelection("monitor_selection", m_monitorSelection);
	ConfigPropRegistery windowInitialPos("window_initial_position", m_windowInitialPosition);
	Config* conf = new Config(size,windowMode);
	return conf;
}

void WindowServer::destroy()
{
	delete singleton;
}