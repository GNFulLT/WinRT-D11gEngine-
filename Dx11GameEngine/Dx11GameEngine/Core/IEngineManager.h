#pragma once


#include <taskflow/taskflow.hpp>
#include <functional>
#include "Common/Logger.h"
#include "Common/IResource.h"
namespace GNF::Core
{
	class IEngineManager : public Common::IResource
	{
	public:
		IEngineManager(const char* name = "UNKNOWN") : IResource(name)
		{

		}
		virtual ~IEngineManager() = default;
	};
}