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

		virtual tf::Task Async_Init(tf::Taskflow& flow) override;
		inline tf::Subflow** GetSubflow()
		{
			return &m_subflow;
		}
	public:
		virtual void Subflow_Init() = 0;
	protected:
		//!: Subflow
		tf::Task AddAsSubflowTask(Common::IResource* res);
		tf::Task AddAsSubflowTask(Common::IResource_1* res);

		tf::Task InitAsTask();
	private:
		tf::Subflow* m_subflow = nullptr;

	};
}