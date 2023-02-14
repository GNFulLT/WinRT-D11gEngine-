#pragma once

#include <taskflow/taskflow.hpp>

namespace GNF::Common
{
	class IResource
	{
	public:
		IResource(const char* name = "UNKNOWN") : m_name(name), m_isInitialized(false)
		{

		}
		virtual ~IResource() = default;

		inline bool IsInitialized()
		{
			return m_isInitialized;
		}
		virtual void Init() = 0;
		inline void Initialized()
		{
			m_isInitialized = true;
		}

		tf::Task Async_Init(tf::Taskflow& flow)
		{
			auto task = flow.emplace([n = this] 
				{
					bool initalizedSucceed = true;
					try
					{
						n->Init();
					} 
					catch (const std::exception& ex)
					{
						initalizedSucceed = false;
						Logger::LogError(ex.what());
					}
					if(initalizedSucceed)
						n->Initialized(); 
				});
#ifdef _DEBUG
			task.name(GetName());
#endif
			return task;
		}

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