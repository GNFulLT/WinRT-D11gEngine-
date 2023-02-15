#include "pch.h"
#include "IResource.h"

namespace GNF::Common
{
	tf::Task IResource::Async_Init(tf::Taskflow& flow)
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
			if (initalizedSucceed)
				n->Initialized();
		});
#ifdef _DEBUG
	task.name(GetName());
#endif
	return task;
		}
}