#include "pch.h"
#include "Logger.h"
namespace GNF::Common
{
	inline void Logger::LogError(const char* msg)
	{
		spdlog::error(msg);
	}

	inline void Logger::LogCritical(const char* msg)
	{
		spdlog::critical(msg);
	}

	inline void Logger::LogDebug(const char* msg)
	{
		spdlog::debug(msg);
	}

	inline void Logger::LogInfo(const char* msg)
	{
		spdlog::info(msg);
	}

	inline void Logger::LogWarn(const char* msg)
	{
		spdlog::warn(msg);
	}

}