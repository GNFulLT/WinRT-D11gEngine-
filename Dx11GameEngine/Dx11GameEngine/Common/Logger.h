#pragma once

#include <spdlog/spdlog.h>

namespace GNF::Common::Logger
{
	inline void LogError(const char* msg)
	{
		spdlog::error(msg);
	}

	inline void LogCritical(const char* msg)
	{
		spdlog::critical(msg);
	}

	inline void LogDebug(const char* msg)
	{
		spdlog::debug(msg);
	}

	inline void LogInfo(const char* msg)
	{
		spdlog::info(msg);
	}

	inline void LogWarn(const char* msg)
	{
		spdlog::warn(msg);
	}
	
	inline void LogTrace(const char* msg)
	{
		spdlog::trace(msg);
	}

}