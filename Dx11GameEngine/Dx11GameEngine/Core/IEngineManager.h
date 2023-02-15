#pragma once


#include <taskflow/taskflow.hpp>
#include <functional>
#include "Common/Logger.h"
namespace GNF::Core
{
	class IEngineManager
	{
	public:
		IEngineManager(const char* name = "UNKNOWN") : m_name(name), m_isInitialized(false)
		{

		}
		virtual ~IEngineManager() = default;
		
		inline bool IsInitialized()
		{
			return m_isInitialized;
		}
		virtual void Init() = 0;
		inline void Initialized()
		{
			m_isInitialized = true;
		}

		virtual tf::Task Async_Init(tf::Taskflow& flow);

		//!: For Debug purposes
	public:
		const std::string& GetName()
		{
			return m_name;
		}



	private:
		std::string m_name;
		bool m_isInitialized;
	};
}