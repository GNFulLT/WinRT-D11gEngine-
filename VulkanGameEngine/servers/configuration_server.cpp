#include "configuration_server.h"


void ConfigurationServer::scope_init_begins()
{
	m_init_scope_started = true;
}

void ConfigurationServer::scope_init_ends()
{
	m_init_scope_started = false;
	m_init_scope_finished = true;
}

void ConfigurationServer::scope_expose_begins()
{
	m_expose_scope_started = true;
}

void ConfigurationServer::scope_expose_ends()
{
	m_expose_scope_started = false;
	m_expose_scope_finished = true;
}

void ConfigurationServer::scope_change_begins()
{
	m_change_scope_started = true;
}

void ConfigurationServer::scope_change_ends()
{
	m_change_scope_started = false;
	m_change_scope_finished = true;
}

void ConfigurationServer::destroy()
{
	delete singleton;
}