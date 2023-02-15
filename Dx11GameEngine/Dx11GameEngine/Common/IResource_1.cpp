#include "pch.h"
#include "IResource_1.h"

namespace GNF::Common
{
	tf::Task IResource_1::Async_Init(tf::Taskflow& flow)
	{
		auto task = flow.emplace([n = this, sub = &this->m_subflow](tf::Subflow& subflow)
			{
				(*sub) = &subflow;
				n->Subflow_Init();

			});
#ifdef _DEBUG
		task.name(GetName());
#endif
		return task;
	}

	tf::Task IResource_1::InitAsTask()
	{
		tf::Task& task = m_subflow->emplace([n = this]
			{
				bool initalizedSucceed = true;
				try
				{
					n->Init();
				}
				catch (const std::exception& ex)
				{
					initalizedSucceed = false;
					Common::Logger::LogError(std::format("Exception in method named with {}\n{}", n->GetName(), ex.what()).c_str());
				}
				if (initalizedSucceed)
					n->Initialized();
			}).name(std::format("{} : Init", GetName()));

			return task;
	}

	tf::Task IResource_1::AddAsSubflowTask(Common::IResource_1* res)
	{
		tf::Task& task = m_subflow->emplace([res = res, sub = res->GetSubflow()](tf::Subflow& subflow) {
			(*sub) = &subflow;
			res->Subflow_Init();
			}).name(res->GetName());

			return task;
	}

	tf::Task IResource_1::AddAsSubflowTask(Common::IResource* res)
	{
		tf::Task& task = m_subflow->emplace([res = res]() {
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
}