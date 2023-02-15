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

		virtual tf::Task Async_Init(tf::Taskflow& flow) override;

	public:
		virtual void Subflow_Init() = 0;
	protected:
		//!: Subflow
		tf::Task AddAsSubflowTask(Common::IResource* res);
		tf::Task InitAsTask();
	private:
		tf::Subflow* m_subflow = nullptr;
		
	};
}