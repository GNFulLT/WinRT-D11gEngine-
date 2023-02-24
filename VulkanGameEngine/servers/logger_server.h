#ifndef LOGGER_SERVER_H
#define LOGGER_SERVER_H

#include "../core/object/object.h"
#include "../core/typedefs.h"
#include "../core/io/logger.h"
#include "../core/string/string_utils.h"
#include <memory>
#include <boost/format.hpp>

class CreationServer;
class ConfigurationServer;
class EventBusServer;

class LoggerServer : public Object
{
	OBJECT_DEF(LoggerServer,Object)
public:
	LoggerServer() = default;
	LoggerServer(const LoggerServer&) = delete;
	LoggerServer& operator=(const LoggerServer&) = delete;

	_INLINE_ static LoggerServer* get_singleton()
	{
		return singleton;
	}

	void log_cout(const Object* who, const String& msg, Logger::LOG_LEVEL level) const;
	void log_cout(const Object* who, const char* msg, Logger::LOG_LEVEL level) const;
	_INLINE_ Logger::LOG_LEVEL get_log_level_cout()
	{
		return m_coutLogger->get_log_level();
	}

	_INLINE_ void set_log_level_cout(Logger::LOG_LEVEL level)
	{
		m_coutLogger->select_log_level(level);
	}
	void destroy();
private:
	friend class CreationServer;
	friend class ConfigurationServer;
	friend class EventBusServer;
	static LoggerServer* create_singleton();
	
	

	_INLINE_ void log_cout(const String& msg, Logger::LOG_LEVEL level)
	{
		m_coutLogger->log(msg.c_str(), level);

	}
	_INLINE_ void log_cout(const char* msg, Logger::LOG_LEVEL level)
	{
		m_coutLogger->log(msg, level);

	}

	_INLINE_ static LoggerServer* singleton;
private:
	std::unique_ptr<Logger> m_coutLogger;
};

#endif // LOGGER_SERVER_H