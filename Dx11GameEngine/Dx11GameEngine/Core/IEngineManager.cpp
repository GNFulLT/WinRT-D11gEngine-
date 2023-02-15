#include "pch.h"
#include "IEngineManager.h"

namespace GNF::Core
{
	tf::Task IEngineManager::Async_Init(tf::Taskflow& flow)
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
					Common::Logger::LogError(ex.what());
				}
				if (initalizedSucceed)
					n->Initialized();
			});
#ifdef _DEBUG
		task.name(GetName());
#endif
		return task;
	}
}