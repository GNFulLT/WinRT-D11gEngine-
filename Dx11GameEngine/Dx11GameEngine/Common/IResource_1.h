#pragma once	
#include "IResource.h"
#include <vector>
namespace GNF::Common
{
	//!: This interface helps to implement subflow tasking
	class IResource_1 : public IResource
	{
	public:
		IResource_1(const char* name = "UNKNOWN") : IResource(name)
		{}

		virtual ~IResource_1() = default;

		virtual tf::Task Async_Init(tf::Taskflow& flow)
		{
			//!: Calls Subflow Init and initialize subflow tasks fills inside of m_subflowTasks
			Subflow_Init();
			auto task = flow.emplace([n = this](tf::Subflow& subflow) {

				});
		}
	protected:
		virtual void AddAsSubflowTask(const tf::Task& task)
		{
			m_subflowTasks.push_back(task);
		}

	private:
		std::vector<tf::Task> m_subflowTasks;
		virtual void Subflow_Init() = 0;
	};
}