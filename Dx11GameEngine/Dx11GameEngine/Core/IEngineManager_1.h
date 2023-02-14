#pragma once	
#include "IEngineManager.h"
#include <vector>
#include "Common/IResource.h"
namespace GNF::Core
{
	//!: This interface helps to implement subflow tasking
	class IEngineManager_1 : public IEngineManager
	{
	public:
		IEngineManager_1(const char* name = "UNKNOWN") : IEngineManager(name)
		{}

		virtual ~IEngineManager_1() = default;

		
	protected:
		virtual void Subflow_Init() = 0;

		tf::Subflow& GetSubflow()
		{
			return m_subflow;
		}

		//!: Subflow
		tf::Task AddAsSubflowTask(Common::IResource* res)
		{
			tf::Task& task = m_subflow.emplace([res = res]() {
				bool initalizedSucceed = true;
				try
				{
					res->Init();
				}
				catch (const std::exception& ex)
				{
					initalizedSucceed = false;
					Common::Logger::LogError(std::format("Exception in method named with {}\n{}", res->GetName(), ex.what()).c_str());
				}
				if (initalizedSucceed)
					res->Initialized();
			}).name(res->GetName());
			
			return task;
		}

	private:
		tf::Subflow m_subflow;
		
	};
}