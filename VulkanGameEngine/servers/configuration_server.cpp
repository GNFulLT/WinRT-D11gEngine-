#include "configuration_server.h"


void ConfigurationServer::scope_int_begins()
{
	m_init_scope_started = true;
	m_init_scope_finished = false;
}

void ConfigurationServer::scope_int_ends()
{
	m_init_scope_started = false;
	m_init_scope_finished = true;
	m_inited = true;
}
