#include "logger_server.h"

void LoggerServer::log_cout(Object* who, const String& msg, Logger::LOG_LEVEL level)
{
	m_coutLogger->log(std::format("{} [Logged by {}]",msg ,who->get_class_name()).c_str(), level);
}

void LoggerServer::log_cout(Object* who, const char* msg, Logger::LOG_LEVEL level)
{
	m_coutLogger->log(std::format("{} [Logged by {}]", msg, who->get_class_name()).c_str(), level);
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
