#include "logger_server.h"
#include <boost/foreach.hpp>

void LoggerServer::log_cout(const Object* who, const String& msg, Logger::LOG_LEVEL level) const
{
	m_coutLogger->log(boost::str(boost::format("%1% [Logged by %2%]") % msg % who->get_class_name()).c_str(), level);
}

void LoggerServer::log_cout(const Object* who, const char* msg, Logger::LOG_LEVEL level) const
{
	m_coutLogger->log(boost::str(boost::format("%1% [Logged by %2%]") % msg % who->get_class_name()).c_str(), level);
}

void LoggerServer::destroy()
{
	delete singleton;
}

LoggerServer* LoggerServer::create_singleton()
{
	singleton = new LoggerServer();
	singleton->m_coutLogger.reset(new Logger());
	return singleton;
}
